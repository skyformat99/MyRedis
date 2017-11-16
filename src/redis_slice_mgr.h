#ifndef _REDIS_SLICE_MGR_H_
#define _REDIS_SLICE_MGR_H_
#include <list>
#include <unordered_map>
#include "redis_node_info.h"

namespace MyRedis {
class RedisConn;
class RedisClient;
class RedisSlice;

class RedisSliceMgr {
public:
	RedisSliceMgr(RedisClient* client);
	bool init(const std::unordered_map<int, RedisNodeInfoList>& key_2_node_info);

	RedisConn* conn(int key, RedisNodeInfo::REDIS_ROLE role);
private:
	RedisClient* _client;
	std::unordered_map<int, RedisSlice*> _key_2_slice;
};
}
#endif //_REDIS_SLICE_MGR_H_