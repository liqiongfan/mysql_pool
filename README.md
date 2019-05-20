## MySQL 数据库连接池

一个使用C语言实现的超简单易用的MySQL数据库连接池

### Demo

```
/* 生成数据库连接池 */
EXSQL_POOL *pools = INIT_DB_POOL("root", "3333", "127.0.0.1", 3306, "books", 3);

/* 从数据库连接池里面获取一个未使用的连接 */
MYSQL *db1 = exsql_getdb(pools), *db2 = exsql_getdb(pools);

/* 执行Db1操作 */
....

/* 将Db1链接收回连接池中，以便复用 */
exsql_release_db(pools, db1);

/* 执行Db2操作 */
....

/* 将Db2链接收回连接池中，以便复用 */
exsql_release_db(pools, db2);

/* 销毁数据库连接池释放所有的内存 */
DESTROY_POOL(pools);
```

## APIs

1：初始化数据库连接池

```c
EXSQL_POOL *INIT_DB_POOL(char *user, char *passwd, char *host,
      unsigned int port, char *db_name, unsigned long instance_number);
```

2：从连接池获取一个未使用的数据库链接

```c
void *exsql_getdb(EXSQL_POOL *pools);
/* 返回值的具体类型就是 (MYSQL *) */
示例如下：
MYSQL *db1 = exsql_getdb(pools);
```

3：将使用完的链接收回连接池，以便复用

```c
void  exsql_release_db(EXSQL_POOL *, void *db);
```

4：释放整个数据库连接池

```c
void DESTROY_POOL(EXSQL_POOL *pools);
```