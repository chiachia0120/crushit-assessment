#include "crushit.h"

#include "mydefs.h"

// Check if each character in the string is valid (A-Z)
bool isValidCharacterString(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if ((str[i] < 'A' || str[i] > 'Z') && str[i] != '.') {
      return false;
    }
  }
  return true;
}

bool isValidBoardContent(state *s, const char *str) {
  int len = strlen(str);
  int height = len / WIDTH;

  // Check if the string length is a multiple of WIDTH
  if (len % WIDTH != 0) {
    return false;
  }

  // Check if the number of rows exceeds 20
  if (height > MAXROWS) {
    return false;
  }

  // Check the validity of each character
  if (!isValidCharacterString(str)) {
    return false;
  }

  // Populate the board
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < WIDTH; j++) {
      s->board[i][j] = str[i * WIDTH + j];
    }
    s->board[i][WIDTH] = '\0';
  }
  s->currentHeight = height;
  return true;
}

void removeTopRowIfEmpty(state *s) {
  // Check if the top row is empty
  bool topRowEmpty = true;
  for (int j = 0; j < WIDTH; j++) {
    if (s->board[0][j] != '.') {
      topRowEmpty = false;
      break;
    }
  }

  // Shift rows up if the top row is empty
  if (topRowEmpty) {
    for (int i = 0; i < s->currentHeight - 1; i++) {
      for (int j = 0; j < WIDTH; j++) {
        s->board[i][j] = s->board[i + 1][j];
      }
    }

    // Clear the last row
    for (int j = 0; j < WIDTH; j++) {
      s->board[s->currentHeight - 1][j] = '.';
    }
  }
}

bool initialise(state *s, const char *str) {
  if (s == NULL || str == NULL) {
    return false;
  }

  FILE *file = fopen(str, "r");
  if (file != NULL) {
    char buffer[WIDTH + 1];
    char *fileContent = (char *)malloc((WIDTH * MAXROWS) + 1);
    int total_length = 0;

    // Read fileContent while ignoring new lines and spaces
    while (fgets(buffer, sizeof(buffer), file)) {
      for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] != '\n' && buffer[i] != ' ') {
          fileContent[total_length++] = buffer[i];
        }
      }
    }
    fclose(file);

    fileContent[total_length] = '\0';

    bool result = isValidBoardContent(s, fileContent);
    free(fileContent);
    return result;
  }

  // If input is not a file, validate it directly
  return isValidBoardContent(s, str);
}

bool tostring(const state *s, char *str) {
  if (s == NULL || str == NULL) {
    return false;
  }

  int index = 0;

  for (int i = 0; i < s->currentHeight; i++) {
    for (int j = 0; j < WIDTH; j++) {
      str[index++] = s->board[i][j];
    }
  }

  str[index] = '\0';

  // Remove leading rows of empty blocks
  while (strncmp(str, ".....", 5) == 0) {
    memmove(str, str + 5, strlen(str) - 5 + 1);
  }

  return true;
}

bool matches(state *s) {
  bool found_match = false;
  int startRow = (s->currentHeight > 6) ? s->currentHeight - 6 : 0;
  bool to_remove[MAXROWS][WIDTH] = {false};

  // Check horizontal matches
  for (int i = startRow; i < s->currentHeight; i++) {
    for (int j = 0; j < WIDTH - 2; j++) {
      if (s->board[i][j] != '.' && s->board[i][j] == s->board[i][j + 1] &&
          s->board[i][j] == s->board[i][j + 2]) {
        to_remove[i][j] = true;
        to_remove[i][j + 1] = true;
        to_remove[i][j + 2] = true;
        found_match = true;

        int k = j + 3;
        while (k < WIDTH && s->board[i][k] == s->board[i][j]) {
          to_remove[i][k] = true;
          k++;
        }
        j = k - 1;
      }
    }
  }

  // Check vertical matches
  for (int j = 0; j < WIDTH; j++) {
    for (int i = startRow; i < s->currentHeight - 2; i++) {
      if (s->board[i][j] != '.' && s->board[i][j] == s->board[i + 1][j] &&
          s->board[i][j] == s->board[i + 2][j]) {
        to_remove[i][j] = true;
        to_remove[i + 1][j] = true;
        to_remove[i + 2][j] = true;
        found_match = true;
        int k = i + 3;
        while (k < s->currentHeight && s->board[k][j] == s->board[i][j]) {
          to_remove[k][j] = true;
          k++;
        }
        i = k - 1;
      }
    }
  }

  // Remove marked blocks
  for (int i = 0; i < s->currentHeight; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (to_remove[i][j]) {
        s->board[i][j] = '.';
      }
    }
  }

  return found_match;
}

bool dropblocks(state *s) {
  // // Drop blocks down in each column
  for (int j = 0; j < WIDTH; j++) {
    int emptyRow = s->currentHeight - 1;

    for (int i = s->currentHeight - 1; i >= 0; i--) {
      if (s->board[i][j] != '.') {
        if (emptyRow != i) {
          s->board[emptyRow][j] = s->board[i][j];
          s->board[i][j] = '.';
        }
        emptyRow--;
      }
    }
  }
  return true;
}

void test_isValidCharacterString() {
  assert(isValidCharacterString("ABCDE") == true);
  assert(isValidCharacterString("A.B.C") == true);
  assert(isValidCharacterString("AB+CD") == false);
}

void test_isValidBoardContent() {
  state s;
  assert(isValidBoardContent(&s, "ABCDEABCDEABCDEABCDEABCDE") == true);
  assert(isValidBoardContent(&s, "ABCDEABCDEABCDEABCDEABCD") == false);
}

void test_removeTopRowIfEmpty() {
  state s = {.board = {".....", "ABCDE", "FGHIJ"}, .currentHeight = 3};
  removeTopRowIfEmpty(&s);
  assert(strcmp(s.board[0], "ABCDE") == 0);
  assert(strcmp(s.board[1], "FGHIJ") == 0);
}

void test_initialise() {
  state s;
  assert(initialise(&s, "ABCDEABCDEABCDEABCDEABCDE") == true);
  assert(initialise(&s, "ABCDE+ABCDE") == false);
}

void test_tostring() {
  state s = {.board = {"ABCDE", "FGHIJ", "KLMNO"}, .currentHeight = 3};
  char output[MAXROWS * (WIDTH + 1)];
  tostring(&s, output);
  assert(strcmp(output, "ABCDEFGHIJKLMNO") == 0);
}

void test_matches() {
  state s = {.board = {"AAAAB", "CCCDE", "FGGGH"}, .currentHeight = 3};
  assert(matches(&s) == true);
  assert(strcmp(s.board[0], "....B") == 0);
}

void test_dropblocks() {
  state s = {.board = {".....", "A.B.C", "AABBC"}, .currentHeight = 3};
  dropblocks(&s);
  assert(strcmp(s.board[0], ".....") == 0);
  assert(strcmp(s.board[1], "A.B.C") == 0);
  assert(strcmp(s.board[2], "AABBC") == 0);
}

void test(void) {
  test_isValidCharacterString();
  test_isValidBoardContent();
  test_removeTopRowIfEmpty();
  test_initialise();
  test_tostring();
  test_matches();
  test_dropblocks();
}
