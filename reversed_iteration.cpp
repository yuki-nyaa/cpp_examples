#include<cstdio>

int main(){
    int arr[10] = {2,3,5,7,11,13,17,19,23,29};
    for(int* e = arr+10;e!=arr+1;){
        --e;
        printf("%d ",*e);
    }
}