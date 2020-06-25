/**
AUTHOR: SAIMA ALI
LATEST WORKING VERSION 
FEBRUARY 2ND, 2020
ESC190H1S PROJECT
SNAKE API
 **/

#include <stdlib.h>
#include <stdio.h>

#define CYCLE_ALLOWANCE 1.5
#define BOARD_SIZE 10

#define LIFE_SCORE 1 //score awarded for simply staying alive one frame

#define AXIS_X -1
#define AXIS_Y 1

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

#define AXIS_INIT AXIS_Y
#define DIR_INIT DOWN

#define x 0
#define y 1

#define MOOGLE_POINT 20
#define HARRY_MULTIPLIER 3

int CURR_FRAME;
int SCORE;
int MOOGLE_FLAG;

typedef struct SnekBlock{
	int coord[2];
	struct SnekBlock* next;
} SnekBlock;

typedef struct Snek{
	struct SnekBlock* head;
	struct SnekBlock* tail;
	int length;
} Snek;

typedef struct GameBoard {
	int cell_value[BOARD_SIZE][BOARD_SIZE];
	int occupancy[BOARD_SIZE][BOARD_SIZE];
	struct Snek* snek;
} GameBoard;

typedef struct Node {
  int data;
  struct Node* next;
} Node;

typedef struct LinkedList {
  int size;
  struct Node* head;
} LinkedList;




GameBoard *init_board();
Snek *init_snek(int a, int b);
int hits_edge(int axis, int direction,  GameBoard *gameBoard);
int hits_self(int axis, int direction,  GameBoard *gameBoard);
int is_failure_state(int axis, int direction,  GameBoard *gameBoard);
int advance_frame(int axis, int direction,  GameBoard *gameBoard);
void end_game(GameBoard **board);
void show_board(GameBoard* gameBoard);
int get_score();
LinkedList* create_list(void);
void deleteList(LinkedList* list);
void push(LinkedList* list, int data);
int pop(LinkedList* list);
void enqueue(LinkedList* list, int data);
int dequeue(LinkedList*);
int numInList(LinkedList* list, int num);
LinkedList* bfs(int hamiltonian[BOARD_SIZE][BOARD_SIZE], int cellValue[BOARD_SIZE][BOARD_SIZE], int currNum, int endNum, int length);
int distanceCircuit(int n1, int n2);

