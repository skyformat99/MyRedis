#include "redis_config.h"
//#include "my_logging.h"

namespace MyRedis {
RedisConfig::RedisConfig() {

}

bool RedisConfig::init(const std::string& json_str) {
	Json::Reader reader;
	Json::Value root;
    if (!reader.parse(json_str, root, false) || 
		!root["slice"].isArray()) {
		//LOG_FATAL("config formate error json:%s", json_str.c_str());
        return false;
    }
	
	Json::Value json_slice_arr = root["slice"];
	RedisNodeInfoList node_info_list;
	for (int idx = 0; idx < json_slice_arr.size(); ++idx) {
		node_info_list.clear();
		JSON_GET_OBJ_ARRAY(json_slice_arr, idx, node_info_list, RedisNodeInfo);
		if (node_info_list.empty()) {
			//LOG_WARN("config formate error json:%s", json_slice_arr[idx].toStyledString().c_str());
			continue;
		}
		if (_key_2_redis_list.find(node_info_list.begin()->key()) == _key_2_redis_list.end()) {
				_key_2_redis_list[node_info_list.begin()->key()] = node_info_list;
		} else {
			//LOG_FATAL("config redis slice repeated json:%s", json_str.c_str());
			return false;
		}
	}
	
	return true;
}

const std::unordered_map<int, RedisNodeInfoList>& RedisConfig::redis_config() const {
	return _key_2_redis_list;
}

}
