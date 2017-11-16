#include "redis_node_info.h"
namespace MyRedis {
	RedisNodeInfo::RedisNodeInfo() :
		_host("127.0.0.1"),
		_port(6379),
		_timeout(10),
		_role(REDIS_ROLE_MASTER) {

	}

		
	bool RedisNodeInfo::from_json(const Json::Value& json_obj) {
		JSON_GET_INT(json_obj, "key", _key);
		JSON_GET_STRING(json_obj, "host", _host);
		JSON_GET_STRING(json_obj, "pwd", _password);
		JSON_GET_INT(json_obj, "port", _port);
		JSON_GET_INT(json_obj, "conn_timeout", _timeout);
		JSON_GET_INT(json_obj, "role", _role);
		JSON_GET_INT(json_obj, "max_conn_count", _max_conn_count);
		JSON_GET_INT(json_obj, "min_conn_count", _min_conn_count);
		
		return true;
	}

	std::string RedisNodeInfo::to_json() {
		Json::Value root;
    	root["key"] = _key;
		root["host"] = _host;
		root["pwd"] = _password;
		root["port"] = _port;
		root["conn_timeout"] = _timeout;
		root["role"] = _role;
		root["max_conn_count"] = _max_conn_count;
		root["min_conn_count"] = _min_conn_count;

   		return root.toStyledString();
	}

	void RedisNodeInfo::set_host(const std::string& host) {
		_host = host;
	}
	void RedisNodeInfo::set_port(int port) {
		_port = port;
	}
	void RedisNodeInfo::set_password(const std::string& pwd) {
		_password = pwd;
	}
	void RedisNodeInfo::set_timeout(int timeout) {
		_timeout = timeout;
	}

	void RedisNodeInfo::set_min_conn_count(int min_count) {
		_min_conn_count = min_count;
	}

	void RedisNodeInfo::set_max_conn_count(int max_count) {
			_max_conn_count = max_count;
	}

	void RedisNodeInfo::set_role(int role) {
		_role = (REDIS_ROLE)role;	
	}

	void RedisNodeInfo::set_key(int key) {
		_key = key;
	}
	const std::string& RedisNodeInfo::host()const {
		return _host;
	}
	int RedisNodeInfo::port()const {
		return _port;
	}
	const std::string& RedisNodeInfo::password()const {
		return _password;
	}

	int RedisNodeInfo::timeout()const {
		return _timeout;
	}

	int RedisNodeInfo::min_conn_count()      const {
		return _min_conn_count;
	}

	int RedisNodeInfo::max_conn_count() const{
		return _max_conn_count;
	}

	RedisNodeInfo::REDIS_ROLE RedisNodeInfo::role() const {
		return (REDIS_ROLE)_role;
	}

	int RedisNodeInfo::key() const {
		return _key;
	}
}
