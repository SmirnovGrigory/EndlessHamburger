#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strPrint( char* string)
{
    for (int i = 0; i < strlen(string); i++)
        printf("%c", string[i]);
}

void replace(char** hamburger)
{
    int cnt = 0;
    for (int j = 0; j < strlen(*hamburger); j++)
        if ((*hamburger)[j] == '\n' || (*hamburger)[j] == '\\' || (*hamburger)[j] == '\"')
            cnt++;
    char* new_hamb = (char*)malloc(strlen(*hamburger) + cnt+1);

    int i = 0;
    for (int j = 0; j < strlen(*hamburger); j++)
    {
        if ((*hamburger)[j] == '\n')
        {
            new_hamb[i] = '\\';
            new_hamb[i+1] = 'n';
            i += 2;
        }
        else if ((*hamburger)[j] == '\\')
        {
            new_hamb[i] = '\\';
            new_hamb[i + 1] = '\\';
            i += 2;
        }
        else if ((*hamburger)[j] == '\"')
        {
            new_hamb[i] = '\\';
            new_hamb[i + 1] = '\"';
            i += 2;
        }
        else
        {
            new_hamb[i] = (*hamburger)[j];
            i++;
        }

    }
    new_hamb[i] = '\0';
    *hamburger = (char*)realloc(*hamburger, strlen(*hamburger) + cnt+1);
    strcpy(*hamburger, new_hamb);
    free(new_hamb);
}

void createHamb(char* top, char** hamb, char* bot)
{
    char* new_hamb = (char*)malloc (strlen(top) + strlen(*hamb) + strlen(bot) + 1);
    strcpy(new_hamb, top);
    strcat(new_hamb, *hamb);
    strcat(new_hamb, bot);
    *hamb = (char*)realloc(*hamb, strlen(top) + strlen(*hamb) + strlen(bot) + 1);
    strcpy(*hamb, new_hamb);
    free(new_hamb);
}

void replaceDigit(char** str, int digit )
{
    int my_digit;
    if (digit > 9)
        my_digit = digit + 'A';
    else
        my_digit = digit + 48;
    for (int i = 0; i < strlen(*str); i++)
        if ((*str)[i] >= '0' && (*str)[i] <= '9')
            (*str)[i] = my_digit;
}

int main()
{
    char child0[100] = "int main(){ printf(\"Hello, world!\"); }";
    char* hamburger = (char*)malloc(strlen(child0));
    strcpy(hamburger, child0);
    //strPrint(strlen(hamburger),hamburger);
    char* top_sh = (char*)malloc(100*sizeof(char));
    char* bot_sh = (char*)malloc(50 * sizeof(char));
    char* top_c = (char*)malloc(150 * sizeof(char));
    char* bot_c = (char*)malloc(50 * sizeof(char));
    strcpy(top_sh,"touch child1.c; echo -e \"#include <stdlib.h>\n#include <stdio.h>\n\" > child1.c; echo \"");
    strcpy(bot_sh,"\" >> child1.c ; gcc child1.c -o child1; ./child1");
    strcpy(top_c, "int main() { FILE *text = fopen(\"child1.sh\",\"w\"); fprintf(text,\"#!/bin/bash\\n\"); fprintf(text,\"");
    strcpy(bot_c,"\"); fclose(text); system(\"bash child1.sh\"); }");
    int N;
    scanf("%d", &N);

    for (int i = 1; i <= N; i++)
    {
        if (i % 2 == 1)
        {
            replace(&hamburger);
            replaceDigit(&top_sh, i);
            replaceDigit(&bot_sh, i);
            createHamb(top_sh, &hamburger, bot_sh);

            strPrint(hamburger);
            printf("\n\n");
        }
        else
        {
            replace(&hamburger);
            replaceDigit(&top_c, i);
            replaceDigit(&bot_c, i);
            createHamb(top_c, &hamburger, bot_c);

            strPrint(hamburger);
            printf("\n\n");
        }
    }

    if (N % 2 == 1)
    {
        FILE* mother = fopen("mother.sh", "w");
        fprintf(mother, "#!/bin/bash\n");
        fprintf(mother, hamburger);
        fclose(mother);
    }
    else
    {
        FILE* mother = fopen("mother.c", "w");
        fprintf(mother, "#include <stdlib.h>\n#include <stdio.h>\n");
        fprintf(mother, hamburger);
        fclose(mother);
    }

   
    free(hamburger);
    free(top_sh);
    free(bot_sh);
    free(top_c);
    free(bot_c);
    return 0;
}