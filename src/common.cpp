#include "common.h"
#include <sstream>


namespace MyRedis {

long get_time_stamp() {  
    return time(NULL);
}  

std::tm* gettm(long timestamp) {  
	long milli = timestamp+ (long)8*60*60*1000;//此处转化为东八区北京时间，如果是其它时区需要按需求修改  
	auto mTime = std::chrono::milliseconds(milli);	
	auto tp=std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds>(mTime);  
	auto tt = std::chrono::system_clock::to_time_t(tp);  
	std::tm* now = std::gmtime(&tt);  
	printf("%4d年%02d月%02d日 %02d:%02d:%02d\n",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);	
   	return now;	
}  


int get_rand_with_left(int min, int max) {
	//产生一定范围随机数的通用表示公式 
	//要取得[a,b)的随机整数，使用(rand() % (b-a))+ a; 
	//要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a; 
	//要取得(a,b]的随机整数，使用(rand() % (b-a))+ a + 1; 
	if (max <= 1) {
		return 0;
	}
	srand((unsigned)time(NULL));  
	return (rand() % (max-min+1))+ min;
}


void str2int(int &int_temp,const std::string &string_temp) {  
	std::stringstream stream(string_temp);  
    stream>>int_temp; 					
}  

std::string int2str(int int_tem) {  
	std::stringstream stream;  
    stream << int_tem;  
    return stream.str();
}

// AP Hash Function
int hash_ap(const std::string& src_str) {
	const char* str = src_str.c_str();
    unsigned int hash = 0;
    int i;
    for (i=0; *str; i++) {
        if ((i&  1) == 0) {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }
    return (hash&  0x7FFFFFFF);
}


}
