#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

#include "bloom.c"

int main()
{
    struct bloom_filter bf;
    bf_init(&bf, 64 * 8, 8); // m in bit, 512 bits in all

    char i;
    for (i = 'a'; i < 'z'; i++)
    {
        bf_insert(&bf, &i, sizeof(char));
    }
    printf("insert done\n");

    for (i = 'a'; i < 'z'; i++)
    {
        assert(bf_may_contain(&bf, &i, sizeof(char)));
    }
    printf("lookup done\n");

    bf_destroy(&bf);
    exit(0);
}