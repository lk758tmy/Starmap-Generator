#include <stdio.h>
#include <time.h>
#include <random>
using namespace std;
int starmap[32][32],starX[64],starY[64],B[32],C[32];
bool stars[64];
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
void _init(){
	int a,b;
	for(int i=0;i<32;i++){
		stars[i]=stars[63-i]=false;
		B[i]=C[i]=0;
		a=((15-i)>0)?(15-i):0;
		b=((47-i)<32)?(47-i):32;
		for(int j=0;j<a;j++) starmap[i][j]=0;
		for(int j=a;j<b;j++) starmap[i][j]=1;
		for(int j=b;j<32;j++) starmap[i][j]=0;
	}
	return ;
}
bool _rand(){
	int cnt=0,x,y;
	for(x=0;x<32;x++){
		do{
			y=rand()%32;
		}while(starmap[x][y]!=1);
		starX[cnt]=x; starY[cnt]=y;
		starmap[x][y]=2;
		B[y]++; C[x+y-15]++; cnt++;
	}
	for(y=0;y<32;y++){
		if(B[y]>0) continue;
		do{
			x=rand()%32;
		}while(starmap[x][y]!=1);
		starX[cnt]=x; starY[cnt]=y;
		starmap[x][y]=2;
		C[x+y-15]++; cnt++;
	}
	for(int z=0;z<32;z++){
		if(C[z]>0) continue;
		if(cnt==64) return false;
		do{
			x=rand()%32;
			y=z+15-x;
		}while(y<0 || y>31 || starmap[x][y]!=1);
		starX[cnt]=x; starY[cnt]=y;
		starmap[x][y]=2;
		cnt++;
	}
	while(cnt<64){
		x=rand()%32; y=rand()%32;
		if(starmap[x][y]==1){
			starX[cnt]=x; starY[cnt]=y;
			starmap[x][y]=2;
			C[x+y-15]++; cnt++;
		}
	}
	return true;
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
	fprintf(fp,"\n\n");
	return ;
}
int main() {
	for(int i=0;i<10000;i++){ //1=3.142s
		srand(time(0));
		for(int j=0;j<10000000;j++){
			_init();
			if(!_rand()) continue;
			if(_solved()){
				FILE *fp = fopen("out.txt", "a");
				_print(fp); fclose(fp); return 0;
			}
		}
	}
	printf("Fail.");
	return 0;
}
