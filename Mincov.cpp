#include "Mincov.h"

PointSet::PointSet(){
	this->firstPoint = NULL;
	this->countPoint = 0;
}
PointSet::PointSet(Point* firstPoint){
	this->firstPoint = firstPoint;
	this->countPoint = 0;
};
PointSet::~PointSet(){
	Point* tempPoint;
	Point* tempPointNext;

	tempPoint = this->firstPoint;
	while(tempPoint != NULL){
		tempPointNext = tempPoint->next;
		delete tempPoint;
		tempPoint = tempPointNext;
	}
}
int PointSet::getCountPoint(){
	return this->countPoint;
}
void PointSet::addPoint(Point* nextPoint){
	if(nextPoint->y <= -1 || nextPoint->y >=1)	//ignore the point whose y-coordinate is not within (0,1)
		return;

	Point* tempPoint = this->firstPoint;
	this->firstPoint = nextPoint;
	this->firstPoint->next = tempPoint;

	(this->countPoint)++;
};
void PointSet::addPoint(float pointX, float pointY){
	if(pointY <= -1 || pointY >=1)	//ignore the point whose y-coordinate is not within (0,1)
		return;

	Point* nextPoint = new Point;
	nextPoint->x = pointX;
	nextPoint->y = pointY;
	nextPoint->cost = 0;
	nextPoint->next = NULL;

	this->addPoint(nextPoint);
};
void PointSet::addPoint(float pointX, float pointY, float cost){
	if(pointY <= -1 || pointY >=1)	//ignore the point whose y-coordinate is not within (0,1)
		return;

	Point* nextPoint = new Point;
	nextPoint->x = pointX;
	nextPoint->y = pointY;
	nextPoint->cost = cost;
	nextPoint->next = NULL;

	this->addPoint(nextPoint);
};
void PointSet::printAll(){
	int i = 0;
	Point* tempPoint = this->firstPoint;

	while(tempPoint != NULL){
		cout<<"Point("<<++i<<") = ("<<tempPoint->x<<", "<<tempPoint->y<<") : "<<tempPoint->cost<<endl;
		tempPoint = tempPoint->next;
	}
}

