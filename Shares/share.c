#include <stdio.h>
#include <stdlib.h>
#include "share.h"

int main(int argc, char *argv[])
{
    char *prog = argv[0];
    int i;
    unsigned long amount;           // Amount of shares bought.
    double buyprice, sellprice;     // Price per share.
    double initcost, finalcost;     // Initial cost (not including fee.
    double buyfee, sellfee, totfee; // Trading fees.

    if (argc == 3)
    {
        amount = strtoul(argv[1], NULL, 10);
        buyprice = strtod(argv[2], NULL);

        if (!amount)
        {
            printf("amount of shares needs to be non-zero positive integer.\n");
            return 1;
        }
        else if (buyprice <= 0) 
        {
            printf("price per share needs to be a positive (>0) value.\n");
            return -1;
        }

        initcost = amount * buyprice;
        for (i = 0; i < LIMITS; i++)
        {
            if (initcost <= share.limits[i])
            {
                // Assumption that no straight fee is less than a dollar.
                // So if it is, then it's a percentage fee.
                buyfee = (share.ulimit[i] < 1.0 ? initcost * share.ulimit[i] : share.ulimit[i]);
                break;
            }
            else if (i + 1 == LIMITS) // Already checked last limit, thus it can only be over.
            {
                buyfee = (share.olimit[i] < 1.0 ? initcost * share.olimit[i] : share.olimit[i]);
            }
        }
        
        // PROFIT on initial cost.
        finalcost = (1 + PROFIT) * (initcost + buyfee);

        for (i = 0; i < LIMITS; i++)
        {
            if (finalcost <= share.limits[i])
            {
                sellfee = (share.ulimit[i] < 1.0 ? finalcost * share.ulimit[i] : share.ulimit[i]);
                break;
            }
            else if (i + 1 == LIMITS)
            {
                sellfee = (share.olimit[i] < 1.0 ? finalcost * share.olimit[i] : share.olimit[i]);
            }
        }

        totfee = buyfee + sellfee;
        finalcost += sellfee;

        printf("initial cost: %8s%g\nsell: %16s%.3g ec (%g%% inc)\nprofit (after fees): $%g (%g%%)\nsell price: %10s%g\nsell price (a/fees): $%g\n",
                "$", initcost + buyfee, "$", finalcost / amount, ((finalcost / amount) / buyprice) * 100 - 100, finalcost - initcost - totfee, 
                (finalcost - initcost - totfee) / (initcost + buyfee) * 100, "$", finalcost, finalcost - sellfee);
    }
    else if (argc == 4)
    {
        amount = strtoul(argv[1], NULL, 10);
        buyprice = strtod(argv[2], NULL);
        sellprice = strtod(argv[3], NULL);

        if (!amount)
        {
            printf("amount of shares needs to be non-zero positive integer.\n");
            return 1;
        }
        else if (buyprice <= 0) 
        {
            printf("price per share needs to be a positive (>0) value.\n");
            return -1;
        }

        // Calculate costs.
        initcost = amount * buyprice;
        finalcost = amount * sellprice;

        // Calculate fees.
        buyfee = sellfee = 0;
        for (i = 0; i < LIMITS; i++)
        {
            if (initcost <= share.limits[i] && !buyfee)
            {
                // Assumption that no straight fee is less than a dollar.
                // So if it is, then it's a percentage fee.
                buyfee = (share.ulimit[i] < 1.0 ? initcost * share.ulimit[i] : share.ulimit[i]);
            }
            if (finalcost <= share.limits[i] && !sellfee)
            {
                sellfee = (share.ulimit[i] < 1.0 ? finalcost * share.ulimit[i] : share.ulimit[i]);
            }
            if (i + 1 == LIMITS)
            {
                if (!buyfee) {buyfee = (share.olimit[i] < 1.0 ? initcost * share.olimit[i] : share.olimit[i]);}
                if (!sellfee) {sellfee = (share.olimit[i] < 1.0 ? finalcost * share.olimit[i] : share.olimit[i]);}
            }
        }
        totfee = buyfee + sellfee;

        printf("initial cost: %8s%g\nsell: %16s%.3g ec (%g%% inc)\nprofit (after fees): $%g (%g%%)\nsell price: %10s%g\nsell price (a/fees): $%g\n",
                "$", initcost + buyfee, "$", finalcost / amount, ((finalcost / amount) / buyprice) * 100 - 100, finalcost - initcost - totfee, 
                (finalcost - initcost - totfee) / (initcost + buyfee) * 100, "$", finalcost, finalcost - sellfee);

    }
    else
    {
        printf("%s: takes 2 inputs 'amount (int) buy ec (decimal)' e.g. %s 200 0.10\n", prog, prog); 
        printf("%s: takes 3 inputs 'amount (int), buy ec (decimal), sell ec (decimal)'\n", prog);
        return 2;
    }

    return 0;
}

