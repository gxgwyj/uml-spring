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
        // 先循环在原来的数组中下标为0的位置，循环找出是否存在key=null的元素，如果存在替换成为新值         // 返回老值，为什么要将key=null的元素放到数组中第0个位置，因为key=null，无法调用               // hashCode方法
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

#### resize方法扩容

```java
 void resize(int newCapacity) {
        Entry[] oldTable = table;
        // 获取到原来数组的长度
        int oldCapacity = oldTable.length;
     	// 如果原来的长度是最大值（整型最大）
        if (oldCapacity == MAXIMUM_CAPACITY) {
            // 阀值也为整形最大
            threshold = Integer.MAX_VALUE;
            return;
        }
		// 根据新的容量创建新的数组
        Entry[] newTable = new Entry[newCapacity];
     	// 将旧数组的元素移动到新的数组中
        transfer(newTable);
        table = newTable;
     	//重新计算阀值  容量*负载因子
        threshold = (int)(newCapacity * loadFactor);
    }
```

#### transfer方法将旧数组转移到新数组

```java
void transfer(Entry[] newTable) {
        Entry[] src = table;
        int newCapacity = newTable.length;
    	// 循环遍历原来的数组
        for (int j = 0; j < src.length; j++) {
            Entry<K,V> e = src[j];
            if (e != null) {
                src[j] = null;
                // 遍历数组中的链表
                do {
                    // 把当前元素的下一个元素付给一个临时变量
                    Entry<K,V> next = e.next;
                    // 重新计算旧元素的新的位置，因为数组的长度变化后，必须重新计算数组下标
                    int i = indexFor(e.hash, newCapacity);
                    // 此处为什么这么写，第一，解决Hash冲突问题，第二，头插入法
                    // 首先先把数组中的i位置的放到新元素的下一个位置（即把原来数组的位置的头，放到新插入元素的下一个）
                    e.next = newTable[i];
                    // 将新插入的元素放入数组链表的头位置
                    newTable[i] = e;
                    // 重新循环链表的下一个元素
                    e = next;
                } while (e != null);
            }
        }
    }
```

#### indexFor计算数组下标位置

```java
  static int indexFor(int h, int length) {
      	// 此处为什么不用 h%length？，效率高
        // 数组的长度为2的n次方，为了减少hash碰撞
        return h & (length-1);
    }
```

