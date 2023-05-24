#include <cstdio>
#include <ctime>
#include <random>
#include <vector>
using namespace std;
int starmap[34][34],starX[64],starY[64],height;
vector<int> stars[64];
const int dY[6]={-1,1,0,1,-1,0},dX[6]={0,0,-1,-1,1,1};
int _track(int x,int y){
	int star=0,tx,ty; bool flag;
	for(int i=0;i<6;i++){
		flag=false; tx=x; ty=y;
		while(1){
			tx+=dX[i]; ty+=dY[i];
			if(tx<0||ty<0||tx>31||ty>31) break;
			if(starmap[tx][ty]==2) break;
			if(starmap[tx][ty]==1){
				flag=true;
				break;
			}
		}
		if(flag==true) star+=(1<<i);
	}
	return star;
}
int _check(){
	bool star_appear[64],flag=false;
	int cnt=0,star;
	for(int i=0;i<64;i++){
		star_appear[i]=false;
		stars[i].clear();
	}
	for(int i=0;i<64;i++){
		if(starX[i]==-1) continue;
		star=_track(starX[i],starY[i]);
		stars[star].push_back(i);
		if(star_appear[star]==false){
			star_appear[star]=true; cnt++;
		}else flag=true;
	}
	if(flag==true) cnt+=64;
	return cnt;
}
int _reduce(){
	int r_tmp,x_tmp,y_tmp;;
	do{
		for(int i=0;i<64;i++){
			if(stars[i].size()<2) continue;
			r_tmp=rand()%stars[i].size();
			x_tmp=starX[stars[i][r_tmp]];
			y_tmp=starY[stars[i][r_tmp]];
			starmap[x_tmp][y_tmp]=0;
			starX[stars[i][r_tmp]]=-1;
			stars[i].erase(stars[i].begin()+r_tmp);
		}
		printf("%d ",_check());
	}while(_check()>64);
	return _check();
}
int _read(){
	FILE *fp = fopen("\in.txt", "r");
	int cnt=0;
	for(int i=0;i<34;i++)
		starmap[0][i]=starmap[33][i]=2;		
	for(int i=1;i<33;i++){
		starmap[i][0]=starmap[i][33]=2;
		for(int j=1;j<33;j++){
			fscanf(fp,"%d",&starmap[i][j]);
			if(starmap[i][j]==1){
				starX[cnt]=i; starY[cnt]=j; cnt++;
	}}}
	return _check();
}
int main() {
	srand(time(0));
	printf("%d ",_read());
	_reduce();
	//printf("%d ",_reduce());
	return 0;
}
