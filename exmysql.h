/**
 * File:mysql.h for project library.
 * Author: Josin
 * Email: xeapplee@gmail.com
 * Website: https://www.supjos.cn
 */

#ifndef LIBRARY_MYSQL_H
#define LIBRARY_MYSQL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Should remember that include the mysql directory*/
#include <mysql.h>

enum __MYSQL_INSTANCE_STATUS
{
	EXSQL_DB_READY = 1,
	EXSQL_DB_IN_USE
};
typedef struct __EXSQL_INSTANCE
{
	void *instance;
	unsigned char status;
	struct __EXSQL_INSTANCE *next;
} EXSQL_INSTANCE;
#define EXSQL_INS_INSTANCE(inst) (inst).instance
#define EXSQL_INS_INSTANCE_P(inst) EXSQL_INS_INSTANCE(*(inst))
#define EXSQL_INS_STATUS(instance) (instance).status
#define EXSQL_INS_STATUS_P(instance) EXSQL_INS_STATUS(*(instance))
#define EXSQL_INS_NEXT(instance) (instance).next
#define EXSQL_INS_NEXT_P(instance) EXSQL_INS_NEXT(*(instance))

typedef struct __EXSQL_POOL
{
	struct __EXSQL_INSTANCE *head;
	struct __EXSQL_INSTANCE *tail;
	char *user, *passwd, *host, *db_name;
	unsigned int port;
	unsigned long pool_number;
} EXSQL_POOL;
#define EXSQL_POOL_HEAD(pool) (pool).head
#define EXSQL_POOL_HEAD_P(pool) EXSQL_POOL_HEAD(*(pool))
#define EXSQL_POOL_TAIL(pool) (pool).tail
#define EXSQL_POOL_TAIL_P(pool) EXSQL_POOL_TAIL(*(pool))
#define EXSQL_POOL_USER(pool) (pool).user
#define EXSQL_POOL_USER_P(pool) EXSQL_POOL_USER(*(pool))
#define EXSQL_POOL_PASSWD(pool) (pool).passwd
#define EXSQL_POOL_PASSWD_P(pool) EXSQL_POOL_PASSWD(*(pool))
#define EXSQL_POOL_HOST(pool) (pool).host
#define EXSQL_POOL_HOST_P(pool) EXSQL_POOL_HOST(*(pool))
#define EXSQL_POOL_PORT(pool) (pool).port
#define EXSQL_POOL_PORT_P(pool) EXSQL_POOL_PORT(*(pool))
#define EXSQL_POOL_DBNAME(pool) (pool).db_name
#define EXSQL_POOL_DBNAME_P(pool) EXSQL_POOL_DBNAME(*(pool))
#define EXSQL_POOL_NUMBER(pool) (pool).pool_number
#define EXSQL_POOL_NUMBER_P(pool) EXSQL_POOL_NUMBER(*(pool))

EXSQL_POOL *INIT_DB_POOL(char *user, char *passwd, char *host,
		unsigned int port, char *db_name, unsigned long instance_number);
void *exsql_getdb(EXSQL_POOL *pools);
void  exsql_release_db(EXSQL_POOL *, void *db);
void DESTROY_POOL(EXSQL_POOL *pools);


#endif /* LIBRARY_MYSQL_H */
