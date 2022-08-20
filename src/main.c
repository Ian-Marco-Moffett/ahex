#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define DUMP_WIDTH 15


void print_asciibuf(char* asciibuf) {
  printf("  ");
  for (int i = 0; i < DUMP_WIDTH; ++i) {
    if (asciibuf[i] < 32 || asciibuf[i] > 122) {
      printf(".");
    } else {
      printf("%c", asciibuf[i]);
    }
  }

  printf("\n");
}


void hexdump(const char* filename) {
  FILE* fp = fopen(filename, "r");

  if (fp == NULL) {
    perror(filename);
    exit(1);
  }

  struct stat st;
  stat(filename, &st);

  char* buf = calloc(st.st_size + 1, sizeof(char));
  char* ascii_buf = calloc(DUMP_WIDTH + 1, sizeof(char));
  fread(buf, sizeof(char), st.st_size, fp);
  
  int i;
  int j = 0;

  for (i = 0; i < st.st_size; ++i) { 
    if (i % DUMP_WIDTH == 0 && i != 0) {
      j = 0;
      print_asciibuf(ascii_buf);
      memset(ascii_buf, 0, DUMP_WIDTH);
    }

    ascii_buf[j++] = buf[i];
    printf("%02hhX ", buf[i]);
  }

  // We are done, put some spaces to align the last ascii dump.
  for (int i = j; i < DUMP_WIDTH; ++i) {
    printf("   ");
  }

  // Dump the last ascii characters in the ascii buffer.
  print_asciibuf(ascii_buf);

  // Newline to flush stdout.
  printf("\n");

  free(buf);
  free(ascii_buf);
  fclose(fp);
}


int main(int argc, char** argv) {
  // To few arguments, give usage and exit.
  if (argc < 2) {
    printf("Usage: %s <filename(s)>\n", argv[0]);
    return 1;
  }

  // Loop through all files to dump and dump them.
  for (int i = 1; i < argc; ++i) {
    if (access(argv[i], F_OK) != 0) {
      printf("File \"%s\" not found!\n", argv[i]);
      return 1;
    }

    hexdump(argv[i]);
  }

  return 0;
}
