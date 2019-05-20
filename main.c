#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "exmysql.h"
#include <mysql.h>


int main(int argc, char *argv[])
{
	EXSQL_POOL *pools = INIT_DB_POOL("root", "3333", "127.0.0.1", 3306, "books", 3);
	MYSQL *mysql = exsql_getdb(pools), *db2 = exsql_getdb(pools);

    int id = 1, rid; unsigned long name_len = 0;
    char name[30] = {0}, is_null;
    MYSQL_STMT *stmt = mysql_stmt_init(mysql);
    char *prepare_sql = "SELECT id, book_name FROM books";

    if ( mysql_stmt_prepare(stmt, prepare_sql , strlen(prepare_sql)) == 1)
    {
        printf("%s\n", mysql_stmt_error(stmt));
    }

    MYSQL_BIND  bind[1] = {0}, bind_result[2] = {0};

    bind->buffer_type = MYSQL_TYPE_LONG;
    bind->buffer = &id;
    mysql_stmt_bind_param(stmt, bind);

    mysql_stmt_execute(stmt);

    bind_result[0].buffer_type = MYSQL_TYPE_LONG;
    bind_result[0].buffer = &rid;
    bind_result[0].buffer_length = stmt->fields->length;
    bind_result[0].length = &name_len;
    bind_result[0].is_null = &is_null;
    bind_result[1].buffer_type = MYSQL_TYPE_VAR_STRING;
    bind_result[1].buffer = name;
    bind_result[1].buffer_length = (stmt->fields + 1)->length;
    bind_result[1].length = &name_len;
    bind_result[1].is_null = &is_null;

    mysql_stmt_bind_result(stmt, bind_result);

    while(!mysql_stmt_fetch(stmt)){
        printf("%d:%s\n", rid, name);
    }
	exsql_release_db(pools, mysql);
    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);
    // mysql_close(mysql);
    DESTROY_POOL(pools);
    
    return 0;
}


