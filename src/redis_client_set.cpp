#include "redis_client.h"
#include "redis_conn.h"
#include "common.h"

namespace MyRedis {
	bool RedisClient::set(const std::string& key, const std::string& value,
 		int timeout /*= -1*/, SET_MODE set_mode/* = SET_MODE_NORMAL*/) {
		
		MyRedis::RedisConn* conn = _slice_mgr.conn(hash_ap(key), MyRedis::RedisNodeInfo::REDIS_ROLE_MASTER);
		if (!conn) {
			return false;
		}
	
		redisReply* reply = NULL;
		if (timeout > 0) {
			if (SET_MODE_NX == set_mode) {
				reply = static_cast<redisReply*>(conn->redis_command("SET %s %s EX %d NX",
					key.c_str(), value.c_str(), timeout));
			} else if (SET_MODE_XX == set_mode) {
				reply = static_cast<redisReply*>(conn->redis_command("SET %s %s EX %d XX",
					key.c_str(), value.c_str(), timeout));
			} else {
				reply = static_cast<redisReply*>(conn->redis_command("SET %s %s EX %d",
					key.c_str(), value.c_str(), timeout));
			}
		} else {
			if (SET_MODE_NX == set_mode) {
				reply = static_cast<redisReply*>(conn->redis_command("SET %s %s NX",
					key.c_str(), value.c_str()));
			} else if (SET_MODE_XX == set_mode) {
				reply = static_cast<redisReply*>(conn->redis_command("SET %s %s XX",
					key.c_str(), value.c_str()));
			} else {
				reply = static_cast<redisReply*>(conn->redis_command("SET %s %s",
					key.c_str(), value.c_str()));
			}
		}
	
		conn->free_self();

		bool ret = false;
		if (check_reply(reply)) {
			if (REDIS_REPLY_STATUS == reply->type) {
				ret = true;
			} else {
				ret = (reply->integer == 1) ? true : false;
			}
		}
	
		if (NULL != reply) {
			free_reply(reply);
		}
	
		return ret;
	}
}
