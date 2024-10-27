#include <stdio.h>
#include <string.h>

int add(char * a, int len){
	
	int c = 0;
	for(int i=0;i<len;i++){
    	a[i];
		printf("%d\n",a[i]);
		
		switch (a[i]) {
		case 48:
			c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 49:
			c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 50:
		    c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 51:
		    c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 52:
			c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 53:
		    c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);

			break;
		case 54:
			c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 55:
			c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 56:
			c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		case 57:
			c++;
			printf("c=%d  这个字符为数字%c\n", c, a[i]);
			break;
		default:
			//TODO
			break;
		}
	}
	

    return 0;	
}

int main() {
   // char * a;
	char a[100];
	while(1){
		printf("请输入一个字符串： ");
		
		scanf("%s",a);
		
		printf("%s\n",a);
		
		int len = strlen( a);
		
		printf("%d\n",len);
		add(a,len);
	}

	return 0;
}
