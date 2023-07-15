

#include "pch.h"
#include "jni-hello.h"
#include <iostream>

using namespace std;

JNIEXPORT void JNICALL Java_com_xyz_java_base_jni_TestHello_hello(JNIEnv *, jclass, jstring){
	cout << "Hello World!" << endl;
 }

