### 基于jdk1.8分析HashMap

##### 根据key计算hash值得方法

```java
 static final int hash(Object key) {
        int h;
        //key == null 时hash计算为0，否则使用key的hashCode与低16位进行与运算
        return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
    }
```

##### 1.8中的构造方法

```java
// 初始化容量和负载因子    
public HashMap(int initialCapacity, float loadFactor) {
        if (initialCapacity < 0)
            throw new IllegalArgumentException("Illegal initial capacity: " +
                                               initialCapacity);
        if (initialCapacity > MAXIMUM_CAPACITY)
            initialCapacity = MAXIMUM_CAPACITY;
        if (loadFactor <= 0 || Float.isNaN(loadFactor))
            throw new IllegalArgumentException("Illegal load factor: " +
                                               loadFactor);
        this.loadFactor = loadFactor;
    	// 根据传入的初始化容量计算阀值（即在容器容量达到什么数量的时候容器去扩容）
        this.threshold = tableSizeFor(initialCapacity);
    }
```

###### tableSizeFor方法（总是返回2的n次方）

```java
 static final int tableSizeFor(int cap) {
        int n = cap - 1;
        n |= n >>> 1;
        n |= n >>> 2;
        n |= n >>> 4;
        n |= n >>> 8;
        n |= n >>> 16;
        return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
    }
```

##### put方法分析

```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,boolean evict) {
    Node<K,V>[] tab;
    Node<K,V> p;
    int n, 
    int i; 
    // 判断数组是否为空
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length; // 生成数组，并且生成新数组的长度
    // 如果该位置的元素为空，直接赋值
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    else {
        // 如果计算的位置已经存在值
        Node<K,V> e; 
        K k;
        // 判断原来的key值是否与新增的key值完全相同
        if (p.hash == hash && ((k = p.key) == key || (key != null && key.equals(k))))
            e = p;
        // 原来的元素是否是红黑树
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        else {
            // 数组元素不为红黑树，并且已经存在
            for (int binCount = 0; ; ++binCount) {
                // 
                if ((e = p.next) == null) {
                    p.next = newNode(hash, key, value, null);
                    // 如果链表的个数超过了红黑树转化的阀值，直接将链表转换成红黑树	
                    if (binCount >= TREEIFY_THRESHOLD - 1)
                        treeifyBin(tab, hash);
                    break;
                }
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    break;
                p = e;
            }
        }
        if (e != null) { // existing mapping for key
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value;
            afterNodeAccess(e);
            return oldValue;
        }
    }
    ++modCount;
    // 如果容器的中元素的个数超过阀值，触发扩容机制
    if (++size > threshold)
        resize();
    afterNodeInsertion(evict);
    return null;
}
```

##### resize（扩容）方法分析

```java
final Node<K,V>[] resize() {
    	// 获取原来的数组
        Node<K,V>[] oldTab = table;
    	// 获取原来数组长度
        int oldCap = (oldTab == null) ? 0 : oldTab.length;
    	// 获取原来的阀值
        int oldThr = threshold;
    	// 定义新的容量
        int newCap, newThr = 0;
        if (oldCap > 0) {
            // 如果原数组的容量大于最大允许的容量，则不需要扩容，返回老的数组
            if (oldCap >= MAXIMUM_CAPACITY) {
                threshold = Integer.MAX_VALUE;
                return oldTab;
            }
            // 将新的容量和阀值扩大为原来的二倍
            else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                     oldCap >= DEFAULT_INITIAL_CAPACITY)
                newThr = oldThr << 1; // double threshold
        }
        else if (oldThr > 0) // initial capacity was placed in threshold
            newCap = oldThr;
        else {
            // zero initial threshold signifies using defaults
            // 计算新的容量和阀值（使用默认的）
            newCap = DEFAULT_INITIAL_CAPACITY;
            newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
        }
    	// 计算新的阀值
        if (newThr == 0) {
            float ft = (float)newCap * loadFactor;
            newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                      (int)ft : Integer.MAX_VALUE);
        }
        threshold = newThr;
        @SuppressWarnings({"rawtypes","unchecked"})
    	// 重新生成新的数组（扩容后的数组）
        Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
        table = newTab;
        if (oldTab != null) {
            // 循环老数组将老数组重新分配到新的数组中
            for (int j = 0; j < oldCap; ++j) {
                Node<K,V> e;
                if ((e = oldTab[j]) != null) {
                    // 将老数组的第一个引用赋值为空，利于回收老的数组
                    oldTab[j] = null;
                    // 首个元素
                    if (e.next == null)
                        // 重新计算元素在新数组中的位置
                        newTab[e.hash & (newCap - 1)] = e;
                    // 节点为树（红黑树）的元素
                    else if (e instanceof TreeNode)
                        ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                    else { // preserve order
                        Node<K,V> loHead = null, loTail = null;
                        Node<K,V> hiHead = null, hiTail = null;
                        Node<K,V> next;
                        do {
                            // 将本元素的下一个元素复制出来
                            next = e.next;
                            // 通过计算e.hash&oldCap==0构造一条链
                            if ((e.hash & oldCap) == 0) {
                                if (loTail == null)
                                    loHead = e;
                                else
                                    loTail.next = e;
                                loTail = e;
                            }
                            // 通过e.hash&oldCap!=0构造另外一条链
                            else {
                                if (hiTail == null)
                                    hiHead = e;
                                else
                                    hiTail.next = e;
                                hiTail = e;
                            }
                        } while ((e = next) != null);
                        if (loTail != null) {
                            loTail.next = null;
                            newTab[j] = loHead;
                        }
                        if (hiTail != null) {
                            hiTail.next = null;
                            newTab[j + oldCap] = hiHead;
                        }
                    }
                }
            }
        }
        return newTab;
    }
https://www.jianshu.com/p/0ab3e05b1d23
```