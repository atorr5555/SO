#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main (int argc, char *argv[]) {
	if (argv[1] == NULL){
		printf("Se debe indicar la ruta del archivo\n");
		return;
	}
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL){
		printf("Archivo no encontrado \n");
		return;
	}

	char line[64];
  char *data;

	while (fgets(line, sizeof(line), fp) != NULL){
    char *pos;
    if ((pos=strchr(line, '\n')) != NULL)
      *pos = '\0';
    data = strtok(line, " ");
    while (data != NULL){
      printf("%s ", data);
      data = strtok(NULL, " ");
    }
    printf("\n");
    
	}
	
	
	fclose(fp);
}