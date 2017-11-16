#include "redis_slice.h"
#include "my_logging.h"

namespace MyRedis {

RedisSlice::RedisSlice(RedisSliceMgr* mgr) : _slice_mgr(mgr),
	_master_pool_mgr(this),
	_slave_pool_mgr(this) {

}

bool RedisSlice::init(const std::list<RedisNodeInfo>& node_info_list) {
	std::list<RedisNodeInfo> master_node_list;
	std::list<RedisNodeInfo> slave_node_list;
	for (RedisNodeInfo node_info : node_info_list) {
		if (RedisNodeInfo::REDIS_ROLE_MASTER == node_info.role() ) {
			master_node_list.push_back(node_info);
			LOG_INFO("redis info master redis:%s", node_info.to_json().c_str());
		} else {
			slave_node_list.push_back(node_info);
			LOG_INFO("redis info master redis:%s", node_info.to_json().c_str());
		}
	}

	if (!_master_pool_mgr.init(master_node_list)) {
		return false;
	}
	if (!_slave_pool_mgr.init(slave_node_list)) {
		return false;
	}

	return true;
}

RedisConn* RedisSlice::conn(RedisNodeInfo::REDIS_ROLE role) {
	if (RedisNodeInfo::REDIS_ROLE_MASTER == role) {
		return _master_pool_mgr.conn();
	} else if (RedisNodeInfo::REDIS_ROLE_SLAVE == role){
		return _slave_pool_mgr.conn();
	} else {
		LOG_ERROR("role error role:%d\n", role);
		return NULL;
	}
}


}
