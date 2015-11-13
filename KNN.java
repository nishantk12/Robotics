

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;

import weka.classifiers.Classifier;
import weka.core.Instance;
import weka.core.Instances;

public class KNN {
	public static void main(String[] args) {
		performKNN();
	}
	
	public static void performKNN(){
		try {
			//1. Load the classifier
			Classifier cls = (Classifier) weka.core.SerializationHelper.read("/home/ni/Robotics/testbed/Arun.model");
			System.out.println("1");
			
			//2. get the instances for testing
			//2.1 read the data file
			BufferedReader datafile = readDataFile("/home/ni/Robotics/testbed/irisData.csv");
			
			//2.2 Extract data from the file
			Instances data = new Instances(datafile);
			data.setClassIndex(data.numAttributes() - 1);
			
			//2.3 Extract 1st and 2nd data instance from the data set
			Instance first = data.instance(0);
			Instance second = data.instance(92);
			
			double class1 = cls.classifyInstance(first);
			double class2 = cls.classifyInstance(second);
			
			System.out.println("first: " + class1 + "\nsecond: " + class2);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static BufferedReader readDataFile(String filename) {
		BufferedReader inputReader = null;
 
		try {
			inputReader = new BufferedReader(new FileReader(filename));
		} catch (FileNotFoundException ex) {
			System.err.println("File not found: " + filename);
		}
 
		return inputReader;
	}
	

}

