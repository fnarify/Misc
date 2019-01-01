#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char test[] = "this is a test";
    int len = sizeof(test) - 1;

    char *tokens = strtok(test, " ");
    int i, j;

    // create new output string
    char *output = malloc(sizeof(output) * sizeof(test));
    if (!output) {return 1;}
    output[sizeof(test) - 1] = '\0';

    while (tokens)
    {
        i = len - strlen(tokens);
        j = 0;

        // prepend delimiter
        if (i) {output[i - 1] = ' ';}

        while (i < len)
        {
            output[i++] = tokens[j++];
        }
        len -= strlen(tokens) + 1;

        // go to next token
        tokens = strtok(NULL, " ");
    }

    printf("string: %s\n", output);
    
    free(output);

    return 0;
}
