#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 32 * 1024 //32KB

#define ID_FNAME 0
#define ID_OPT 1

#define SUCCESS 0
#define ERR -1

typedef struct{
    const char *name;
    const char *help;
    short argId;
} NEWARGLIST ;

/* FUNÇÕES DECLARADAS AQUI */
double fsize(void);
int parseArgs(int argc, char **argv);
void shelp(void);
