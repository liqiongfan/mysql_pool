/**
 * File:mysql.c for project library.
 * Author: Josin
 * Email: xeapplee@gmail.com
 * Website: https://www.supjos.cn
 */

#include "exmysql.h"

static EXSQL_INSTANCE *INIT_DB_INSTANCE()
{
	EXSQL_INSTANCE *ptr = malloc(sizeof(EXSQL_INSTANCE));
	if ( ptr == NULL ) return NULL;
	memset(ptr, 0, sizeof(EXSQL_INSTANCE));
	EXSQL_INS_STATUS_P(ptr) = EXSQL_DB_READY;
	EXSQL_INS_NEXT_P(ptr) = NULL;
	return ptr;
}

EXSQL_POOL *INIT_DB_POOL(char *user, char *passwd, char *host, unsigned int port, char *db_name, unsigned long instance_number)
{
	int _i;
	EXSQL_POOL *ptr = malloc(sizeof(EXSQL_POOL));
	if ( ptr == NULL ) return NULL;
	memset(ptr, 0, sizeof(EXSQL_POOL));

	EXSQL_POOL_USER_P(ptr) = user;
	EXSQL_POOL_PASSWD_P(ptr) = passwd;
	EXSQL_POOL_DBNAME_P(ptr) = db_name;
	EXSQL_POOL_HOST_P(ptr) = host;
	EXSQL_POOL_PORT_P(ptr) = port;
	EXSQL_POOL_NUMBER_P(ptr) = instance_number;

	EXSQL_INSTANCE *temp_ptr = NULL;

	/* Initialise The Db pool */
	for (_i = 0; _i < EXSQL_POOL_NUMBER_P(ptr); ++_i) {
		temp_ptr = INIT_DB_INSTANCE();
		MYSQL *mysql = mysql_init(NULL);
		mysql_options(mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8mb4");
		void *db_ptr = mysql_real_connect(mysql, host, user, passwd, db_name, port, NULL, 0);
		if ( db_ptr == NULL )
		{
			printf("%s", mysql_error(mysql));
			free(temp_ptr);
			return NULL;
		}
		mysql = db_ptr;
		EXSQL_INS_INSTANCE_P(temp_ptr) = mysql;
		/* if empty head or tail */
		if ( EXSQL_POOL_HEAD_P(ptr) == NULL ) { EXSQL_POOL_HEAD_P(ptr) = temp_ptr; EXSQL_POOL_TAIL_P(ptr) = temp_ptr; }
		else
		{
			/* Not first add to the end */
			EXSQL_INS_NEXT_P(EXSQL_POOL_TAIL_P(ptr)) = temp_ptr;
			EXSQL_POOL_TAIL_P(ptr) = temp_ptr;
		}
	}
	return ptr;
}

void *exsql_getdb(EXSQL_POOL *pools)
{
	if ( pools == NULL ) return NULL;
	EXSQL_INSTANCE *next = EXSQL_POOL_HEAD_P(pools);
	for (; next != NULL ; next = EXSQL_INS_NEXT_P(next)) {
		if (EXSQL_INS_STATUS_P(next) == EXSQL_DB_READY)
		{
			EXSQL_INS_STATUS_P(next) = EXSQL_DB_IN_USE;
			return EXSQL_INS_INSTANCE_P(next);
		}
	}
	/* If not ready db status, generate new one append to the end of the pool */
	EXSQL_INSTANCE *instance = INIT_DB_INSTANCE();
	MYSQL *mysql = mysql_init(NULL);
	mysql_options(mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8mb4");
	void *db_ptr = mysql_real_connect(mysql, EXSQL_POOL_HOST_P(pools), EXSQL_POOL_USER_P(pools),
			EXSQL_POOL_PASSWD_P(pools), EXSQL_POOL_DBNAME_P(pools), EXSQL_POOL_PORT_P(pools), NULL, 0);
	if ( db_ptr == NULL )
	{
		printf("%s", mysql_error(mysql));
		free(instance);
		mysql_close(mysql);
		return NULL;
	}
	mysql = db_ptr;
	EXSQL_INS_INSTANCE_P(instance) = mysql;

	/* Append to the end of the pools */
	EXSQL_INS_NEXT_P(EXSQL_POOL_TAIL_P(pools)) = instance;
	EXSQL_POOL_TAIL_P(pools) = instance;
	return mysql;
}

void exsql_release_db(EXSQL_POOL *pools, void *db)
{
	if ( pools == NULL || db == NULL ) return;
	EXSQL_INSTANCE *next = EXSQL_POOL_HEAD_P(pools);
	for (; next != NULL ; next = EXSQL_INS_NEXT_P(next)) {
		if ( EXSQL_INS_INSTANCE_P(next) == db ) {
			if (EXSQL_INS_STATUS_P(next) == EXSQL_DB_IN_USE) {
				EXSQL_INS_STATUS_P(next) = EXSQL_DB_READY;
				return;
			}
		}
	}
}

void DESTROY_POOL(EXSQL_POOL *pools)
{
	if ( pools == NULL ) return;
	EXSQL_INSTANCE *temp = EXSQL_POOL_HEAD_P(pools), *next;
	for (; temp != NULL ; temp = next) {
		next = EXSQL_INS_NEXT_P(temp);
		mysql_close(EXSQL_INS_INSTANCE_P(temp));
		EXSQL_INS_INSTANCE_P(temp) = NULL;
		free(temp);
	}
	free(pools);
}