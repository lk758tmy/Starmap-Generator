#include <cstdio>
#include <random>
#include <time.h>
using namespace std;
int starmap[34][34],starX[64],starY[64];
//Use the 16-17 hex-board as shown on the website  
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
int min_height=65;//for debug
void _init(){//initialize
	srand(time(0));
	int a,b;
	for(int j=0;j<34;j++)
		starmap[0][j]=starmap[33][j]=-2;
	for(int i=1;i<33;i++){
		a=((17-i)>1)?(17-i):1;
		b=((49-i)<33)?(49-i):33;
		for(int j=0;j<a;j++) starmap[i][j]=-2;
		for(int j=a;j<b;j++) starmap[i][j]=0;
		for(int j=b;j<34;j++) starmap[i][j]=-2;
		/*Outside the board:-2 Star:-1
		Empty:>=0 (>=1 if can't be a star)*/
	}
	return ;
}
void _print(){//print the starmap to a txt file
	int cnt=0;
	FILE *fp = fopen("out.txt", "a");
	for(int i=1;i<33;i++){
		for(int j=1;j<i;j++) fprintf(fp," ");
		for(int j=1;j<33;j++)
			if(starmap[i][j]==-2)
				fprintf(fp,"X ");
			//else if(starmap[i][j]>99)
			else if(starmap[i][j]==-1)
				fprintf(fp,"O ");
			else fprintf(fp,"- ");
		fprintf(fp,"\n");
	}
	for(int i=0;i<64;i++)
		fprintf(fp,"(%d,%d) ",starX[i],starY[i]);
	fprintf(fp,"\n\n");
	fclose(fp);
	return ;
}
bool __check(int id,int x,int y){
//Check if the certain star can be put here
/*id ranges from 0 to 63, its binary form shows
whether the star has a neigbour in this direction.
Eg. 0 has no neibour and 63 have all 6 neigbours.*/
	bool flag=true,_flag;
	int ex,ey;
	for(int i=0;i<6;i++){
		ex=x+dX[i]; ey=y+dY[i];
		if((id>>i)%2==0){//No stars in this direction
			while(starmap[ex][ey]!=-2){
				if(starmap[ex][ey]==-1){
					flag=false; break;
				}
				ex+=dX[i]; ey+=dY[i];
			}
		}else{//Able to be stars in this direction
			_flag=false;
			while(starmap[ex][ey]!=-2){
				if(starmap[ex][ey]==0){
					_flag=true; break;
				}
				ex+=dX[i]; ey+=dY[i];
			}
			if(_flag==false) flag=false;
		}
		if(flag==false) break;
	}
	return flag;
}
void __change_board(int id,int x,int y,int d){
//Change or re-change the state of the starmap board
	int ex,ey;
	//starmap[x][y]+=100*d;
	if(d==1) starmap[x][y]=-1;
	else starmap[x][y]=0;
	for(int i=0;i<6;i++){
		if((id>>i)%2==1) continue;
		ex=x+dX[i]; ey=y+dY[i];
		while(starmap[ex][ey]!=-2){
			starmap[ex][ey]+=d;
			ex+=dX[i]; ey+=dY[i];
		}
	}
	return ;
}
bool _dfs(int height){//dfs search
//height equals to star's id, see above 
	if(height<min_height){
		min_height=height;
		printf("%d ",height);
	}
	if(height==0){
		_print(); return true;
	}
	int x,y;
	if(height<43){//traditional
		for(x=1;x<33;x++)
		for(y=1;y<33;y++){
			if(starmap[x][y]!=0) continue;
			if(!__check(height,x,y)) continue;
			__change_board(height,x,y,1);
			starX[height]=x; starY[height]=y;
			if(_dfs(height-1)) return true;
			__change_board(height,x,y,-1);
		}
	}else{//using randoms
		int randmax=700-50*(64-height);
		if(randmax<50) randmax=50;
		for(int i=0;i<randmax;i++){
			do{
				x=rand()%32+1,y=rand()%32+1;
			}while(starmap[x][y]!=0||!__check(height,x,y));
			__change_board(height,x,y,1);
			starX[height]=x; starY[height]=y;
			if(_dfs(height-1)) return true;
			__change_board(height,x,y,-1);
		}
	}
	return false;
}
int main(){
	_init();
	if(!_dfs(64))
		printf("Fail");
	return 0;
}
