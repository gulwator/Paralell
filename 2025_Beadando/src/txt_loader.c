#include <stdio.h>
#include <stdlib.h>


char* load_text_file(char *filename, long *txt_length ){
    printf("elertem a file beolvasot \n");
    printf("%s \n",filename);
    
    FILE* file = fopen(filename,"r");

    fseek(file, 0, SEEK_END);
    *txt_length = ftell(file);
    // printf("a fajl hossza: %ld\n",txt_length);
    rewind(file);

    char *text_in_txt= (char *)malloc(txt_length+1);
    size_t bytesRead = fread(text_in_txt, 1, txt_length, file);
    text_in_txt[bytesRead] = '\0';
    fclose(file);
    // printf("a szoveg: \n%s \n", text_in_txt);
    return text_in_txt;
}