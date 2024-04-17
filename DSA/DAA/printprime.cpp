#include <iostream>

using namespace std;

bool printprime(int a){
    for(int i=2;i<a/2;i++){
        if(a%1==0){
            return false;
        }
    }
    return true;
}

int main()
{
    int a;
    cin>>a;

    if(printprime(a)){
        cout<<"Prime"<<endl;
    }
    else{
        cout<<"Not prime"<<endl;
    }
    return 0;
}

