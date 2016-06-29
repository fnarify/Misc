#include <stdio.h>
#include <stdlib.h>

#define PROFIT    0.05    // Percentage profit on initial cost.
#define TRADE_LIM 10000   // Limit that defines fees paid
#define UND_LIM   29.95   // Trading fees for costs below TRADE_LIM.
#define OVR_LIM   0.0031  // Trading fees for costs above.

int main(int argc, char *argv[])
{
    char *prog = argv[0];
    unsigned int amount;            // Amount of shares bought.
    double buyprice, sellprice;     // Price per share.
    double initcost, finalcost;     // Initial cost (not including fee.
    double buyfee, sellfee, totfee; // Trading fees.

    if (argc == 3)
    {
        amount = strtoul(argv[1], NULL, 10);
        buyprice = strtod(argv[2], NULL);

        if (amount == 0)
        {
            printf("amount of shares needs to be non-zero positive integer.\n");
            exit(1);
        }
        else if (buyprice <= 0) 
        {
            printf("price per share needs to be a positive (>0) value.\n");
            exit(-1);
        }

        initcost = amount * buyprice;
        if (initcost <= TRADE_LIM)
        {
            buyfee = UND_LIM;
            finalcost = (1 + PROFIT) * (initcost + buyfee);
            sellfee = (finalcost <= TRADE_LIM ? UND_LIM : finalcost * OVR_LIM);
            finalcost += sellfee;
        }
        else
        {
            buyfee = initcost * OVR_LIM;
            finalcost = (initcost * (1 + OVR_LIM) / (1 - OVR_LIM)) * (1 + PROFIT);
            sellfee = finalcost * OVR_LIM;
        }

        totfee = buyfee + sellfee;

        printf("initial cost: %8s%g\nsell: %16s%.3g ec (%g%% inc)\nprofit (after fees): $%g (%g%%)\nsell price: %10s%g\nsell price (a/fees): $%g\n",
                "$", initcost + buyfee, "$", finalcost / amount, ((finalcost / amount) / buyprice) * 100 - 100, finalcost - initcost - totfee, 
                (finalcost - initcost - totfee) / (initcost + buyfee) * 100, "$", finalcost, finalcost - sellfee);
    }
    else if (argc == 4)
    {
        amount = strtoul(argv[1], NULL, 10);
        buyprice = strtod(argv[2], NULL);
        sellprice = strtod(argv[3], NULL);

        if (amount == 0)
        {
            printf("amount of shares needs to be non-zero positive integer.\n");
            exit(1);
        }
        else if (buyprice <= 0 || sellprice <= 0)
        {
            printf("price per share needs to be a positive (>0) value.\n");
            exit(-1);
        }

        initcost = amount * buyprice;
        buyfee = (initcost <= TRADE_LIM ? UND_LIM : initcost * OVR_LIM);

        finalcost = amount * sellprice;
        sellfee = (finalcost <= TRADE_LIM ? UND_LIM : finalcost * OVR_LIM);

        totfee = buyfee + sellfee;

        printf("initial cost: %8s%g\nsell: %16s%.3g ec (%g%% inc)\nprofit (after fees): $%g (%g%%)\nsell price: %10s%g\nsell price (a/fees): $%g\n",
                "$", initcost + buyfee, "$", finalcost / amount, ((finalcost / amount) / buyprice) * 100 - 100, finalcost - initcost - totfee, 
                (finalcost - initcost - totfee) / (initcost + buyfee) * 100, "$", finalcost, finalcost - sellfee);
    }
    else
    {
        printf("%s: takes 2 inputs 'amount (int) buy ec (decimal)' e.g. %s 200 0.10\n", prog, prog); 
        printf("%s: takes 3 inputs 'amount (int), buy ec (decimal), sell ec (decimal)'\n", prog);
        exit(2);
    }

    exit(0);
}
