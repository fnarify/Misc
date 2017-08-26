#define CDIA        1    // Commonwealth direct investment account.
//#define CMC_CLASSIC 1    // CMC classic trader.
//#define CMC_ACTIVE  1    // CMC active trader.
//#define CMC_PREM    1    // CMC premium trader.
#define PROFIT      0.05 // Percentage profit on initial cost.

#ifdef CDIA
    #define LIMITS 3
#elif (defined CMC_CLASSIC) || (defined CMC_ACTIVE) || (defined CMC_PREM)
    #define LIMITS 1
#else
    #define LIMITS 1
#endif

struct Share {
    unsigned limits[LIMITS]; // Price barriers on fees.
    double ulimit[LIMITS];   // Fee under limit, corresponding to the value in limit[] at that position.
    double olimit[LIMITS];   // Fee above limit.
};

#ifdef CDIA
    const struct Share share = {.limits = {1000, 10000, 25000}, .ulimit = {10.00, 19.95, 29.95}, .olimit = {19.95, 29.95, 0.0012}};
#elif defined CMC_CLASSIC
    const struct Share share = {.limits = {11000}, .ulimit = {11.00}, .olimit = {0.001}};
#elif defined CMC_ACTIVE
    const struct Share share = {.limits = {12375}, .ulimit = {9.90}, .olimit = {0.0008}};
#elif defined CMC_PREM
    const struct Share share = {.limits = {13200}, .ulimit = {9.90}, .olimit = {0.00075}};
#else
    const struct Share share = {.limits = {9999.99}, .ulimit = {29.95}, .olimit = {0.0031}};
#endif
