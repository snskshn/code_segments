VPATH = test
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) test.o

#%.o: %.c
.c.o:
	$(CC) -DDEBUG -c -o $@ $<

all: diary
diary: $(OBJECTS)
	$(CC) -o $@ $^
clean:
	rm -rf *.o diary

# miscellaneous tests
echo:
	@echo "hi"
echo_name:
	for NAME in woos test; do echo $${NAME}; done
ifeq_test:
ifeq ($(CC),gcc)
	@echo "GNU"
else
	@echo "CC"
endif
.DEFAULT:
	@echo ".DEFAULT target test!"
