CC=gcc
CFG=`mysql_config --cflags --libs`

version: version.c
	$(CC) $^ -o $@ $(CFG)
