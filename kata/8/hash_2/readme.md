This is much faster primarily on the basis of when encountering a collision during a find operation it ignores searching for it. Otherwise it's still tremendously slow. There is also a MIN_WORD_SIZE macro that provides a means of ignoring subwords all below that many characters, it doesn't even add them to the hashmap too.

You can make the program run at a reasonable speed by using any of the gcc optimisation flags, ergo -O2 or -O3.
