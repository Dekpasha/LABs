#include <stdio.h>
#include <io.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fread;
    fread = fopen(argv[2], "r");

    if (argc != 3)
        return 1; 
    
    if (!strcmp(argv[1], "-l") || !strcmp(argv[1], "--lines")){
        char line;
        int count = 0;
        while ((line = fgetc(fread)) != EOF){
            while (line != '\n')
                if ((line = fgetc(fread)) == EOF)
                    break;
            count ++;
        }
        printf("%d\n", count);      
    }

    if (!strcmp(argv[1], "-w") || !strcmp(argv[1], "--words")){
        char line;
        int count = 0;
        short is_Word;
        while ((line = fgetc(fread)) != EOF){
            is_Word = 0;
            while (!(line == '\n' || line == ' ' || line == '\t')){
                if ((line = fgetc(fread)) == EOF)
                    break;
                is_Word = 1;
            }
            if (is_Word == 1)
                count++;
        }
        printf("%d\n", count);
    }

    if (!strcmp(argv[1], "-c" ) || !strcmp(argv[1], "--bytes")){
        fseek(fread, 0L, SEEK_END);
        int size = ftell(fread);
        printf("%d", size);
    }

    fclose(fread);
    return 0;
}