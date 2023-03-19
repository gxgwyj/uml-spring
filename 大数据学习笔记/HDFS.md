#### hdfs环境搭建

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

#### hdfs伪分布式环境搭建



第2步：修改hadoop配置文件etc/hadoop/core-site.xml:

```xml
<configuration>
    <property>
        <!-- NameNode URI -->
        <name>fs.defaultFS</name>
        <value>hdfs://192.168.202.129:9000</value>
        <!-- 此处一定要写成ip地址 -->
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
    <property>
        <name>dfs.namenode.name.dir</name>
        <value>file:/opt/hadoop-2.10.1/data/namenode</value>
    </property>
    <property>
        <name>dfs.datanode.data.dir</name>
        <value>file:/opt/hadoop-2.10.1/data/datanode</value>
    </property>
   <property>
     <name>dfs.permissions.enabled</name>
     <value>false</value>
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

控制台设置参考文档  https://blog.csdn.net/u011331844/article/details/103917480

File /hbase/.tmp/hbase.version could only be replicated to 0 nodes instead of minReplication (=1).  There are 1 datanode(s) running and 1 node(s) are excluded in this operation.

#### HDFS介绍

分布式的文件系统，HDFS提供对应用程序数据的高吞吐量访问，适用于拥有大型数据集的应用程序，如大量数据的批处理。

**特点：**

- 检测硬件故障并且快速恢复（软件系统中，硬件故障属于常态无法完全避免，系统架构设计的时候必须考虑到这一点）。
- 对数据的高吞吐量的访问以及批处理。
- 大量数据集的存储。
- 保证数据一致性。
- 计算比数据移动更加高效。
- 跨平台移植。



#### HDFS架构

https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsDesign.html

主从架构，HDFS集群由单个**NameNode**组成，主节点管理文件系统命名空间并调节客户端对文件的访问。还有许多**DataNode**（数据节点），管理连接到它们运行的节点上的存储。HDFS公开了一个文件系统命名空间，允许将用户数据存储在文件中。在内部，**文件被分割成一个或多个块，这些块存储在一组DataNode中**。**NameNode**执行文件系统名称空间操作，如打开、关闭和重命名文件和目录。**DataNode**负责为来自文件系统客户端的读写请求提供服务，还根据来自**NameNode**的指令执行块的创建、删除和复制。



NameNode：管理文件系统命名空间，规范客户端对文件的访问（元数据的管理）集群中单个NameNode的存在极大地简化了系统的体系结构。NameNode是所有HDFS元数据的仲裁者和存储库。该系统被设计成这样一种方式，用户数据永远不会流经NameNode。

DataNode：数据存储节点。

**命名空间**

类似于文件目录结构

**数据复制**

HDFS设计用于在大型集群中跨机器可靠地存储非常大的文件。它将每个文件存储为块序列。复制文件的块是为了容错。每个文件的块大小和复制因子都是可配置的。

一个文件中除了最后一个块之外的所有块的大小都是相同的，而在添加了对可变长度块的支持后，用户可以开始一个新的块，而不需要按照配置的块大小填写最后一个块。

应用程序可以指定文件的副本数量。复制因子可以在文件创建时指定，以后可以更改。HDFS中的文件都是一次写入(除了追加和截断)，并且在任何时候都只有一个写入器。

关于块复制的所有决定都由NameNode做出。它定期从集群中的每个datanode接收一个Heartbeat和一个Blockreport。接收到心跳意味着DataNode工作正常。Blockreport包含DataNode上所有块的列表。