Heap::Heap(){
	this->root = NULL;
	this->endParent = NULL;
	this->last = NULL;
}
hnode_t* Heap::getRoot(){
	return this->root;
}
Point* Heap::min(){
	Point* tempMin;

	if(this->root == NULL)		//0 node
		return NULL;
	else if(this->last == this->root){	//1 node
		tempMin =  this->root->point;

		delete this->root;

		this->root = NULL;
		this->endParent = NULL;
		this->last = NULL;

		return tempMin;
	}
	else if(this->endParent->leftChild != NULL && this->endParent->rightChild == NULL){
		tempMin = this->root->point;

		this->root->point = this->last->point;

		delete this->last;

		this->endParent->leftChild = NULL;

		this->last = this->endParent;
		this->resetLast();
	}
	else if(this->endParent->leftChild == NULL && this->endParent->rightChild == NULL){
		tempMin = this->root->point;

		this->root->point = this->last->point;

		this->endParent = this->last->parent;

		delete this->last;

		this->endParent->rightChild = NULL;
		this->last = this->endParent->leftChild;
	}

	hnode_t* tempHnode = this->root;
	Point* tempPoint;
	while(!(tempHnode->leftChild == NULL && tempHnode->rightChild == NULL)){
		if(tempHnode->leftChild != NULL && tempHnode->rightChild == NULL){
			if(tempHnode->leftChild->point->x < tempHnode->point->x){
				tempPoint = tempHnode->point;
				tempHnode->point = tempHnode->leftChild->point;
				tempHnode->leftChild->point = tempPoint;
				
				tempHnode = tempHnode->leftChild;
			}
			else
				break;
		}
		else if(tempHnode->leftChild == NULL && tempHnode->rightChild != NULL){
			if(tempHnode->rightChild->point->x < tempHnode->point->x){
				tempPoint = tempHnode->point;
				tempHnode->point = tempHnode->rightChild->point;
				tempHnode->rightChild->point = tempPoint;

				tempHnode = tempHnode->rightChild;
			}
			else
				break;		
		}
		else if(tempHnode->leftChild != NULL && tempHnode->rightChild != NULL){
			if(tempHnode->leftChild->point->x < tempHnode->rightChild->point->x){
				if(tempHnode->leftChild->point->x < tempHnode->point->x){
					tempPoint = tempHnode->point;
					tempHnode->point = tempHnode->leftChild->point;
					tempHnode->leftChild->point = tempPoint;
					
					tempHnode = tempHnode->leftChild;
				}
				else
					break;				
			}
			else{
				if(tempHnode->rightChild->point->x < tempHnode->point->x){
					tempPoint = tempHnode->point;
					tempHnode->point = tempHnode->rightChild->point;
					tempHnode->rightChild->point = tempPoint;

					tempHnode = tempHnode->rightChild;
				}
				else
					break;		
			}
			
		}
	}


	return tempMin;
}
void Heap::addNode(Point* point){
	if(this->root == NULL){
		this->root = new hnode_t;
			this->root->point = point;
			this->root->parent = NULL;
			this->root->leftChild = NULL;
			this->root->rightChild = NULL;
		this->endParent = this->root;
		this->last = this->root;
	}
	else{
		hnode_t* tempHnode;

		if(this->endParent->leftChild == NULL){
			this->endParent->leftChild = new hnode_t;
				this->endParent->leftChild->point = point;
				this->endParent->leftChild->parent = NULL;
				this->endParent->leftChild->leftChild = NULL;
				this->endParent->leftChild->rightChild = NULL;
			this->endParent->leftChild->parent = this->endParent;
			tempHnode = this->endParent->leftChild;
		}
		else{	//this->endParent->rightChild == NULL
			this->endParent->rightChild = new hnode_t;
				this->endParent->rightChild->point = point;
				this->endParent->rightChild->parent = NULL;
				this->endParent->rightChild->leftChild = NULL;
				this->endParent->rightChild->rightChild = NULL;
			this->endParent->rightChild->parent = this->endParent;
			tempHnode = this->endParent->rightChild;

			this->resetEndParent();
		}

		this->last = tempHnode;

		//unheap
		while(tempHnode->point->x < tempHnode->parent->point->x){
			Point* tempPoint = tempHnode->point;
			tempHnode->point = tempHnode->parent->point;
			tempHnode->parent->point = tempPoint;
			
			if(tempHnode->parent->parent != NULL)
				tempHnode = tempHnode->parent;
		}
	}
};
void Heap::addNodeSet(PointSet* pointSet){
	Point* tempPoint = pointSet->firstPoint;

	while(tempPoint != NULL){
		this->addNode(tempPoint);
		tempPoint = tempPoint->next;
	}
};
void Heap::resetEndParent(){
	while(this->endParent->parent != NULL){
		if(this->endParent == this->endParent->parent->leftChild){
			this->endParent = this->endParent->parent->rightChild;
			break;
		}

		this->endParent = this->endParent->parent;
	}
		
	while(this->endParent->leftChild != NULL)
		this->endParent = this->endParent->leftChild;
}
void Heap::resetLast(){
	while(this->last->parent != NULL){
		if(this->last == this->last->parent->rightChild){
			this->last = this->last->parent->leftChild;
			break;
		}

		this->last = this->last->parent;
	}
		
	while(this->last->rightChild != NULL)
		this->last = this->last->rightChild;
}
void Heap::printAll(hnode_t* tempHnode){
	if(tempHnode->leftChild != NULL)
		this->printAll(tempHnode->leftChild);

	if(tempHnode->rightChild != NULL)
		this->printAll(tempHnode->rightChild);

	if(tempHnode != NULL)
		cout<<"("<<tempHnode->point->x<<", "<<tempHnode->point->y<<")"<<endl;
}