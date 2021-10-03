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


  size_t str_cnt = 0;
  size_t n_size = 10;
  char** array = (char**)calloc(n_size, sizeof(char*));
  
  char * str = strtok(buff, "\n");
 
  while (str != NULL)
  {
    array[str_cnt] = str;
    str = strtok (NULL, "\n");
    str_cnt++;
    
    if (str_cnt >= n_size) {
      n_size += 10;
      array = (char**)realloc(array, n_size * sizeof(char*));
    }    
  }
  
  for (size_t i = 0; i < str_cnt; i++)
  {
    printf("%s\n", array[i]);
  }

  


  // fprintf(stdout, "%s\n", buff);

  printf("%lu\n", sz);

  free(array);
  free(buff);
  fclose(fin);

  return 0;
}

