#include <jni.h>
#include <iostream>

using namespace std;

int main(int n, char *argv[]) {
	
	JavaVM *jvm;
	JNIEnv *env;
	JavaVMInitArgs vm_args;
	JavaVMOption* options = new JavaVMOption[1];
	//TODO Change to System Variable CLASSPATH
	options[0].optionString = "-Djava.class.path=.:/home/milind/Desktop/JNI_Test/weka-3-6-13/weka.jar"; 

	vm_args.version = JNI_VERSION_1_6;
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
			jobject inputStr = env->NewStringUTF("/home/milind/workspace_robotics/CPP/Arun.model");
			jobject inputDataFile = env->NewStringUTF("/home/milind/workspace_robotics/CPP/irisData.csv");
			env->CallStaticVoidMethod(cls2, mid,inputStr,inputDataFile,(jint)5);
 			if (env->ExceptionCheck()) {
                		env->ExceptionDescribe();
			}
			cout << endl << "Model Loaded " << endl;
		}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		jmethodID mid2 = env->GetStaticMethodID(cls2, "classify", "([D)D");
		 
		
		if(mid2 == NULL)
			cerr << "ERROR: Method not found!" << endl;
		else {

			const double values[] = {4.8,3.4,1.6,0.2};

			jdoubleArray outJNIArray = env->NewDoubleArray(sizeof(values)/sizeof(*values));
			if (NULL != outJNIArray) //TODO Find what is the second variable
				(*env).SetDoubleArrayRegion(outJNIArray, 0 , sizeof(values)/sizeof(*values), values);
			else
				cout << "ERROR: Java Double Array not created";
			jdouble j = env->CallStaticDoubleMethod(cls2, mid2,outJNIArray);
 			if (env->ExceptionCheck()) {
                		env->ExceptionDescribe();
			}
			cout << endl << "Class : " << j << endl;



////////////////////////////////
			const double value1[] = {6.0,2.9,4.5,1.5};

			jdoubleArray outJNIArray1 = env->NewDoubleArray(sizeof(value1)/sizeof(*value1));
			if (NULL != outJNIArray1) //TODO Find what is the second variable
				(*env).SetDoubleArrayRegion(outJNIArray1, 0 , sizeof(value1)/sizeof(*value1), value1);
			else
				cout << "ERROR: Java Double Array not created";
			jdouble j1 = env->CallStaticDoubleMethod(cls2, mid2,outJNIArray1);
 			if (env->ExceptionCheck()) {
                		env->ExceptionDescribe();
			}
			cout << endl << "Class : " << j1 << endl;


/////////////////////////////////////
			const double value2[] = {7.2,3.2,6.0,1.8};

			jdoubleArray outJNIArray2 = env->NewDoubleArray(sizeof(value2)/sizeof(*value2));
			if (NULL != outJNIArray2) //TODO Find what is the second variable
				(*env).SetDoubleArrayRegion(outJNIArray2, 0 , sizeof(value2)/sizeof(*value2), value2);
			else
				cout << "ERROR: Java Double Array not created";
			jdouble j2 = env->CallStaticDoubleMethod(cls2, mid2,outJNIArray2);
 			if (env->ExceptionCheck()) {
                		env->ExceptionDescribe();
			}
			cout << endl << "Class : " << j2 << endl;




		}
	}
	
	jvm->DestroyJavaVM();
	cout <<  "JVM Destroyed!" << endl;
	return -1;
}
