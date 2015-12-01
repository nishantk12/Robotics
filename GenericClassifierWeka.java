import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import weka.classifiers.Classifier;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.SerializationHelper;


public class GenericClassifierWeka {

	private static Classifier cls;
	private static Instances data = null;
	private static Integer numOfAttributes;
	private static Integer indexOfClassAttr;

	public static int loadClassifier(String modelFileName, String dataFile, int indexOfClass) throws Exception{
		System.out.println();
		// 1. READ CLASSIFIER MODEL
		//try {
			cls = (Classifier) SerializationHelper.read(modelFileName);
		//} catch (FileNotFoundException fileNotFoundException) {
		//	System.out.println(fileNotFoundException.getMessage());
		//	return 0;
		//}
		
		if(cls!=null){
			// 2. READ DATAFILE FOR INSTANCES
			BufferedReader datafile = readDataFile(dataFile);
			// 3. Extract data from the file
			try {
				data = new Instances(datafile);
			} catch (IOException e) {
				e.printStackTrace();
				return 0;
			}
			// 4. SET COLUMN OF CLASS IN DATA SET
			data.setClassIndex(data.numAttributes() - 1);
			numOfAttributes = data.numAttributes();
			System.out.println("Number of attr: "+ numOfAttributes);

			if (indexOfClass - 1 < numOfAttributes) {
				indexOfClassAttr = indexOfClass;
			} else {
				System.out.println("ERROR: Class attribute index inconsistent");
				return 0;
			}
			System.out.println("Class of model: "+cls.getClass());
			return numOfAttributes;
		}else{
			System.out.println("Some error in loading model file");
			return -1;
		}
	}

	public static double classify(double[] arr) throws Exception {
		if(arr==null || arr.length==0){
			System.out.println("ERROR: Error in passed array");
			return -1;
		}
		//1. Check the number of attributes are consistent or not
		if (arr.length != numOfAttributes - 1) {
			System.out.println("ERROR: Number of attributes are inconsistent");
			return -1;
		}
		
		// 2. Create an Instance Object for classifying
		Instance ins = new Instance(numOfAttributes);
		// 3. Assign the dataset of Weka Model.
		ins.setDataset(data);

		// 4. Set Attributes of the Instance
		for (int i = 0; i < numOfAttributes; i++) {
			if (i != indexOfClassAttr - 1) {
				ins.setValue(i, arr[i]);
			} else {
				i++;
			}
		}
		// 5. Set the missing attribute //TODO Are MIssing parameters allowed in any classification ASK PROF
		ins.setMissing(indexOfClassAttr - 1);

		return cls.classifyInstance(ins);
	}

	private static BufferedReader readDataFile(String filename) {
		BufferedReader inputReader = null;

		try {
			inputReader = new BufferedReader(new FileReader(filename));
		} catch (FileNotFoundException ex) {
			System.out.println("ERROR: File not found: " + filename);
		}

		return inputReader;
	}
}
