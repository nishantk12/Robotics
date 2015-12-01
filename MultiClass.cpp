#include "GenericClassifierWeka.h"
using namespace std;	

int main(int n, char *argv[]) {
	
	GenericClassifierWeka classifier;
	
	
	
	classifier.loadClassifier("/home/ni/Robotics/CPP/Robotics/Arun.model",
				 "/home/ni/Robotics/CPP/Robotics/irisData.csv",5);
	//double values[] = {4.8,3.4,1.6,0.2};
	double p1[] = {5.0,3.6,1.4,0.2};
	int sizeOfVector = sizeof(p1)/sizeof(*p1);
	vector<double> a1(p1, p1+4);   
	double classOfInstance = classifier.classify(p1,sizeOfVector);
	cout << "Class of instance : " << classOfInstance << endl;

	double p2[] = {7.0,3.2,4.7,1.4};
	vector<double> a2(p2, p2+4);   
	classifier.classify(a2);

	double p3[] = {6.3,3.3,6.0,2.5};
	vector<double> a3(p3, p3+4);   
	classifier.classify(a3);
	
	double p4[] = {1.0,1.2,1.4,1.6};
	vector<double> a4(p4, p4+4);   
	classifier.classify(a4);

	return -1;
}


