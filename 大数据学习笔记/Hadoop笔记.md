#### hadoop环境搭建

（1）下载hadoop软件包并且上传至linux服务器的目录

（2）解压hadoop软件文件

（3）设置JAVA_HOME环境变量，比如hadoop安装在/opt/hadoop-2.10.1/  目录下面，则修改  /opt/hadoop-2.10.1/etc/hadoop/ 目录下的hadoop-env.sh 文件，添加export JAVA_HOME=/usr/java/jdk1.8.0_91 配置

（4）默认情况下，hadoop使用非分布式的方式配置，使用非分布式的方式启动

（5）第一个标准的运行实例，使用bin目录下面的hadoop命令

hadoop jar ../share/hadoop/mapreduce/hadoop-mapreduce-examples-2.10.1.jar grep ../gaoxugang_test/ output 'dfs[a-z.]+'

以上命令的大致意思是：使用hadoop的mapreduce计算组件查找出/gaoxugang_test 目录下面匹配dfs开头的文本。