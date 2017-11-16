#include "redis_conn_pool.h"
#include "redis_pool_mgr.h"
//#include "my_logging.h"
namespace MyRedis {
	RedisConnPool::RedisConnPool(RedisPoolMgr * pool_mgr) : _pool_mgr(pool_mgr),
		_need_adjust_pool_conn_count(false),
		_adjust_conn_pool_count(0) {
		_keep_alive_timer.StartTimer(g_default_keep_alive_interval, std::bind(&RedisConnPool::timer_run, this));
	}

	RedisConnPool::~RedisConnPool() {
	}

	bool RedisConnPool::init(const RedisNodeInfo& node_info) {
		_node_info = node_info;
		adjust_pool_conn_count();

		return true;
	}

	RedisConn* RedisConnPool::conn() {
		RedisConn* conn = NULL;
		std::unique_lock<std::mutex> locker(_conn_pool_mutex);

		//从队列中找到一个可用的连接,把_coon_pool中不可用的连接加入到保活队列中
		do {
			if (_conn_pool.empty() && 
				_node_info.min_conn_count()*_adjust_conn_pool_count < _node_info.max_conn_count()) {
				_need_adjust_pool_conn_count = true;
			}
			
			while(_conn_pool.empty()) {
		   		_conn_pool_cond.wait(locker);
			}
			conn = _conn_pool.front();
			_conn_pool.pop_front();
			if (conn->can_use() && RedisConn::CONN_STAT_CONNED == conn->stat()) {
				break;
			} else {
				add_keep_alive_conn(conn);
				conn = NULL;
			}
		} while(1);
			
		return conn;
	}
	
	void RedisConnPool::free_conn(RedisConn * conn) {
		add_conn(conn);
	}

	void RedisConnPool::keep_alive() {		
		std::list<RedisConn*> need_keep_alive_pool;//需要保活的连接
		std::list<RedisConn*> succ_keep_alive_pool;//保活成功的连接
		std::list<RedisConn*> err_keep_alive_pool;//保活失败的连接
		
		//获取待保活列表
		get_keep_alive_conns(need_keep_alive_pool);
	
		//保活开始
		RedisConn* conn = NULL;
		while(!need_keep_alive_pool.empty()) {
			conn = need_keep_alive_pool.front();
	   		need_keep_alive_pool.pop_front();
			if (conn->keep_alive()) {
				succ_keep_alive_pool.push_back(conn);
			} else {
				err_keep_alive_pool.push_back(conn);
			}
		}

		// 保活成功的，加入到正式的工作队列中
		add_conns(succ_keep_alive_pool);
		succ_keep_alive_pool.clear();

		// 激活失败的，下次继续激活
		if (!err_keep_alive_pool.empty()) {
			//LOG_WARN("err keep alive count:%d", err_keep_alive_pool.size());
			add_keep_alive_conns(err_keep_alive_pool);
		}

		//LOG_INFO("keep alive succ:%d, error:%d", succ_keep_alive_pool.size(), err_keep_alive_pool.size());
	}

	void RedisConnPool::add_conns(std::list<RedisConn*>& add_conn_pool) {
		if (!add_conn_pool.empty()) {
			_conn_pool_mutex.lock();
   			const bool need_notify = _conn_pool.empty();
   			_conn_pool.insert(_conn_pool.begin(),
				add_conn_pool.begin(), add_conn_pool.end());
  			_conn_pool_mutex.unlock();
   			if (need_notify) {
	   			_conn_pool_cond.notify_all();
   			}
		}
	}

	void RedisConnPool::add_conn(RedisConn* conn) {
		_conn_pool_mutex.lock();
   		const bool need_notify = _conn_pool.empty();
   		_conn_pool.push_back(conn);
  		_conn_pool_mutex.unlock();
   		if (need_notify) {
			_conn_pool_cond.notify_all();
   		}
	}

	void RedisConnPool::add_keep_alive_conns(std::list<RedisConn*>& keep_alive_conn_pool) {
		if (!keep_alive_conn_pool.empty()) {
			_keep_alive_pool_mutex.lock();
   			_keep_alive_pool.insert(_keep_alive_pool.begin(),
				keep_alive_conn_pool.begin(), keep_alive_conn_pool.end());
  			_keep_alive_pool_mutex.unlock();
		}
	}

	void RedisConnPool::add_keep_alive_conn(RedisConn* conn) {
		_keep_alive_pool_mutex.lock();
		_keep_alive_pool.push_back(conn);
		_keep_alive_pool_mutex.unlock();
	}

	void RedisConnPool::get_keep_alive_conns(std::list<RedisConn*>& need_keep_alive_pool) {
		//遍历conn_pool 查找需要保活的连接
		_conn_pool_mutex.lock();


		std::list<RedisConn*>::iterator iter = _conn_pool.begin();
		while (iter != _conn_pool.end()){
			if ((*iter)->can_use() && RedisConn::CONN_STAT_CONNED == (*iter)->stat()) {
				++iter;
			} else {
				need_keep_alive_pool.push_back(*iter);
				iter = _conn_pool.erase(iter);
			}
		}
		_conn_pool_mutex.unlock();
		
		//待保活列表
		_keep_alive_pool_mutex.lock();
		if (!_keep_alive_pool.empty()) {
			need_keep_alive_pool.insert(need_keep_alive_pool.begin(), _keep_alive_pool.begin(), _keep_alive_pool.end());
			_keep_alive_pool.clear();
		}
		_keep_alive_pool_mutex.unlock();
	}
	int RedisConnPool::conn_count() const  {
		return _node_info.min_conn_count() * _adjust_conn_pool_count;
	}

	void RedisConnPool::timer_run() {
		// 保活
		keep_alive();

		// 动态扩容连接池
		if (_need_adjust_pool_conn_count) {
			adjust_pool_conn_count();
		}
	}

	void RedisConnPool::adjust_pool_conn_count() {
		_need_adjust_pool_conn_count = false;
		++_adjust_conn_pool_count;
		std::list<RedisConn*> conn_pool;
		std::list<RedisConn*> keep_alive_pool;
		
		for (int idx = 0; idx < _node_info.min_conn_count(); ++idx) {
			RedisConn* conn = new RedisConn(this);
			if (conn->init(_node_info)) {
				conn_pool.push_back(conn);
			} else {
				keep_alive_pool.push_back(conn);
			}
		}

		//可用队列
		_conn_pool_mutex.lock();
		_conn_pool.swap(conn_pool);
		_conn_pool_mutex.unlock();
		
		//待保活队列
		_keep_alive_pool_mutex.lock();
		_keep_alive_pool.swap(keep_alive_pool);
		_keep_alive_pool_mutex.unlock();

		std::cout << __FUNCTION__ << ":" << __LINE__ 
			<< ":" << _adjust_conn_pool_count 
			<< ":" << _node_info.min_conn_count()  
			<< ":" << _node_info.max_conn_count() << std::endl;
	}

}
