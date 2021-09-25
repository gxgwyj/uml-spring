### 一、定义：

抽象队列监视器

CLH锁：基于链表的可扩展、高性能、公平的**自旋锁**，它不断轮询前驱的状态，如果发现前驱释放了锁就结束自旋

### 二、代码分析

解释：

等待队列是“CLH”锁队列的变体。CLH锁通常用于自旋锁。相反，我们使用它们来阻塞同步器，但是使用相同的基本策略，即在其节点的前身中保存关于线程的一些控制信息。每个节点中的状态字段跟踪线程是否应该阻塞。节点在其前任节点释放时发出信号。否则，队列的每个节点都充当一个特定的通知样式的监视器，其中包含一个等待线程。状态字段不控制线程是否被授予锁等。一个线程可能试图获取队列中的第一个线程。但领先并不能保证成功，它只会给你竞争的权利。因此，当前发布的竞争者线程可能需要重新等待。

###### 成员变量

```java


```



###### 内部类(等待队列数据模型)

```java
static final class Node {
    	// 共享模式
        static final Node SHARED = new Node();
    	// 独占模式
        static final Node EXCLUSIVE = null;
    	// 取消
        static final int CANCELLED =  1;
        // 停止
        static final int SIGNAL    = -1;
        // 条件等待
        static final int CONDITION = -2;
       	// 条件传播
        static final int PROPAGATE = -3;
		// 节点状态
        volatile int waitStatus;

        volatile Node prev;

        volatile Node next;

        volatile Thread thread;

        Node nextWaiter;

        final boolean isShared() {
            return nextWaiter == SHARED;
        }

        final Node predecessor() throws NullPointerException {
            Node p = prev;
            if (p == null)
                throw new NullPointerException();
            else
                return p;
        }

        Node() {    // Used to establish initial head or SHARED marker
        }

        Node(Thread thread, Node mode) {     // Used by addWaiter
            this.nextWaiter = mode;
            this.thread = thread;
        }

        Node(Thread thread, int waitStatus) { // Used by Condition
            this.waitStatus = waitStatus;
            this.thread = thread;
        }
    }
```

###### 成员变量

```java
 	// 阻塞等待队列的头
    private transient volatile Node head;

   	// 阻塞队列的尾部
    private transient volatile Node tail;
	
	// 状态
    private volatile int state;
```

###### 改变状态的方法

```java
// 改变state的值，使用（CAS）原子操作  
protected final boolean compareAndSetState(int expect, int update) {
 		// 如果内存中的值跟期望值相同，则更新成新的值
        return unsafe.compareAndSwapInt(this, stateOffset, expect, update);
    }
```

###### 入队列（放入到队列的最后一个）

```java
 private Node enq(final Node node) {
        for (;;) {
            Node t = tail;
            // 
            if (t == null) { // Must initialize
                if (compareAndSetHead(new Node()))
                    tail = head;
            } else {
                // 进入排队等待
                node.prev = t;
                if (compareAndSetTail(t, node)) {
                    t.next = node;
                    return t;
                }
            }
        }
    }
```

###### 将线程添加到等待队列里

```java
 private Node addWaiter(Node mode) {
     	// 创建一个线程节点，放到等待队列中
        Node node = new Node(Thread.currentThread(), mode);
        // Try the fast path of enq; backup to full enq on failure
        Node pred = tail;
        if (pred != null) {
            node.prev = pred;
            if (compareAndSetTail(pred, node)) {
                pred.next = node;
                return node;
            }
        }
        enq(node);
        return node;
    }
```

###### 唤起当前节点的后继节点

```java

private void unparkSuccessor(Node node) {
        int ws = node.waitStatus;
        if (ws < 0)
            compareAndSetWaitStatus(node, ws, 0);
        Node s = node.next;
        if (s == null || s.waitStatus > 0) {
            s = null;
            for (Node t = tail; t != null && t != node; t = t.prev)
                if (t.waitStatus <= 0)
                    s = t;
        }
        if (s != null)
            // 启动线程
            LockSupport.unpark(s.thread);
    }
```

