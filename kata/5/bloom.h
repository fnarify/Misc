#include <inttypes.h>
#include <stdlib.h>

typedef uint32_t elem_f; // Field element.

elem_f *initfilter(size_t fnum);
int savefilter(elem_f *bloom, size_t fnum);
int lookup(unsigned char *w, elem_f *bloom, size_t fnum);
void addword(unsigned char *w, elem_f *bloom, size_t fnum);
void addwords(elem_f *bloom, size_t fnum, size_t n);
