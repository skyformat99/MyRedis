#ifndef _REDIS_CONN_H_
#define _REDIS_CONN_H_
#include <hiredis/hiredis.h>
#include "redis_node_info.h"
#include "common.h"

namespace MyRedis {

class RedisConnPool;

class RedisConn {
public:
	const int default_conn_timeout = 1;//1s
	const int default_keep_alive_timeout = 20; //20s
public:
	enum CONN_STAT { 
		CONN_STAT_UNCONN ,
		CONN_STAT_CONNED ,
		CONN_STAT_ERROR ,
	};

public:
	RedisConn(RedisConnPool* pool);
	bool init(const RedisNodeInfo & node_info);
	bool ping();
	bool conn();
	bool reconn();
	bool auth();
	bool can_use();
	bool keep_alive();
	void free_self();/*释放连接到连接池*/

	/*成员*/
	CONN_STAT stat();
	long last_stamp();
	void set_stat(CONN_STAT stat);
	void set_last_stamp(long stamp);
	
	/**发送命令请求*/
	redisReply* redis_command(const char *format, ...);
	redisReply* redis_command_argv(int argc, const char **argv, const size_t *argvlen);
	int redis_append_command(const char *format, ...);
	int redis_append_command_argv(int argc, const char **argv, const size_t *argvlen);

private:
	void redis_free();//关闭redis连接
	
private:
	RedisConnPool *_pool;
	RedisNodeInfo _node_info;
	redisContext *_ctx;
	CONN_STAT _stat;
	long _last_active_stamp;
};

}

#endif