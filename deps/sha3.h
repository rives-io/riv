#ifndef SHA3_H
#define SHA3_H

#include <stddef.h>
#include <stdint.h>

/* -------------------------------------------------------------------------
 * Works when compiled for either 32-bit or 64-bit targets, optimized for 
 * 64 bit.
 *
 * Canonical implementation of Init/Update/Finalize for SHA-3 byte input. 
 *
 * SHA3-256, SHA3-384, SHA-512 are implemented. SHA-224 can easily be added.
 *
 * Based on code from http://keccak.noekeon.org/ .
 *
 * I place the code that I wrote into public domain, free to use. 
 *
 * I would appreciate if you give credits to this work if you used it to 
 * write or test * your code.
 *
 * Aug 2015. Andrey Jivsov. crypto@brainhub.org
 * ---------------------------------------------------------------------- */

/* 'Words' here refers to uint64_t */
#define SHA3_KECCAK_SPONGE_WORDS 25
typedef struct sha3_context {
    uint64_t saved;             /* the portion of the input message that we
                                 * didn't consume yet */
    union {                     /* Keccak's state */
        uint64_t s[SHA3_KECCAK_SPONGE_WORDS];
        uint8_t sb[SHA3_KECCAK_SPONGE_WORDS * 8];
    } u;
    unsigned byteIndex;         /* 0..7--the next byte after the set one
                                 * (starts from 0; 0--none are buffered) */
    unsigned wordIndex;         /* 0..24--the next word to integrate input
                                 * (starts from 0) */
    unsigned capacityWords;     /* the double size of the hash output in
                                 * words (e.g. 16 for Keccak 512) */
} sha3_context;

typedef enum SHA3_FLAGS {
    SHA3_FLAGS_NONE=0,
    SHA3_FLAGS_KECCAK=1
} sha3_flags_t;

typedef enum SHA3_RETURN {
    SHA3_RETURN_OK=0,
    SHA3_RETURN_BAD_PARAMS=1
} sha3_return_t;

/* For Init or Reset call these: */
sha3_return_t sha3_Init(void *priv, unsigned bitSize);

void sha3_Init256(void *priv);
void sha3_Init384(void *priv);
void sha3_Init512(void *priv);

sha3_flags_t sha3_SetFlags(void *priv, sha3_flags_t);

void sha3_Update(void *priv, const void* bufIn, size_t len);

const void *sha3_Finalize(void *priv);

/* Single-call hashing */
sha3_return_t sha3_HashBuffer( 
    unsigned bitSize,   /* 256, 384, 512 */
    sha3_flags_t flags, /* SHA3_FLAGS_NONE or SHA3_FLAGS_KECCAK */
    const void *inBuf, unsigned inBytes,
    void *outBuf, unsigned outBytes );     /* up to bitSize/8; truncation OK */

#endif
