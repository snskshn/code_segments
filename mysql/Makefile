CC=gcc
CFG=`mysql_config --cflags --libs`

all: version create_db

version: version.c
	$(CC) $^ -o $@ $(CFG)

create_db: create_db.c
	$(CC) $^ -o $@ $(CFG)
