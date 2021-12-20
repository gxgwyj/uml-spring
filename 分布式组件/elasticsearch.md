Elasticsearch和RDBMS之间的比较

| Elasticsearch | 关系数据库 |
| ------------- | ---------- |
| 索引          | 数据库     |
| 映射          | 表         |
| 字段          | 字段       |
| JSON对象      | 元组       |

（1）创建索引：http://localhost:9200/<index>/<type>/

入参为：

```json
{
    "title": "java编程思想",
    "name": "java编程思想",
    "amount": 10000,
    "author":"埃克尔"
}
```

