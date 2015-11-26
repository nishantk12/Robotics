#include <jni.h>
#include <iostream>

using namespace std;

jdouble classify(double values[],int sizeOfArray, jclass cls2,jmethodID mid2,JNIEnv *env){
	jdoubleArray outJNIArray = env->NewDoubleArray(sizeOfArray);
	if (NULL != outJNIArray) //TODO Find what is the second variable
		(*env).SetDoubleArrayRegion(outJNIArray, 0 , sizeOfArray, values);
	else
		cout << "ERROR: Java Double Array not created";
	jdouble foundClass = env->CallStaticDoubleMethod(cls2, mid2,outJNIArray);
	if (env->ExceptionCheck()) {
        	env->ExceptionDescribe();
	}
	return foundClass;
}

int main(int n, char *argv[]) {
	
	JavaVM *jvm;
	JNIEnv *env;
	JavaVMInitArgs vm_args;
	JavaVMOption* options = new JavaVMOption[1];
	//TODO Change to System Variable CLASSPATH
	options[0].optionString = "-Djava.class.path=.:/opt/weka/weka.jar"; 

	vm_args.version = JNI_VERSION_1_8;
	vm_args.nOptions = 1;
	vm_args.options = options;
	vm_args.ignoreUnrecognized = false;
	jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
	delete options;
	if (rc != JNI_OK) {
		cin.get();
	}
	jint ver = env->GetVersion();
	cout << "JVM load succeeded: Version " << ((ver>>16)&0x0f) << "."<<(ver&0x0f) << endl;
	
	
	jclass cls2 = env->FindClass("GenericClassifierWeka");
	if(cls2 == NULL) {
		cerr << "ERROR: class not found !" << endl;
	} else {
		cout << "Class GenericClassifierWeka found" << endl;
		jmethodID mid = env->GetStaticMethodID(cls2, "loadClassifier", "(Ljava/lang/String;Ljava/lang/String;I)V");
		if(mid == NULL)
			cerr << "ERROR: Method not found !" << endl;
		else {
			jobject inputStr = env->NewStringUTF("Arun.model");
			jobject inputDataFile = env->NewStringUTF("irisData.csv");
			env->CallStaticVoidMethod(cls2, mid,inputStr,inputDataFile,(jint)5);
 			if (env->ExceptionCheck()) {
                		env->ExceptionDescribe();
			}
			cout << endl << "Model Loaded " << endl;
		}



		jmethodID mid2 = env->GetStaticMethodID(cls2, "classify", "([D)D");
		 
		
		if(mid2 == NULL)
			cerr << "ERROR: Method not found!" << endl;
		else {

			double values[] = {4.8,3.4,1.6,0.2};
			
			int sizeOfArray = sizeof(values)/sizeof(*values);

			int classOfSample = classify(values,sizeOfArray,cls2,mid2,env);
			cout <<  "Class of provided input is " << classOfSample  << endl;
		}
	}
	
	jvm->DestroyJavaVM();
	cout <<  "JVM Destroyed!" << endl;
	return -1;
}


