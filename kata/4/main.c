#define _WEATHER

#ifdef _WEATHER
    #include "weather.c"
    void (*data)(FILE *) = &weather;
#elif defined _FOOTBALL
    #include "football.c"
    void (*data)(FILE *) = &football;
#endif

int main()
{
    FILE *fp = fopen(FNAME, "r");
    if (!fp)
    {
        printf("Could not open file: %s\n", FNAME);
        return -1;
    }
    fseek(fp, INITOFST, SEEK_SET);

    data(fp);

    fclose(fp);
    return 0;
}
