#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>

#define PASS_SYMBOLS "!?.,;:-_\"'«»() "
/*
struct fstring 
{
  char * content;
  size_t size;
};
*/

enum ErrorCodes
{
  NO_ERROR = 0,
  OPEN_ERROR,
  READ_ERROR,
};
/*
int strcmp(const char* s1, const char* s2)
{
  // assert(s1 != NULL);
  // assert(s2 != NULL);
  
  while (*s1 && (*s1 == *s2))
  {
    s1++, s2++;
  }
    
  return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}*/

int StrCmp(const char* s1, const char* s2)
{
  do
  {
    s1 += strspn(s1, PASS_SYMBOLS);
    s2 += strspn(s2, PASS_SYMBOLS);

    if (*s1 == '\n')
      return 0;

    if (*s1 == *s2)
    {
      ++s1;
      ++s2;
    }

  } while(*s1 == *s2);

  return (unsigned char)*s1 - (unsigned char)*s2;
}

int main() 
{
  
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
    
    if (str_cnt >= n_size) 
    {
      n_size += 10;
      array = (char**)realloc(array, n_size * sizeof(char*));
    }    
  }
  
  for (size_t i = 0; i < str_cnt; i++)
  {
    printf("%s\n", array[i]);
  }
  printf("------------------%lu------------------\n", sz);



  char * str_buff;

  for(size_t i = 0; i < str_cnt - 1; i++)
  {
    for(size_t j = 0; j < str_cnt - i - 1; j++)
    {
      if(StrCmp(array[j], array[j + 1]) > 0)
      {
        str_buff = array[j];
        array[j] = array[j + 1];
        array[j + 1] = str_buff;
      } 
    }
  }

  FILE* fout = fopen("output.txt", "w");

    for (size_t i = 0; i < str_cnt; i++) {
        fprintf(fout, "%s\n", array[i]);
    }
    // fprintf(fout, "\n\n");


  for (size_t i = 0; i < str_cnt; i++)
  {
    printf("%s\n", array[i]);
  }
  printf("------------------%lu------------------\n", sz);



  fclose(fin);
  fclose(fout);
  free(array);
  free(buff);

  return 0;
}

