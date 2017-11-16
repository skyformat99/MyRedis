#include "redis_client.h"
namespace MyRedis {

bool RedisClient::del(const std::string& key) {
	bool ret = false;
	RedisConn* conn = _slice_mgr.conn(hash_ap(key), RedisNodeInfo::REDIS_ROLE_MASTER);
	if (NULL == conn) {
		return false;
	}
	redisReply* reply = static_cast<redisReply*>(conn->redis_command("DEL %s", key.c_str()));
	conn->free_self();
	
	if (check_reply(reply)) {
		if (REDIS_REPLY_ERROR != reply->type) {
            ret = true;
        }
	}

	if (NULL != reply) {
        free_reply(reply);
    }

	return ret;
}

/*释放资源*/
void RedisClient::free_reply(const redisReply *reply){
    if (NULL != reply) {
        freeReplyObject((void*)reply);
    }
}

/*参数检测*/
 bool RedisClient::check_reply(const redisReply *reply){
    if (NULL == reply) {
        return false;
    }

    switch (reply->type){
    case REDIS_REPLY_STRING:{
        return true;
    }
    case REDIS_REPLY_ARRAY:{
        return true;
    }
    case REDIS_REPLY_INTEGER:{
        return true;
    }
    case REDIS_REPLY_NIL:{
        return false;
    }
    case REDIS_REPLY_STATUS:{
        return true;
    }
    case REDIS_REPLY_ERROR:{
        return false;
    }
    default:{
        return false;
    }
    }

    return false;
}

}
