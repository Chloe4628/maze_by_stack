// 迷宫 栈dfs求解 
#include <stdio.h> 
#include <stdlib.h>

#define MAX_SIZE 10000

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1 

#define MAZE_SIZE 10
int maze[MAZE_SIZE + 2][MAZE_SIZE + 2] = {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,1,1,1,0,1,0,1,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,1},
    {1,1,1,0,1,1,1,0,1,0,1,1},
    {1,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,1,0,1},
    {1,0,1,0,0,0,1,0,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,1,0,1},
    {1,0,0,0,1,1,1,0,0,0,0,1},
    {1,1,1,0,0,0,1,1,1,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
};

typedef int Status;
typedef struct{
	int x;
	int y;
}PosType; 
typedef struct{
	int ord; // 通道在路径序号 
	PosType seat; // 坐标位置 
	int di; // 方向 取1234 	
}ElemType;
typedef struct{
	ElemType data[MAX_SIZE]; 
	int top;
}SqStack;

Status Init_Stack(SqStack &s);
Status IsEmpty(SqStack s);
Status IsFull(SqStack s);
Status Push(SqStack &s, ElemType e);
Status Pop(SqStack &s, ElemType &e);
Status GetTop(SqStack s, ElemType &e);
Status Display_Maze();
Status IsRoad(ElemType path); 
Status FindNext(ElemType &now_path);
Status MazePath();

int main(){
	Display_Maze();
	if (MazePath()){
		Display_Maze();
	}
	return 0;
}

Status Init_Stack(SqStack &s){
	s.top = -1;
	return OK;
}

Status IsEmpty(SqStack s){
	if (s.top != -1){
		return FALSE;
	} else {
		return TRUE;
	}
}

Status IsFull(SqStack s){
	if (s.top != MAX_SIZE - 1){
		return FALSE;
	} else {
		return TRUE;
	}
}

Status Push(SqStack &s, ElemType e){
	if (IsFull(s)){
		return OVERFLOW;
	}
	s.top++;
	s.data[s.top] = e;
	return OK;
}

Status Pop(SqStack &s, ElemType &e){
	if (IsEmpty(s)){
		return OVERFLOW;
	}
	e = s.data[s.top];
	s.top--;
	return OK;
}

Status GetTop(SqStack s, ElemType &e){
	if (IsEmpty(s)){
		return ERROR;
	}
	e = s.data[s.top];
	return OK;
}

Status Display_Maze(){
	for (int i = 0; i < MAZE_SIZE + 2; i++){
		for (int j = 0; j < MAZE_SIZE + 2; j++){
			if (maze[i][j] == 1){
				printf("[]");
			} else if (maze[i][j] == -1){
				printf("**");
			} else {
				printf("  ");
			}
		}
		putchar('\n');
	}
	return OK;
}

Status IsRoad(ElemType path){
	if (maze[path.seat.x][path.seat.y] == 0){
		return TRUE;
	} else {
		return FALSE;
	}
}

Status FindNext(ElemType &now_path){
	switch (now_path.di){
		case 1: now_path.seat.y++; break;
		case 2: now_path.seat.x++; break;
		case 3: now_path.seat.y--; break;
		case 4: now_path.seat.x--; break;
	}
	now_path.di = 1; // 新的方向仍按照1234顺序 
	return OK;
}

Status MazePath(){
	int step = 0;
	SqStack s;
	Init_Stack(s);
	
	PosType ini_pos = {1, 1};
	ElemType ini_path = {1, ini_pos, 1};
	ElemType now_path = ini_path; 
	do {
		Status isfind = IsRoad(now_path);
		if (isfind){ // 当前位置可通 
			maze[now_path.seat.x][now_path.seat.y] = -1; // 路径内标记为-1 
			Push(s, now_path);
			step++;
			now_path.ord = step;
			if (now_path.seat.x == MAZE_SIZE && now_path.seat.y == MAZE_SIZE){
				printf("found the path!step = %d\n", step);
				return TRUE;
			} else {
				FindNext(now_path);
			}
		} else { // 当前位置不可通 
			if (!IsEmpty(s)){
				Pop(s, now_path);
				step--;
				while (now_path.di == 4 && !IsEmpty(s)){
					maze[now_path.seat.x][now_path.seat.y] = -2; // 已经搜索过周围发现均不通标记为-2 
					Pop(s, now_path);
					step--;
				}
				if (now_path.di < 4){ 
					now_path.di++;
					now_path.ord = ++step;
					Push(s, now_path); // 一直退到可以重新搜索的地方（必然是可通的 可以直接入栈 实际上就是换了方向） 
					maze[now_path.seat.x][now_path.seat.y] = -1; // 路径内标记为-1 其实应该本就是-1 
					FindNext(now_path); 
				}
			}
		}
	} while (!IsEmpty(s));
	printf("cannot find a path!\n");
	return FALSE;
}
