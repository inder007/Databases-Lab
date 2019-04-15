#include <bits/stdc++.h>

using namespace std;


/*this class contains declarations of all the data structures that are used to perform the given tasks*/

class Transactions{


public:
	void readInput();   // to read the given input file
	void PossibleChains();  // to find all possible chains
	void isRecoverable();  // to check whether schedule is recoverable or not
	void isCascadeless();  // to check whether schedule is cascadeless or not
    void isSerializable();
    void populateGrapgh();
    void isStrict();
    void dfs(int st, map<int,int> &color, bool &check);

private:
	vector<pair<int, pair<char,int> > > transaction; 
	// it stores all the trasaction details
    unordered_map<int,int> commit;
    // it stores the position of commit operation of a trasaction 
    vector<pair<int,vector<int> > > chain;
    // to store all the possible chains
    vector< pair<int,pair<int,int> > > firstReadArray; 
    vector< pair<int,pair<int,int> > > firstReadWriteArray;
    // it represents a vector where each elements is a tuple containing tid of write operation, position of first read in a chain and its tid  
    // vector<pair<int,vector<int>>> adj;
    map<int,set<int>> adj;
    map<int,int> color;
    vector<int> print;
};

void Transactions::readInput(){
	/*
        -->this function read the given input file and store all the transactions details in d.s. 'transactions'
         
	*/
	int Tid, DataItem=0;
    char Opr;
    while(cin>>Tid){
        cin>>Opr;
        if(Opr!='C')
            cin>>DataItem;
        else{
            commit[Tid]=transaction.size();
        }
        // Not covering if input format is wrong.
        transaction.push_back({Tid,{Opr,DataItem}});
    }
}


void Transactions::PossibleChains(){
	/*
	    --> this method find all the possible chains and store it in d.s. 'chain'
	    --> each chain represents collection of read operations on a dataitem that is written by a transaction before those read operations
        
	*/
	vector<int> temp;
	pair<int,int> firstRead;
	for(int i=0; i<transaction.size(); i++){
        if(transaction[i].second.first!='W')
            continue;

        int DataItem=transaction[i].second.second;
        bool check=false;
        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].second.second!=DataItem)
                continue;
            if(!check && transaction[i].first!=transaction[j].first && transaction[j].second.first!='C'){
                firstReadWriteArray.push_back({transaction[i].first, {transaction[j].first,j}});
                check=true;
            }
            if(transaction[j].second.first=='R'){
                if(transaction[j].first==transaction[i].first){
                    continue;
                }
                
            	if(temp.size()==0)
            		firstRead={transaction[j].first,j};
            	temp.push_back(transaction[j].first);
            }
            if(transaction[j].second.first=='W')
                break;
        }
        if(temp.size()){
            chain.push_back({transaction[i].first,temp});
            firstReadArray.push_back({transaction[i].first,{firstRead}});
        }
        temp.clear();
    }
}


void Transactions::isRecoverable(){
	/*
         --> this method checks whether the given schedule is recoverable or not
	*/
	for(int i=0; i<chain.size(); i++){
        int x = commit[chain[i].first];
        for(int j=0; j<chain[i].second.size(); j++){
            if(x>=commit[chain[i].second[j]]){
            	cout<<"No"<<endl;
            	cout<<"Transaction "<<chain[i].second[j]<<" is reading DataItem written by Transaction "<<chain[i].first<<endl;
            	cout<<"And Transaction "<<chain[i].first<<" commits after Transaction "<<chain[i].second[j]<<endl;
            	return;
            }
        }
    }
    cout<<"Yes"<<endl;
}


void Transactions::isCascadeless(){
	/*
         --> this method checks whether the given schedule is cascadeless or not
	*/
	for(int i=0; i<firstReadArray.size(); i++){
        int x = commit[firstReadArray[i].first];
        if(x>=firstReadArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstReadArray[i].second.first<<" is reading DataItem written by Transaction "<<firstReadArray[i].first<<endl;
        	cout<<"And Transaction "<<firstReadArray[i].first<<" commits after reading by Transaction "<<firstReadArray[i].second.first<<endl;
        	return;
        }
    }
    cout<<"Yes"<<endl;
}

void Transactions::isStrict(){
    cout<<"Strict ";
    for(int i=0; i<firstReadWriteArray.size(); i++){
        int x = commit[firstReadWriteArray[i].first];
        if(x>=firstReadWriteArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstReadWriteArray[i].second.first<<" is reading DataItem written by Transaction "<<firstReadWriteArray[i].first<<endl;
        	cout<<"And Transaction "<<firstReadWriteArray[i].first<<" commits after reading by Transaction "<<firstReadWriteArray[i].second.first<<endl;
        	return;
        }
    }
    cout<<"Yes"<<endl;
}

void Transactions::dfs(int st, map<int,int> &color, bool &check){
    color[st]=1;
    for(auto it=adj[st].begin();it!=adj[st].end();it++){
        if(color[*it]==0){
            dfs(*it,color,check);
        }
        else if(color[*it]==1){ 
            // cout<<"asdasd"<<endl;
            check=true;
        }
    }
    color[st]=2;
    print.push_back(st);
}

void Transactions::populateGrapgh(){
    for(int i=0;i<transaction.size();i++){
        set<int> s;
        adj[transaction[i].first]=s;
    }
    for(int i=0;i<transaction.size();i++){
        int DataItem=transaction[i].second.second;
        set<int> s;
        if(transaction[i].second.first=='R'){
            for(int j=i+1;j<transaction.size();j++){
                if(transaction[j].second.second!=DataItem || transaction[j].first==transaction[i].first){
                    continue;
                }
                if(transaction[j].second.first=='W'){
                    adj[transaction[i].first].insert(transaction[j].first);
                }
            }
            
        }
        else if(transaction[i].second.first=='W'){
            for(int j=i+1;j<transaction.size();j++){
                if(transaction[j].second.second!=DataItem || transaction[j].first==transaction[i].first){
                    continue;
                }
                if(transaction[j].second.first=='W' || transaction[j].second.first=='R'){
                    adj[transaction[i].first].insert(transaction[j].first);
                }
            }
            
        }
        color[transaction[i].first]=0;
    }
}

void Transactions::isSerializable(){
    this->populateGrapgh();
    // for(auto i=adj.begin();i!=adj.end();i++){
    //     cout<<i->first<<" ";
    //     for(auto it=i->second.begin();it!=i->second.end();it++){
    //         cout<<*it<<" ";
    //     }
    //     cout<<endl;
    // }
    bool check=false;
    for(auto it=adj.begin();it!=adj.end();it++){
        if(color[it->first]==0){
            dfs(it->first,color,check);
        }
    }
    if(!check){
        cout<<"Conflict Serializable: YES"<<endl;
        reverse(print.begin(),print.end());
        cout<<"Order: ";
        for(auto it=print.begin();it!=print.end();it++){
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    else{
        cout<<"Conflict Serializable: NO"<<endl;
    }
}

int main(){

	Transactions obj;

	obj.readInput();   // to read input file
    
	obj.PossibleChains(); //to find all possible chains

	/*Each chain represents collection of read operation on a dataitem that is written by a transaction before those read operations*/

	obj.isRecoverable();  // to check whether schedule is recoverable or not

	obj.isCascadeless(); // to check whether schedule is cascadeless or not 
    obj.isStrict();
    obj.isSerializable();
	return 0;

}