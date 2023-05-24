#include <stdio.h>
#include <stdlib.h>
using namespace std;
int starmap[32][32],starX[64],starY[64];
bool stars[64];
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
int __track(int x,int y){
	int star=0,tx,ty; bool flag;
	for(int i=0;i<6;i++){
		flag=false; tx=x; ty=y;
		while(1){
			tx+=dX[i]; ty+=dY[i];
			if(tx<0||ty<0||tx>31||ty>31) break;
			if(starmap[tx][ty]==0) break;
			if(starmap[tx][ty]==2){
				flag=true;
				break;
			}
		}
		if(flag==true) star+=(1<<i);
	}
	return star;
}
bool _check() {
	int star;
	for(int i=0;i<64;i++) stars[i]=false;
	for(int i=0;i<64;i++){
			star=__track(starX[i],starY[i]);
			printf("%d:%d ",i,star);
			if(stars[star]==false) stars[star]=true;
			else return false;	
	}
	return true;
}
void _read() {
	FILE *fp = fopen("\in.txt", "r");
	int cnt=0;
	for(int i=0;i<32;i++){
		for(int j=0;j<32;j++){
			fscanf(fp,"%d",&starmap[i][j]);
			if(starmap[i][j]==2){
				if(cnt==64){
					printf("Wrong");
					exit(0);
				}
				starX[cnt]=i; starY[cnt]=j; cnt++;
	}}}
	if(cnt<64){
		printf("Wrong",cnt);
		exit(0);
	}
	fclose(fp);
	return ;
}
int main() {
	_read();
	if(_check()==true)
		printf("Correct");
	else
		printf("Wrong");
	getchar();getchar();
	return 0;
}
