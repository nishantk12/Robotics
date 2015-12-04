import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.File;

import weka.classifiers.Classifier;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.SerializationHelper;


public class GenericClassifierWeka {

	private static Classifier cls;
	private static Instances data = null;
	private static Integer numOfAttributes;
	private static Integer indexOfClassAttr;

	public static int loadClassifier(String modelFilePath, String propertyFilePath, int indexOfClass) throws Exception{
		//0.0 Check indexOfClass
		if(indexOfClass < 1){
			throw new Exception("Index of Class cannot be less than 1");
		}
	
		//0.1 Check if file is present at given path model file path
		try{
			if(modelFilePath==null || modelFilePath.equals("")){
				throw new Exception("Please provide a correct path for model file");
			}
			File modelFile = new File(modelFilePath);
			if(!modelFile.exists() || modelFile.isDirectory()) { 
			    throw new Exception("Provided path \"" + modelFilePath + "\" is not a file");
			}
		}catch(NullPointerException nullPointerException){
			throw new Exception("Model File path not provided");
		}

		//0.2 Check if file is present at given properties file path
		try{
			if(propertyFilePath==null || propertyFilePath.equals("")){
				throw new Exception("Please provide a correct path for property file");
			}
			File propertyFile = new File(propertyFilePath);
			if(!propertyFile.exists() || propertyFile.isDirectory()) { 
			    throw new Exception("Provided path \"" + propertyFilePath + "\" is not a file");
			}
		}catch(NullPointerException nullPointerException){
			throw new Exception("Property File path not provided");
		}

		// 1. READ CLASSIFIER MODEL
		try {
			cls = (Classifier) SerializationHelper.read(modelFilePath);
		} catch (Exception ex) {
			throw new Exception("File present at \"" + modelFilePath + "\" is not a model file");
		}
		
		// 2. READ DATAFILE FOR INSTANCES
		BufferedReader propertyfile = new BufferedReader(new FileReader(propertyFilePath));

		// 3. Extract data from the file
		try{
			data = new Instances(propertyfile);
		} catch (IOException iOException) {
			throw new Exception("Problem reading the property file : \n\t" + iOException.getMessage());
		}
		

		// 4. SET COLUMN OF CLASS IN DATA SET
		numOfAttributes = data.numAttributes();
		if (indexOfClass - 1 < numOfAttributes) {
			data.setClassIndex(indexOfClass-1);
			indexOfClassAttr = indexOfClass;
		} else {
			throw new Exception("As per property file, index of Class must be between 1 and "
						 + numOfAttributes + " and provided is " + indexOfClass);
		}
		
		return numOfAttributes;
	}

	public static double classify(double[] candidateArray) throws Exception {
		if(candidateArray==null || candidateArray.length==0){
			throw new Exception("Input Array must not be empty");
		}
		//1. Check the number of attributes are consistent or not
		if (candidateArray.length != numOfAttributes-1) {
			throw new Exception("Size mismatch : Input array provided is of size " + candidateArray.length + 
						" and expected is " + String.valueOf(numOfAttributes-1));
		}
		// 2. Create an Instance Object for classifying
		Instance instance = new Instance(numOfAttributes);

		// 3. Assign the dataset of Weka Model.
		instance.setDataset(data);

		// 4. Set Attributes of the Instance
		int attributeIndex = 0;
		for (int index = 0; index < candidateArray.length;index++) {
			if(index == indexOfClassAttr-1){
				attributeIndex++;
			}
			instance.setValue(attributeIndex,candidateArray[index]);
			attributeIndex++;			
		}
		// 5. Set the missing attribute //TODO Are MIssing parameters allowed in any classification ASK PROF
		//ins.setMissing(indexOfClassAttr - 1);
		try{
			return cls.classifyInstance(instance);
		}catch(ArrayIndexOutOfBoundsException arrayIndexOutOfBoundsException){
			throw new Exception("Probable mismatch of class Index : " + indexOfClassAttr + 
					" was provided as input,\n please check in model and property file");
		}
	}
}
