#include <stdio.h>
#include <string.h>

void do_select(char *sql)
{
}

int main()
{
    char sql[128];
    const char *sql2 = "select * from tblGirl";

    strcpy(sql, "select * from tblGirl where height > 165");
    do_select(sql);
    strcat(sql, " and weight < 55");
    do_select(sql);

    do_select(sql2);

    return 0;
}
