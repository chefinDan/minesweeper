#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CIRCUM 8

struct square {
	int x;
	int y;
	int adjMines;
	int isRevealed;
	int hasMine;
	int isFlagged;
	int offGrid;
};

struct map {
	int size;
	int mineCnt;
	struct square **grid;
	struct square *root;
};

int myRand(int n){
	return (rand() % n) +1; 
}

void mySrand(){
	time_t t;
	srand((unsigned) time(&t));
}

void printTopLabel(int n){
	int i;
	printf("  ");
	for(i = 0; i < n; ++i){
		printf(" %c", 97+i);
	}
	printf("\n");
}

void printRow(struct square *m, int n){
	int i;
	printf("%c", '|');
	for(i = 0; i < n; ++i){
		if(m[i].isRevealed){
			if(m[i].hasMine){
				printf("%c", 'x');
			}
			else{
				printf("%c", 'o');
			}
		}
		else{
			printf("%c", ' ');
		}
		printf("%c", '|');
	}
	printf("\n");
}

void printMap(struct map *m){
	int i, n;
	n = m->size;
	printTopLabel(n);

	for(i = 0; i < n; ++i){
		printf("%d ", i+1);
		printRow(m->grid[i], n);
	}
}

void makeOffGrid(struct square **sq){
	*sq = (struct square*)malloc(sizeof(struct square));
	(*sq)->offGrid = 1;
}

struct square *click(struct map *m, char _x, int y){
	struct square *sq;
	int x;
	x = (_x-97);
	if(x < 0 || x >= m->size || y < 1 || y > m->size){
		return NULL;
	}
	y = y-1;

	sq = &m->grid[y][x];
	sq->isRevealed = 1;
	return sq;
}

void buildMap(struct map *m){
	int i, j, n;
	mySrand();
	n = m->size;
	m->mineCnt = 0;
	m->grid = malloc(n * sizeof(struct square*));
	for(i = 0; i < n; ++i){
		m->grid[i] = malloc(n * sizeof(struct square));
	}

	for(i = 0; i < n; ++i){
		for(j = 0; j < n; ++j){
			m->grid[i][j].x = j;
			m->grid[i][j].y = i;
			if(myRand(10) == 5){
				m->grid[i][j].hasMine = 1;
				m->mineCnt++;
			}
			else{
				m->grid[i][j].hasMine = 0;
			}
			m->grid[i][j].isRevealed = 0;
		}
	}
}

void getInput(char *buf){

	if(fgets(buf, 3, stdin)){
		char *p;
		if((p=strchr(buf, '\n'))){//check exist newline
			*p = 0;
		} else {
			scanf("%*[^\n]");scanf("%*c");//clear upto newline
		}
	}
	buf[1] = buf[1]-48;
}


int main(int argc, char *argv[]){
	struct map map;
	struct square *sq;
	char buf[3];
	char tmp[3];

	if(argc < 2){
		printf("Usage: minesweeper <map_size>\n");
		return 1;
	}

	map.size = atoi(argv[1]);

	buildMap(&map);
	while(1){
		printMap(&map);
		getInput(buf);
		sq = click(&map, buf[0], buf[1]);
		if(sq == NULL){
			dprintf(2, "Error: click out of bounds\n");
			continue;
		}
		if(sq->hasMine && !sq->isFlagged){
			printMap(&map);
			printf("BOOM\nGame Over\n");
			return 0;
		}
		system("clear");
	}
	



	return 0;
}