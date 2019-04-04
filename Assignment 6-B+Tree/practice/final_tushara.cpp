#include<bits/stdc++.h>

using namespace std; 

class Node{
	public:
		Node(int totalKeys);

    	int getNumKeys();
    	void setNumKeys(int val);

	    int getKeyAtIndex(int i);
    	void setKeyAtIndex(int key, int i);

	private:
		int numKeys;
		int *keys;// even for leaf nodes odd for data nodes
};

class IndexNode: public Node{
	public:
		IndexNode(int numIndexkeys);
		void *getIndexPointerAt(int i);
		void setIndexPointerAt(void *p,int i);
	private:
		void **indexPointers;// for children even number of children
};

class DataNode: public Node{
	public:
		DataNode(int numDatakeys);
		DataNode* getDataNodeLeft();
		void setDataNodeLeft(DataNode *p);
		DataNode* getDataNodeRight();
		void setDataNodeRight(DataNode *p);
	private:
		DataNode *left, *right;// no need for children
};

class BPlusTree{
public:
  BPlusTree(int numIndexkeys,int numDatakeys);
  IndexNode* toIndexNode(void *p);
  DataNode* toDataNode(void *p);
  void insert(void *root, int key,void* &newchildentry,int depth,int &a);
  DataNode* splitandInsertLeafChild(DataNode *y,int key);
  IndexNode* splitNonLeafChild(IndexNode *y,int newchild,void *newchildentry,int depth,int &keyToUP);
  void insertNonFull(void *node, int key, int depth,void *newchildentry);
  DataNode* search(void *node, int key, int depth);
  void LevelOrderTraversal(void *node, int depth);
  void LevelOrder(void *node, int depth);

  void* root;
  int numIndexkeys;// odd
  int numDatakeys;// even
  int depth;
};


// constructor of Node
Node::Node(int totalKeys){
	numKeys = 0;// initially 0
	keys = new int[totalKeys];
}
int Node::getNumKeys(){
  return numKeys;
}
void Node::setNumKeys(int val){
	numKeys = val;
}
int Node::getKeyAtIndex(int i){
  return keys[i];
}
void Node::setKeyAtIndex(int key, int i){
	keys[i] = key;
}


// for indexnode
IndexNode::IndexNode(int numIndexkeys)
	: Node(numIndexkeys)
{
	indexPointers = new void*[numIndexkeys+1];// pointers one more than the keys
}
void* IndexNode::getIndexPointerAt(int i){
	return indexPointers[i];
}
void IndexNode::setIndexPointerAt(void* p, int i){
	indexPointers[i] = p;
}

// for datanode
DataNode::DataNode(int numDatakeys)
	: Node(numDatakeys)
{
	left = NULL;
	right = NULL;
}
DataNode* DataNode::getDataNodeLeft(){
	return left;
}
void DataNode::setDataNodeLeft(DataNode *p){
	left = p;
}
DataNode* DataNode::getDataNodeRight(){
	return right;
}
void DataNode::setDataNodeRight(DataNode *p){
	right = p;
}




BPlusTree::BPlusTree(int numIndexkeys,int numDatakeys){
	root = new DataNode(numDatakeys);
	this->numIndexkeys = numIndexkeys;
	this->numDatakeys = numDatakeys;
	depth = 0;
}

IndexNode* BPlusTree::toIndexNode(void *p){
	return static_cast<IndexNode*>(p);
}

DataNode* BPlusTree::toDataNode(void *p){
	return static_cast<DataNode*>(p);
}

DataNode* BPlusTree::search(void *node, int key, int depth){
	
	if(depth==0){// leaf node
		DataNode *x = toDataNode(node);	
		for(int i = 0; i < x->getNumKeys(); i++)
			if(x->getKeyAtIndex(i) == key)
				return x;
		return NULL;	
	}
	else{
		IndexNode *x = toIndexNode(node);// ki<=k<ki+1 return pi
		int i = 0;
		while(i < x->getNumKeys() && key >= x->getKeyAtIndex(i))// looking for correct position
			i = i + 1;
		return search(x->getIndexPointerAt(i), key, depth-1);
	}
}

