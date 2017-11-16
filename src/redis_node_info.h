#ifndef _REDIS_NODE_INFO_H_
#define _REDIS_NODE_INFO_H_
#include <iostream>
#include <list>
#include "my_json.h"

namespace MyRedis {

class RedisNodeInfo {
public:
	enum REDIS_ROLE {
		REDIS_ROLE_MASTER,
		REDIS_ROLE_SLAVE,
	};
public:
	RedisNodeInfo();

	bool from_json(const Json::Value& json_obj);
	std::string to_json();
	void set_host(const std::string& host);
	void set_port(int port);
	void set_password(const std::string& pwd);
	void set_timeout(int timeout);
	void set_min_conn_count(int min_count);
	void set_max_conn_count(int max_count);
	void set_role(int role);
	void set_key(int key);

	const std::string& host()const;
	int port()const;
	const std::string& password()const;
	int timeout()const;
	int min_conn_count()const;
	int max_conn_count()const;
	REDIS_ROLE role() const;
	int key() const;
	
private:
	std::string _host;
	int _port;
	std::string _password;
	int _timeout;
	int _role;
	int _min_conn_count;
	int _max_conn_count;
	int _key;
};

typedef std::list<RedisNodeInfo> RedisNodeInfoList;

}

#endif //_REDIS_NODE_INFO_H_