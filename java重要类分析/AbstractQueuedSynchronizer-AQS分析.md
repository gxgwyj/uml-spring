### 一、定义：

抽象队列监视器

CLH锁：基于链表的可扩展、高性能、公平的**自旋锁**，它不断轮询前驱的状态，如果发现前驱释放了锁就结束自旋

### 二、代码分析

解释：

等待队列是“CLH”锁队列的变体。CLH锁通常用于自旋锁。相反，我们使用它们来阻塞同步器，但是使用相同的基本策略，即在其节点的前身中保存关于线程的一些控制信息。每个节点中的状态字段跟踪线程是否应该阻塞。节点在其前任节点释放时发出信号。否则，队列的每个节点都充当一个特定的通知样式的监视器，其中包含一个等待线程。状态字段不控制线程是否被授予锁等。一个线程可能试图获取队列中的第一个线程。但领先并不能保证成功，它只会给你竞争的权利。因此，当前发布的竞争者线程可能需要重新等待。

内部类

```java
static final class Node {
    	// 共享模式
        static final Node SHARED = new Node();
    	// 独占模式
        static final Node EXCLUSIVE = null;
    	// 线程被取消时  waitStatus的值
        static final int CANCELLED =  1;
        // 发出需要取消的信号
        static final int SIGNAL    = -1;
        // 线程在特定条件下等待
        static final int CONDITION = -2;
       	// 传播线程的状态
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

