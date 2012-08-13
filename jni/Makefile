all: HelloWorld libHelloWorld.so

libHelloWorld.so: helloworld.c
	gcc -shared -I/usr/lib/jvm/java-6-sun-1.6.0.26/include -I/usr/lib/jvm/java-6-sun-1.6.0.26/include/linux -o $@ helloworld.c

HelloWorld.class: HelloWorld.java
	javac HelloWorld.java

HelloWorld.h: HelloWorld.class
	javah HelloWorld

HelloWorld: HelloWorld.h HelloWorld.class

clean:
	rm -f HelloWorld.h HelloWorld.class libHelloWorld.so
