#include <jni.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>


using namespace std;

class GenericClassifierWeka {
	 JavaVM *jvm;
	 JNIEnv *env;
	 JavaVMInitArgs vm_args;
	 JavaVMOption* jvm_options;
	
	 jclass jclass_classifier;
	 jmethodID jmethodId_loadModel;
	 jmethodID jmethodId_classify;
	 int numberOfVariables;
	
	public:
	void loadClassifier(char* modelFilePath, char* dataFilePath, int indexOfClass) {
		jvm_options  = new JavaVMOption[1];
		const char *c_cpkey = "-Djava.class.path=";
		const char *c_cpvalue = getenv("CLASSPATH");
		char c_classpath[18 + strlen(c_cpvalue)];
		strcpy(c_classpath , c_cpkey);
		strcat(c_classpath , c_cpvalue);
		jvm_options[0].optionString = c_classpath;
		jthrowable exc;

		vm_args.version = JNI_VERSION_1_6;
		vm_args.nOptions = 1;
		vm_args.options = jvm_options;
		vm_args.ignoreUnrecognized = false;
		jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
		delete jvm_options;	//TODO delete others as well

		if (rc != JNI_OK) {
			cin.get();
		}
		jint ver = env->GetVersion();
		cout << "JVM load succeeded: Version " << ((ver>>16)&0x0f) << "."<<(ver&0x0f) << endl;
		jclass_classifier = env->FindClass("GenericClassifierWeka");
		if(jclass_classifier == NULL) {
			cerr << "ERROR: class not found !" << endl;
		} else {
			cout << "Class GenericClassifierWeka found" << endl;
			jmethodId_loadModel = env->GetStaticMethodID(jclass_classifier, 
				"loadClassifier", "(Ljava/lang/String;Ljava/lang/String;I)I");
			if(jmethodId_loadModel == NULL)
				cerr << "ERROR: Method not found !" << endl;
			else {
				jobject inputStr = env->NewStringUTF(modelFilePath);
				jobject inputDataFile = env->NewStringUTF(dataFilePath);
				jint nov = env->CallStaticIntMethod(
					jclass_classifier, jmethodId_loadModel,inputStr,inputDataFile,(jint)indexOfClass);
				
 				if (env->ExceptionCheck()) {
					cout << endl << "Model Not Loaded " << endl;
                			env->ExceptionDescribe();
					//exc = env->ExceptionOccurred(env); TODO : print execption as a single string
					exit(1);
				}
				numberOfVariables = nov;
				cout << endl << "Model Loaded " << endl;
			}
			jmethodId_classify = env->GetStaticMethodID(jclass_classifier, "classify", "([D)D");
			if(jmethodId_classify == NULL)
				cerr << "ERROR: Method 'classify' not found!" << endl;
		}		
	}

	double classify(vector<double> inst) {
		double arr[inst.size()];
		std::copy(inst.begin(), inst.end(), arr);

		jdoubleArray outJNIArray = env->NewDoubleArray(inst.size());
		if (NULL != outJNIArray) //TODO Find what is the second variable
			(*env).SetDoubleArrayRegion(outJNIArray, 0 , inst.size(), arr);
		else
			cout << "ERROR: Java Double Array not created";
		jdouble classOfVector = env->CallStaticDoubleMethod(jclass_classifier , jmethodId_classify , outJNIArray);
		if (env->ExceptionCheck()) {
	             	env->ExceptionDescribe();
		}
		return classOfVector;
	}

	double classify(double *inst, int size) {
		//double arr[inst.size()];
		//std::copy(inst.begin(), inst.end(), arr);

		jdoubleArray outJNIArray = env->NewDoubleArray(size);
		if (NULL != outJNIArray) //TODO Find what is the second variable
			(*env).SetDoubleArrayRegion(outJNIArray, 0 , size, inst);
		else
			cout << "ERROR: Java Double Array not created";
		jdouble classOfVector = env->CallStaticDoubleMethod(jclass_classifier , jmethodId_classify , outJNIArray);
		if (env->ExceptionCheck()) {
	             	env->ExceptionDescribe();
		}
		return classOfVector;
	}
};


