#include <bits/stdc++.h>
using namespace std;


/*this class contains declarations of all the data structures that are used to perform the given tasks*/

class Transactions{
    
    
    public:
        void readInput();   // to read the given input file
        void PossibleChains();  // to find all possible chains
        void isRecoverable();  // to check whether schedule is recoverable or not
        void isCascadeless();  // to check whether schedule is cascadeless or not
        void isCS();
        void prece();
        bool cycle(map<int,int>&visited,map<int,int>&rec_stack,vector<int>&serial_order,int v);
    
    private:
        vector<pair<int, pair<char,int> > > transaction; 
        // it stores all the trasaction details
        unordered_map<int,int> commit;
        // it stores the position of commit operation of a trasaction 
        vector<pair<int,vector<int> > > chain;
        // to store all the possible chains
        vector< pair<int,pair<int,int> > > firstReadArray; 
        // it represents a vector where each elements is a tuple containing tid of write operation, position of first read in a chain and its tid  
        set<int>tran_list;
        map<int,vector<int> >graph;
    };

void Transactions::readInput(){
    /*
        -->this function read the given input file and store all the transactions details in d.s. 'transactions'
            
    */
    int Tid, DataItem=0;
    char Opr;
    while(cin>>Tid){
        cin>>Opr;
        tran_list.insert(Tid);
        if(Opr!='C')
            cin>>DataItem;
        else{
            commit[Tid]=transaction.size();// position of commiting
        }
        // Not covering if input format is wrong.
        transaction.push_back({Tid,{Opr,DataItem}});
    }
    for(auto it=tran_list.begin();it!=tran_list.end();it++)
    {
        if(commit.find(*it)==commit.end())
            commit[*it]=INT_MAX;
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
        if(transaction[i].second.first!='W')// looking for frst write
            continue;

        int DataItem=transaction[i].second.second;

        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].second.second!=DataItem)// if not on same data no error possible
                continue;
            if(transaction[j].second.first=='R' && transaction[j].first!=transaction[i].first){// as this is read in cs it should come after t1
                if(temp.size()==0)
                    firstRead={transaction[j].first,j};// frst time trying to read
                    
                    temp.push_back(transaction[j].first);// storing id which should come after this write id
            }
            if(transaction[j].second.first=='W')// 2nd write may belong to domrothrrr trsndsction do break
                break;
        }
        if(temp.size()){
            chain.push_back({transaction[i].first,temp});
            
            firstReadArray.push_back({transaction[i].first,{firstRead}});// firstread array useful for cascadeless
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
                if(x==INT_MAX &&commit[chain[i].second[j]]==INT_MAX )continue;
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

void Transactions::prece(){
    for(int i=0; i<transaction.size(); i++){
        if(transaction[i].second.first=='C')// looking for frst write
            continue;

        int DataItem=transaction[i].second.second;

        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].second.first=='C')// looking for frst write
            continue;
            if(transaction[j].second.second!=DataItem)// if not on same data no error possible
                continue;
            if(transaction[j].first!=transaction[i].first){
                if(transaction[j].second.first=='R' && transaction[i].second.first=='R')continue;
                if(find(graph[transaction[i].first].begin(),graph[transaction[i].first].end(),transaction[j].first)==graph[transaction[i].first].end())
                {   graph[transaction[i].first].push_back(transaction[j].first);}
            }
    }
    }

}


void Transactions::isCS(){
    
    prece();
    /*for(auto it=graph.begin();it!=graph.end();it++)
    {
        for(int i=0;i<it->second.size();i++)
            cout<<it->second[i];
            cout<<"\n";
    }*/
    map<int,int>v;vector<int>serial_order;
    map<int,int>rec_stack;
    for(auto it=tran_list.begin();it!=tran_list.end();it++)
    {
        if(cycle(v,rec_stack,serial_order,*it))
        {
            cout<<"No CS\n";return ;
        }
    }
    
    cout<<"CS\n";reverse(serial_order.begin(),serial_order.end());
    for(int i=0;i<serial_order.size();i++)
        cout<<serial_order[i];
}
bool Transactions::cycle(map<int,int>&visited,map<int,int>&rec_stack,vector<int>&serial_order,int v)
{
    if(visited[v] == 0) 
    { 
        // Mark the current node as visited and part of recursion stack 
        visited[v] = 1; 
        rec_stack[v] = 1; 
  
        // Recur for all the vertices adjacent to this vertex 
        
        for(auto i = graph[v].begin(); i != graph[v].end(); ++i) 
        { 
            if ( !visited[*i] && cycle(visited,rec_stack,serial_order,*i) ) 
                return true; 
            else if (rec_stack[*i]) 
                return true; 
        } 
        serial_order.push_back(v);
    } 
    rec_stack[v] = 0;  // remove the vertex from recursion stack 
    
    return false;
}


int main(){
    
    Transactions obj;

    obj.readInput();   // to read input file

    obj.PossibleChains(); //to find all possible chains

    /*Each chain represents collection of read operation on a dataitem that is written by a transaction before those read operations*/

    obj.isRecoverable();  // to check whether schedule is recoverable or not

    obj.isCascadeless(); // to check whether schedule is cascadeless or not 
    
   obj.isCS();
    return 0;

}