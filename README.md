# MyRedis
## 特点
* 支持集群分片存储，跟具配置文件自动对key进行hash分片
* 支持主从模式，可实现读写分离
* 支持连接池，可依据配置实现连接池自动扩容
* 依赖hiredis库和jsoncpp库，提供和hiredis等同的接口操作
* 线程安全
* 对于部分常用接口实现封装
## 不足
* 封装的操作接口较少，需要用户根据需求自行封装。（有兴趣的朋友可以一起开发）

## 依赖
* 无日志版本只依赖hiredis，使用前先安装hiredis，然后修改CMakeList.txt中的依赖路径即可
* 有日志版本额外以来boost_log

## 安装
### 安装依赖
* hiredis 安装路径
> /opt/hiredis/
├── include
│   └── hiredis
│       ├── adapters
│       │   ├── ae.h
│       │   ├── glib.h
│       │   ├── ivykis.h
│       │   ├── libevent.h
│       │   ├── libev.h
│       │   ├── libuv.h
│       │   ├── macosx.h
│       │   └── qt.h
│       ├── async.h
│       ├── hiredis.h
│       ├── read.h
│       └── sds.h
├── lib
│   ├── libhiredis.a
│   ├── libhiredis.so -> libhiredis.so.0.13
│   └── libhiredis.so.0.13
└── pkgconfig
    └── hiredis.pc


* jsoncpp安装路径
>/opt/jsoncpp-0.10.6/
├── include
│   ├── CMakeLists.txt
│   └── json
│       ├── assertions.h
│       ├── autolink.h
│       ├── config.h
│       ├── features.h
│       ├── forwards.h
│       ├── json.h
│       ├── reader.h
│       ├── value.h
│       ├── version.h
│       └── writer.h
└── lib
    ├── libjsoncpp.a
    └── libjsoncpp.so

### 安装MyRedis
> git clone https://github.com/flywe/MyRedis.git
> cd MyRedis
> mkdir bulid
> cd build
> cmake ..
> make

### 配置文件
redis配置支持分片，集群，主从，需要写好配置文件即可。
如下配置：数据被分片存储到两个redis分片（key=0/1）中，每一个redis分片包含一主一从。
```
{
    "slice":[  //redis分片
        [//分片0
            {
                "key":0,//根据hash进行分片key
                "role":0,//该redis角色 0-->master, 1--->slave
                "host":"127.0.0.1", 
                "port":6379,
                "pwd":"",
                "conn_timeout":10, //连接超时时间秒
                "min_conn_count":20,//redis连接池初始大小
                "max_conn_count":100//redis动态增加的最大连接数
            },
            {
                "key":0,
                "role":1,
                "host":"127.0.0.1",
                "port":6379,
                "pwd":"",
                "conn_timeout":10,
                "min_conn_count":20,
                "max_conn_count":100
            }
        ],
        [//分片1
            {
                "key":1,
                "role":0,
                "host":"127.0.0.1",
                "port":6379,
                "pwd":"",
                "conn_timeout":10,
                "min_conn_count":20,
                "max_conn_count":100
            },
            {
                "key":1,
                "role":1,
                "host":"127.0.0.1",
                "port":6379,
                "pwd":"",
                "conn_timeout":10,
                "min_conn_count":20,
                "max_conn_count":100
            }
        ]
    ]
}
```

## 分布图
RedisClient通过连接池与Redis集群进行交互，底层分片对用户透明设计
![enter description here][1]


  [1]: ./images/1510807236523.jpg
  

## 共同进步

作者：flywe
群名称：MyRedis技术交流
群   号：126835938