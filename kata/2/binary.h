// Binary search function headers.
#include <stddef.h>

/**
 * Given a target integer and an array of sorted integers.
 * Returns the position in the array of the target, or -1
 * if not in the array.
 */
int chop(int tar, int size, int *p);

/**
 * Similar to above except also given a start and end position
 * instead of a size.
 * Function recursive method, instead of iterative.
 */
int chopr(int tar, int start, int end, int *p);

/**
 * This method instead is pointer recursive.
 * So it performs the binary search on sections of the initially given
 * pointer, and returns the location (if any) via an accumulator.
 */
void chopp(int tar, int size, int *p, int *acc);

/**
 * This method continually halves the search space, and does not perform the check
 * until there is only one element in p (e.g, size == 1). It then performs pointer
 * arithmetic to calculate the position in the original array, where inip is the original
 * starting position of p.
 */
ptrdiff_t choppa(int tar, int size, int *p, int *inip);
