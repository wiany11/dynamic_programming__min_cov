#define DEBUGMODE 0

#include "Mincov.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <fstream>

using namespace std;

void readInputFile(char* fileName, targetSet_t* targetSet, sensorSet_t* sensorSet);
void dynamicFindMincov(opt_t* OPT, sensor_t* sensorList, int** M, int row, int column);
int printMincov(opt_t* OPT, int j, int initJ, int** M);		//print all optimal solutions
void printMincov(opt_t* OPT, int j, int** M);				//ptint one optimal solution





int main(int argc, char** argv){
	if(argc != 2){
		cout<<"Please input a filename . . ."<<endl;
		return 1;
	}


   
	targetSet_t targetSet;
	sensorSet_t sensorSet;
	


	readInputFile(argv[1], &targetSet, &sensorSet);


	
	int row = sensorSet.getCountPoint();
	int column = targetSet.getCountPoint();


	
	//align the target and sensor lists
	Heap heap;

	heap.addNodeSet(&sensorSet);	//align sensors in order of increasing x-coordinate by heap sorting
	sensor_t* sensorList = new sensor_t[++row];
	for(int i = 1; i < row; i++)
		sensorList[i] = *(heap.min());

	heap.addNodeSet(&targetSet);	//align targets in order of increasing x-coordinate by heap sorting
	target_t* targetList = new target_t[++column];
	for(int j = 1; j < column; j++)
		targetList[j] = *(heap.min());


	
#if DEBUGMODE==1
for(int i = 1; i < row; i++)
	cout<<sensorList[i].x<<", "<<sensorList[i].y<<endl;
for(int j = 0; j < column; j++)
	cout<<targetList[j].x<<", "<<targetList[j].y<<endl;
#endif



	//set subproblem
	int** M = new int*[row];
	for(int j = 0; j < row; j++)
		M[j] = new int[column];

	M[0][0] = 0;
	for(int i = 0; i < row; i++)		//bountdary condition : 0
		M[i][0] = 0;

	for(int j = 0; j < column; j++)		//bountdary condition : 0
		M[0][j] = 0;

	//fill up the array M
	for(int j = 1; j < column; j++)	
		for(int i = 1; i < row; i++)
			if(sqrt(pow((sensorList[i].x - targetList[j].x), 2) + pow((sensorList[i].y - targetList[j].y), 2)) <= 1)	//a distance between Target[j] and Sensor[i]
				M[i][j] = M[i][j-1] + 1;
			else
				M[i][j] = 0;


	
#if DEBUGMODE==1
for(int j = column - 1; j >= 0; j--){
	for(int i = 0; i < row; i++)
			cout<<M[i][j]<<" ";
	cout<<endl;
}
cout<<endl;
#endif



	opt_t* OPT = new opt_t[column];	//column is the number of targets
	OPT[0].val = 0;					//bountdary condition : 0
	OPT[0].next = NULL;
	
	for(int j = 1; j < column; j++){
		OPT[j].val = FLT_MAX;		//infinity
		OPT[j].next = NULL;
	}


	
	//Find the optimal(minimum cost) solution
	dynamicFindMincov(OPT, sensorList, M, row, column);

	

#if DEBUGMODE==1
optSensor_t* temp;
	for(int j = 0; j < column; j++){
		cout<<"OPT["<<j<<"]("<<OPT[j].val<<") = ";
		temp = OPT[j].next;
		while(temp != NULL){
			cout<<temp->index<<"("<<sensorList[temp->index].cost<<") + OPT["<<j - M[temp->index][j]<<"], ";
			temp = temp->next;
		}
		if(temp == NULL)
			cout<<"NULL";
		cout<<endl;
	}
	cout<<endl;
#endif



	//print the target list and the sensor list
	cout<<"Target set : "<<endl;
	for(int j = 1; j < column; j++)
		cout<<"    Target "<<j<<" = ( "<<targetList[j].x<<", "<<targetList[j].y<<" )"<<endl;

	cout<<endl;
	cout<<"Sensor set : "<<endl;

	for(int i = 1; i < row; i++)
		cout<<"    Sensor "<<i<<" = ( "<<sensorList[i].x<<", "<<sensorList[i].y<<" ), cost = "<<sensorList[i].cost<<endl;
	cout<<endl;

	

	//print all optimal solutions
	//it can possibly take exponentially long time in the case there are many optimal cover sets
	int i=0;
	int flag = 0;		//print the set of sensors of the minimum cost
	cout<<"Cover sets of the minimum cost( "<<OPT[column-1].val<<" ) : "<<endl;
	while(flag == 0){	//there can be more than one optimal solution
		cout<<"    Cover set "<<++i<<" = { ";
		flag = printMincov(OPT, column-1, column-1, M);
		cout<<"\b\b }"<<endl;
	}
	
	//print one optimal solution
	//It take O(n) time
	/*cout<<"Cover sets of the minimum cost( "<<OPT[column-1].val<<" ) = { ";
	printMincov(OPT, column-1, column-1, M);
	cout<<"\b\b }"<<endl;
	*/



	//deallocation of dynamic memory
	delete[] sensorList;
	delete[] targetList;
	
	for(int i = 0; i < row; i++)
		delete[] M[i];
	delete[] M;
	
	optSensor_t* tempOptSensor;
	optSensor_t* tempOptSensorNext; 
	for(int j = 0; j < column; j++){
		tempOptSensor = OPT[j].next;
		while(tempOptSensor != NULL){
			tempOptSensorNext = tempOptSensor->next;
			delete tempOptSensor;
			tempOptSensor = tempOptSensorNext;
		}
	}
	delete[] OPT;



	return 0;
}





