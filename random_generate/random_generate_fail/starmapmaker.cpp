#include <stdio.h>
#include <time.h>
#include <random>
using namespace std;
int starmap[32][32],starX[64],starY[64];
bool stars[64];
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
void _init(){
	int a,b;
	for(int i=0;i<32;i++){
		a=((15-i)>0)?(15-i):0;
		b=((47-i)<32)?(47-i):32;
		for(int j=0;j<a;j++) starmap[i][j]=0;
		for(int j=a;j<b;j++) starmap[i][j]=1;
		for(int j=b;j<32;j++) starmap[i][j]=0;
	}
	return ;
}
void _rand(){
	int cnt=0,x,y;
	srand(time(0));
	while(cnt<64){
		x=rand()%32; y=rand()%32;
		if(starmap[x][y]==1){
			starX[cnt]=x; starY[cnt]=y;
			starmap[x][y]=2; cnt++;
		}
	}
	return ;
}
int __track(int x,int y){
	int star=0,tx,ty; bool flag;
	for(int i=0;i<6;i++){
		flag=false; tx=x; ty=y;
		while(1){
			tx+=dX[i]; ty+=dY[i];
			if(tx<0||ty<0||tx>31||ty>31) break;
			if(starmap[tx][ty]==0) break;
			if(starmap[tx][ty]==2){
				flag=true; break;
			}
		}
		if(flag==true) star+=(1<<i);
	}
	return star;
}
bool _solved() {
	int star;
	for(int i=0;i<64;i++) stars[i]=false;
	for(int i=0;i<64;i++){
			star=__track(starX[i],starY[i]);
			if(stars[star]==false) stars[star]=true;
			else return false;	
	}
	return true;
}
void _print(FILE *fp) {
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
	return ;
}
int main() {
	long long counter=0;
	do{
		counter++;
		_init();
		_rand();
	}while(!_solved() && counter<1e10); //500 minutes
	FILE *fp = fopen("out.txt", "a");
	if(counter>=1e10)
		fprintf(fp,"Fail after 1e10 tries.\n\n");
	else {
		fprintf(fp,"%d\n",counter);
		_print(fp);
	}
	fclose(fp);
	return 0;
}
