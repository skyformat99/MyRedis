#ifndef _REDIS_CONN_POOL_H_
#define _REDIS_CONN_POOL_H_
#include "redis_conn.h"
#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "my_timer.h"

namespace MyRedis {

class RedisPoolMgr;

class RedisConnPool {
public:
	const int g_default_keep_alive_interval = 10 * 1000; //毫秒
		
public:
	RedisConnPool(RedisPoolMgr* pool_mgr);
	~RedisConnPool();
	bool init(const RedisNodeInfo& node_info);

	//从连接池中获取一个连接
	RedisConn* conn();
	//释放连接到连接池
	void free_conn(RedisConn* conn);
	//保活
	void keep_alive();
	//获取连接数量
	int conn_count() const ;

private:
	//定时执行函数
	void timer_run();
	//动态扩容连接池中连接数量
	void adjust_pool_conn_count();
	
	//获取待保活队列
	void get_keep_alive_conns(std::list<RedisConn*>& need_keep_alive_pool);
	//添加链接到保活池中
	void add_keep_alive_conns(std::list<RedisConn*>& need_keep_alive_pool);
	void add_keep_alive_conn(RedisConn*);
	//添加链接到工作池中
	void add_conns(std::list<RedisConn*>& add_conn_pool);
	void add_conn(RedisConn*);
	
private:
	//工作连接池
	std::mutex _conn_pool_mutex;
   	std::condition_variable _conn_pool_cond;
	std::list<RedisConn*> _conn_pool;
	//保活连接池
	std::mutex _keep_alive_pool_mutex;
	std::list<RedisConn*> _keep_alive_pool;
	
	RedisPoolMgr* _pool_mgr; // 连接池管理类
	MyTimer _keep_alive_timer; // 定时器
	RedisNodeInfo _node_info; //redis信息
	bool _need_adjust_pool_conn_count; //是否需要扩容连接池
	int _adjust_conn_pool_count; //调整连接池大小的次数，每次增加最小值一倍
};

}
#endif