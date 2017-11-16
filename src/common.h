#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <string>
#include <ctime>
#include <chrono>

namespace MyRedis {	

#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6

	long get_time_stamp();
	std::tm* gettm(long timestamp);

	int get_rand_with_left(int min, int max);

	void str2int(int &int_temp,const std::string &string_temp) ;
	std::string int2str(int int_tem);  

	//哈希函数
	int hash_ap(const std::string& src_str);
}

#endif //_COMMON_H_