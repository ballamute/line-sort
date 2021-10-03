#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

enum ErrorCodes {
  NO_ERROR = 0,
  OPEN_ERROR,
  READ_ERROR,
};

int main() {
  
  FILE* fin = fopen("poem.txt", "rb+");

  fseek(fin, 0L, SEEK_END);
  size_t sz = ftell(fin);
  fseek(fin, 0L, SEEK_SET);

  char* buff = (char*)calloc(sz + 1, sizeof(char));
  buff[sz] = '\0';
  fread(buff, sizeof(char), sz, fin);

  char * pch = strtok(buff, "\n");
 
  while (pch != NULL)
  {
    printf("%s\n", pch);
    pch = strtok (NULL, "\n");
  }


  // fprintf(stdout, "%s\n", buff);

  printf("%lu\n", sz);

  free(buff);
  fclose(fin);

  return 0;
}

