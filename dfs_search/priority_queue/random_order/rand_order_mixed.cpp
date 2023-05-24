#pragma GCC optimize(3)
#include <cstdio>
#include <random>
#include <time.h>
#include <queue>
#include <vector>
using namespace std;
int starmap[34][34],starX[64],starY[64],order[64];
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
struct star_pos{
	int x,y,r;
    bool operator< (const star_pos& t) const{
        return r<t.r;
    }
};
priority_queue<star_pos> Q[64];
vector<int> posX[64],posY[64];
int max_depth=-1;
void _init(){
	srand(time(0));
	int a,b,r,tmp;
	for(int j=0;j<34;j++)
		starmap[0][j]=starmap[33][j]=-2;
	for(int i=1;i<33;i++){
		a=((17-i)>1)?(17-i):1;
		b=((49-i)<33)?(49-i):33;
		for(int j=0;j<a;j++) starmap[i][j]=-2;
		for(int j=a;j<b;j++) starmap[i][j]=0;
		for(int j=b;j<34;j++) starmap[i][j]=-2;
	}
	for(int i=0;i<64;i++) order[i]=i;
	for(int j=63;j>0;j--){
		r=rand()%j;
		tmp=order[j];
		order[j]=order[r];
		order[r]=tmp;
	}
	return ;
}
void _print(){
	int cnt=0;
	FILE *fp = fopen("out.txt", "a");
	for(int i=1;i<33;i++){
		for(int j=1;j<i;j++) fprintf(fp," ");
		for(int j=1;j<33;j++)
			if(starmap[i][j]==-2)
				fprintf(fp,"X ");
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
int _check(int id,int x,int y){
	bool _flag1,_flag2;
	int ex,ey,cnt=0;
	for(int i=0;i<6;i++){
		ex=x+dX[i]; ey=y+dY[i];
		if((id>>i)%2==0){
			while(starmap[ex][ey]!=-2){
				if(starmap[ex][ey]==-1) return -1;
				ex+=dX[i]; ey+=dY[i];
			}
		}else{
			_flag1=_flag2=false;
			while(starmap[ex][ey]!=-2){
				if(starmap[ex][ey]==0){
					_flag1=true;
					if(_flag2==true) break;
				}else if(starmap[ex][ey]==-1){
					if(_flag2==false){
						cnt++; _flag2==true;
						if(_flag1==true) break;
					}
				}
				ex+=dX[i]; ey+=dY[i];
			}
			if(_flag1==false) return -1;
		}
	}
	return cnt;
}
void _change_board(int id,int x,int y,int d){
	int ex,ey;
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
bool _dfs(int depth){ 
	if(depth>max_depth){
		max_depth=depth;
		printf("%d ",depth);
	}
	if(depth==64){
		_print(); return true;
	}
	int x,y,r,height=order[depth];
if(height>21){
	star_pos tmp;
	for(x=1;x<33;x++){
		for(y=1;y<33;y++){
			if(starmap[x][y]!=0) continue;
			r=_check(height,x,y);
			if(r==-1) continue;
			tmp.x=x; tmp.y=y; tmp.r=(r*32+rand()%32);
			Q[height].push(tmp);
		}
	}
	while(!Q[height].empty()){
		tmp=Q[height].top();
		x=tmp.x; y=tmp.y;
		Q[height].pop();
		_change_board(height,x,y,1);
		starX[height]=x; starY[height]=y;
		if(_dfs(depth+1)) return true;
		_change_board(height,x,y,-1);
	}
}else{
	for(x=1;x<33;x++){
		for(y=1;y<33;y++){
			if(starmap[x][y]!=0) continue;
			if(_check(height,x,y)==-1) continue;
			posX[height].push_back(x);
			posY[height].push_back(y);
		}
	}
	while(!posX[height].empty()){
		r=rand()%posX[height].size();
		x=posX[height][r]; y=posY[height][r];
		posX[height].erase(posX[height].begin()+r);
		posY[height].erase(posY[height].begin()+r);
		_change_board(height,x,y,1);
		starX[height]=x; starY[height]=y;
		if(_dfs(depth+1)) return true;
		_change_board(height,x,y,-1);
	}
}
	return false;
}
int main(){
	_init();
	if(!_dfs(0))
		printf("Fail");
	return 0;
}
