OBJ : sfprimetable.o\
      sfmemcap.o \
      sfhashfcn.o \
      sfxhash.o 
#DEP
sfxhash.o : sfhashfcn.c sfhashfcn.h sfprimetable.h
sfhashfcn.o : sfhashfcn.c sfhashfcn.h sfprimetable.h
sfmemcap.o : sfmemcap.c sfmemcap.h 
sfprimetable.o : sfprimetable.c sfprimetable.h 

.c: 
	gcc -Wall -g $? 

clean:
	rm -rf *.o
