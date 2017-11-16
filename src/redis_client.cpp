#include "redis_client.h"
#include "common.h"

namespace MyRedis {
RedisClient::RedisClient() : _slice_mgr(this) {
}

bool RedisClient::init(const std :: string & json_conf_str) {
	if (!_config.init(json_conf_str)) {
		return false;
	}

	if (!_slice_mgr.init(_config.redis_config())) {
		return false;
	}
	return true;
}

RedisConn* RedisClient::conn(int key, RedisNodeInfo::REDIS_ROLE role) {
	return _slice_mgr.conn(key, role);
}


}
