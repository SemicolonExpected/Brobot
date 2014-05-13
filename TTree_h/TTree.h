#ifndef TTree_h
#define TTree_h

#include <cstddef> //to use NULL constant because...well because C++11

using namespace std;

class TTree
{
public:
	TTree();
	//~TTree();

	TTree *back;
    TTree *forward;
    TTree *left;
	TTree *right;
	
	int numsteps;
	bool ifVisited;
	//bool ifExpended;

	void insert(int steps, int direction);

/*private:
	TTree *root */
};

//TTree::TTree();
//TTree::insert(int steps, TTree *direction);

TTree::TTree(){
	this->back = NULL;
	this->forward = NULL;
	this->left = NULL;
	this->right = NULL;

	this->numsteps = 0;
	//first TTree ie the node will be = 0

        this->ifVisited = false;
        //this->ifExpended = false;
}

/**********************************************
 If there is no tree where we want to insert
 a new node, then create one, else it does
 nothing as there is already a node.
**********************************************/
void TTree::insert(int steps, int direction){
	if(direction == 0){
		if(this->left != NULL){
			this->left = new TTree();
			this->left->numsteps = steps;
			this->left->back = this;
			this->left->forward = NULL;
			this->left->left = NULL;
			this->left->right = NULL;
		}
	}
	if(direction == 1){
		if(this->forward != NULL){
			this->forward = new TTree();
			this->forward->numsteps = steps;
			this->forward->back = this;
			this->forward->forward = NULL;
			this->forward->left = NULL;
			this->forward->right = NULL;
		}
	}
	if(direction == 2){
		if(this->right != NULL){
			this->right = new TTree();
			this->right->numsteps = steps;
			this->right->back = this;
			this->right->forward = NULL;
			this->right->left = NULL;
			this->right->right = NULL;
		}
	}
}


#endif
