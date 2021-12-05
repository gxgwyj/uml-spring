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

## 搭建本地伪分布式hbase环境