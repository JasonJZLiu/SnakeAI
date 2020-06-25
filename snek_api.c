/** snake API for C **/
// need to add the elongating of snek after nomming
#include "snek_api.h"
#include <string.h>
#include <time.h>

int CURR_FRAME = 0;
int SCORE = 0;
int MOOGLE_FLAG = 0;
int MOOGLES_EATEN = 0;
int TIME_OUT = ((BOARD_SIZE * 4) - 4) * CYCLE_ALLOWANCE;

GameBoard* init_board(){
	srand(time(0));
	GameBoard* gameBoard = (GameBoard*)(malloc(sizeof(GameBoard)));

	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			gameBoard->cell_value[i][j] = 0;
			gameBoard->occupancy[i][j] = 0;
		}
	}
	gameBoard->occupancy[0][0] = 1; //snake initialized
	gameBoard->snek = init_snek(0, 0);
	return gameBoard;
}

Snek* init_snek(int a, int b){
	Snek* snek = (Snek *)(malloc(sizeof(Snek)));

	snek->head = (SnekBlock *)malloc(sizeof(SnekBlock));
	snek->head->coord[x] = a;
	snek->head->coord[y] = b;

	snek->tail = (SnekBlock *)malloc(sizeof(SnekBlock));
	snek->tail->coord[x] = a;
	snek->tail->coord[y] = b;

	snek->tail->next = NULL;
	snek->head->next = snek->tail;
	
	snek->length = 1;

	return snek;
}

int hits_edge(int axis, int direction, GameBoard* gameBoard){
	// Replaced gameBoard->snek->head[y] with gameBoard->snek->head->coord[y]
	if (((axis == AXIS_Y) && ((direction == UP && gameBoard->snek->head->coord[y] + UP < 0) || (direction == DOWN && gameBoard->snek->head->coord[y] + DOWN > BOARD_SIZE - 1)))
	   || (axis == AXIS_X && ((direction == LEFT && gameBoard->snek->head->coord[x] + LEFT < 0) || (direction == RIGHT && gameBoard->snek->head->coord[x] + RIGHT > BOARD_SIZE-1))))
	{
		return 1;
	} else {
		return 0;
	}

}


int hits_self(int axis, int direction, GameBoard *gameBoard){
	int new_x, new_y;
	if (axis == AXIS_X){
		new_x = gameBoard->snek->head->coord[x] + direction;
		new_y = gameBoard->snek->head->coord[y];
	} else if (axis == AXIS_Y){
		new_x = gameBoard->snek->head->coord[x];
		new_y = gameBoard->snek->head->coord[y] + direction;
	}
	return gameBoard->occupancy[new_y][new_x]; //1 if occupied
}

int time_out(){
	return (MOOGLE_FLAG == 1 && CURR_FRAME > TIME_OUT);

}

int is_failure_state(int axis, int direction, GameBoard *gameBoard){
	return (hits_self(axis, direction, gameBoard) || hits_edge(axis, direction, gameBoard) || time_out());
}

void populate_moogles(GameBoard *gameBoard){
	if (MOOGLE_FLAG == 0){
		int r1 = rand() % BOARD_SIZE;
		int r2 = rand() % BOARD_SIZE;
		
		int r3 = rand() % (BOARD_SIZE * 10);
		if (r3 == 0){
			gameBoard->cell_value[r1][r2] = MOOGLE_POINT * HARRY_MULTIPLIER;
			MOOGLE_FLAG = 1;
		} else if (r3 < BOARD_SIZE){
			gameBoard->cell_value[r1][r2] = MOOGLE_POINT;
			MOOGLE_FLAG = 1;
		}
	}
}

void eat_moogle(GameBoard* gameBoard, int head_x, int head_y) {
	SCORE = SCORE + gameBoard->cell_value[head_y][head_x];
	gameBoard->cell_value[head_y][head_x] = 0;

	gameBoard->snek->length ++;
	MOOGLES_EATEN ++;
	MOOGLE_FLAG = 0;
	CURR_FRAME = 0;
}

