#include "MyRedis/redis_node_info.h"
#include "MyRedis/redis_conn.h"
#include "MyRedis/redis_config.h"
#include "MyRedis/redis_client.h"

#define NUM_THREADS 100

std::string json_str = "{\"slice\":[[{\"key\":0,\"role\":0,\"host\":\"127.0.0.1\",\"port\":6379,\"pwd\":\"\",\"conn_timeout\":10,\"min_conn_count\":2,\"max_conn_count\":10},{\"key\":0,\"role\":0,\"host\":\"127.0.0.1\",\"port\":6379,\"pwd\":\"\",\"conn_timeout\":10,\"min_conn_count\":2,\"max_conn_count\":10}],[{\"key\":1,\"role\":0,\"host\":\"127.0.0.1\",\"port\":6379,\"pwd\":\"\",\"conn_timeout\":10,\"min_conn_count\":2,\"max_conn_count\":10}]]}";
MyRedis::RedisClient client;

void test_conn() {
	MyRedis::RedisNodeInfo node_info;

	MyRedis::RedisConn redis(NULL);
	redis.init(node_info);
	redis.conn();

	char hkey[] = "123456";
	char hset[] = "hset";
	char key[] = "testkey";
	char hvalue[] = "3210";
	int argc = 4;
	char *argv[] = {hset,key,hkey,hvalue};
	size_t argvlen[] = {4,7,4,3};
	redis.redis_command_argv(argc, (const char**)argv, argvlen);
}

void test_config() {
	MyRedis::RedisConfig config;
	config.init("");
	std::cout << "cout:" << config._key_2_redis_list.size() << std::endl;;
}

void* test_set(void *t) {
	int idx = 0;
	do {
		if (client.set("test_"+MyRedis::int2str(idx), MyRedis::int2str(idx), -1, MyRedis::RedisClient::SET_MODE_NX)) {
			//std::cout << "set test_01 123 NE 100000 succ" << std::endl;
		} else {
			//std::cout << "set failed" << std::endl;
		}
		++idx;
		//break;
	} while(true);
	return NULL;
}

void* test_del(void *t) {
	int idx = 0;
	do {
		if (client.del("test_"+MyRedis::int2str(idx))) {
			//std::cout << "del succ" << std::endl;
		} else {
			//std::cout << "del failed" << std::endl;
		}

		++idx;
		//break;

	} while(true);
	return NULL;
}

void multi_thread_test_set() {
	// 定义线程的 id 变量，多个变量使用数组
	pthread_t tids[NUM_THREADS];
	for(int i = 0; i < NUM_THREADS; ++i) {
		//参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
		int ret = pthread_create(&tids[i], NULL, test_set, NULL);
		if (ret != 0) {
			std::cout << "pthread_create error: error_code=" << ret << std::endl;
		}
	}
}

void multi_thread_test_del() {
	pthread_t tids[NUM_THREADS];
	for(int i = 0; i < NUM_THREADS; ++i) {
		//参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
		int ret = pthread_create(&tids[i], NULL, test_del, NULL);
		if (ret != 0) {
			std::cout << "pthread_create error: error_code=" << ret << std::endl;
		}
	}
}

int main() {
	//test_config();

	if (!client.init(json_str)) {
		std::cout <<"cache init error" << std::endl;
		return 0;
	}
	//单线程测试
	//test_set(NULL);
	//test_del(NULL);
	//多线程测试
	multi_thread_test_set();
	multi_thread_test_del();
	//等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
	pthread_exit(NULL);
	return 0;
}
