#define CDIA   1    // Commonwealth direct investment account.
#define PROFIT 0.05 // Percentage profit on initial cost.

#ifdef CDIA
    #define LIMITS 2
#else
    #define LIMITS 1
#endif

struct Share {
    unsigned limits[LIMITS]; // Price barriers on fees.
    double ulimit[LIMITS];   // Fee under limit, corresponding to the value in limit[] at that position.
    double olimit[LIMITS];   // Fee above limit.
};

#ifdef CDIA
    const struct Share share = {.limits = {10000, 25000}, .ulimit = {19.95, 29.95}, .olimit = {29.95, 0.0012}};
#else
    const struct Share share = {.limits = {10000}, .ulimit = {29.95}, .olimit = {0.0031}};
#endif
