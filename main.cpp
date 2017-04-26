#include <iostream>
#include "SM3.h"

bool hashEqual(Byte* hash1, Byte* hash2) {
    for (int i = 0; i < 7; ++i) {
        if (hash1[i] != hash2[i]) return false;
    }
    return true;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    /* use Floyd's Cycle Detection Algorithm. */

    /* initialize */
    Byte* input = new Byte[4];
    SM3::word2byte(0x61626300, input);
    Word* hash = SM3::hash(input, 3);
    Byte* hash_bytes = new Byte[32];
    for (int i = 0; i < 8; ++i) {
        SM3::word2byte(hash[i], hash_bytes+4*i);
    }
    for (int i = 0; i < 32; ++i) {
        printf("%hhx ", hash_bytes[i]);
    }
    printf("\n");

    /* cycle detection */
    Byte* t = new Byte[32];
    Byte* h = new Byte[32];
    memcpy(t, hash_bytes, 32*sizeof(Byte));
    memcpy(h, hash_bytes, 32* sizeof(Byte));

    do {
        /* t := t->next */
        Word* h_t = SM3::hash(t, 32);
        for (int i = 0; i < 8; ++i) {
            SM3::word2byte(h_t[i], t+4*i);
        }
        delete h_t;

        /* h:= h->next->next */
        Word* h_h = SM3::hash(h, 32);
        for (int i = 0; i < 8; ++i) {
            SM3::word2byte(h_h[i], h+4*i);
        }
        delete h_h;
        h_h = SM3::hash(h, 32);
        for (int i = 0; i < 8; ++i) {
            SM3::word2byte(h_h[i], h+4*i);
        }
        delete h_h;

    } while (!hashEqual(t, h));

    printf("t:\n");
    for (int i = 0; i < 32; ++i) {
        printf("%hhx ", t[i]);
    }
    printf("\n");

    printf("h:\n");
    for (int i = 0; i < 32; ++i) {
        printf("%hhx ", h[i]);
    }
    printf("\n");
    return 0;
}