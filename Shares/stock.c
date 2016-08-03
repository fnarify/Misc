/**
 * Gets stock prices using the Yahoo API and curl.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSIN 5       // Max stock price input size (inc. null string).
#define MAXCMD 200     // Max amount of chars in command call.
#define EXC    ".AX"   // Default exchange location Australian exchange.
#define TAG    "&f=l1" // Tag for information (currently last trade price).

int main(int argc, char *argv[])
{
    char stock[MAXSIN];
    char *filename = "quotes.csv";
    char command[MAXCMD];
    char c;
    int cnt = 0;

    if (argc == 1)
    {
        printf("%s: takes a stock ticker between 1 and 4 characters\nand retrieves it's current trading price.\n", argv[0]);
        return 1;
    }
    while (--argc)
    {
        strncpy(stock, argv[argc], sizeof(stock));

        // Check that the input isn't too long, and that the input is always uppercase.
        while (stock[cnt] != '\0') {stock[cnt] = toupper(stock[cnt]); cnt++;}

        sprintf(command, "curl -o %s \"download.finance.yahoo.com/d/%s?s=%s%s%s\" | nc download.finance.yahoo.com 80",
                filename, filename, stock, EXC, TAG);

        system(command);

        FILE *file = fopen(filename, "r");

        if (!file)
        {
            fprintf(stderr, "Could not acquire stock value.\n");
            return -1;
        }

        // Print last traded stock price.
        printf("%s: $", stock);
        while (1)
        {
            c = fgetc(file);

            if (feof(file) || c == '\n') {break;}
            
            printf("%c", c);
        }
        printf(" last traded\n");

        fclose(file);
    }

    return 0;
}
