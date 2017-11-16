#ifndef _REDIS_CLIENT_H_
#define _REDIS_CLIENT_H_
#include "redis_config.h"
#include "redis_conn.h"
#include "redis_slice_mgr.h"

namespace MyRedis {
class RedisClient {

public:
	RedisClient();
	bool init(const std::string& json_str);

/**********************common**********************/
public:
	bool del(const std::string& key);
	
private:			
/*释放连接到连接池*/
static void free_reply(const redisReply *reply);
/*参数解析*/
static bool check_reply(const redisReply *reply);

/**********************string**********************/
public:
	enum SET_MODE {
		SET_MODE_NORMAL,
		SET_MODE_NX,
		SET_MODE_XX,
	};
	bool set(const std::string& key, const std::string& value,
		int timeout = -1, SET_MODE mode = SET_MODE_NORMAL);
	

private:
	RedisConn* conn(int key, RedisNodeInfo::REDIS_ROLE);

	
private:
	RedisConfig _config;
	RedisSliceMgr _slice_mgr;			
};

}

#endif //_REDIS_CLIENT_H_