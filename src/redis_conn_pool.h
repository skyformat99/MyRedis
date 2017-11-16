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
	enum RedisRole {
		role_master = 0,
		role_slave  = 1,
		role_unknow = 2,
	};
		
public:
	RedisConnPool(RedisPoolMgr* pool_mgr);
	~RedisConnPool();
	bool init(const RedisNodeInfo& node_info);
	RedisConn* conn();
	void free_conn(RedisConn* conn);
	void keep_alive();
	int conn_count() const ;

private:
	//获取待保活队列
	void get_keep_alive_conns(std::list<RedisConn*>& need_keep_alive_pool);
	void add_keep_alive_conns(std::list<RedisConn*>& need_keep_alive_pool);
	void add_keep_alive_conn(RedisConn*);
	void add_conns(std::list<RedisConn*>& add_conn_pool);
	void add_conn(RedisConn*);
	
private:
	std::mutex _conn_pool_mutex;
   	std::condition_variable _conn_pool_cond;
	std::list<RedisConn*> _conn_pool;

	std::mutex _keep_alive_pool_mutex;
	std::list<RedisConn*> _keep_alive_pool;
	
	RedisRole _role;
	RedisPoolMgr* _pool_mgr;

	MyTimer _keep_alive_timer;

	RedisNodeInfo _node_info;
};

}
#endif