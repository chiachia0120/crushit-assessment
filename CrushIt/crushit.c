#include "crushit.h"

#include "mydefs.h"

bool isValidCharacterString(const char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if ((str[i] < 'A' || str[i] > 'Z') && str[i] != '.') {
      printf("Invalid character '%c' at position %d\n", str[i], i);
      return false;
    }
  }
  return true;
}

bool isValidBoardContent(state* s, const char* str, int height) {
  printf("String from input:\n%s\n", str);

  // 檢查是否超過最大行數
  if (height > MAXROWS) {
    printf("Error: Maximum row limit is %d\n", MAXROWS);
    return false;
  }

  // 檢查字符有效性
  if (!isValidCharacterString(str)) {
    return false;
  }

  // 填充棋盤
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < WIDTH; j++) {
      s->board[i][j] = str[i * WIDTH + j];
    }
    s->board[i][WIDTH] = '\0';  // 確保每行以 null 結尾
  }

  return true;
}

bool initialise(state* s, const char* str) {
  if (!s || !str) return false;

  FILE* f = fopen(str, "r");
  if (f != NULL) {
    printf("Reading from file: %s\n", str);
    char content[MAXROWS * WIDTH + 1] = {0};
    char buffer[WIDTH + 2];  // +2 for newline and null terminator
    int contentIndex = 0;

    while (fgets(buffer, sizeof(buffer), f)) {
      printf("Read line: %s", buffer);
      for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] != ' ' && buffer[i] != '\n') {
          content[contentIndex++] = buffer[i];
        }
      }
    }

    if (contentIndex == 0) {
      printf("Error: No valid content read from the file.\n");
      fclose(f);
      return false;
    }
    content[contentIndex] = '\0';  // 確保字串以 null 結尾
    printf("Content from file:\n%s\n", content);
    fclose(f);

    // 計算高度
    int height = contentIndex / WIDTH;
    if (contentIndex % WIDTH != 0) {
      printf("Error: Length must be a multiple of %d\n", WIDTH);
      return false;
    }

    return isValidBoardContent(s, content, height);
  }

  // 如果不是文件，直接處理字串
  int total_length = strlen(str);
  if (total_length % WIDTH != 0) {
    printf("Error: Length must be a multiple of %d\n", WIDTH);
    return false;
  }
  int height = total_length / WIDTH;

  return isValidBoardContent(s, str, height);
}

bool tostring(const state* s, char* str) {}

bool matches(state* s) {}

bool dropblocks(state* s) {}

void test(void) {}
