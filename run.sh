#!/bin/bash
clear
##########################################################################
# VARIABLES TO BE SET BY THE USER
export JAVA_HOME=/usr/lib/jvm/java-8-oracle
export WEKA_HOME=/home/milind/Desktop/JNI_Test/weka-3-6-13
export WORK_DIRECTORY=/home/milind/workspace_robotics/gitRepo/Robotics/
##########################################################################

# EXTENDED VARIABLES
export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64/:$JAVA_HOME/jre/lib/amd64/server
export LIBPATH=$JAVA_HOME/jre/lib/amd64/server/
export CLASSPATH=$WEKA_HOME/weka.jar:$WORK_DIRECTORY
export PATH=$PATH:$WEKA_HOME/weka.jar

echo '---------------------------'
echo 'JAVA_HOME='       $JAVA_HOME
echo 'WEKA_HOME='       $WEKA_HOME
echo 'WORK_DIRECTORY='  $WORK_DIRECTORY
echo ''
echo 'Compilation starts Here:'
echo '---------------------------'
cd $WORK_DIRECTORY
if [ -a GenericClassifierWeka.class ]
	then
		echo 'Compiling Java'
		rm GenericClassifierWeka.class
		
fi
javac GenericClassifierWeka.java
if [ -a executable ]
	then
		echo 'Compiling C++'
		rm executable
		
fi
g++ -Wno-write-strings -I $JAVA_HOME/include/ -I $JAVA_HOME/include/linux/ $WORK_DIRECTORY/GenericClassifierWeka.cpp -o $WORK_DIRECTORY/executable -L${LIBPATH} -ljvm 

echo 'Execution starts here:'
echo '---------------------------'
./executable Arun.model irisData.csv 5
