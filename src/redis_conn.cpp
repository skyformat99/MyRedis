#include "redis_conn.h"
#include "redis_conn_pool.h"
#include <hiredis/hiredis.h>
#include "my_logging.h"

namespace MyRedis {
RedisConn::RedisConn(RedisConnPool * pool) : _pool(pool),
	_stat(CONN_STAT_UNCONN),
	_last_active_stamp(0) {
	
}

bool RedisConn::init(const RedisNodeInfo & node_info) {
	_node_info = node_info;
	return conn();
}

bool RedisConn::keep_alive() {
	if (ping()) {
		return true;
	} else {
		return reconn();
	}
}

void RedisConn::free_self() {
	_pool->free_conn(this);
}

bool RedisConn::ping() {
	redisReply *reply = static_cast<redisReply *>(redisCommand(_ctx, "PING"));
	if (NULL == reply) {
		set_stat(CONN_STAT_ERROR);
		LOG_WARN("ping error host:%s, port:%d, pwd:",
			_node_info.host().c_str(), _node_info.port(), _node_info.password().c_str());
		return false;
	} else {
		set_stat(CONN_STAT_CONNED);
		set_last_stamp(get_time_stamp());
		freeReplyObject(reply);
		return true;
	}
}

bool RedisConn::conn() {
	if (_node_info.timeout() > 0) {
		struct timeval timeoutVal;
		timeoutVal.tv_sec = _node_info.timeout();
		timeoutVal.tv_usec = 0;
		_ctx = redisConnectWithTimeout(_node_info.host().c_str(), _node_info.port(), timeoutVal);
	} else {
		_ctx = redisConnect(_node_info.host().c_str(), _node_info.port());
	}
	
	if (_ctx == NULL || _ctx->err) {
		set_stat(CONN_STAT_ERROR);
		if (_ctx) {
			LOG_ERROR("conn error err:%s, host:%s, port:%d, pwd:",
				_ctx->errstr, _node_info.host().c_str(), _node_info.port(), _node_info.password().c_str());
		} else {
			std::cout << "Cant allocate redis context" << std::endl;
			LOG_ERROR("conn error err:Cant allocate redis context, host:%s, port:%d, pwd:",
				_node_info.host().c_str(), _node_info.port(), _node_info.password().c_str());
		}

		return false;
	}

	set_last_stamp(get_time_stamp());
	set_stat(CONN_STAT_CONNED);
	return true;
}

bool RedisConn::reconn() {
	if (NULL != _ctx) {
		redisFree(_ctx);
	}
	
	return conn();
}
	
bool RedisConn::auth() {
	/*bool ret = false;
	if (_node_info._password.empty()) {
		ret = true;
	} else {
		redisReply *reply = static_cast<redisReply*>(redisCommand(_ctx, "AUTH %s", _redis_node._password.c_str()));
		if ((NULL == reply) || (strcasecmp(reply->str, "OK") != 0)) {
			std::cout << __FUNCTION__ << ", error" << std::endl;
			ret = false;
		} else {
			ret = true;	
		}
		freeReplyObject(reply);
	}

	return ret;*/
	return true;
}

bool RedisConn::can_use() {
	if (get_time_stamp() - last_stamp() > _node_info.timeout()) {
		return false;
	}

	return true;
}

void RedisConn::set_stat(CONN_STAT stat) {
	_stat = stat;
}

void RedisConn::set_last_stamp(long stamp) {
	_last_active_stamp = stamp;
}

RedisConn::CONN_STAT RedisConn::stat() {
	return _stat;
}

long RedisConn::last_stamp() {
	return _last_active_stamp;
}

/**发送命令请求*/
redisReply* RedisConn::redis_command( const char *format, ...) {
	set_last_stamp(get_time_stamp());
	va_list ap;
    va_start(ap,format);
    redisReply* reply = static_cast<redisReply *>(redisvCommand(_ctx,format,ap));
    va_end(ap);
    return reply;
}
redisReply* RedisConn::redis_command_argv(int argc, const char **argv, const size_t *argvlen) {
	set_last_stamp(get_time_stamp());
	return static_cast<redisReply *>(redisCommandArgv(_ctx, argc, argv, argvlen));
}

int RedisConn::redis_append_command(const char *format, ...) {
	set_last_stamp(get_time_stamp());

	va_list ap;
	int ret;

	va_start(ap,format);
	ret = redisvAppendCommand(_ctx, format, ap);
	va_end(ap);
	return ret;
}
int RedisConn::redis_append_command_argv(int argc, const char **argv, const size_t *argvlen) {
	set_last_stamp(get_time_stamp());

	return redisAppendCommandArgv(_ctx, argc, argv, argvlen);
}

/*释放资源*/
void RedisConn::redis_free() {
	if (NULL != _ctx) {
		redisFree(_ctx);
		_ctx = NULL;
		set_stat(CONN_STAT_UNCONN);
	}
}

}


