

#include <iostream>

using namespace std;

int max_num(int a,int b,int c,int d){
   
    if (a>b){
        
        if (a>c){
            if (a>d){
                return a;
            }
        
            else return d;   
        }
        else if (c>a){
            if (c>d){
                return c;
        }

        
    }
    else if (b>a){
        
    }
    
   
    }
   
    
}

   
    




int main(){
    int a,b,c,d;
    cin>>a;
    cin>>b;
    cin>>c;
    cin>>d;
    cout<<max_num(a,b,c,d)<<endl;
   
}

