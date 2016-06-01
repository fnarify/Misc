#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define FNAME    "weather.dat"
#define MAXSTR   6   // Max size (inc. null-string) of any parsed result.
#define MAXLINE  33  // Max vertical lines in our document.
#define MAXSPRD  101
#define MINSPRD  -1
#define INITOFST 91L // Initial offset of file to beginning of data.

// Order of columns to be inspected in our text file.
enum columns
{
    DAY,
    MAXT,
    MINT
};

/**
 * Returns the smallest temperature spread on one of the days
 * as defined in FNAME.
 */
void weather(FILE *fp)
{
    int day = -1, curday = 0, cnt = 0;
    double sprd = MAXSPRD, cursprd = 0.0;
    char c, value[MAXSTR];

    // Initial linenumber in fp.
    int linenu = 3;

    // Parse the result from the columns in weather.dat and compute the result.
    enum columns weather_col = DAY;
    do
    {
        c = fgetc(fp);
        if (feof(fp)) {break;}
        
        if (isspace(c) && cnt) 
        {
            value[cnt] = '\0';

            // Collected information from this column.
            switch(weather_col)
            {
                case DAY:
                    curday = strtol(value, NULL, 10);
                    weather_col = MAXT;
                    break;
                case MAXT:
                    cursprd = strtod(value, NULL);
                    weather_col = MINT;
                    break;
                case MINT:
                    cursprd -= strtod(value, NULL);
                    // Search for the smallest spread and on which day.
                    if (cursprd < sprd) 
                    {
                        day = curday;
                        sprd = cursprd;
                    }
                    weather_col = DAY;
                    // Increment our cursor to the next line.
                    while ((c = fgetc(fp)) != '\n')
                    {
                        // If EOF reached print results.
                        // This won't occur during normal runtime though.
                        if (feof(fp)) {goto print;}
                    }
                    linenu++;
                    break;
                default:
                    break;
            }
            cnt = 0;
        }
        else if (isdigit(c) || c == '.')
        {
            value[cnt++] = c;
        }
    } while (linenu < MAXLINE);

print:
    printf("Day %d has the smallest spread of %g\n", day, sprd);
}

