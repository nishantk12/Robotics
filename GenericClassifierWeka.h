#include <jni.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>


using namespace std;
// The GenericClassifierWeka.h is used as the C++ interface
// to access Weka models. The class allows trained Weka models
// to be used for classification purposes. The Class provides two method
// calls in total. These are loadClassifier and classify. The header file
// needs to be included aGenericClassifierWeka.h" at the top of your C++
// driver program.

class GenericClassifierWeka {

	 JavaVM *javaVM;
	 JNIEnv *jniEnv;
	 JavaVMInitArgs javaVMInitArgs;
	 JavaVMOption* jvmOptions;
	
	 jclass jclassClassifier;
	 jmethodID jmethodIdLoadModel;
	 jmethodID jmethodIdForClassification;
	 static const int JVM_NO_OF_OPTIONS = 1;
	 int numberOfModelAttributes ;

// The method loadClassifier provieds an interface to load the classifier
// in your driver program. The class takes 3 inputs namely:
// inputModelFilePath : the path of your Weka model 
// inputPropertyFilePath : The path of your DataFile , weka needs the propertyfile to ??
// indexOfClass : 
// Example loadClassifier("/myFolder/Weka.model","/myFolder/myData.property", 1);	
// The method only sets the Classpath parameter while initailaizing the JVM
// User may add set more option parameter to the JVM by modifying the heaader file
// The method throws four Exceptions such as the Java class GenericWeka not found,
// loadClassifier method not found, model not loaded and method classify not found.
	
	public:
	void loadClassifier(char* inputModelFilePath, char* inputDataFilePath, int indexOfClass) {
		
		jvmOptions  = new JavaVMOption[JVM_NO_OF_OPTIONS];
		const char *C_CLASSPATH_KEY = "-Djava.class.path=";
		const char *C_CLASSPATH_VALUE = getenv("CLASSPATH");
		const int C_CLASSPATH_KEY_LENGTH = 18;
		char c_classpath[C_CLASSPATH_KEY_LENGTH + strlen(C_CLASSPATH_VALUE)];
		const bool INGNORE_UNRECOGNIZED = false;


		strcpy(c_classpath , C_CLASSPATH_KEY);
		strcat(c_classpath , C_CLASSPATH_VALUE);

		jvmOptions[0].optionString = c_classpath;
		jthrowable exc;

		javaVMInitArgs.version = JNI_VERSION_1_6;
		javaVMInitArgs.nOptions = JVM_NO_OF_OPTIONS;
		javaVMInitArgs.options = jvmOptions;
		javaVMInitArgs.ignoreUnrecognized = INGNORE_UNRECOGNIZED;


		jint  javaVMCreate = JNI_CreateJavaVM(&javaVM, (void**)&jniEnv, &javaVMInitArgs);
		delete jvmOptions;	

		if (javaVMCreate != JNI_OK) {
			cin.get();
		}


		//jint version = jniEnv->GetVersion();
		//cout << "JVM load succeeded: Version " << ((version>>16)&0x0f) << "."<<(version&0x0f) << endl;
		jclassClassifier = jniEnv->FindClass("GenericClassifierWeka");

		if(jclassClassifier == NULL) {
			 	throw "ERROR: java class GenericClassifierWeka not found ";
			
		} else {
				//cout << "Class GenericClassifierWeka found" << endl;
				jmethodIdLoadModel = jniEnv->GetStaticMethodID(jclassClassifier,"loadClassifier", "(Ljava/lang/String;Ljava/lang/String;I)I");
				
				if(jmethodIdLoadModel == NULL){
				   	 throw "ERROR: java Method loadClassifier of class GenericClassifierWeka not found !";
				}else {
						
					jobject  modelFilePath = jniEnv->NewStringUTF(inputModelFilePath);
					jobject  dataFilePath = jniEnv->NewStringUTF(inputDataFilePath);
					jint numberOfModelAttributes = jniEnv->CallStaticIntMethod(jclassClassifier,jmethodIdLoadModel,modelFilePath,dataFilePath,(jint)indexOfClass);
						
 					if (jniEnv->ExceptionCheck()) {
						jniEnv->ExceptionDescribe();
						throw " ERROR : Model Not Loaded ";
						exit(1);
					}
						
			}
			
			jmethodIdForClassification = jniEnv->GetStaticMethodID(jclassClassifier, "classify", "([D)D");
			if(jmethodIdForClassification == NULL){
				throw "ERROR: Method 'classify' not found!";
			}
		}		
}

// The method classify is overloaded to accept 
// vector and array inputs. The method takes
// a double array ( along with it's size) or a
// vector and returns the class corresponding 
// to the feature vector ( or array).
// The method throws 3 exceptions namely,
// No data in the vector, JavaDouble Array Creation Failed,
// class of the vector could not be found.
// Example of method call : classify(inputVector);

	double classify(vector<double> inputVector) {

		int inputVectorsize = inputVector.size(); 

		if(inputVectorsize==0) {
			throw "ERROR : No data in the vector";
			exit(1);
		}

		double doubleArray[inputVectorsize];

		std::copy(inputVector.begin(), inputVector.end(), doubleArray);

		jdoubleArray outJNIArray = jniEnv->NewDoubleArray(inputVectorsize);

		if (NULL != outJNIArray) {
			(*jniEnv).SetDoubleArrayRegion(outJNIArray, 0 , inputVectorsize, doubleArray);
		}
		else{
			 throw "ERROR : Java Double Array Creation Failed";
		}

		jdouble classOftheVector = jniEnv->CallStaticDoubleMethod(jclassClassifier , jmethodIdForClassification, outJNIArray);

		if (jniEnv->ExceptionCheck()) {
			jniEnv->ExceptionDescribe();
	             	throw "ERROR : Class of the Vector could not be found";
		}
		return classOftheVector;
	}

// The method classify that takes a double array and its size
// as input. Returns the class corresponding to the feature
// array.
	double classify(double *inputArray, int inputArraySize) {

		if(inputArraySize==0){
			throw "ERROR : No data in the array";
			exit(1);
		}
		
		jdoubleArray outJNIArray = jniEnv->NewDoubleArray(inputArraySize);
		if (NULL != outJNIArray) {
			(*jniEnv).SetDoubleArrayRegion(outJNIArray, 0 , inputArraySize, inputArray);
		}else{
			throw "ERROR : Java Double Array not created";
		}

		jdouble classOftheVector = jniEnv->CallStaticDoubleMethod(jclassClassifier , jmethodIdForClassification , outJNIArray);
		if (jniEnv->ExceptionCheck()) {
			jniEnv->ExceptionDescribe();
	             	throw "ERROR : Class of the Vector could not be found";
		}
		return classOftheVector;
	}
};


