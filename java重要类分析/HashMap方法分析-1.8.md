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
    // 第一步、判断数组是否为空,如果为空生成新的数组（与前面所说的1.8中的hashMap数组在使用的时候才会去初始化对应）
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length;
    
    // 第二步、将元素放置数组中的位置（把新元素加到table中的位置，并且把就元素找出来）
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    else {
        Node<K,V> e;
        K k;
        // key值相同
        if (p.hash == hash && ((k = p.key) == key || (key != null && key.equals(k))))
            e = p;
        // 原来的节点是红黑树
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        else {
            // 数组元素不为红黑树，并且已经存在
            for (int binCount = 0; ; ++binCount) {
                // 
                if ((e = p.next) == null) {
                    // 尾插入法
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
        // 第三步，如果key存在元素，则直接返回原来的元素的旧值
        if (e != null) { // existing mapping for key
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value;
            afterNodeAccess(e);
            return oldValue;
        }
    }
    // 改变的次数累加
    ++modCount;
    // 如果容器的中元素的个数超过阀值，触发扩容机制
    if (++size > threshold)
        resize();
    afterNodeInsertion(evict);// 
    return null;
}
```

###### putTreeVal方法

```java
final TreeNode<K,V> putTreeVal(HashMap<K,V> map, Node<K,V>[] tab,
                                       int h, K k, V v) {
            Class<?> kc = null;
            boolean searched = false;
            TreeNode<K,V> root = (parent != null) ? root() : this;
            for (TreeNode<K,V> p = root;;) {
                int dir, ph; K pk;
                if ((ph = p.hash) > h)
                    dir = -1;
                else if (ph < h)
                    dir = 1;
                else if ((pk = p.key) == k || (k != null && k.equals(pk)))
                    return p;
                else if ((kc == null &&
                          (kc = comparableClassFor(k)) == null) ||
                         (dir = compareComparables(kc, k, pk)) == 0) {
                    if (!searched) {
                        TreeNode<K,V> q, ch;
                        searched = true;
                        if (((ch = p.left) != null &&
                             (q = ch.find(h, k, kc)) != null) ||
                            ((ch = p.right) != null &&
                             (q = ch.find(h, k, kc)) != null))
                            return q;
                    }
                    dir = tieBreakOrder(k, pk);
                }

                TreeNode<K,V> xp = p;
                if ((p = (dir <= 0) ? p.left : p.right) == null) {
                    Node<K,V> xpn = xp.next;
                    TreeNode<K,V> x = map.newTreeNode(h, k, v, xpn);
                    if (dir <= 0)
                        xp.left = x;
                    else
                        xp.right = x;
                    xp.next = x;
                    x.parent = x.prev = xp;
                    if (xpn != null)
                        ((TreeNode<K,V>)xpn).prev = x;
                    moveRootToFront(tab, balanceInsertion(root, x));
                    return null;
                }
            }
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
                    else {
                        // 原来数组的元素被分成两段，目的为了分布均匀
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
                       	// 第一段
                        if (loTail != null) {
                            loTail.next = null;
                            newTab[j] = loHead;
                        }
                        // 第二段
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