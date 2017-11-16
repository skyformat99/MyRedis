#ifndef _REDIS_POOL_MGR_H_
#define _REDIS_POOL_MGR_H_
#include <unordered_map>
#include <list>
#include "redis_node_info.h"

namespace MyRedis {
class RedisConnPool;
class RedisSlice;
class RedisConn;
class RedisPoolMgr {

public:
	RedisPoolMgr(RedisSlice* slice);
	bool init(const std::list<RedisNodeInfo>& node_list);
	RedisConn* conn();
	
private:
	RedisSlice* _slice;
	std::unordered_map<std::string, RedisConnPool*> _ip_2_pool;
	std::vector<RedisConnPool*> _pool_vec;
};

}
#endif // _REDIS_POOL_MGR_H_