#ifndef _MY_JSON_H_
#define _MY_JSON_H_
#include <json/json.h>
#include <iostream>

namespace MyRedis {

#define JSON_GET_STRING(JSON_OBJ, FIELD_NAME, VAR_NAME) \
do { \
	if (JSON_OBJ[FIELD_NAME].isString()) {\
		VAR_NAME=JSON_OBJ[FIELD_NAME].asString();\
 	} else {\
		VAR_NAME="";\
	}\
} while(0);

#define JSON_GET_INT(JSON_OBJ, FIELD_NAME, VAR_NAME) \
do {\
	VAR_NAME=(JSON_OBJ[FIELD_NAME].isInt() ? JSON_OBJ[FIELD_NAME].asInt() : 0);\
} while(0);

#define JSON_GET_OBJ_ARRAY(JSON, FIELD_NAME, VAR_NAME, TYPE) \
do {\
	if (JSON[FIELD_NAME].isArray()){\
        Json::Value JSON_ARRAY = JSON[FIELD_NAME];\
		for(int idx = 0; idx < JSON_ARRAY.size(); ++idx) {\
			if(!JSON_ARRAY[idx].isObject()){continue;}\
            Json::Value JSON_OBJ = JSON_ARRAY[idx]; \
            TYPE info; \
			info.from_json(JSON_OBJ);\
			VAR_NAME.push_back(info);\
		}\
	} \
} while (0);

#define JSON_GET_STRING_ARRAY(JSON, FIELD_NAME, VAR_NAME) \
do {\
	if (JSON[FIELD_NAME].isArray()){\
        Json::Value JSON_ARRAY = JSON[FIELD_NAME].asArray();\
		for(int idx = 0; idx < JSON_ARRAY.size(); ++idx) {\
			if(!JSON_ARRAY[idx].isString()){continue;}\
			VAR_NAME.push_back(JSON_ARRAY[idx].toString());\
		}\
	}\
} while (0);

#define JSON_GET_OBJ(JSON, FIELD_NAME, VAR_NAME) \
do { \
	if (JSON[FIELD_NAME].isObject()){\
		VAR_NAME.from_json(JSON[FIELD_NAME]);\
	}\
} while(0); 

#define JSON_GET_BOOL(JSON, FIELD_NAME, VAR_NAME) \
do {\
	if (JSON[FIELD_NAME].isBool()){\
		VAR_NAME = JSON[FIELD_NAME].asBool();\
	}\
} while (0);	

}
#endif // _MY_JSON_H_
