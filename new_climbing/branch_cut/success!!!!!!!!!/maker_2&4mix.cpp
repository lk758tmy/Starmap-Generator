#pragma GCC optimize(3)
#include <stdio.h>
#include <time.h>
#include <random>
#include <vector>
using namespace std;
int starmap[34][34],A[34],B[34],C[34];
int starX[64],starY[64],stars[64];
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
vector<int> relocate;
void _init(){
	int a,b;
	for(int j=0;j<34;j++)
		starmap[0][j]=starmap[33][j]=2;
	for(int i=1;i<33;i++){
		A[i]=B[i]=C[i]=0;
		a=((17-i)>1)?(17-i):1;
		b=((49-i)<33)?(49-i):33;
		for(int j=0;j<a;j++) starmap[i][j]=2;
		for(int j=a;j<b;j++) starmap[i][j]=0;
		for(int j=b;j<34;j++) starmap[i][j]=2;
	}
	return ;
}
int _add_star(int x,int y,int cnt){
	starX[cnt]=x; starY[cnt]=y; starmap[x][y]=1;
	A[x]++; B[y]++; C[x+y-16]++; 
	return cnt+1;
}
int _track(int x,int y){
	int star=0,tx,ty; bool flag;
	for(int i=0;i<6;i++){
		flag=false; tx=x; ty=y;
		while(1){
			tx+=dX[i]; ty+=dY[i];
			if(starmap[tx][ty]==2) break;
			if(starmap[tx][ty]==1){
				flag=true; break;
			}
		}
		if(flag==true) star+=(1<<i);
	}
	return star;
}
int _check(){
	for(int i=0;i<64;i++) stars[i]=0;
	int star,cnt=0;
	for(int i=0;i<64;i++){
		star=_track(starX[i],starY[i]);
		if(stars[star]==0){
			stars[star]=1;
			cnt++;
		}
	}
	return cnt;
}
void _find_relocate(){
	for(int i=0;i<64;i++) stars[i]=-1;
	relocate.clear();
	int star;
	for(int i=0;i<64;i++){
		star=_track(starX[i],starY[i]);
		if(stars[star]==-1){
			stars[star]=i;
		}else{
			relocate.push_back(star);
			if(stars[star]>-1){
				relocate.push_back(stars[star]);
				stars[star]=-2;
			}
		}
	}
	return ;
}
int _rand_gene(){
	int cnt=0,x,y;
	for(x=1;x<33;x++){
		do{
			y=rand()%32+1;
		}while(starmap[x][y]!=0);
		_add_star(x,y,cnt); cnt++;
	}
	for(y=1;y<33;y++){
		if(B[y]>0) continue;
		do{
			x=rand()%32+1;
		}while(starmap[x][y]!=0);
		_add_star(x,y,cnt); cnt++;
	}
	for(int z=1;z<33;z++){
		if(C[z]>0) continue;
		if(cnt==64) return -1;
		do{
			x=rand()%32+1;
			y=z+16-x;
		}while(y<1||y>32||starmap[x][y]!=0);
		_add_star(x,y,cnt); cnt++;
	}
	while(cnt<64){
		x=rand()%32+1; y=rand()%32+1;
		if(starmap[x][y]==0){
			_add_star(x,y,cnt); cnt++;
		}
	}
	return _check();
}
void _change_board(int id,int ex,int ey){
	A[starX[id]]--; B[starY[id]]--;
	C[starX[id]+starY[id]-16]--;
	A[ex]++; B[ey]++; C[ex+ey-16]++;	
	starmap[starX[id]][starY[id]]=0;
	starmap[ex][ey]=1;
	starX[id]=ex; starY[id]=ey;
	return ;
}
bool _necessary(int x,int y){
	if(A[x]==0||B[y]==0||C[x+y-16]==0)
		return false;	
	return true;
}
int _rand_move(int height,int M){
	int new_height,id[4],sx[4],sy[4],ex[4],ey[4];
	id[0]=relocate[rand()%relocate.size()];
	if(M==2){
		id[1]=rand()%63;
		if(id[1]==id[0]) id[1]++;
	}else{
		do{
			id[1]=relocate[rand()%relocate.size()];
		}while(id[1]==id[0]);
		do{
			id[2]=rand()%64;
		}while(id[2]==id[0]||id[2]==id[1]);
		if(rand()%100>0){
			do{
				id[3]=rand()%64;
			}while(id[3]==id[0]||id[3]==id[1]||id[3]==id[2]);
		}else M=3;
	}

	for(int i=0;i<M;i++){
		sx[i]=starX[id[i]]; sy[i]=starY[id[i]];
		do{
			ex[i]=rand()%32+1; ey[i]=rand()%32+1;
		}while(starmap[ex[i]][ey[i]]!=0);
		_change_board(id[i],ex[i],ey[i]);
	}
	for(int i=0;i<M;i++){
		if(!_necessary(sx[i],sy[i])) goto FAIL;
	}
	new_height=_check();
	if(new_height>height) return new_height;
	else if(height==62 && new_height==height) return new_height;
FAIL:
	for(int i=M-1;i>-1;i--)
		_change_board(id[i],sx[i],sy[i]);
	return -1;
}
bool try_once(){
	int height,new_height,cnt;
	bool flag;
	do{
		_init();
		height=_rand_gene();
	}while(height<49);
	do{
		_find_relocate(); flag=false;
		if(height<62){
			for(cnt=0;cnt<12000000;cnt++){
				new_height=_rand_move(height,2);
				if(new_height>height){
					height=new_height;
					printf("%d ",height);
					flag=true;
					break;
			}}
		}else{
			for(cnt=1;cnt<100000001;cnt++){
				if(cnt%10000000==0) printf("-");
				new_height=_rand_move(height,2);
				if(new_height>=height){
					height=new_height;
					printf("%d ",height);
					flag=true;
					break;
			}}	
		}
		if(flag==false){
			printf("\n"); return false;
		}
	}while(height<64);
	return true;
}
void _print(){
	FILE *fp = fopen("out.txt", "a");
	for(int i=1;i<33;i++){
		for(int j=1;j<i;j++) fprintf(fp," ");
		for(int j=1;j<33;j++)
				fprintf(fp,"%d ",starmap[i][j]);
		fprintf(fp,"\n");
	}
	for(int i=0;i<64;i++)
		fprintf(fp,"(%d,%d) ",starX[i],starY[i]);
	fprintf(fp,"\n\n");
	fclose(fp);
	return ;
}
int main() {
	srand(time(0));
	while(try_once()==false){
		srand(time(0));
	}
	_print();
	return 0;
}