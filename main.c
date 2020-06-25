#include "snek_api.h"
#include <unistd.h>
int out_nextNum;
int new_neighbours[4];
int moogleloc;
int di;
int temp_int = 0;
int new_head;
int new_tail;
int updated_dir;


void play_game() {
	printf("starting\n");
	//board initialized, struct has pointer to snek
	GameBoard* board = init_board();
	show_board(board);

	int axis = AXIS_INIT;
	int direction = DIR_INIT;
	
	int play_on = 1;
	int coord[2];
  int moogleLocX;
  int moogleLocY;

	int n = BOARD_SIZE;
  int arr_count = 1;
  
  /*
  int arr[10][10] = {
    {1,2,5,6,25,26,29,30,31,32},
    {100,3,4,7,24,27,28,35,34,33},
    {99,10,9,8,23,22,21,36,37,38},
    {98,11,12,13,18,19,20,41,40,39},
    {97,90,89,14,17,58,57,42,43,44},
    {96,91,88,15,16,59,56,55,54,45},
    {95,92,87,76,75,60,51,52,53,46},
    {94,93,86,77,74,61,50,49,48,47},
    {83,84,85,78,73,62,63,64,65,66},
    {82,81,80,79,72,71,70,69,68,67}
  }; 
  */

  //this one has too many turns
  /*
  int arr[10][10] = {
    {1,2,5,6,9,10,13,14,17,18},
    {100,3,4,7,8,11,12,15,16,19},
    {99,96,95,32,31,28,27,24,23,20},
    {98,97,94,33,30,29,26,25,22,21},
    {91,92,93,34,37,38,41,42,45,46},
    {90,89,88,35,36,39,40,43,44,47},
    {81,82,87,86,65,64,63,62,49,48},
    {80,83,84,85,66,67,60,61,50,51},
    {79,76,75,72,71,68,59,56,55,52},
    {78,77,74,73,70,69,58,57,54,53}
  };*/
  
  int arr[n][n];

  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
            arr[i][j] = -1;
        }
    }
    //First Row
    while(arr_count <= n) {
        arr[0][arr_count-1] = arr_count;
	      arr_count++;
    }
    //Subsequent Rows
    for(int i = 1; i < n; i++) {
        if(i % 2 == 0) { //iterate right
            for(int j = 1; j < n; j++) {
                arr[i][j] = arr_count++;
            }
        }
        else { //iterate left
            for(int j = n-1; j > 0; j--) {
                arr[i][j] = arr_count++;
            }
        }
    }

    //First Column
    for(int i = n-1; i > 0; i--) {
        arr[i][0] = arr_count++;   
    }
  
	int neighbour_down;
  int neighbour_up;
  int neighbour_left;
  int neighbour_right;

  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      printf("%d ", arr[i][j]);
    }
    printf("\n");
  }

  //play_on = 0;
	while (play_on){	
		coord[x] = board->snek->head->coord[x];
		coord[y] = board->snek->head->coord[y];
		int curr = arr[coord[y]][coord[x]];
		int found = 0;
		int newX, newY;
    int goHam = 1;
    int a = -1;
		//int axis_temp = AXIS_X;
     
    int tailNum = arr[board->snek -> tail -> coord[x]][board->snek -> tail -> coord[y]];
    int headNum = arr[board->snek -> head -> coord[x]][board->snek -> head -> coord[y]];
    new_head = headNum;
    new_tail = tailNum;
    int moogleNum;
    int greatest = -1;
    int leftNum = -1;
    int rightNum = -1;
    int upNum = -1;
    int downNum = -1;
    int diversion = 0;
    int leftOffset = 0;
    int rightOffset = 0;
    int upOffset = 0;
    int downOffset = 0;

    if (MOOGLE_FLAG) {
      for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
          if(board->cell_value[i][j] > 1) {
            moogleLocX = j;
            moogleLocY = i;
          }
        }
      }
      moogleNum = arr[moogleLocX][moogleLocY]; 


      int distTail = distance(headNum, tailNum);
      int distMoogle = distance(headNum, moogleNum);
      int available = distTail-4;

      if(distMoogle < distTail)
        available--;

      if(distMoogle < available)
        available = distMoogle;
      if(available < 0)
        available = 0;

      if(coord[x] > 0)
        leftNum = arr[coord[x]-1][coord[y]];
      if(coord[x] < BOARD_SIZE-1)
        rightNum = arr[coord[x]+1][coord[y]];
      if(coord[y] > 0)
        upNum = arr[coord[x]][coord[y]-1];
      if(coord[y] < BOARD_SIZE-1)
        downNum = arr[coord[x]][coord[y]+1];

      int greatestDist = -1;
      if(coord[x] > 0 && board->occupancy[coord[y]][coord[x]-1] != 1) {
        int leftDist = distance(headNum, leftNum);
        if(leftDist <= available && leftDist > greatestDist) {
          greatestDist = leftDist;
          greatest = leftNum;
        }
      }
      if(coord[x] < BOARD_SIZE-1 && board->occupancy[coord[y]][coord[x]+1] != 1) {
        int rightDist = distance(headNum, rightNum);
        if(rightDist <= available && rightDist > greatestDist) {
          greatestDist = rightDist;
          greatest = rightNum;
        }
      }
      if(coord[y] > 0 && board->occupancy[coord[y]-1][coord[x]] != 1) {
        int upDist = distance(headNum, upNum);
        if(upDist <= available && upDist > greatestDist) {
          greatestDist = upDist;
          greatest = upNum;
        }
      }
      if(coord[y] < BOARD_SIZE-1 && board->occupancy[coord[y]+1][coord[x]] != 1) {
        int downDist = distance(headNum, downNum);
        if(downDist <= available && downDist > greatestDist) {
          greatestDist = downDist;
          greatest = downNum;
        }
      }
    }

    
    if(greatest == -1) 
      greatest = (headNum < BOARD_SIZE*BOARD_SIZE) ? headNum + 1 : 1;
    for(newX = 0; newX < n && found == 0; newX += found == 0) { 
      for(newY = 0; newY < n && found == 0; newY += found == 0) {
        if(greatest == arr[newX][newY]) 
          found = 1;
      }
    } 

    if(board->occupancy[newY][newX] == 1) {
      diversion = 1;
      newY = board->snek->head->coord[y];
      newX = board->snek->head->coord[x];
      if(newX > 0 && board->occupancy[newY][newX-1] != 1 && coord[x] != newX-1) {
        newX--;
        rightOffset = 1;
      }
      else if(newX < BOARD_SIZE-1 && board->occupancy[newY][newX+1] != 1 && coord[x] != newX+1) {
        newX++;
        leftOffset = 1;
      }
      else if(newY > 0 && board->occupancy[newY-1][newX] != 1 && coord[y] != newY-1) {
        newY--;
        upOffset = 1;
      }
      else if(newY < BOARD_SIZE-1 && board->occupancy[newY+1][newX] != 1 && coord[y] != newY+1) {
        newY++;
        downOffset = 1;
      }
    }

    if(newX > coord[x]) {
      axis = AXIS_X;
      direction = RIGHT;
    }
    else if(newX < coord[x]) {
      direction = LEFT;
      axis = AXIS_X;

    }
    else if(newY > coord[y]) {
      axis = AXIS_Y;
      direction = DOWN;
    }
    else if(newY < coord[y]) {
      axis = AXIS_Y;
      direction = UP;
    }


		show_board(board);
		play_on = advance_frame(axis, direction, board);
		printf("\nGoing \n");
    printf("tailNum: %d   ", tailNum);
    printf("headNum: %d\n", headNum);
    printf("diversion: %d\n", diversion);
    printf("moogle is at %d \n", moogleNum);
    printf("shortcut num: %d  adjusted num: %d\n", a, greatest);
    printf("leftNum: %d\n", leftNum);
    printf("rightNum: %d\n", rightNum);
    printf("upNum: %d\n", upNum);
    printf("downNum: %d\n", downNum); 
    printf("left: %d  right: %d  up: %d  down: %d\n", leftOffset, rightOffset, upOffset, downOffset);
    printf("Occupancy: %d\n", board->occupancy[newY][newX]);
    
		if (axis == AXIS_X){
			if (direction == RIGHT){
				printf("RIGHT");
			} else {
				printf("LEFT");
			}
		} else {
			if (direction == UP){
				printf("UP");
			} else {
				printf("DOWN");
			}
		} printf("\n");
		usleep(10000);
    //usleep(1000000);
    //usleep(90000);
	}
	end_game(&board);
  
  

}

int main(){
	play_game();
	return 0;
}
