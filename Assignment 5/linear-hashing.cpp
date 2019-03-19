#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define popb pop_back
#define vi vector<int>
#define vll vector<ll>
#define vvi vector < vi >
#define pii pair<int,int>
#define pll pair<long long, long long>
#define all(c) c.begin(),c.end()
#define mp make_pair
#define f first
#define s second
typedef long long ll;
typedef unsigned long long ull;
#define MAXSIZE 2

struct bucket{
    vector<int> arr;
    int ct;
};

struct head{
    bucket* h;
};

bucket* createBucket(){
    bucket* temp = new bucket;
    temp->ct=0;
    return temp;
}

int getBucketNo(int n,int l,int next){
    if(n%l > next){
        return n%l;
    }
    else{
        return n%(2*l);
    }
    // return 0;
}

void insertBucket(vector<head> &a, int index, int n,int &next, int &level){
    if(a[index].h == NULL){
        a[index].h = createBucket();
    }
    a[index].h->ct++;
    a[index].h->arr.push_back(n);
    if(a[index].h->ct<=MAXSIZE){
        return;
    }
    else{
        head temp;
        temp.h=NULL;
        a.push_back(temp);
        if(index==next){
            next=(next+1)%(level);
            bucket* t = a[index].h;
            a[index].h=NULL; 
            for(int i=0;i<t->ct;i++){
                int x = getBucketNo(a[index].h->arr[i],level,next);
                if(a[x].h == NULL){
                    a[x].h = createBucket();
                }
                a[x].h->ct++;
                a[x].h->arr.push_back(n);
            }
            if(next==0){
                level=2*level;
            }
        }
    }
    // return a;
}


int main(){
    vector<head> heads(2);
    // bucket* next=plist[0];
    // bucket* a=createBucket();
    // plist.pb(a);
    int level=2;
    int next=0,n,index;
    while(1){
        char in;
        cin>>in;
        switch(in){
            case 'i': 
                cin>>n;
                index = getBucketNo(n,level,next);
                insertBucket(heads,index,n,next,level);
                break;
            case 'p' :
                for(int i=0;i<heads.size();i++){
                    if(heads[i].h != NULL){
                        for(int j=0;j<heads[i].h->arr.size();j++){
                            cout<<heads[i].h->arr[i]<<" ";
                        }
                        cout<<endl;
                    }
                    else{
                        cout<<"NULL"<<endl;
                    }
                }
                break;
            case 'q' :
                return 0;
            default :
                break;
        }
    }
    return 0;
}