void readInputFile(char* fileName, targetSet_t* targetSet, sensorSet_t* sensorSet){
	char buf;
	float pointX, pointY;
	float cost;

	ifstream File(fileName);
	if(!File.is_open()){
		cout<<"The input file is not opened"<<endl;
		exit(0);
	}

	while(!File.eof()){
		buf = File.get();
		if(buf == 't'){
			File>>pointX>>pointY;	//a target has x, y coordinates

			if(pointY >= 0 && pointY <= 1)
				targetSet->addPoint(pointX, pointY);
		}
		else if(buf == 's'){		//a sensor has x, y coordinates and its cost
			File>>pointX>>pointY>>cost;

			if(pointY >= -1 && pointY <= 0)
				sensorSet->addPoint(pointX, pointY, cost);
		}
	}
}



void dynamicFindMincov(opt_t* OPT, sensor_t* sensorList, int** M, int row, int column){
	for(int j = 1; j < column; j++){		//OPT[j] = min( c(i) + OPT[j - M[i][j]] ) for each i(1 <= i <= m, where m is the number of sensors) 
		for(int i = 1; i < row; i++){		//this means m-subprblems -> we can chose min(1 <= 1 <= m)( c(i) + OPT[j - M[i][j]] )
			if((sensorList[i].cost + OPT[j - M[i][j]].val) > 0){	//exception handling
				if(OPT[j].next == NULL){
					OPT[j].next = new optSensor_t;
					OPT[j].next->index = i;
					OPT[j].next->next = NULL;
					OPT[j].next->discovered = 0;

					OPT[j].val = sensorList[i].cost + OPT[j - M[i][j]].val;
				}
				else if(OPT[j].val > (sensorList[i].cost + OPT[j - M[i][j]].val)){
					optSensor_t* tempOptSensor1 = OPT[j].next;
					optSensor_t* tempOptSensor2 = NULL;

					while(tempOptSensor1 != NULL){
						tempOptSensor2 = tempOptSensor1->next;
						delete tempOptSensor1;
						tempOptSensor1 = tempOptSensor2;
					}

					OPT[j].next = new optSensor_t;
					OPT[j].next->index = i;
					OPT[j].next->next = NULL;
					OPT[j].next->discovered = 0;

					OPT[j].val = sensorList[i].cost + OPT[j - M[i][j]].val;
				}
				else if(OPT[j].val == (sensorList[i].cost + OPT[j - M[i][j]].val)){
					optSensor_t* tempOptSensor = OPT[j].next;
					OPT[j].next = new optSensor_t;
					OPT[j].next->index = i;
					OPT[j].next->next = tempOptSensor;
					OPT[j].next->discovered = 0;
					OPT[j].val = sensorList[i].cost + OPT[j - M[i][j]].val;
				}
			}
		}
	}
}



//print all optimal solutions, it take O(n) time per each optimal solution
int printMincov(opt_t* OPT, int j, int initJ, int** M){
	int flag = 0;
	optSensor_t* tempOptSensor = OPT[j].next;

	if(j == 0)
		flag = 1;
	else if(tempOptSensor != NULL){
		while(tempOptSensor->discovered)
			tempOptSensor = tempOptSensor->next;

		flag = printMincov(OPT, j - M[tempOptSensor->index][j], initJ, M);
		cout<<tempOptSensor->index<<", ";

		if(flag == 1){
			flag = 0;

			tempOptSensor->discovered = 1;

			if(tempOptSensor->next == NULL){
				flag = 1;

				tempOptSensor = OPT[j].next;
				while(tempOptSensor != NULL){
					tempOptSensor->discovered = 0;
					tempOptSensor = tempOptSensor->next;
				}
			}
		}
	}

	return flag;
}



//Print one optimal solution, it take O(n) time
void printMincov(opt_t* OPT, int j, int** M){
	if(j == 0)
		return;
	else
		printMincov(OPT, j - M[OPT[j].next->index][j], M);
	
	cout<<OPT[j].next->index<<", ";
}
