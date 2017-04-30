/**
  Copyright © 2017 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include "sm3.h"

#define  F(x, y, z) (((x) ^ (y) ^ (z)))
#define FF(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define GG(x, y, z) ((z)  ^ ((x) & ((y) ^ (z))))

#define P0(x) x ^ ROTL32(x,  9) ^ ROTL32(x, 17)
#define P1(x) x ^ ROTL32(x, 15) ^ ROTL32(x, 23)

/************************************************
*
* update state with block of data
*
************************************************/
void SM3_Transform (SM3_CTX *ctx)
{
    uint32_t tt1, tt2, i, t, ss1, ss2, x, y;
    uint32_t w[68], s[8];

#define a s[0]
#define b s[1]
#define c s[2]
#define d s[3]
#define e s[4]
#define f s[5]
#define g s[6]
#define h s[7]

    // load state into local buffer
    memcpy((uint8_t*)&s[0], (uint8_t*)&ctx->s.w[0], 8*4);

    // load data in big endian format
    for (i=0; i<16; i++) {
        w[i] = SWAP32(ctx->buf.w[i]);
    }

    // expand message
    for (i=16; i<68; i++) {
        x = ROTL32(w[i- 3], 15);
        y = ROTL32(w[i-13],  7);

        x ^= w[i-16];
        x ^= w[i- 9];
        y ^= w[i- 6];

        w[i] = P1(x) ^ y;
    }

    // compression function
    for (i=0; i<64; i++) {

        t  = (i < 16) ? 0x79cc4519 : 0x7a879d8a;

        ss2 = ROTL32(a, 12);
        ss1 = ROTL32(ss2 + e + ROTL32(t, i), 7);
        ss2 ^= ss1;

        tt1 = d + ss2 + (w[i] ^ w[i+4]);
        tt2 = h + ss1 + w[i];

        if (i < 16) {
            tt1 += F(a, b, c);
            tt2 += F(e, f, g);
        } else {
            tt1 += FF(a, b, c);
            tt2 += GG(e, f, g);
        }
        d = c;
        c = ROTL32(b, 9);
        b = a;
        a = tt1;
        h = g;
        g = ROTL32(f, 19);
        f = e;
        e = P0(tt2);

    }

    // Davies–Meyer idea for compression function
    for (i=0; i<8; i++) {
        ctx->s.w[i] ^= s[i];
    }
#undef a
#undef b
#undef c
#undef d
#undef e
#undef f
#undef g
#undef h
}

/************************************************
*
* initialize context
*
************************************************/
void SM3_Init (SM3_CTX *ctx) {
    ctx->s.w[0] = 0x7380166f;
    ctx->s.w[1] = 0x4914b2b9;
    ctx->s.w[2] = 0x172442d7;
    ctx->s.w[3] = 0xda8a0600;
    ctx->s.w[4] = 0xa96f30bc;
    ctx->s.w[5] = 0x163138aa;
    ctx->s.w[6] = 0xe38dee4d;
    ctx->s.w[7] = 0xb0fb0e4e;
    ctx->len    = 0;
}

/************************************************
*
* update state with input
*
************************************************/
void SM3_Update (SM3_CTX *ctx, void *in, uint32_t len) {
    uint8_t *p = (uint8_t*)in;
    uint32_t r, idx;

    if (len==0) return;

    // get buffer index
    idx = ctx->len & (SM3_CBLOCK - 1);

    // update length
    ctx->len += len;

    while (len) {
        r = MIN(len, SM3_CBLOCK - idx);
        memcpy (&ctx->buf.b[idx], p, r);
        if ((idx + r) < SM3_CBLOCK) break;

        SM3_Transform (ctx);
        len -= r;
        idx = 0;
        p += r;
    }
}

/************************************************
*
* finalize.
*
************************************************/
void SM3_Final (void *out, SM3_CTX *ctx)
{
    int i;

    // get the current index
    uint32_t idx = ctx->len & (SM3_CBLOCK - 1);
    // fill remaining with zeros
    memset (&ctx->buf.b[idx], 0, SM3_CBLOCK - idx);
    // add the end bit
    ctx->buf.b[idx] = 0x80;
    // if exceeding 56 bytes, transform it
    if (idx >= 56) {
        SM3_Transform (ctx);
        // clear buffer
        memset (ctx->buf.b, 0, SM3_CBLOCK);
    }
    // add total bits
    ctx->buf.q[7] = SWAP64((uint64_t)ctx->len * 8);

    // compress
    SM3_Transform(ctx);

    for (i=0; i<SM3_LBLOCK; i++) {
        ((uint32_t*)out)[i] = SWAP32(ctx->s.w[i]);
    }
}