#include <stdio.h>
#include <time.h>
#include <random>
using namespace std;
int starmap[32][32],starX[64],starY[64];
int A[32],B[32],C[32],move_id,move_sx,move_sy;
bool stars[64];
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
void _init(){
	int a,b;
	for(int i=0;i<32;i++){
		A[i]=B[i]=C[i]=0;
		a=((15-i)>0)?(15-i):0;
		b=((47-i)<32)?(47-i):32;
		for(int j=0;j<a;j++) starmap[i][j]=0;
		for(int j=a;j<b;j++) starmap[i][j]=1;
		for(int j=b;j<32;j++) starmap[i][j]=0;
	}
	return ;
}
bool _rand_gene(){
	int cnt=0,x,y;
	for(x=0;x<32;x++){
		do{
			y=rand()%32;
		}while(starmap[x][y]!=1);
		starX[cnt]=x; starY[cnt]=y; starmap[x][y]=2;
		A[x]++; B[y]++; C[x+y-15]++; cnt++;
	}
	for(y=0;y<32;y++){
		if(B[y]>0) continue;
		do{
			x=rand()%32;
		}while(starmap[x][y]!=1);
		starX[cnt]=x; starY[cnt]=y; starmap[x][y]=2;
		A[x]++; B[y]++; C[x+y-15]++; cnt++;
	}
	for(int z=0;z<32;z++){
		if(C[z]>0) continue;
		if(cnt==64) return false;
		do{
			x=rand()%32;
			y=z+15-x;
		}while(y<0 || y>31 || starmap[x][y]!=1);
		starX[cnt]=x; starY[cnt]=y; starmap[x][y]=2;
		A[x]++; B[y]++; C[x+y-15]++; cnt++;
	}
	while(cnt<64){
		x=rand()%32; y=rand()%32;
		if(starmap[x][y]==1){
			starX[cnt]=x; starY[cnt]=y;
			starmap[x][y]=2;
			A[x]++; B[y]++; C[x+y-15]++; cnt++;
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
int _check(){
	for(int i=0;i<64;i++) stars[i]=false;
	int star,cnt=0;
	for(int i=0;i<64;i++){
		star=__track(starX[i],starY[i]);
		if(stars[star]==false){
			stars[star]=true;
			cnt++;
		}
	}
	return cnt;
}
bool __single(int id){
	if(A[starX[id]]==1) return false;
	if(B[starY[id]]==1) return false;
	if(C[starX[id]+starY[id]-15]==1) return false;
	return true;
}
void __change_board(int id,int ex,int ey){
	A[starX[id]]--; B[starY[id]]--;
	C[starX[id]+starY[id]-15]--;
	A[ex]++; B[ey]++; C[ex+ey-15]++;
	starmap[starX[id]][starY[id]]=1;
	starmap[ex][ey]=2;
	starX[id]=ex; starY[id]=ey;
	return ;
}
void _remove(){
	__change_board(move_id,move_sx,move_sy);
	return ;
}
void _rand_move(){
	int ex,ey;
	_RM_BACK:
		move_id=rand()%64;
		move_sx=starX[move_id]; move_sy=starY[move_id];
		do{
			ex=rand()%64; ey=rand()%64;
		}while(starmap[ex][ey]!=1);
		__change_board(move_id,ex,ey);
	if(__single(move_id)){
		_remove(); goto _RM_BACK;
	}
	return ;
}
void _print(){
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
	fprintf(fp,"\n\n");
	fclose(fp);
	return ;
}
bool try_once(){
	int height,new_height,cnt;
	do{
		do{
			_init();
		}while(!_rand_gene());
		height=_check();
	}while(height<49);
	do{
		for(cnt=0;cnt<30000;cnt++){
			_rand_move();
			new_height=_check();
			if(new_height>height) break;
			_remove();
		}
		if(cnt==30000){
			printf("%d ",height);
			return false;
		}
		height=new_height;
	}while(height<64);
	_print();
	return true;
}
int main() {
	int cnt=0;
	srand(time(0));
	/*while(try_once()==false){
		cnt++;
		if(cnt%4==0) srand(time(0));
	}*/
	try_once(); 
	_print();
	return 0;
}