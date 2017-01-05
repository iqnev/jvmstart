/**
 * Copyright (c) 2017 Ivelin Yanev .
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA
 **/

//============================================================================
// Name        : JVMStart.cpp
// Author      : Ivelin Yanev
// Version     : 1.0
// Since       : 2017
// Description : Construct a JVM from C++ and invoke Java method.
//============================================================================
#include <iostream>
#include <cstddef>
#include <jni.h>
using namespace std;

/**
 * Load and initialize Java VM.
 * @param jvm
 */
JNIEnv* create_vm(JavaVM **jvm);


/**
 * Calling Java method.
 */
void invoke_class(JNIEnv* env);

int main() {
  JavaVM *jvm;
  JNIEnv *env;

  env = create_vm(&jvm);
  invoke_class(env);

  jvm->DestroyJavaVM();

  return 0;
}

JNIEnv* create_vm(JavaVM **jvm) {
  JNIEnv *env;

  // Initialization arguments
  JavaVMInitArgs vm_args;
  JavaVMOption* options = new JavaVMOption[1];
  options[0].optionString = const_cast<char*>("-Djava.class.path=.");
  vm_args.version = JNI_VERSION_1_8;
  vm_args.nOptions = 1;
  vm_args.options = options;
  vm_args.ignoreUnrecognized = false;

  jint vm = JNI_CreateJavaVM(jvm, (void**) &env, &vm_args);

  delete options;
  if (vm != JNI_OK) {
    cin.get();
    exit(1);
  }

  return env;
}

void invoke_class(JNIEnv* env) {

  cout << "JVM version: ";
  jint version = env->GetVersion();
  cout << ((version >> 16) & 0x0f) << "." << (version & 0x0f) << endl;

  jclass clazz = env->FindClass("SimpleTest");
  if (clazz == nullptr) {
    cerr << "ERROR: class not found !";
  } else {
    cout << "Class MyTest found" << endl;
    jmethodID jMethod = env->GetStaticMethodID(clazz, "customMain", "()V");
    if (jMethod == nullptr)
      cerr << "ERROR: method void mymain() not found !" << endl;
    else {
      env->CallStaticVoidMethod(clazz, jMethod);
      cout << endl;
    }
  }
}
