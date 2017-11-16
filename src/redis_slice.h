#ifndef _REDIS_SLICE_H_
#define _REDIS_SLICE_H_
#include <list>
#include <string>
#include "redis_node_info.h"

#include "redis_pool_mgr.h"

namespace MyRedis {
class RedisConn;
class RedisSliceMgr;

class RedisSlice {

public:
	RedisSlice(RedisSliceMgr *_mgr);

	bool init(const std::list<RedisNodeInfo>& node_info);

	RedisConn* conn(RedisNodeInfo::REDIS_ROLE role);

private:
	RedisSliceMgr *_slice_mgr;
	RedisPoolMgr _master_pool_mgr;
	RedisPoolMgr _slave_pool_mgr;
	std::string _key;
};

}

#endif //_REDIS_SLICE_H_