void BPlusTree::insert(void *node,int key,void* &newchildentry,int depth,int &newchild)
{
    if(depth==0)//leaf node
    {
        DataNode *root = toDataNode(node);
        if(root->getNumKeys()==numDatakeys)
        {//cout<<root->getKeyAtIndex(0);
            newchildentry=splitandInsertLeafChild(root,key);// inserting also inside
            DataNode *ne= toDataNode(newchildentry);
            newchild=ne->getKeyAtIndex(0);//(ne->getKeyAtIndex(0)+root->getKeyAtIndex(root->getNumKeys()-1)+1)/2;
            if(this->root==root)
            {
                this->depth++;
                IndexNode *newnode=new IndexNode(numIndexkeys);
                newnode->setNumKeys(1);
                newnode->setKeyAtIndex(newchild,0);
                newnode->setIndexPointerAt(root,0);
                newnode->setIndexPointerAt(newchildentry,1);
                this->root=newnode;
                return ;
            }
            
        }
        else
        {
            insertNonFull(root,key,depth,NULL);
            newchildentry=NULL;
            newchild=0;
        }
    }
    else
    {
        IndexNode *x = toIndexNode(node);// ki<=k<ki+1 return pi
		int i = 0;
		while(i < x->getNumKeys() && key >= x->getKeyAtIndex(i))// looking for correct position
			i = i + 1;
        insert(x->getIndexPointerAt(i),key,newchildentry,depth-1,newchild);//cout<<newchild;
        if(newchildentry==NULL) {return;}
        if(x->getNumKeys()==numIndexkeys)
        {
            int keyToUP=0;
            newchildentry=splitNonLeafChild(x,newchild,newchildentry,depth,keyToUP);
            newchild=keyToUP;
            if(x==this->root)
            {
                this->depth++;
                IndexNode *newnode=new IndexNode(numIndexkeys);
                newnode->setNumKeys(1);
                newnode->setKeyAtIndex(newchild,0);
                newnode->setIndexPointerAt(root,0);
                newnode->setIndexPointerAt(newchildentry,1);
                this->root=newnode;
                return ;
            }
        }
        else
        {
            insertNonFull(x,newchild,depth,newchildentry);// key and pointer inserted 
            newchild=0;newchildentry=NULL;       
        }
        return;
    }
    return;
}

DataNode* BPlusTree::splitandInsertLeafChild(DataNode *y,int key){
	DataNode *z = new DataNode(numDatakeys);
	//DataNode *y = toDataNode(x->getIndexPointerAt(i));
	int mid = numDatakeys/2;//cout<<y->getKeyAtIndex(0);
    if(key >= y->getKeyAtIndex(mid))// implies key must be in second half
    {
        // we want left to have more children  so mid--
        mid--;
    }
	z->setNumKeys(mid);// z will be the second half

    if(mid==numDatakeys/2)
    {	
        for(int j = 0; j < mid; j++){
        z->setKeyAtIndex(y->getKeyAtIndex(j + mid), j);
        //y->setKeyAtIndex(0,j+mid);
		//z->setDataPointerAt(y->getDataPointerAt(j + mid), j);
        }
        y->setNumKeys(mid);// resetting y numkeys
       // cout<<y->getKeyAtIndex(0);
        insertNonFull(y,key,0,NULL);
    }
    else
    {
        for(int j = 0; j < mid; j++){
            z->setKeyAtIndex(y->getKeyAtIndex(j + mid+2), j);
            //y->setKeyAtIndex(0,j+mid);
            //z->setDataPointerAt(y->getDataPointerAt(j + mid), j);
        }
        y->setNumKeys(mid+2);// resetting y numkeys
        insertNonFull(z,key,0,NULL);
    }
    
	z->setDataNodeLeft(y);
	z->setDataNodeRight(y->getDataNodeRight());
    y->setDataNodeRight(z);
    return z;
}

IndexNode* BPlusTree::splitNonLeafChild(IndexNode *y,int newchild,void *newchildentry,int depth,int &keyToUP){
    IndexNode *z = new IndexNode(numIndexkeys);
    // we want t on left t-1 on right 
	//IndexNode *y = toIndexNode(x->getIndexPointerAt(i));
    int mid = (numIndexkeys)/2;// index of mid mid=t-1 tth element
     keyToUP = y->getKeyAtIndex(mid);
    
    if(newchild>=keyToUP)// inserted on right side but then right will be t so we should adjust
    {
        
        keyToUP=y->getKeyAtIndex(mid+1);// t+1th 
        if(mid==0 || newchild<keyToUP)// then newchild will go up from t+1th here
        {
            keyToUP=newchild;
            int j;
            for(j = 0; j <= mid-1; j++){
                z->setKeyAtIndex(y->getKeyAtIndex(j + mid + 1), j);
                z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
            }
            z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
            z->setIndexPointerAt(newchildentry,0);//newchild entry
            y->setNumKeys(mid+1);z->setNumKeys(mid);//right t-1

        }
        else// t+1 key[t] th going up
        {
            int j;mid++;// from t+2 
            for(j = 0; j <= mid-1; j++){
                z->setKeyAtIndex(y->getKeyAtIndex(j + mid + 1), j);
                z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
            }
            z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
            z->setNumKeys(mid-2);//right t-1
            y->setNumKeys(mid);
            insertNonFull(z,newchild,depth,newchildentry);
        }
    }	
    else// left will have t with the new one
    {
        int j;
        for(j = 0; j <= mid-1; j++){// z from t+1
            z->setKeyAtIndex(y->getKeyAtIndex(j + mid + 1), j);
            z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
        }
        z->setIndexPointerAt(y->getIndexPointerAt(j + mid + 1), j);
        y->setNumKeys(mid);z->setNumKeys(mid);//right t-1
        insertNonFull(y,newchild,depth,newchildentry);
    }
	
    return z;
	
}

