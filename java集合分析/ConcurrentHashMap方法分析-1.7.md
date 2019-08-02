一、1.7中ConcurrentHashMap特点：

（1）线程安全

（2）获取元素不需要加锁,非阻塞

（3）不允许key=null的值？为什么？

二、1.7基本原理：分段table,减少锁的粒度，最大提升并发量

三、几个主要的成员变量

```java
// 默认的并发级别
static final int DEFAULT_CONCURRENCY_LEVEL = 16;
// 分段表的最小容量
static final int MIN_SEGMENT_TABLE_CAPACITY = 2;
// 分段表最大的数量
static final int MAX_SEGMENTS = 1 << 16;
// 锁定之前的重试次数
static final int RETRIES_BEFORE_LOCK = 2;
// 段表
final Segment<K,V>[] segments;
```

#### 构造方法分析

```java
    public ConcurrentHashMap(int initialCapacity,float loadFactor, int concurrencyLevel) {
        // 初始化容量、负载因子、并发级别参数的校验
        if (!(loadFactor > 0) || initialCapacity < 0 || concurrencyLevel <= 0)
            throw new IllegalArgumentException();
        // 最大并发级别的校验，如果传入的并发级别超过最大并发级别，默认并发级别
        if (concurrencyLevel > MAX_SEGMENTS)
            concurrencyLevel = MAX_SEGMENTS;
        // Find power-of-two sizes best matching arguments
        // 根据传入的并发级别参数，重新计算，计算结果为2的幂次方
        int sshift = 0;
        int ssize = 1;
        while (ssize < concurrencyLevel) {
            ++sshift;
            ssize <<= 1;
        }
        this.segmentShift = 32 - sshift;
        this.segmentMask = ssize - 1;
        if (initialCapacity > MAXIMUM_CAPACITY)
            initialCapacity = MAXIMUM_CAPACITY;
        // 初始化容量除以计算出来的大小=需要的分段表的个数
        int c = initialCapacity / ssize;
        if (c * ssize < initialCapacity)
            ++c;
        int cap = MIN_SEGMENT_TABLE_CAPACITY;
        while (cap < c)
            cap <<= 1;
        // create segments and segments[0]
        Segment<K,V> s0 =
            new Segment<K,V>(loadFactor, (int)(cap * loadFactor),
                             (HashEntry<K,V>[])new HashEntry[cap]);
        Segment<K,V>[] ss = (Segment<K,V>[])new Segment[ssize];
        UNSAFE.putOrderedObject(ss, SBASE, s0); // ordered write of segments[0]
        this.segments = ss;
    }
```

