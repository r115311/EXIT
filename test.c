#include<stdio.h>
#include<string.h>
int main(){
	char a[6][8];
	char b[6][8];
	printf("unchi:");
	scanf("%s",a[0]);
	printf("unchi2:");
	scanf("%s",b[0]);
	printf("%s\n",a[0]);
	strcat(a[0],b[0]);
	printf("%s\n",a[0]);
	return 0;
}
