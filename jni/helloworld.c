#include <jni.h>
#include "HelloWorld.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_HelloWorld_printHelloWorld(JNIEnv *env, jobject obj)
{
	printf("Hello world!\n");

	return;
}
