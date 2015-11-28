#!/bin/bash
clear

##########################################################################
# VARIABLES TO BE SET BY THE USER
export JAVA_HOME=/usr/lib/jvm/java-8-oracle
export WORK_DIRECTORY=/home/milind/workspace_robotics/gitRepo/Robotics/
export WEKA_HOME=/home/milind/Desktop/JNI_Test/weka-3-6-13
##########################################################################

# EXTENDED VARIABLES
export LIBPATH=$JAVA_HOME/jre/lib/amd64/server/
export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64/:$JAVA_HOME/jre/lib/amd64/server
export CLASSPATH=$CLASSPATH:$WORK_DIRECTORY:$WEKA_HOME/weka.jar
export PATH=$PATH:$WEKA_HOME/weka.jar

if [ -z ${WEKA_HOME} ]; 
	then 
		echo "ERROR: WEKA_HOME is not set"; 
		exit 1;
	else 
		echo "INFO: WEKA_HOME:" $CLASSPATH; 	
fi

if [ -z ${JAVA_HOME} ]; 
	then 
		echo "ERROR: JAVA_HOME is not set"; 
		exit 1;
	else 
		echo "INFO: JAVA_HOME:" $JAVA_HOME; 	
fi

if [ -z ${WORK_DIRECTORY} ]; 
	then 
		echo "ERROR: WORK_DIRECTORY is not set"; 
		exit 1;
	else 
		echo "INFO: WORK_DIRECTORY:" $WORK_DIRECTORY; 	
fi

if [ -a GenericClassifierWeka.class ]
	then
		rm GenericClassifierWeka.class		
fi
javac GenericClassifierWeka.java

if [ -a executable ]
	then
		rm executable	
fi
g++ -Wno-write-strings -I $JAVA_HOME/include/ -I $JAVA_HOME/include/linux/ GenericClassifierWeka.cpp -o executable -L${LIBPATH} -ljvm 


./executable /home/milind/workspace_robotics/CPP/Arun.model /home/milind/workspace_robotics/CPP/irisData.csv 5
