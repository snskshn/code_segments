#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	MYSQL *conn;

	conn = mysql_init(NULL);

	if (conn == NULL) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	if (mysql_real_connect(conn, "10.51.51.1", "root",
				"011d", NULL, 0, NULL, 0) == NULL) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	if (mysql_query(conn, "create database testdb")) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	mysql_close(conn);

	return 0;
}
