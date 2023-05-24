#include <stdio.h>
#include <time.h>
#include <random>
using namespace std;
int starmap[32][32],starX[64],starY[64];
int A[64],B[64],C[64],A_t,B_t,C_t,cnt,x,y;
void _first_init(){
	int a,b;
	for(int i=0;i<32;i++){
		a=((15-i)>0)?(15-i):0;
		b=((47-i)<32)?(47-i):32;
		for(int j=0;j<a;j++) starmap[i][j]=0;
		for(int j=a;j<b;j++) starmap[i][j]=1;
		for(int j=b;j<32;j++) starmap[i][j]=0;
	}
	for(int i=0;i<64;i++) A[i]=B[i]=C[i]=-1;
	return ;
}
void __init(){
	for(int i=0;i<64;i++){
		starmap[starX[i]][starY[i]]=1;
		A[i]=B[i]=C[i]=-1;
	}
	cnt=x=y=A_t=B_t=C_t=0;
	return ;
}
void __fprint() {
	FILE *fp = fopen("out.txt", "a");
	int cnt=0;
	for(int i=0;i<32;i++){
		for(int j=0;j<i;j++) fprintf(fp," ");
		for(int j=0;j<32;j++)
			fprintf(fp,"%d ",starmap[i][j]);
		fprintf(fp,"\n");
	}
	for(int i=0;i<64;i++)
		fprintf(fp,"(%d,%d) ",starX[i],starY[i]);
	fprintf(fp,"\n");
	fclose(fp);
	return ;
}
void _rand(){
	srand(time(0));
	while(cnt<64){
		x=rand()%32; y=rand()%32;
		if(starmap[x][y]==1){
			starX[cnt]=x; starY[cnt]=y;
			A[x]++; B[y]++; C[x+y-15]++;
			starmap[x][y]=2; cnt++;
		}
	}
	for(int i=0;i<64;i++){
		A_t+=(A[i]>0)?A[i]:0;
		B_t+=(B[i]>0)?B[i]:0;
		C_t+=(C[i]>0)?C[i]:0;
	}
	if(A_t==32&&B_t==32&&C_t==32) __fprint();
	__init();
//Easy to prove: there're exactly 32 beams in each direction.
	return ;
}
int main() {
	_first_init();
	long long counter=0;
	do{
		counter++;
		_rand();
	}while(counter<1e10);//35 minutes
	FILE *fp = fopen("out.txt", "a");
	fprintf(fp,"1e10 tries ends.\n\n");
	fclose(fp);
	printf("1e10 tries ends.\n\n");
	return 0;
}
