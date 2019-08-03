一、1.7中ConcurrentHashMap特点：

（1）线程安全

（2）获取元素不需要加锁,非阻塞

（3）不允许key=null的值？为什么？

二、1.7基本原理：分段table,减少锁的粒度，最大提升并发量

三、几个主要的成员变量

```java
// 段掩码
final int segmentMask;
// 段偏移量
final int segmentShift;
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
// Segment 定义  继承了 ReentrantLock 可重入锁
transient volatile HashEntry<K,V>[] table;

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
        // 根据传入的并发级别参数，重新计算，计算结果为2的幂次方，比如并发级别传15，意味着分段数组的大小为离15最近的2的n次方，即2的4次方为16；
        int sshift = 0;
        // 分段表的长度
        int ssize = 1;
        while (ssize < concurrencyLevel) {
            ++sshift;
            ssize <<= 1;
        }
        this.segmentShift = 32 - sshift;
        this.segmentMask = ssize - 1;
        if (initialCapacity > MAXIMUM_CAPACITY)
            initialCapacity = MAXIMUM_CAPACITY;
        // 每个分段表中链表数组的个数 = 元素总数  / 分段表个数
        int c = initialCapacity / ssize;
        // 做一次取整
        if (c * ssize < initialCapacity)
            ++c;
        // 每个分段表中链表数组的个数也是2的n次方
        int cap = MIN_SEGMENT_TABLE_CAPACITY;
        while (cap < c)
            cap <<= 1;
        // create segments and segments[0]
        // 创建分段数组中的第一个元素
        Segment<K,V> s0 =
            new Segment<K,V>(loadFactor, (int)(cap * loadFactor),
                             (HashEntry<K,V>[])new HashEntry[cap]);
        Segment<K,V>[] ss = (Segment<K,V>[])new Segment[ssize];
        UNSAFE.putOrderedObject(ss, SBASE, s0); // ordered write of segments[0]
        this.segments = ss;
    }
```

#### put方法解释

```java
    public V put(K key, V value) {
        // ConcurrentHashMap 中不允许value为空
        Segment<K,V> s;
        if (value == null)
            throw new NullPointerException();
        // 根据传入的key计算key的hash值
        int hash = hash(key);
        // 计算key所在段表的下标  segmentShift = 28，segmentMask = 16-1 =15 也就是 1111
        int j = (hash >>> segmentShift) & segmentMask;
        if ((s = (Segment<K,V>)UNSAFE.getObject          // nonvolatile; recheck
             (segments, (j << SSHIFT) + SBASE)) == null) //  in ensureSegment
            s = ensureSegment(j);
        return s.put(key, hash, value, false);
    }

```

#### ConcurrentHashMap中计算段Hash的算法解释

```java
 private int hash(Object k) {
        int h = hashSeed;

        if ((0 != h) && (k instanceof String)) {
            return sun.misc.Hashing.stringHash32((String) k);
        }

        h ^= k.hashCode();

        // Spread bits to regularize both segment and index locations,
        // using variant of single-word Wang/Jenkins hash.
        h += (h <<  15) ^ 0xffffcd7d;
        h ^= (h >>> 10);
        h += (h <<   3);
        h ^= (h >>>  6);
        h += (h <<   2) + (h << 14);
        return h ^ (h >>> 16);
    }
```

