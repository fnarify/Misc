Slow version of searching for subwords in a dictionary. The dictionary is quite large at approximately 350,000 words. The reason for the slowness comes down to the shear amount of subwords found due to 1 and 2-letter words being counted.

It uses hashmap indexing for lookup, with two hashes to reduce the number of collisions to approximately 0.5%, or ~1700 words.
There is a commented out portion for addpair which is cleaner than the uncommented version, but runs much slower due to some overhead.
