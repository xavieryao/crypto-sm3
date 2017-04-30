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

#ifndef SM3_H
#define SM3_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#if defined(__INTEL_COMPILER)
      #define SWAP32(v) _bswap(v)
      #define SWAP64(v) _bswap64(v)
    #else
      #define SWAP32(v) _byteswap_ulong(v)
      #define SWAP64(v) _byteswap_uint64(v)
    #endif

    #define memcpy(x,y,z) __movsb(x,y,z)
    #define memset(x,y,z) __stosb(x,y,z)

    #define ROTL32(x, y) _lrotl(x, y)
    #define ROTR32(x, y) _lrotr(x, y)
#else

#define U8V(v)  (uint8_t)((uint8_t)(v)  & 0xFFU)
#define U16V(v) (uint16_t)((uint16_t)(v) & 0xFFFFU)
#define U32V(v) (uint32_t)((uint32_t)(v) & 0xFFFFFFFFUL)
#define U64V(v) (uint64_t)((uint64_t)(v) & 0xFFFFFFFFFFFFFFFFULL)

#define ROTL8(v, n) \
    (U8V((v) << (n)) | ((v) >> (8 - (n))))

#define ROTL16(v, n) \
    (U16V((v) << (n)) | ((v) >> (16 - (n))))

#define ROTL32(v, n) \
    (U32V((v) << (n)) | ((v) >> (32 - (n))))

#define ROTL64(v, n) \
    (U64V((v) << (n)) | ((v) >> (64 - (n))))

#define ROTR8(v, n) ROTL8(v, 8 - (n))
#define ROTR16(v, n) ROTL16(v, 16 - (n))
#define ROTR32(v, n) ROTL32(v, 32 - (n))
#define ROTR64(v, n) ROTL64(v, 64 - (n))

#define SWAP16(v) \
    ROTL16(v, 8)

#define SWAP32(v) \
    ((ROTL32(v,  8) & 0x00FF00FFUL) | \
     (ROTL32(v, 24) & 0xFF00FF00UL))

#define SWAP64(v) \
    ((ROTL64(v,  8) & 0x000000FF000000FFULL) | \
     (ROTL64(v, 24) & 0x0000FF000000FF00ULL) | \
     (ROTL64(v, 40) & 0x00FF000000FF0000ULL) | \
     (ROTL64(v, 56) & 0xFF000000FF000000ULL))
#endif

#define SM3_CBLOCK        64
#define SM3_DIGEST_LENGTH 32
#define SM3_LBLOCK        SM3_DIGEST_LENGTH/4

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define XCHG(x, y) (t) = (x); (x) = (y); (y) = (t);

#pragma pack(push, 1)
typedef struct _SM3_CTX {
    uint64_t len;
    union {
        uint8_t  b[SM3_DIGEST_LENGTH];
        uint32_t w[SM3_DIGEST_LENGTH/4];
        uint64_t q[SM3_DIGEST_LENGTH/8];
    } s;
    union {
        uint8_t  b[SM3_CBLOCK];
        uint32_t w[SM3_CBLOCK/4];
        uint64_t q[SM3_CBLOCK/8];
    } buf;
} SM3_CTX;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

void SM3_Init(SM3_CTX*);
void SM3_Update(SM3_CTX*, void*, uint32_t);
void SM3_Final(void*, SM3_CTX*);

void SM3_Initx(SM3_CTX*);
void SM3_Updatex(SM3_CTX*, void*, uint32_t);
void SM3_Finalx(void*, SM3_CTX*);

#ifdef __cplusplus
}
#endif

#endif