void BPlusTree::insertNonFull(void *node, int key, int depth,void *newchildentry){
    if(depth==0)
    {
        DataNode *x=toDataNode(node);
        int i=x->getNumKeys()-1;
        while(i>=0&& key<x->getKeyAtIndex(i))// looking for position
        {
            x->setKeyAtIndex(x->getKeyAtIndex(i),i+1);i--;
        }//cout<<x->getKeyAtIndex(0);
        x->setKeyAtIndex(key, i+1);
		x->setNumKeys(x->getNumKeys()+1);
    }
    else
    {
        IndexNode *x=toIndexNode(node);
        int i=x->getNumKeys()-1;
        while(i>=0&& key<x->getKeyAtIndex(i))// looking for position
        {
            x->setKeyAtIndex(x->getKeyAtIndex(i),i+1);
            x->setIndexPointerAt(x->getIndexPointerAt(i+1),i+2);
            i--;
        }
        x->setKeyAtIndex(key, i+1);x->setIndexPointerAt(newchildentry,i+2);
		x->setNumKeys(x->getNumKeys()+1);
    }
} 


void BPlusTree::LevelOrderTraversal(void *root, int depth){
	queue<pair<int,void *> > q;
	queue<int> level;
	int nodeID = 1;
	pair<int, void*> curNode;
	int curLevel=0,prevLevel=-1;
	depth = depth + 1;
	q.push({nodeID, root});// root 1
	level.push(0);
	cout<<"\n*** NodeID :: Values ***\n"<<endl;
	while(!q.empty()){
		curNode = q.front(); q.pop();
		if(prevLevel!=level.front()){
			depth = depth-1;
			prevLevel = curLevel++;
		}
		level.pop();
		cout<<curNode.first<< " :: ";// node id
		if(depth==0){
			DataNode *node = toDataNode(curNode.second);
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<endl;
		}
		else{
			IndexNode *node = toIndexNode(curNode.second);
			for(int i=0; i <= node->getNumKeys(); i++){
				q.push({++nodeID, node->getIndexPointerAt(i)});
				level.push(curLevel);
			}
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<endl;
		}
	}
	cout<<endl<<endl;
}

void BPlusTree::LevelOrder(void *root, int depth){
	queue<pair<int,void *> > q;
	queue<int> level;
	int nodeID = 1;
	pair<int, void*> curNode;
	int curLevel=0,prevLevel=-1;
	depth = depth + 1;
	q.push({nodeID, root});// root 1
	level.push(0);
	cout<<"\n*** LevelID :: Values ***\n"<<endl;
	while(!q.empty()){
		curNode = q.front(); q.pop();
		if(prevLevel!=level.front()){
			depth = depth-1;
			prevLevel = curLevel++;cout<<endl<<curLevel<<":";
		}
		level.pop();
		//cout<<curNode.first<< " :: ";// node id
		if(depth==0){
			DataNode *node = toDataNode(curNode.second);
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<"|";
		}
		else{
			IndexNode *node = toIndexNode(curNode.second);
			for(int i=0; i <= node->getNumKeys(); i++){
				q.push({++nodeID, node->getIndexPointerAt(i)});
				level.push(curLevel);
			}
			for(int i=0; i<node->getNumKeys(); i++){
				cout<<node->getKeyAtIndex(i)<<" ";
			}
			cout<<"|";
		}
	}
	cout<<endl<<endl;
}


int main()
{
    BPlusTree* tree = NULL;
    int key,choice;
    int numIndexkeys, numDatakeys;
    cout << "1. Create new tree" << endl;
    cout << "2. Add new key to the tree" << endl;
    cout << "3. Search a key in the tree" << endl;
    cout << "4. Postorder traversal of the tree" << endl;
    cout << "Any other choice to exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
     int searchResultIndex;
 while(1) {

 switch(choice){
     case 1:
       if(tree != NULL){
         delete tree;
       }
       cout<<"Enter number of Index Keys: ";
       cin>>numIndexkeys;
       cout<<"Enter number of Data Keys: ";
       cin>>numDatakeys;
       if(numIndexkeys%2==0 || numDatakeys%2){
           cout<<"Note:: Index Keys must be in odd number and Data Keys must be in even number"<<endl;
           exit(0);
       }
       tree = new BPlusTree(numIndexkeys, numDatakeys);
       break;
     
     case 2:
       if(tree == NULL){
         cout << "Create a new tree first" << endl;
         break;
       }
       cout << "Enter key to add: ";
       cin >> key;
       void *node;int a;
       tree->insert(tree->root, key,node,tree->depth,a);
       break;
     case 3:
       if(tree == NULL){
         cout << "Create a new tree first" << endl;
         break;
       }
       cout << "Enter key to search: ";
       cin >> key;
       {
           DataNode *searchResult = tree->search(tree->root, key, tree->depth);
           if(searchResult == NULL){
              cout << "Key " << key << " not found." << endl;
           }
           else{
             cout << "Key " << key << " found" << endl;
           }
       }
       break;
     case 4:
       if(tree == NULL){
         cout << "Create a new tree first" << endl;
         break;
       }
       tree->LevelOrder(tree->root, tree->depth);
       break;

     default:
       return 0;
   }
   cout << "Enter your choice: ";
   cin >> choice;
 }

   return 0;
}