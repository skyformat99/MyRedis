#include "redis_slice_mgr.h"
#include "redis_slice.h"
//#include "my_logging.h"

namespace MyRedis {

RedisSliceMgr::RedisSliceMgr(RedisClient *client) : _client(client) {

}

bool RedisSliceMgr::init(const std::unordered_map<int, RedisNodeInfoList>& key_2_node_info) {
	for (std::pair<int, RedisNodeInfoList> key_value : key_2_node_info) {
		RedisSlice* slice = new RedisSlice(this);
		if (!slice->init(key_value.second)) {
			return false;
		}

		_key_2_slice[key_value.first] = slice;
	}

	return true;
}

RedisConn* RedisSliceMgr::conn(int key, RedisNodeInfo::REDIS_ROLE role) {
	if (_key_2_slice.size() <= 0) {
		//LOG_FATAL("slice is empty func:%s, line:%d, key:%d\n", __FUNCTION__, __LINE__, key);
		return NULL;
	}
	std::cout << "key:" << key << ",slice_size:" << _key_2_slice.size() << std::endl;
	return _key_2_slice[key%_key_2_slice.size()]->conn(role);
	//return _key_2_slice[0]->conn(role);
}


}
