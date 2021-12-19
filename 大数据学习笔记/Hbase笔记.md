## Hbase架构图

![img](https://bkimg.cdn.bcebos.com/pic/b3b7d0a20cf431ad8b01445e4b36acaf2fdd9881?x-bce-process=image/watermark,image_d2F0ZXIvYmFpa2U4MA==,g_7,xp_5,yp_5/format,f_auto)



## Hbase是什么？

（1）K-V数据库

（2）BigTable

（3）rowKey

## Hbase数据模型

Table & Column Family

| **Row Key** | **Timestamp** | **Column Family** |
| ----------- | ------------- | ----------------- |
|             |               |                   |
|             |               |                   |
|             |               |                   |



## **Hbase命令**

使用hbase命令工具：/hbase shell

（1）create 'user', 'base_info' 

创建user表，设置列族base_info

（2）list 'user'

查看user的现状

（3）describe 'user'

查看user表的结构

（4）put 'user', 'gaoxugang', 'base_info:name', '高旭刚'

向user表中保存‘rowKey=gaoxugang，name=高旭刚’ 的数据

（5）get 'user','gaoxugang'

获取user中rowKey=gaoxugang的信息

（6）disable 'user',enable 'user'

disable 禁用user表，enable 启用user表

（7）drop 'user'  删除user表

（8）scan 'user'  扫秒user表所有的数据 

## Hbase系统表

hbase:meta

hbase:namespace

## 搭建本地伪分布式hbase环境

修改*hbase-site.xml* 配置文件如下：

```xml
<property>
  <name>hbase.cluster.distributed</name>
  <value>true</value>
</property>
```

配置hbase的hdfs地址，（数据将要存储到hdfs中）：

```xml
<property>
  <name>hbase.rootdir</name>
  <value>hdfs://localhost:8020/hbase</value>
</property>
```

注意：单机模式的hbase运行，所依赖的zookeeper、HMaster、HRegionServer都在同一个JVM进程中，数据也是存储在本地的/tmp/目录下面，在真实的生产环境中，数据肯定是存储在HDFS中