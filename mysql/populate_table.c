#include <my_global.h>
#include <mysql.h>

int main(int argc, char **argv)
{
	MYSQL *conn;

	conn = mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "zetcode", "passwd", "testdb", 0, NULL, 0);

	mysql_query(conn, "CREATE TABLE writers(name VARCHAR(25))");

	mysql_query(conn, "INSERT INTO writers VALUES('Leo Tolstoy')");
	mysql_query(conn, "INSERT INTO writers VALUES('Jack London')");
	mysql_query(conn, "INSERT INTO writers VALUES('Honore de Balzac')");
	mysql_query(conn, "INSERT INTO writers VALUES('Lion Feuchtwanger')");
	mysql_query(conn, "INSERT INTO writers VALUES('Emile Zola')");

	mysql_close(conn);

	return 0;
}
