#### hadoop环境搭建

（1）下载hadoop软件包并且上传至linux服务器的目录

（2）解压hadoop软件文件

（3）设置JAVA_HOME环境变量，比如hadoop安装在/opt/hadoop-2.10.1/  目录下面，则修改  /opt/hadoop-2.10.1/etc/hadoop/ 目录下的hadoop-env.sh 文件，添加export JAVA_HOME=/usr/java/jdk1.8.0_91 配置

（4）默认情况下，hadoop使用非分布式的方式配置，使用非分布式的方式启动

（5）第一个标准的运行实例，使用bin目录下面的hadoop命令

```shell
// 第一步：创建一个输入目录
mkdir /opt/hadoop-2.10.1/gaoxugang_test/input

// 第二步：将需要计算的文件放入输入目录
cp etc/hadoop/*.xml input

// 第三步：切换至hadoop安装目录bin,运行hadoop命令
// 以下命令的大致意思是：使用hadoop的mapreduce计算组件查找出/gaoxugang_test/input目录下面匹配dfs
// 开头的文件

./hadoop jar ../share/hadoop/mapreduce/hadoop-mapreduce-examples-2.10.1.jar grep ../gaoxugang_test/input ../gaoxugang_test/out  'dfs[a-z.]+'

// ../gaoxugang_test/input是hadoop命令执行的输入参数
// ../gaoxugang_test/out是hadoop命令执行的输出参数
```

#### hadoop伪分布式环境搭建

第1步：设置host文件 vi/etc/hosts,添加如下：

```shell
0.0.0.0     hadoop-domain
```

目的是为了 dfs的端口能通过其他机器访问



第2步：修改hadoop配置文件etc/hadoop/core-site.xml:

```xml
<configuration>
    <property>
        <name>fs.defaultFS</name>
        <value>hdfs://hadoop-domain:9000</value>
    </property>
</configuration>
```

第2步：修改hadoop配置文件etc/hadoop/hdfs-site.xml:

```xml
<configuration>
    <property>
        <name>dfs.replication</name>
        <value>1</value>
    </property>
</configuration>
```

第3步：执行命令 hdfs namenode

```shell
bin/hdfs namenode -format
```

第4步：执行命令 启动hdfs

```shell
sbin/start-dfs.sh
```

第5步，访问

http://192.168.202.129:50070/dfshealth.html#tab-overview

控制台设置餐口文档  https://blog.csdn.net/u011331844/article/details/103917480

File /hbase/.tmp/hbase.version could only be replicated to 0 nodes instead of minReplication (=1).  There are 1 datanode(s) running and 1 node(s) are excluded in this operation.