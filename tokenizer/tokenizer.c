/*
 * Michael Newman && Mark Conley
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
typedef enum {false, true} Boolean;
 
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
 
struct TokenizerT_ {
    char *reducedTokens;  
    char *reducedDelimiters;
    int tokenIndex;   // current index in the reduced tokens array (used when getting single tokens)
    int length;      // length of the reducedTokens char array
};
 
typedef struct TokenizerT_ TokenizerT;
 
int isSpecialCharacter(char c){
  switch(c){
    case '\n': return 'c'; break;
    case '\t': return 'c'; break;
    case '\v': return 'c'; break;
    case '\b': return 'c'; break;
    case '\r': return 'c'; break;
    case '\f': return 'c'; break;
    case '\a': return 'c'; break;
    case '\\': return 'c'; break;
    case '\"': return 'c'; break;
  }
  return 0;
}
 
/* prints string with special characters in hex notation */
void printString(char *str){
  if(str == NULL || str[0] == '\0'){    // dont print new line if given null pointer or empty string
    return;
  }
  int i;
  for (i = 0; i < strlen(str); i++){
    char c = str[i];
    if (isSpecialCharacter(c)){
      printf("[0x%.2x]", c);
    } else {
      printf("%c", c);
    }
  }
  printf("\n");
  return;
}
 
/* 
 * reduce_string takes in a char array pointer to the string to be reduced, a
 * pointer to the character array to store the result in, and the length of the result array.
 * Note that we reduce and copy the string in the same step for added efficiency
 */
 
char* reduce_string(char *inputString){
    int i, index = 0;
    int len = strlen(inputString);
    char *reducedTmp = (char*) malloc((len)*sizeof(char));  // temp string to store reduced
 
    /* 
     * Loop through input string until '\0' null terminator or one less than
     * the length of the result. We do one less so we can still put '\0'
     * at the end of the string.
     */
    for(i = 0; inputString[i] != '\0' && i < len; i++){  
        // If we find '\' check next to see if special character
        if(inputString[i] == '\\'){
            switch(inputString[i+1]){
                  case 'n':
                      reducedTmp[index] = 0x0a;
                      index++;
                      i++;
                      break;
                  case 't':
                      reducedTmp[index] = 0x09;
                      index++;
                      i++;
                      break;
                  case 'v':
                      reducedTmp[index] = 0x0b;
                      index++;
                      i++;
                      break;
                  case 'b':
                      reducedTmp[index] = 0x08;
                      index++;
                      i++;
                      break;
                  case 'r':
                      reducedTmp[index] = 0x0d;
                      index++;
                      i++;
                      break;
                  case 'f':
                      reducedTmp[index] = 0x0c;
                      index++;
                      i++;
                      break;
                  case 'a':
                      reducedTmp[index] = 0x07;
                      index++;
                      i++;
                      break;
                  case '\\':
                      reducedTmp[index] = 0x5c;
                      index++;
                      i++;
                      break;
                  case '"':
                      reducedTmp[index] = 0x22;
                      index++;
                      i++;
                      break;
                  case '\0':
                  default:
                      break;
            }
        } else {   // Not an escape character, copy to new array
            reducedTmp[index] = inputString[i];
            index++;
        }    
    }
 
    int reducedSize = index+2;   // +2 because we add one to index for size, then one more for '\0'
    char *reduced = (char *) malloc((reducedSize)*sizeof(char));
    snprintf(reduced, reducedSize, "%s", reducedTmp);
    free(reducedTmp);   // free the space we allocated for this as its not used outside this function
 
    return reduced;
}
 
/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
 
TokenizerT *TKCreate(char *separators, char *ts){
    if(ts == NULL || strlen(ts) <= 0) return NULL;
 
    // Allocate memory for our tokenizer;
    TokenizerT * tk = (TokenizerT*)malloc(sizeof(TokenizerT));
     
    // send in strings, get back pointers for the vars in tk (reduce_string does the malloc)
    tk->reducedTokens     = reduce_string(ts);
    tk->reducedDelimiters = reduce_string(separators);
 
    tk->length = strlen(tk->reducedTokens);  // get len of reduced token string
    tk->tokenIndex = 0;
 
    return tk;
}
 
/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
 
void TKDestroy(TokenizerT *tk){
    if(tk == NULL)  return;
    free(tk->reducedTokens);
    free(tk->reducedDelimiters);
    free(tk);
}
 
/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
 
char *TKGetNextToken(TokenizerT *tk){
    int startIndex = tk->tokenIndex;
    if(tk == NULL || startIndex >= (tk->length))  return NULL;  // Current index is at the end of the character array return
 
    char *tokenString = tk->reducedTokens;
    char *delimString = tk->reducedDelimiters;
     
    int i;
    Boolean foundDelim = false;
    Boolean foundEnd = false;
 
    /* Loop through the token string till we find a delimiter or the end of the string */
    for(i = startIndex; !foundDelim && !foundEnd; i++){
        if(tokenString[i] == '\0'){  // end of string
           foundEnd = true;
        } else {
          char currentChar = tokenString[i];
          int j;
 
          // loop through the delimiters to check if current character is delimiter
          for(j = 0; delimString[j] != '\0'; j++){
                if(delimString[j] == currentChar){
                    foundDelim = true;
                    break;
                }
          }
        }
    }
 
    int tokenLength = (i - startIndex);  // get the length from the start index and current index
    char *token = (char*) malloc( sizeof(char) * (tokenLength) );
    snprintf(token, tokenLength, "%s", &tokenString[startIndex]);
 
    tk->tokenIndex = i;
 
    return token;
}
 
/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
 
int main(int argc, char **argv){
    if(argc != 3){
        printf("   ERROR:     Incorrect number of arguments \n\n");
        printf("   Usage:     Takes in two string arguments \n");
        printf("                    1) Sperator characters \n");
        printf("                    2) String of tokens seperated by seperator characters \n");
        printf("              Returns each token printed on seperate lines \n");
        exit(-1);
    }
 
    /* empty string */
    if(strlen(argv[2]) == 0){
      exit(8);
    }

    TokenizerT *tk;
    tk = TKCreate(argv[1], argv[2]);
     
    char *token;
    while( (token = TKGetNextToken(tk)) != NULL ){
      printString(token);   // Print token with bracketed hex
      free(token);         // TKGetNextToken() mallocs so we must free here
    }
 
    TKDestroy(tk);
    return 0;
}