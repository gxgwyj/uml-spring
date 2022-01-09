# JVM学习

## class文件结构

参考java官方文档：https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html

定义：java源代码文件经过编译后紧凑的有一定规律的字节码文件。

```
ClassFile {
    u4             magic;--魔数（4个字节）固定值：CAFEBABE
    u2             minor_version;--次版本号（2个字节）
    u2             major_version;--主版本号（2个字节）class文件的版本号：【主版本号.次版本号】，如【52.0】
    u2             constant_pool_count;--常量池的长度
    cp_info        constant_pool[constant_pool_count-1];--常量池表
    u2             access_flags;--访问标识（public、final、interface、枚举、注解 等等）
    u2             this_class;--类索引，指向常量池的有效索引
    u2             super_class;--父类索引 指向常量池的有效索引
    u2             interfaces_count;
    u2             interfaces[interfaces_count];
    u2             fields_count;
    field_info     fields[fields_count];
    u2             methods_count;
    method_info    methods[methods_count];
    u2             attributes_count;
    attribute_info attributes[attributes_count];
}
```

**注：u代表无符号的整型，u4代表4个字节（32位）无符号的整型，以此类推**

可以结合使用idea中的JclassLib插件学习class文件，更容易理解。

**jdk中JVM版本与可以执行的class文件版本之间的关系**

class版本号从45开始

| JDK       | Class_Version |
| --------- | ------------- |
| 1.0.2     | 45.0~45.3     |
| 1.1.*     | 45.0~45.65535 |
| 1.k(k>=2) | 45.0~44+k.0   |



### 常量池cp_info结构

cp_info是一个数组形式的数据结构，其中每一项的格式如下：

```
cp_info {
    u1 tag;--代表类型，下面info的值随着tag的变化而变化，如下tags表所示
    u1 info[];
}
```

| CONSTANT_Class              | 7    | 类或接口的符号引用                                           |
| --------------------------- | ---- | ------------------------------------------------------------ |
| CONSTANT_Fieldref           | 9    | 代表字段的**符号引用**                                       |
| CONSTANT_Methodref          | 10   | 代表方法的**符号引用**                                       |
| CONSTANT_InterfaceMethodref | 11   | 代表接口中方法的**符号引用**                                 |
| CONSTANT_String             | 8    | 代表字符串类型的**字面量**                                   |
| CONSTANT_Integer            | 3    | 代表整型**字面量**                                           |
| CONSTANT_Float              | 4    | 代表浮点型**字面量**                                         |
| CONSTANT_Long               | 5    | 代表长整型**字面量**                                         |
| CONSTANT_Double             | 6    | 代表双精度浮点型**字面量**                                   |
| CONSTANT_NameAndType        | 12   | 字段或方法的部分**符号引用**【表示字段或方法的名称和类型】，比如类中的字段 private String name  字段名称为“name” 类型为String |
| CONSTANT_Utf8               | 1    | **代表utf-8编码的字符串**                                    |
| CONSTANT_MethodHandle       | 15   | 方法**句柄**，我理解这个代表了某个方法的执行类型（字节码行为），是取值，调用，还是.... |
| CONSTANT_MethodType         | 16   | 方法**类型**                                                 |
| CONSTANT_InvokeDynamic      | 18   | 动态方法调用点                                               |

#### CONSTANT_Class_info 

类描述常量信息结构

```
CONSTANT_Class_info {
    u1 tag;--标志位，值为7
    u2 name_index;--指向常量池中一个CONSTANT_Utf8的类型常量的位置
}
```

#### CONSTANT_Fieldref_info

代表字段的符号引用，CONSTANT_Methodref_info、CONSTANT_InterfaceMethodref_info类似，如下：

```
// 字段符号引用
CONSTANT_Fieldref_info {
    u1 tag;--值为9
    u2 class_index;--指向常量池中一个CONSTANT_Class类型的位置值
    u2 name_and_type_index;
}
// 方法符号引用
CONSTANT_Methodref_info {
    u1 tag;--值为10
    u2 class_index;--指向常量池中一个CONSTANT_Class类型的位置值
    u2 name_and_type_index;
}
// 接口方法符号引用
CONSTANT_InterfaceMethodref_info {
    u1 tag;--值为11
    u2 class_index;指向常量池中一个CONSTANT_Class【接口】类型的位置值
    u2 name_and_type_index;
}
```

#### CONSTANT_String_info

代表字符串类型的**字面量**，结构如下：

```
CONSTANT_String_info {
    u1 tag;--值为8
    u2 string_index;指向常量池中一个CONSTANT_Utf8【uft-8编码】类型的位置值（表示要初始化String对象的Unicode码位）
}
```

#### CONSTANT_Integer_info 

代表整型**字面量**， CONSTANT_Float_info与之类似，结构如下：

```
// 整型
CONSTANT_Integer_info {
    u1 tag;--值为3
    u4 bytes;
}

// 浮点型
CONSTANT_Float_info {
    u1 tag;--值为4
    u4 bytes;
}
```

#### CONSTANT_Long_info 

代表长整型字面量， CONSTANT_Double_info 与之类似，结构如下：

```
// 长整型
CONSTANT_Long_info {
    u1 tag;--值5
    u4 high_bytes;
    u4 low_bytes;
}

// 双精度浮点型
CONSTANT_Double_info {
    u1 tag;--值6
    u4 high_bytes;
    u4 low_bytes;
}
```

#### CONSTANT_NameAndType_info 

CONSTANT_NameAndType_info结构用于表示一个字段或方法，而不指明它属于哪个类或接口类型:

```
CONSTANT_NameAndType_info {
    u1 tag;--值为12
    u2 name_index;--指向常量池中一个CONSTANT_Utf8_info结构
    u2 descriptor_index;--字段或者方法的描述，指向常量池中一个CONSTANT_Utf8_info结构
}
```

#### CONSTANT_Utf8_info 

CONSTANT_Utf8_info结构用于表示Unicode码:

```
CONSTANT_Utf8_info {
    u1 tag;--值为1
    u2 length;--bytes数组中的字节数(而不是结果字符串的长度)。
    u1 bytes[length];--包含的字节数组
}
```

#### CONSTANT_MethodHandle_info 

CONSTANT_MethodHandle_info结构用于表示一个方法句柄:

```
CONSTANT_MethodHandle_info {
    u1 tag;--值为15
    u1 reference_kind;--值必须在1到9之间。该值表示该方法句柄的类型，它描述了字节码行为
    u2 reference_index;
}
```

