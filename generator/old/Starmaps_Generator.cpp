#pragma GCC optimize(3)
#include <stdio.h>
#include <time.h>
#include <random>
#include <vector>
using namespace std;
int starmap[34][34],A[34],B[34],C[34];
int starX[64],starY[64],stars[64];
/*starmap[][] is the board (0=empty, 1=star,
  2=outside the board). A[], B[] and C[] record the
  number of stars on each line in the 3 directions.
  starX[] and starY[] record the coord of each star.
  stars[]: See function _track() (*) */
const int dX[6]={0,0,-1,-1,1,1},dY[6]={-1,1,0,1,-1,0};
vector<int> relocate; //See function _find_relocate().
void _initialize(){//initialize
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
int _track(int x,int y){
//Find the neighbour-condition of the star at (x,y).
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
/*star=(a5 a4 a3 a2 a1 a0) in binary. ai=1 if it has a
  neighbour in direction (dX[i],dY[i]), and ai=0 if not.
  So 0<=star<=63 show the star's neighbour-condition.
  (*) stars[(a5a4a3a2a1a0)] record the amount of stars
  whose neighbour-condition is (a5a4a3a2a1a0).*/
	return star;
}
int _check(){
//Check how many different kinds of stars appear.
	int star,cnt=0;
	for(int i=0;i<64;i++) stars[i]=0;
	for(int i=0;i<64;i++){
		star=_track(starX[i],starY[i]);
		if(stars[star]==0){
			stars[star]=1;
			cnt++;
		}
	}
	return cnt;
}
void _add_star(int x,int y,int cnt){
	starX[cnt]=x; starY[cnt]=y; starmap[x][y]=1;
	A[x]++; B[y]++; C[x+y-16]++; 
	return ;
}
int _rand_setup(){
/*Neccesary condition: At least one star on each line
  all three directions. Cannot reach 49/64 without it.*/
  	int cnt=0,x,y;
	for(x=1;x<33;x++){//Horizon lines
		do{
			y=rand()%32+1;
		}while(starmap[x][y]!=0);
		_add_star(x,y,cnt); cnt++;
	}
	for(y=1;y<33;y++){//Left-up to right-down lines
		if(B[y]>0) continue;
		do{
			x=rand()%32+1;
		}while(starmap[x][y]!=0);
		_add_star(x,y,cnt); cnt++;
	}
	for(int z=1;z<33;z++){//Right-up to left-down lines
		if(C[z]>0) continue;
		if(cnt==64) return -1;
		/*If there's an empty line while there're already
		64 stars, then the setup fails.*/
		do{
			x=rand()%32+1;
			y=z+16-x;
		}while(y<1||y>32||starmap[x][y]!=0);
		_add_star(x,y,cnt); cnt++;
	}
	while(cnt<64){//The remainning stars
		x=rand()%32+1; y=rand()%32+1;
		if(starmap[x][y]==0){
			_add_star(x,y,cnt); cnt++;
		}
	}
	return _check();
}
void _find_relocate(){//Record all repeat stars
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
void _change_board(int id,int ex,int ey){
	//Move the star at (starX[id],starY[id]) to (ex,ey)
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
int _rand_move(int height){
/*Randomly relocate 2 stars. Doesn't work if only
  relocate 1 star, and costs more time if >=3.*/
	int new_h,id[2],sx[2],sy[2],ex[2],ey[2];
	id[0]=relocate[rand()%relocate.size()];
	//Make sure at least one repeat star will be relocated.
	id[1]=rand()%63;
	if(id[1]==id[0]) id[1]++;
	
	for(int i=0;i<2;i++){
		sx[i]=starX[id[i]]; sy[i]=starY[id[i]];
		/*Record the original position, so it'll be able
		to redo the move if it fails.*/
		do{
			ex[i]=rand()%32+1; ey[i]=rand()%32+1;
		}while(starmap[ex[i]][ey[i]]!=0);
		_change_board(id[i],ex[i],ey[i]);
	}
	for(int i=0;i<2;i++)
		if(!_necessary(sx[i],sy[i])) goto FAIL;
/*Do not waste time for checking new_height if it doesn't
  meet the necessary condition (see _rand_setup()).
  Sorry for using "goto", but it's convinient here. */
	
	new_h=_check();
	if(new_h<height||(new_h==height&&height<59)){
		for(int i=1;i>-1;i--)//Redo the move
			_change_board(id[i],sx[i],sy[i]);
	}
	return new_h;
	
	FAIL:
	for(int i=1;i>-1;i--)
		_change_board(id[i],sx[i],sy[i]);	
	return -1;
}
bool try_once(){
	int height,new_h;
	//height=X means X/64 different kinds of stars appear
	
	do{//Part I: Randomly generate a 49/64 setup
		/*49 is the best result I can get using this
		method within acceptable time (several seconds)*/
		_initialize();
		height=_rand_setup();
	}while(height<49);
	
	int cnt,cnt_2=0;
	do{//Part II: Random Hill-Climbing
		_find_relocate();
		for(cnt=0;cnt<12000000;cnt++){
		/*12000000 was a good limit to reach 62/64 before
		  I discovered that new_h==height is also
		  acceptable. Now it seems too large. */
			new_h=_rand_move(height);
			if(new_h>height||(height>=59&&new_h==height)){
			//Allow more tries when X/64 is reached.
			/*Larger X results in more tries in main(), and
			  smaller X results in more tries in this
			  do-while. X=59 seems a good number. */
				height=new_h;
				if(height==64) return true;
				//printf("%d ",height);
				cnt_2++;
				break;
			}
		}
		if(cnt==12000000) return false;
	}while(cnt_2<600);
//Never exceeds so far, but still need a limit for safety.
	return false;
}
void _print(){//Print the starmap to the file.
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
	printf("Please wait for several minutes.\n");
	int cnt=0;
	do{
		printf("Number of tries: %d\n",cnt);
		cnt++;
		srand(time(0));
	}while(try_once()==false);
	_print();
	printf("Success after %d tries.\n",cnt);
	printf("Please check \"out.txt\" for the result.");
	getchar(); getchar();
	return 0;
}