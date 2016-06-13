#include <stdio.h>
#include "bloom.c"

int main()
{
    unsigned char test = "testwordhere";
    elem_f *filt = initfilter(ELEMS);
    addword("hello", filt, ELEMS);
    
    if (lookup("hello", filt, ELEMS))
        printf("Found word.\n\n");
    else
        printf("Word not found.\n\n");
    
    addwords(filt, ELEMS, MAX_WORDS);

    if (lookup("zygapophyseal", filt, ELEMS))
        printf("\nFound word.\n");
    else
        printf("\nWord not found.\n");

    savefilter(filt, ELEMS);
    return 0;
}
