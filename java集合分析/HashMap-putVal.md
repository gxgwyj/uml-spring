```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,boolean evict) {
    Node<K,V>[] tab; // 原来的数组 
    Node<K,V> p;
    int n, // 数组长度
    int i; 
    // 判断数组是否为空
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length; // 生成数组，并且生成新数组的长度
    // 索引位置 = （数组长度 - 1）& hash
    // 如果该位置的元素为空，直接赋值
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    else {
        // 如果计算的位置已经存在值
        Node<K,V> e; 
        K k;
        // 判断原来的key值是否与新增的key值完全相同
        if (p.hash == hash && ((k = p.key) == key || (key != null && key.equals(k))))
            e = p;// 将原值赋给临时变量
        // 原来的元素是否是红黑树
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        else {
            // 数组元素不为红黑树，并且已经存在
            for (int binCount = 0; ; ++binCount) {
                // 
                if ((e = p.next) == null) {
                    p.next = newNode(hash, key, value, null);
                    if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
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
    if (++size > threshold)
        resize();
    afterNodeInsertion(evict);
    return null;
}
```

