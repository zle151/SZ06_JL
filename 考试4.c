#include <stdio.h>

void reverse_arr(int * a, int len){
	int tem;
	len = len - 1;
    for(int i=0;i<=len;len--,i++){
		tem = a[i];
		a[i] = a[len];
		a[len] = tem;	
	}

	
}
int main() {
	
	int a[] = {0,1,2,3,4,5,6,7,8,9};
	
	int tem;
	int len = 10;


	reverse_arr(a,10);
	for(int i=0;i<10;i++){
		printf("a[i]=%d\n",a[i]);
	}
	return 0;
}
