#include "sm3.h"
#include <cstring>
#include <cstdio>

bool hashEqual(uint8_t * hash1, uint8_t * hash2) {
    for (int i = 0; i < 7; ++i) {
        if (hash1[i] != hash2[i]) return false;
    }
    return true;
}

int main() {

    constexpr int CHECKPOINT = 10000000;

    /* use Floyd's Cycle Detection Algorithm. */

    SM3_CTX ctx;
    uint8_t out[32] = {0};
    uint8_t msg[64] = {0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64};

    SM3_Init(&ctx);
    SM3_Update(&ctx, msg, 64);
    SM3_Final(out, &ctx);

    for (int i = 0; i < 32; ++i) {
        printf("%hhx ", out[i]);
    }

    uint8_t t[32];
    uint8_t h[32];
    memcpy(t, out, 32*sizeof(uint8_t));
    memcpy(h, out, 32*sizeof(uint8_t));

    int i = 0;
    do {
        /*
         * t = hash(t)
         */
        SM3_Init(&ctx);
        SM3_Update(&ctx, t, 32);
        SM3_Final(t, &ctx);

        /*
         * h = hash(hash(h))
         */
        SM3_Init(&ctx);
        SM3_Update(&ctx, h, 32);
        SM3_Final(h, &ctx);

        SM3_Init(&ctx);
        SM3_Update(&ctx, h, 32);
        SM3_Final(h, &ctx);

        if (i == CHECKPOINT) {
            printf("t: \n");
            for (int j = 0; j < 32; ++j) {
                printf("%hhx ", t[j]);
            }
            printf("\n");
            printf("h: \n");
            for (int j = 0; j < 32; ++j) {
                printf("%hhx ", h[j]);
            }
            printf("\n");
            i = 0;
        }
        i++;
    } while (!hashEqual(t, h));

    printf("collide!!\n");
    printf("t: \n");
    for (int j = 0; j < 32; ++j) {
        printf("%hhx ", t[j]);
    }
    printf("\n");
    printf("h: \n");
    for (int j = 0; j < 32; ++j) {
        printf("%hhx ", h[j]);
    }
    printf("\n");
    return 0;
}