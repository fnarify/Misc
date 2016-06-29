#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXCMD 200     // Max size of command line call.
#define MAXIN  4       // Max size of an input currency.
#define MAXCUR 180     // How many currencies worldwide.
#define TAG    "&f=l1" // Flags for what to download.

char *currencies[MAXCUR] = {"AUD", "USD", "CAD", "EUR", "GBP"};

/**
 * Converts from one currency to another.
 * Makes use of curl and the Yahoo finance API.
 */
int main(int argc, char *argv[])
{
    char *from = currencies[0];
    char *to = currencies[1];
    char command[MAXCMD];
    char *filename = "quotes.csv";
    char c;
    int cnt = 0;

    if (argc == 3)
    {
        while (argv[1][cnt++] != '\0') {;}
        if (cnt > MAXIN) {fprintf(stderr, "argument size too large\n"); return cnt;}
        
        cnt = 0;
        while (argv[1][cnt++] != '\0') {;}
        if (cnt > MAXIN) {fprintf(stderr, "argument size too large\n"); return cnt;}

        from = argv[1];
        to = argv[2];
    }
    else
    {
        printf("%s: takes two inputs, converts from first currency to next\ne.g, ", argv[0]);    
    }
       
    printf("from %s to %s\n", from, to);

    // Build command to get currency information.
    sprintf(command, "curl -o %s \"download.finance.yahoo.com/d/%s?e=.csv%s&s=%s%s=X\" | nc download.finance.yahoo.com 80",
            filename, filename, TAG, from, to);

    system(command); 

    // Open exchange rate.
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        fprintf(stderr, "Could not acquire exchange rate.\n");
        return -1;
    }

    printf("rate: 1 %s = ", from);
    while (1)
    {
        c = fgetc(file);
        if (feof(file) || c == '\n') {break;}

        printf("%c", c);
    }
    printf(" %s\n", to);
    fclose(file);

    return 0;
}
