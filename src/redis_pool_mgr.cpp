#include "redis_pool_mgr.h"
#include "redis_conn_pool.h"
//#include "my_logging.h"

namespace MyRedis {

RedisPoolMgr::RedisPoolMgr(RedisSlice* slice) : _slice(slice) {

}

bool RedisPoolMgr::init(const std::list<RedisNodeInfo>& node_list) {
	for (RedisNodeInfo node_info : node_list){  
          RedisConnPool* pool = new RedisConnPool(this);
		  if (!pool->init(node_info)) {
		  		return false;
		  }
		  _ip_2_pool[node_info.host()] = pool;
		  _pool_vec.push_back(pool);
    }

	for (std::pair<std::string, RedisConnPool*> key_value: _ip_2_pool) {
		//LOG_INFO("pool info ip:%s, conn_count:%d", key_value.first.c_str(), key_value.second->conn_count());
	}

	return true;
}

RedisConn* RedisPoolMgr::conn() {
	if (_pool_vec.size() <= 0) {
		//LOG_ERROR("pool empty func:%s, line:%d\n", __FUNCTION__, __LINE__);
		return NULL;
	}
	int idx = get_rand_with_left(0, _pool_vec.size());
	std::cout << "pool_vec_size:" << _pool_vec.size() << ",idx" << idx << std::endl;
	return _pool_vec[idx]->conn();
}


}
