#ifndef _REDIS_CONFIG_H_
#define _REDIS_CONFIG_H_
#include "my_json.h"
#include <list>
#include <unordered_map>
#include "redis_node_info.h"

namespace MyRedis {

class RedisConfig {

public:
	RedisConfig();
	bool init(const std::string& json_str);
	const std::unordered_map<int, RedisNodeInfoList>& redis_config() const;

public:
	std::unordered_map<int, RedisNodeInfoList> _key_2_redis_list;
};

}


#endif //_REDIS_CONFIG_H_