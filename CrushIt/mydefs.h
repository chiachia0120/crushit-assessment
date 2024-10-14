#pragma once

/* Put anything you like here, but
   certainly your 'state' structure
   to hold your board e.g.

#define MAXROWS 20
#define WIDTH 5
#define HEIGHT 6
struct st {
   char board[MAXROWS][WIDTH];
};
typedef struct st state;

*/
#define MAXROWS 20
#define WIDTH 5
#define HEIGHT 6
struct st {
  char board[MAXROWS][WIDTH + 1];  // +1 for null terminator
  int rows;
};
typedef struct st state;
