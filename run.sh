#!/bin/bash
clear

export JAVA_HOME=/usr/lib/jvm/java-8-oracle
export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/amd64/:$JAVA_HOME/jre/lib/amd64/server
LIBPATH=$JAVA_HOME/jre/lib/amd64/server/
export WORK_DIR=/home/milind/workspace_robotics/CPP/
export WEKA_HOME=/home/milind/Desktop/JNI_Test/weka-3-6-13
export CLASSPATH=$WEKA_HOME/weka.jar
export PATH=$PATH:$WEKA_HOME/weka.jar

echo '---------------------------'
echo 'PATH=' $PATH
echo 'CLASSPATH='$CLASSPATH
echo ''
echo 'Compilation starts Here:'
echo '---------------------------'

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
g++ -Wno-write-strings -I $JAVA_HOME/include/ -I $JAVA_HOME/include/linux/ GenericClassifierWeka.cpp -o executable -L${LIBPATH} -ljvm 

echo 'Execution starts here:'
echo '---------------------------'
./executable