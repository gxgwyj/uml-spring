```java
// ConcurrentHashMap构造方法
public ConcurrentHashMap(int initialCapacity,
                         float loadFactor, int concurrencyLevel) {
    if (!(loadFactor > 0) || initialCapacity < 0 || concurrencyLevel <= 0)
        throw new IllegalArgumentException();
    //设置最大的并发段
    if (concurrencyLevel > MAX_SEGMENTS)
        concurrencyLevel = MAX_SEGMENTS;
    
    
    int sshift = 0;
    int ssize = 1;
    while (ssize < concurrencyLevel) {
        //这个变量代表什么意思呢？
        ++sshift;
        //扩大两倍
        ssize <<= 1;
    }
    
    // 默认的时候  sshift = 4；ssize = 16
    
    this.segmentShift = 32 - sshift;
    this.segmentMask = ssize - 1;
    
    // segmentShift = 28；segmentMask = 15；
    
    if (initialCapacity > MAXIMUM_CAPACITY)
        initialCapacity = MAXIMUM_CAPACITY; // initialCapacity = 32;
    
    
    int c = initialCapacity / ssize;
    if (c * ssize < initialCapacity)
        ++c;
    int cap = MIN_SEGMENT_TABLE_CAPACITY;
    while (cap < c)
        cap <<= 1;
    // create segments and segments[0]
    Segment<K,V> s0 = new Segment<K,V>(loadFactor, (int)(cap * loadFactor),
                         (HashEntry<K,V>[])new HashEntry[cap]);
    Segment<K,V>[] ss = (Segment<K,V>[])new Segment[ssize];
    
    UNSAFE.putOrderedObject(ss, SBASE, s0); // ordered write of segments[0]
    this.segments = ss;
}
```