int advance_frame(int axis, int direction, GameBoard *gameBoard){
	if (is_failure_state(axis, direction, gameBoard)){
		return 0;
	} else {
		// update the occupancy grid and the snake coordinates
		int head_x, head_y;
		// figure out where the head should now be
		if (axis == AXIS_X) {
			head_x = gameBoard->snek->head->coord[x] + direction;
			head_y = gameBoard->snek->head->coord[y];
		} else if (axis == AXIS_Y){
			head_x = gameBoard->snek->head->coord[x];
			head_y = gameBoard->snek->head->coord[y] + direction;
		}
		
		int tail_x = gameBoard->snek->tail->coord[x];
		int tail_y = gameBoard->snek->tail->coord[y];

		// update the occupancy grid for the head
		gameBoard->occupancy[head_y][head_x] = 1;

		if (gameBoard->snek->length > 1) { //make new head
			SnekBlock *newBlock = (SnekBlock *)malloc(sizeof(SnekBlock));
			newBlock->coord[x] = gameBoard->snek->head->coord[x];
			newBlock->coord[y] = gameBoard->snek->head->coord[y];
			newBlock->next = gameBoard->snek->head->next;

			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->head->next = newBlock;
	
			if (gameBoard->cell_value[head_y][head_x] > 0){  //eat something
				eat_moogle(gameBoard, head_x, head_y);
			} else { //did not eat
				//delete the tail
				gameBoard->occupancy[tail_y][tail_x] = 0;
				SnekBlock *currBlock = gameBoard->snek->head;
				while (currBlock->next != gameBoard->snek->tail){
					currBlock = currBlock->next;
				} //currBlock->next points to tail

				currBlock->next = NULL;
				free(gameBoard->snek->tail);
				gameBoard->snek->tail = currBlock;
			}

		} else if ((gameBoard->snek->length == 1) && gameBoard->cell_value[head_y][head_x] == 0){ // change both head and tail coords, head is tail
			gameBoard->occupancy[tail_y][tail_x] = 0;
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->tail->coord[x] = head_x;
			gameBoard->snek->tail->coord[y] = head_y;
			
		} else { //snake is length 1 and eats something
			eat_moogle(gameBoard, head_x, head_y);
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
		}

		// update the score and board
		SCORE = SCORE + LIFE_SCORE;
		if (MOOGLE_FLAG == 1){
			CURR_FRAME ++;
		}

		// populate moogles
		populate_moogles(gameBoard);
		return 1;
	} 
}

void show_board(GameBoard* gameBoard) {
	fprintf(stdout, "\033[2J"); // clear terminal ANSI code
	fprintf(stdout, "\033[0;0H"); // reset cursor position
	
	char blank = 	43;
	char snek = 	83;
  char snek_head = 72;
  char snek_tail = 84;
  int head_x = gameBoard->snek->head->coord[x];
  int head_y = gameBoard->snek->head->coord[y];
  int tail_x = gameBoard->snek->tail->coord[x];
  int tail_y = gameBoard->snek->tail->coord[y];
	char moogle = 	88;

	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			if (gameBoard->occupancy[i][j] == 1){
				//snake is here
        if(i == head_y && j == head_x)
          fprintf(stdout, "%c", snek_head);
        else if(i == tail_y && j == tail_x)
          fprintf(stdout, "%c", snek_tail);
        else
				  fprintf(stdout, "%c", snek);
			} else if (gameBoard->cell_value[i][j] > 0) {
				//there be a moogle
				fprintf(stdout, "%c", moogle);
			} else {
				//nothing to see here
				fprintf(stdout, "%c", blank);
			}
		} //new line
		fprintf(stdout, "\n");
		
	}

	fprintf(stdout, "\n\n");
	
	if (MOOGLE_FLAG == 1){
		fprintf(stdout, "!..ALERT, MOOGLE IN VICINITY..!\n\n");
	}
	fprintf(stdout, "SCORE: %d\n", SCORE);
	fprintf(stdout, "YOU HAVE EATEN %d MOOGLES\n\n", MOOGLES_EATEN);

	fprintf(stdout, "SNEK HEAD\t(%d, %d)\n", gameBoard->snek->head->coord[x], gameBoard->snek->head->coord[y]);
	fprintf(stdout, "SNEK TAIL\t(%d, %d)\n", gameBoard->snek->tail->coord[x], gameBoard->snek->tail->coord[y]);
	fprintf(stdout, "LENGTH \t%d\n", gameBoard->snek->length);
	fprintf(stdout, "CURR FRAME %d vs TIME OUT %d\n", CURR_FRAME, TIME_OUT);


	fflush(stdout);
}

int get_score() {
	return SCORE;
}

