#include "crypto.h"

/* VARIAVEIS GLOBAIS */
char F_NAME[64];
short opt = 1;

struct CHARS{
    const char chr;
    const char newChr;
};

struct CHARS chrTable[] = {
    {'a', 'z'}
    ,{'b', 'q'}
    ,{'c', 'u'}
    ,{'d', 'f'}
    ,{'e', 'd'}
    ,{'f', 'g'}
    ,{'g', 'y'}
    ,{'h', 'a'}
    ,{'i', 'k'}
    ,{'j', 'i'}
    ,{'k', 'x'}
    ,{'l', 'm'}
    ,{'m', 'r'}
    ,{'n', 'h'}
    ,{'o', 'p'}
    ,{'p', 'c'}
    ,{'q', 't'}
    ,{'r', 'o'}
    ,{'s', 'w'}
    ,{'t', 'b'}
    ,{'u', 'v'}
    ,{'v', 's'}
    ,{'w', 'j'}
    ,{'x', 'e'}
    ,{'y', 'n'}
    ,{'z', 'l'}
};

NEWARGLIST shellArg[] = {
    {"-fname", "nome do arquivo para ser encriptado.", ID_FNAME}
   ,{"-opt", "opção de uso: 1(encrypt) e 0(decrypt).", ID_OPT}
};

int listSize = sizeof(shellArg) / 24;

/* FUNÇÕES DO PROGRAMA */
double fsize(void){
    FILE *f = fopen(F_NAME, "rb");
    double res = 0;
    fseek(f, 0L, SEEK_END);
    res = ftell(f);
    fclose(f);
    return res;
}

char *readf(void){
    char *content = (char*)malloc(MAX_BUFFER * sizeof(char));
    FILE *f = fopen(F_NAME, "rb");
    fread(content, 1, MAX_BUFFER, f);
    fclose(f);
    return content;
}

void encrypt(char buffer[], double size, int v1, int v2){
    int ascii = 0;
    int r = 0;
    FILE *f = fopen(F_NAME, "wb");
    for(int i = 0; i < size; i++){
        for(int j = 0; j < 26; j++){
            if(buffer[i] == chrTable[j].chr){
                buffer[i] = chrTable[j].newChr;
                break;
            } else if(buffer[i] == (chrTable[j].chr)-32){
                buffer[i] = (chrTable[j].newChr)-32;
                break;
            }
        }
        ascii = (int)buffer[i];
//        if(ascii == 0x0) break;
//        else if(ascii == 0xa) break;
        r = ~ascii;
        r+=(v1 | v2);
        buffer[i] = (char)r;
    }
    fwrite(buffer, size, 1, f);
    printf("[*] arquivo criptografado.\n");
    fclose(f);
}

void decrypt(char buffer[], double size, int v1, int v2){
    int ascii = 0;
    int r = 0;
    FILE *f = fopen(F_NAME, "wb");
    for(int i = 0; i < size; i++){
        ascii = (int)buffer[i];
//        if(ascii == 0x0) break;
//        else if(ascii == 0xa) break;
        r = ~ascii;
        r+=(v1 | v2);
        buffer[i] = (char)r;
        for(int j = 0; j < 26; j++){
            if(buffer[i] == chrTable[j].newChr){
                buffer[i] = chrTable[j].chr;
                break;
            } else if(buffer[i] == (chrTable[j].newChr)-32){
                buffer[i] = (chrTable[j].chr)-32;
                break;
            }
        }
    }
    fwrite(buffer, size, 1, f);
    printf("[*] arquivo descriptografado.\n");
    fclose(f);
}

int parseArgs(int argc, char **argv){
    memset(F_NAME, 0x0, sizeof(F_NAME));
    for(int i = 1; i < argc; i++){
        for(int j = 0; j < listSize; j++){
            if((strcmp(argv[i], shellArg[j].name) == 0)
            && (i < argc-1)){
                switch(shellArg[j].argId){
                    case ID_FNAME:
                        i++;
                        strcpy(F_NAME, argv[i]);
                        break;
                    case ID_OPT:
                        i++;
                        opt = atoi(argv[i]);
                        break;
                    default:
                        break;
                }/* FIM DO SWITCH */
            }/* FIM DO IF */
        }/* FIM DO 2nd FOR */
    }/* FIM DO 1st FOR */
    if(F_NAME[0] == '\0'){
        printf("[!] nenhum arquivo para efetuar ação.\n");
        return ERR;
    }
    return SUCCESS;
}

void shelp(void){
    for(int i = 0; i < listSize; i++){
        printf("\"%s\" -> %s\n", shellArg[i].name, shellArg[i].help);
    }
}

int main(int argc, char **argv){
    if(parseArgs(argc, argv) != SUCCESS){
        printf("[!] args não foram identificados.\n");
        shelp();
        return ERR;
    } else printf("[*] argumentos reconhecidos.\n");

    double len = fsize();
    printf(" => nome do arquivo: %s\n"
           " => tamanho do arquivo: %0.2f kb\n"
           " => opção: %hi\n", F_NAME, len/1024, opt);
    char *content = readf();
    if(opt == 1){
        encrypt(content, len, 130, 50);
    } else decrypt(content, len, 130, 50);
    return SUCCESS;
}
