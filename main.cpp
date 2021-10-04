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
  NO_ERROR,
  OPEN_ERROR,
  READ_ERROR
};


int BeginStrCmp(const fstring* s1, const fstring* s2)
{
  assert(s1);
  assert(s2);

  char* str1 = s1->content;
  char* str2 = s2->content;
  int res;
  do
  {
    str1 += strspn(str1, PASS_SYMBOLS);
    str2 += strspn(str2, PASS_SYMBOLS);

    if (*str1 == '\n')
    {
      res = 0;
      break;
    }
    else if (*str1 == *str2)
    {
      ++str1, ++str2;
    }

  } while(*str1 == *str2);
  
  res = (unsigned char)*str1 - (unsigned char)*str2;
  return res;
}


int EndStrCmp(const fstring* s1, const fstring* s2)
{
  assert(s1);
  assert(s2);

  char* str1 = s1->content + s1->size - 1;
  char* str2 = s2->content + s2->size - 1;
  int res;
  do
  {
    while (strspn(str1, PASS_SYMBOLS) || strspn(str2, PASS_SYMBOLS))
    {
      if (strspn(str1, PASS_SYMBOLS))
      {
        --str1;
      }
      if (strspn(str2, PASS_SYMBOLS))
      {
        --str2;
      }
    }

    if (*str1 == '\n')
    {
      res = 0;
      break;
    }
    else if (*str1 == *str2)
    {
      --str1, --str2;
    }

  } while (*str1 == *str2);
  
  res = (unsigned char)*str1 - (unsigned char)*str2;
  return res;
}


void StrSort(fstring* text, size_t str_num, int (*StrCmp)(const fstring*, const fstring*))
{
  assert(text);
  assert(str_num > 0);

  fstring str_buff;

  for(size_t i = 0; i < str_num - 1; i++)
  {
    for(size_t j = 0; j < str_num - i - 1; j++)
    {
      if(StrCmp(text + j, text + j + 1) > 0)
      {
        str_buff = text[j];
        text[j] = text[j + 1];
        text[j + 1] = str_buff;
      } 
    }
  }
}


int FileOutput(const char* name, fstring* text, size_t str_num)
{
  assert(name);
  assert(text);
  assert(str_num > 0);
  
  FILE* fout;
  if ((fout = fopen(name, "a")) == NULL)
  {
    return OPEN_ERROR;
  }
  
  for (size_t i = 0; i < str_num; i++) 
  {
    fprintf(fout, "%s\n", (text + i)->content);
  }
  fprintf(fout, "\n\n");
  fclose(fout);
  return NO_ERROR;
}


int FileInput(const char* name, char** buffer, size_t* char_num)
{
  assert(name);
  assert(buffer);

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


void StrPrintf(const fstring* text, const size_t str_num, const size_t char_num)
{
  assert(text);
  assert(str_num > 0);

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
  assert(buffer);
  assert(str_num);

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

void BackToOrigin(fstring* text, size_t str_num)
{
  assert(text);

  fstring str_buff;

  for(size_t i = 0; i < str_num - 1; i++)
  {
    for(size_t j = 0; j < str_num - i - 1; j++)
    {
      if(((text + j)->content - (text + j + 1)->content) > 0)
      {
        str_buff = text[j];
        text[j] = text[j + 1];
        text[j + 1] = str_buff;
      } 
    }
  }
}


int main() 
{
  char* buffer;

  size_t char_num;

  FileInput("poem.txt", &buffer, &char_num);

  size_t str_num;

  fstring* text = BufferHandle(buffer, &str_num);


 
  StrSort(text, str_num, BeginStrCmp);

  FileOutput("output.txt", text, str_num);

  StrPrintf(text, str_num, char_num);

  
  StrSort(text, str_num, EndStrCmp);

  FileOutput("output.txt", text, str_num);

  StrPrintf(text, str_num, char_num);


  BackToOrigin(text, str_num);

  FileOutput("output.txt", text, str_num);

  StrPrintf(text, str_num, char_num);

  

  free(text);
  
  free(buffer);

  return 0;
}