void end_game(GameBoard **board){
	//fprintf(stdout, "\033[2J");
	//fprintf(stdout, "\033[0;0H"); 
	fprintf(stdout, "\n\n\n--!!---GAME OVER---!!--\n\nYour score: %d\n\n\n\n", SCORE);
	fflush(stdout);
	// need to free all allocated memory
	// first snek
	SnekBlock **snekHead = &((*board)->snek->head);
	SnekBlock *curr;
	SnekBlock *prev;
	while ((*snekHead)->next != NULL) {
		curr = *snekHead;
		while (curr->next != NULL){
			prev = curr;
			curr = curr->next;
		}
		prev->next = NULL;
		free(curr);
	}
	free(*snekHead);
	free((*board)->snek);
	free(*board);
}

LinkedList* create_list(void) {
  LinkedList* list = malloc(sizeof(LinkedList));
  list -> size = 0;
  list -> head = NULL;
  return list;
}
void deleteList(LinkedList* list) {
  while(list -> size != 0)
    pop(list);
  free(list);
}
void push(LinkedList* list, int data) {
  Node* node = malloc(sizeof(Node));
  node -> data = data;
  node -> next = list -> head;
  list -> head = node;
  list -> size ++;
}
int pop(LinkedList* list) {
  Node* ptr = list -> head;
  int data = ptr -> data;
  list -> head = ptr -> next;
  free(ptr);
  ptr = NULL;
  list -> size--;
  return data;
}
void enqueue(LinkedList* list, int data) {
  Node* node = malloc(sizeof(Node));
  Node* curr;
  //when the node's next is NULL, that's how you reach the end of the list
  for(curr = list -> head; curr -> next != NULL; curr = curr -> next);

  curr -> next = node;
  node -> data = data;
  node -> next = NULL;
  list -> size++;
}
int dequeue(LinkedList* list) {
  return pop(list);
}
int numInList(LinkedList* list, int num) {
  int found = 0;
  Node* curr = list -> head;
  for(int i = 0; i < list -> size && !found; i++, curr = curr -> next) {
    if(curr -> data == num)
      found = 1;
  }
  return found;
}

LinkedList* bfs(int hamiltonian[BOARD_SIZE][BOARD_SIZE], int cellValue[BOARD_SIZE][BOARD_SIZE], int currNum, int endNum, int length) {
  LinkedList* queue = create_list();
  LinkedList* visited = create_list();
  LinkedList* path = create_list();
  int prev[length*length];

  enqueue(queue, currNum);
  //Refer to the node by its value in the Hamiltonian Circuit
  int node = 0;

  while(queue -> size > 0 && node != endNum) {
    node = pop(queue);
    
    //visit nodes that have not been visited
    if(!numInList(visited, node)) { 
      int currX = 0, currY = 0;
      
      //getting location of the current node
      for(; currX < length && hamiltonian[currY][currX] != node; currX++) 
        for(; currY < length && hamiltonian[currY][currX] != node; currY++);
      
      //exploring the four possibilities of adjacent nodes
      //updating their prev array
      //exploring the left node
      if(currX > 0 && cellValue[currY][currX-1] != 1) {
        enqueue(queue, hamiltonian[currY][currX-1]);
        prev[hamiltonian[currY][currX-1]] = node;
      }
      //exploring the right node
      if(currX < length-1 && cellValue[currY][currX+1] != 1) {
        enqueue(queue, hamiltonian[currY][currX+1]);
        prev[hamiltonian[currY][currX+1]] = node;
      }
      //exploring the top node
      if(currY > 0 && cellValue[currY-1][currX] != 1) {
        enqueue(queue, hamiltonian[currY-1][currX]);
        prev[hamiltonian[currY-1][currX]] = node;
      }
      //exploring the bottom node
      if(currY < length-1 && hamiltonian[currY+1][currX] != 1) {
        enqueue(queue, hamiltonian[currY+1][currX]);
        prev[hamiltonian[currY+1][currX]] = node;
      }

      push(visited, node);
    } 
  }

  if(node == endNum) {
    push(path, node);
    while(node != currNum) {
      node = prev[node-1];
      push(path, node);
    }
  }

}

int distance(int n1, int n2) {
  return (n1 < n2) ? n2 - n1 -1 : n2-n2-1+BOARD_SIZE*BOARD_SIZE; 
}

