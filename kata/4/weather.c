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
 * Given a file weather.dat return the day with the smallest spread 
 * between the maximum and minimum temperature.
 */
int main()
{
    int day = -1, curday = 0, cnt = 0;
    double sprd = MAXSPRD, cursprd = 0.0;
    char c, value[MAXSTR];
    FILE *weather = fopen(FNAME, "r"); int linenu = 1;
    if (!weather)
    {
        printf("Could not open file: %s\n", FNAME);
        return -1;
    }
    // Go to beginning of 3rd line.
    fseek(weather, INITOFST, SEEK_SET); linenu += 2;

    // Parse the result from the columns in weather.dat and compute the result.
    enum columns weather_col = DAY;
    do
    {
        c = fgetc(weather);
        if (feof(weather)) {break;}
        
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
                    while ((c = fgetc(weather)) != '\n')
                    {
                        // If EOF reached print results.
                        // This won't occur during normal runtime though.
                        if (feof(weather)) {goto print;}
                    }
                    linenu++;
                    break;
                default:
                    break;
            }
            cnt = 0;
        }
        else if ((c >= '0' && c <= '9') || c == '.')
        {
            value[cnt++] = c;
        }
    } while (linenu < MAXLINE);

print:
    printf("Day %d has the smallest spread of %g\n", day, sprd);
    fclose(weather);

    return 0;
}
