#include <iostream>

using namespace std;

class Heap;

typedef struct Point{
	float x, y;
	float cost;
	Point* next;
}target_t, sensor_t;

typedef class PointSet{
private:
	target_t* firstPoint;
	int countPoint;

	friend class Heap;
public:
	PointSet();
	PointSet(target_t* firstPoint);
	~PointSet();

	int getCountPoint();
	
	void addPoint(target_t* nextPoint);	//add a target point at the very fist of the target list -> O(1) time to add a target
	void addPoint(float pointX, float pointY);
	void addPoint(float pointX, float pointY, float cost);

	void printAll();
}targetSet_t, sensorSet_t;

typedef struct HeapNode{
	Point* point;

	HeapNode* parent;
	HeapNode* leftChild;
	HeapNode* rightChild;
}hnode_t;

class Heap{
private:
	hnode_t* root;
	hnode_t* endParent;	//to add a node
	hnode_t* last;	//a node that is input most recently
public:
	Heap();

	hnode_t* getRoot();
	Point* min();

	void addNode(Point* point);
	void addNodeSet(PointSet* pointSet);
	void resetEndParent();
	void resetLast();

	void printAll(hnode_t* tempHnode);
};

typedef struct OptSensor{
	int index;
	OptSensor* next;

	int discovered;
}optSensor_t;

typedef struct Opt{
	float val;
	OptSensor* next;
}opt_t;