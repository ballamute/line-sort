#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>

#define PASS_SYMBOLS "!?.,;:-_\"'«»() "

struct fstring 
{
  char * content;
  size_t size;
};

enum ErrorCodes
{
  NO_ERROR = 0,
  OPEN_ERROR,
  READ_ERROR,
};


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


void BeginSort(fstring* text, size_t str_num)
{
  char * str_buff;

  for(size_t i = 0; i < str_num - 1; i++)
  {
    for(size_t j = 0; j < str_num - i - 1; j++)
    {
      if(StrCmp((text + j)->content, (text + j + 1)->content) > 0)
      {
        str_buff = (text + j)->content;
        (text + j)->content = (text + j + 1)->content;
        (text + j + 1)->content = str_buff;
      } 
    }
  }
}


int FileOutput(const char* name, fstring* text, size_t str_num)
{
  assert(name != NULL);
  assert(text != NULL);
  assert(str_num > 0);
  
  FILE* fout;
  if ((fout = fopen(name, "w")) == NULL)
  {
    return OPEN_ERROR;
  }
  
  for (size_t i = 0; i < str_num; i++) 
  {
    fprintf(fout, "%s\n", (text + i)->content);
  }
  // fprintf(fout, "\n\n");
  fclose(fout);
  return NO_ERROR;
}


int FileInput(const char* name, char** buffer, size_t* char_num)
{
  assert(name != NULL);
  assert(buffer != NULL);

  *char_num = 0;

  FILE* fin; 
  if ((fin = fopen(name, "rb+")) == NULL)
  {
    return OPEN_ERROR;
  }

  fseek(fin, 0L, SEEK_END);
  *char_num = ftell(fin);
  fseek(fin, 0L, SEEK_SET);

  *buffer = (char*)calloc(*char_num + 1, sizeof(char));
  *(*buffer + *char_num) = '\0';
  fread(*buffer, sizeof(char), *char_num, fin);

  fclose(fin);
  return NO_ERROR;
}


void StrPrintf(fstring* text, size_t str_num, size_t char_num)
{
  for (size_t i = 0; i < str_num; i++)
  {
    printf("%s\n", (text + i)->content);
    printf("%ld\n", (text + i)->size);
  }
  printf("---------------------%05lu---------------------\n", char_num);
  printf("---------------------%05lu---------------------\n", str_num);
}


fstring* BufferHandle(char* buffer, size_t* str_num, const char* sym = "\n")
{
  *str_num = 0;
  size_t n_size = 10;
  
  char * str = strtok(buffer, sym);

  fstring* text = (fstring*)calloc(n_size, sizeof(fstring));
 
  while (str != NULL)
  {
    (text + *str_num)->content = str;
    (text + *str_num)->size = strlen(str);
     
    str = strtok (NULL, sym);
    (*str_num)++; 
    
    if (*str_num >= n_size) 
    {
      n_size += 10;
      text = (fstring*)realloc(text, n_size * sizeof(fstring));
    }    
  }
  return text;

}


int main() 
{
  char* buffer;

  size_t char_num;

  FileInput("poem.txt", &buffer, &char_num);

  size_t str_num;

  fstring* text = BufferHandle(buffer, &str_num);

  StrPrintf(text, str_num, char_num);

  BeginSort(text, str_num);

  FileOutput("output.txt", text, str_num);

  StrPrintf(text, str_num, char_num);

  free(text);
  free(buffer);

  return 0;
}
