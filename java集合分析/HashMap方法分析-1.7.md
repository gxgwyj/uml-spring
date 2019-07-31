#### put方法解释

```java
public V put(K key, V value) {
        // 判断key是否为空
        if (key == null)
            // 将key为null的值put到数组下标为0处，因为jdk1.8之前在new HashMap的时候就初始化了
            // 数组，这是1.7和1.8的区别，1.7没有用就初始化了，1.8只有真正put元素的时候才去开辟数             // 组空间。
            return putForNullKey(value);
        int hash = hash(key.hashCode());
        int i = indexFor(hash, table.length);
        for (Entry<K,V> e = table[i]; e != null; e = e.next) {
            Object k;
            if (e.hash == hash && ((k = e.key) == key || key.equals(k))) {
                V oldValue = e.value;
                e.value = value;
                e.recordAccess(this);
                return oldValue;
            }
        }

        modCount++;
        addEntry(hash, key, value, i);
        return null;
    }
```

#### putForNullKey方法解释

```java
// 处理key为null的方法  
private V putForNullKey(V value) {
        // 先循环在原来的数组中下标为0的位置，循环找出是否存在key=null的元素，如果存在替换成为新值         // 返回老值
        for (Entry<K,V> e = table[0]; e != null; e = e.next) {
            if (e.key == null) {
                V oldValue = e.value;
                e.value = value;
                e.recordAccess(this);
                return oldValue;
            }
        }
        // 如果不存在，直接在数组下标为0的位置，添加该key=null的数据
        // 修改HashMap的次数，每次添加或者删除该数据都会自增1
        modCount++;
        addEntry(0, null, value, 0);
    	// 返回null
        return null;
    }
```

#### addEntry解释

```java
// 该方法直接在数组的第n个位置添加首个链表元素
void addEntry(int hash, K key, V value, int bucketIndex) {
        Entry<K,V> e = table[bucketIndex];
    	// 直接在数组的第一个位置添加元素
        table[bucketIndex] = new Entry<>(hash, key, value, e);
    	// 如果发现hashMap元素的个数大于阀值，扩容数组
        if (size++ >= threshold)
            resize(2 * table.length);
    }
```

