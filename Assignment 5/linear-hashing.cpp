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

bucket* createBucket(){
    bucket* temp;
    temp->ct=0;
    return temp;
}

bucket* insertBucket(bucket* a, int i){
    if(a==NULL){
        a=createBucket();
    }
    a->arr.pb(i);
    a->ct++;
    return a;
}

int getBucketNo(int n){
    return 0;
}

int main(){
    vector<bucket*> plist(1);
    bucket* crrPointer=plist[0];
    // bucket* a=createBucket();
    // plist.pb(a);
    int bitcount=0;
    int size=0;
    while(1){
        char in;
        cin>>in;
        switch(in){
            case 'i':   
                int n;
                cin>>n;


        }
    }
    return 0;
}