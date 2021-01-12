#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define maxTailleClef 1000
#define maxTailleFile 1000
#define KEY_SIZE 4


//Fonction de lecture du fichier contenant la clef de chiffrement
unsigned int** ExtractKey(char keyFileName[]){

    FILE* clef = NULL;
    char chaineClef[maxTailleClef] = "";
    unsigned int **key = malloc(sizeof(int *) * KEY_SIZE);
    int count=0;
    char temp[100];
    int cursor = 5;
    int keySize = 0;

    clef = fopen(keyFileName, "r");

    if (clef != NULL){

        printf("\n \nLe fichier contenant la clef a ete ouvert et lu avec succes \n");

        while (fgets(chaineClef, maxTailleClef, clef) != NULL) {

            for(int i=0;i < 4; i++){

                key[i] = malloc(sizeof(int) * 8);

                for(int j = 0;j < 8; j++){

                    strncpy(temp, chaineClef + cursor++, 1);
                    temp[8] = '\0';

                    sscanf(temp, "%d", key[i] + j);
                }

                cursor++;

            }

            break;
        }

        // Toujours fermer un fichier
        fclose(clef);

        return key;
    }else{
        // Message erreur si non succes
        printf("Impossible d'ouvrir le fichier G4C.txt");

        return NULL;
    }
}
//---------------------------------------------------------------------------------------------------------------//

//conversion d�cimal/binaire

int * convert(char stockChar){

    int *array = malloc(sizeof(int) * 8);

    memset(array, 0, 8);

    int i;

    for(i = 7; stockChar > 0; i--){
        array[i] = stockChar % 2;
        stockChar = stockChar / 2;
    }

    return array;

}

//---------------------------------------------------------------------------------------------------------------//

//Extraction du fichier chiffr�
void Encrypt(unsigned int **key){

    FILE* file = NULL;
    FILE* toWrite = NULL;
    char stockChar;
    int count = 0;
    int *tempConvert;
    unsigned int result = 0;
    unsigned int topResult = 0;
    int test = 0;

    file = fopen("./test.img", "r");
    toWrite = fopen("./test.imgc", "w");
    

    if(file != NULL){

        while(!feof(file)){
            stockChar = fgetc(file);
            tempConvert = convert(stockChar);

            for(int l =0;l<2;l++){

                topResult = 0;

                for(int k = 0;k < 8; k++){

                    result = 0;

                    for(int j = 0; j < 4; j++){

                        result ^= tempConvert[j + (l * 4)] * key[j][k];
                    }

                    if (result == 1)
                        topResult += pow(2, 7 - k);

                    // printf("%d", result);
                }

                fputc(topResult, toWrite);
                // printf("\nend line -> count %d -> result: %d\n", count++, topResult);
            }

            free(tempConvert);
        }

        // Toujours fermer un fichier
        fclose(file);
        fclose(toWrite);
    }else{
        // Message erreur si non succes
        printf("Impossible d'ouvrir le fichier");
    }
} 

void Decrypt(unsigned int **key){

    FILE* file = NULL;

    file = fopen("./EPREUVE 1.txtc", "r");

    printf("Decrypt\n");

    if(file != NULL){

        while(!feof(file)){
            printf("%d - ", fgetc(file));
        }
    }

    fclose(file);
}


//Corps principal
int main(int argc, char **argv){

    unsigned int test = 0b1111;
    unsigned int **key;
    char keyFileName[1000];
    //On ouvre et on lit la clef de chiffrement

    printf("Entrez chemin clé\n");
    fgets(keyFileName, 1000, stdin);
    keyFileName[strlen(keyFileName) - 1] = '\0';

    key = ExtractKey(keyFileName);

    // for(int i=0; i < KEY_SIZE;i++){
    //     for(int j=0;j < 8; j++){
    //         printf("%d", key[i][j]);
    //     }
    //     printf("\n");
    // }

    time_t begin = time( NULL );
    
    Encrypt(key);

    time_t end = time( NULL);
    unsigned long secondes = (unsigned long) difftime( end, begin );
    printf( "Finished in %ld sec\n", secondes );
    // Decrypt(key);

    if(key != NULL){
        for(int i=0; i < KEY_SIZE;i++){
            free(key[i]);
        }   
        free(key);
    }

    

    return 0;
}


