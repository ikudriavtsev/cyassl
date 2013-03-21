/* test.c
 *
 * Copyright (C) 2006-2013 wolfSSL Inc.
 *
 * This file is part of CyaSSL.
 *
 * CyaSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CyaSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */


/* Tests Microchip CRYPTO API layer */



/* mc api header */
#include "crypto.h"

/* sanity test against our default implementation, cyassl headers  */
#include <cyassl/ctaocrypt/md5.h>
#include <cyassl/ctaocrypt/sha.h>
#include <cyassl/ctaocrypt/sha256.h>
#include <cyassl/ctaocrypt/sha512.h>
#include <cyassl/ctaocrypt/hmac.h>
#include <cyassl/ctaocrypt/compress.h>
#include <cyassl/ctaocrypt/random.h>
#include <cyassl/ctaocrypt/des3.h>
#include <cyassl/ctaocrypt/aes.h>

/* c stdlib headers */
#include <stdio.h>

/* pic32 specific */
#ifdef MICROCHIP_PIC32
    #define PIC32_STARTER_KIT
    #include <p32xxxx.h>
#endif

#define OUR_DATA_SIZE 1024
static byte ourData[OUR_DATA_SIZE];
static byte* key = NULL;
static byte* iv  = NULL;

static int check_md5(void);
static int check_sha(void);
static int check_sha256(void);
static int check_sha384(void);
static int check_sha512(void);
static int check_hmac(void);
static int check_compress(void);
static int check_rng(void);
static int check_des3(void);
static int check_aescbc(void);
static int check_aesctr(void);


int main(int argc, char** argv)
{
    int ret;
    int i;

    (void)argc;
    (void)argv;

#ifdef MICROCHIP_PIC32
    DBINIT();
#endif

    /* align key, iv pointers */
    key = (byte*)XMALLOC(32, NULL, DYNAMIC_TYPE_KEY);
    if (key == NULL) {
        printf("mcapi key alloc failed\n");
        return -1;
    }

    iv = (byte*)XMALLOC(16, NULL, DYNAMIC_TYPE_KEY);
    if (iv == NULL) {
        printf("mcapi iv alloc failed\n");
        return -1;
    }

    for (i = 0; i < OUR_DATA_SIZE; i++)
        ourData[i] = (byte)i;

    ret = check_md5();
    if (ret != 0) {
        printf("mcapi check_md5 failed\n");
        return -1;
    }

    ret = check_sha();
    if (ret != 0) {
        printf("mcapi check_sha failed\n");
        return -1;
    }

    ret = check_sha256();
    if (ret != 0) {
        printf("mcapi check_sha256 failed\n");
        return -1;
    }

    ret = check_sha384();
    if (ret != 0) {
        printf("mcapi check_sha384 failed\n");
        return -1;
    }

    ret = check_sha512();
    if (ret != 0) {
        printf("mcapi check_sha512 failed\n");
        return -1;
    }

    ret = check_hmac();
    if (ret != 0) {
        printf("mcapi check_hmac failed\n");
        return -1;
    }

    ret = check_compress();
    if (ret != 0) {
        printf("mcapi check_compress failed\n");
        return -1;
    }

    ret = check_rng();
    if (ret != 0) {
        printf("mcapi check_rng failed\n");
        return -1;
    }

    ret = check_des3();
    if (ret != 0) {
        printf("mcapi check_des3 failed\n");
        return -1;
    }

    ret = check_aescbc();
    if (ret != 0) {
        printf("mcapi check_aes cbc failed\n");
        return -1;
    }

    ret = check_aesctr();
    if (ret != 0) {
        printf("mcapi check_aes ctr failed\n");
        return -1;
    }



    XFREE(iv,  NULL, DYNAMIC_TYPE_KEY);
    XFREE(key, NULL, DYNAMIC_TYPE_KEY);

    return 0;
}


/* check mcapi md5 against internal */
static int check_md5(void)
{
    CRYPT_MD5_CTX mcMd5;
    Md5           defMd5;
    byte          mcDigest[CRYPT_MD5_DIGEST_SIZE];
    byte          defDigest[MD5_DIGEST_SIZE];

    CRYPT_MD5_Initialize(&mcMd5);
    InitMd5(&defMd5);

    CRYPT_MD5_DataAdd(&mcMd5, ourData, OUR_DATA_SIZE);
    Md5Update(&defMd5, ourData, OUR_DATA_SIZE);

    CRYPT_MD5_Finalize(&mcMd5, mcDigest);
    Md5Final(&defMd5, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_MD5_DIGEST_SIZE) != 0) {
        printf("md5 final memcmp fialed\n");
        return -1;
    } 
    printf("md5         mcapi test passed\n");

    return 0;
}


/* check mcapi sha against internal */
static int check_sha(void)
{
    CRYPT_SHA_CTX mcSha;
    Sha           defSha;
    byte          mcDigest[CRYPT_SHA_DIGEST_SIZE];
    byte          defDigest[SHA_DIGEST_SIZE];

    CRYPT_SHA_Initialize(&mcSha);
    InitSha(&defSha);

    CRYPT_SHA_DataAdd(&mcSha, ourData, OUR_DATA_SIZE);
    ShaUpdate(&defSha, ourData, OUR_DATA_SIZE);

    CRYPT_SHA_Finalize(&mcSha, mcDigest);
    ShaFinal(&defSha, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA_DIGEST_SIZE) != 0) {
        printf("sha final memcmp fialed\n");
        return -1;
    } 
    printf("sha         mcapi test passed\n");

    return 0;
}


/* check mcapi sha256 against internal */
static int check_sha256(void)
{
    CRYPT_SHA256_CTX mcSha256;
    Sha256           defSha256;
    byte             mcDigest[CRYPT_SHA256_DIGEST_SIZE];
    byte             defDigest[SHA256_DIGEST_SIZE];

    CRYPT_SHA256_Initialize(&mcSha256);
    InitSha256(&defSha256);

    CRYPT_SHA256_DataAdd(&mcSha256, ourData, OUR_DATA_SIZE);
    Sha256Update(&defSha256, ourData, OUR_DATA_SIZE);

    CRYPT_SHA256_Finalize(&mcSha256, mcDigest);
    Sha256Final(&defSha256, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA256_DIGEST_SIZE) != 0) {
        printf("sha256 final memcmp fialed\n");
        return -1;
    } 
    printf("sha256      mcapi test passed\n");

    return 0;
}


/* check mcapi sha384 against internal */
static int check_sha384(void)
{
    CRYPT_SHA384_CTX mcSha384;
    Sha384           defSha384;
    byte             mcDigest[CRYPT_SHA384_DIGEST_SIZE];
    byte             defDigest[SHA384_DIGEST_SIZE];

    CRYPT_SHA384_Initialize(&mcSha384);
    InitSha384(&defSha384);

    CRYPT_SHA384_DataAdd(&mcSha384, ourData, OUR_DATA_SIZE);
    Sha384Update(&defSha384, ourData, OUR_DATA_SIZE);

    CRYPT_SHA384_Finalize(&mcSha384, mcDigest);
    Sha384Final(&defSha384, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA384_DIGEST_SIZE) != 0) {
        printf("sha384 final memcmp fialed\n");
        return -1;
    } 
    printf("sha384      mcapi test passed\n");

    return 0;
}


/* check mcapi sha512 against internal */
static int check_sha512(void)
{
    CRYPT_SHA512_CTX mcSha512;
    Sha512           defSha512;
    byte             mcDigest[CRYPT_SHA512_DIGEST_SIZE];
    byte             defDigest[SHA512_DIGEST_SIZE];

    CRYPT_SHA512_Initialize(&mcSha512);
    InitSha512(&defSha512);

    CRYPT_SHA512_DataAdd(&mcSha512, ourData, OUR_DATA_SIZE);
    Sha512Update(&defSha512, ourData, OUR_DATA_SIZE);

    CRYPT_SHA512_Finalize(&mcSha512, mcDigest);
    Sha512Final(&defSha512, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA512_DIGEST_SIZE) != 0) {
        printf("sha512 final memcmp fialed\n");
        return -1;
    } 
    printf("sha512      mcapi test passed\n");

    return 0;
}


/* check mcapi hmac against internal */
static int check_hmac(void)
{
    CRYPT_HMAC_CTX mcHmac;
    Hmac           defHmac;
    byte           mcDigest[CRYPT_SHA512_DIGEST_SIZE];
    byte           defDigest[SHA512_DIGEST_SIZE];

    strncpy((char*)key, "Jefe", 4);

    /* SHA1 */
    CRYPT_HMAC_SetKey(&mcHmac, CRYPT_HMAC_SHA, key, 4);
    HmacSetKey(&defHmac, SHA, key, 4);

    CRYPT_HMAC_DataAdd(&mcHmac, ourData, OUR_DATA_SIZE);
    HmacUpdate(&defHmac, ourData, OUR_DATA_SIZE);

    CRYPT_HMAC_Finalize(&mcHmac, mcDigest);
    HmacFinal(&defHmac, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA_DIGEST_SIZE) != 0) {
        printf("hmac sha final memcmp fialed\n");
        return -1;
    } 
    printf("hmac sha    mcapi test passed\n");

    /* SHA-256 */
    CRYPT_HMAC_SetKey(&mcHmac, CRYPT_HMAC_SHA256, key, 4);
    HmacSetKey(&defHmac, SHA256, key, 4);

    CRYPT_HMAC_DataAdd(&mcHmac, ourData, OUR_DATA_SIZE);
    HmacUpdate(&defHmac, ourData, OUR_DATA_SIZE);

    CRYPT_HMAC_Finalize(&mcHmac, mcDigest);
    HmacFinal(&defHmac, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA256_DIGEST_SIZE) != 0) {
        printf("hmac sha256 final memcmp fialed\n");
        return -1;
    } 
    printf("hmac sha256 mcapi test passed\n");

    /* SHA-384 */
    CRYPT_HMAC_SetKey(&mcHmac, CRYPT_HMAC_SHA384, key, 4);
    HmacSetKey(&defHmac, SHA384, key, 4);

    CRYPT_HMAC_DataAdd(&mcHmac, ourData, OUR_DATA_SIZE);
    HmacUpdate(&defHmac, ourData, OUR_DATA_SIZE);

    CRYPT_HMAC_Finalize(&mcHmac, mcDigest);
    HmacFinal(&defHmac, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA384_DIGEST_SIZE) != 0) {
        printf("hmac sha384 final memcmp fialed\n");
        return -1;
    } 
    printf("hmac sha384 mcapi test passed\n");

    /* SHA-512 */
    CRYPT_HMAC_SetKey(&mcHmac, CRYPT_HMAC_SHA512, key, 4);
    HmacSetKey(&defHmac, SHA512, key, 4);

    CRYPT_HMAC_DataAdd(&mcHmac, ourData, OUR_DATA_SIZE);
    HmacUpdate(&defHmac, ourData, OUR_DATA_SIZE);

    CRYPT_HMAC_Finalize(&mcHmac, mcDigest);
    HmacFinal(&defHmac, defDigest);

    if (memcmp(mcDigest, defDigest, CRYPT_SHA512_DIGEST_SIZE) != 0) {
        printf("hmac sha512 final memcmp fialed\n");
        return -1;
    } 
    printf("hmac sha512 mcapi test passed\n");

    return 0;
}


/* check mcapi compress against internal */
static int check_compress(void)
{
    const unsigned char text[] =
    "Biodiesel cupidatat marfa, cliche aute put a bird on it incididunt elit\n"
    "polaroid. Sunt tattooed bespoke reprehenderit. Sint twee organic id\n"
    "marfa. Commodo veniam ad esse gastropub. 3 wolf moon sartorial vero,\n"
    "plaid delectus biodiesel squid +1 vice. Post-ironic keffiyeh leggings\n"
    "selfies cray fap hoodie, forage anim. Carles cupidatat shoreditch, VHS\n"
    "small batch meggings kogi dolore food truck bespoke gastropub.\n"
    "\n"
    "Terry richardson adipisicing actually typewriter tumblr, twee whatever\n"
    "four loko you probably haven't heard of them high life. Messenger bag\n"
    "whatever tattooed deep v mlkshk. Brooklyn pinterest assumenda chillwave\n"
    "et, banksy ullamco messenger bag umami pariatur direct trade forage.\n"
    "Typewriter culpa try-hard, pariatur sint brooklyn meggings. Gentrify\n"
    "food truck next level, tousled irony non semiotics PBR ethical anim cred\n"
    "readymade. Mumblecore brunch lomo odd future, portland organic terry\n"
    "four loko whatever street art yr farm-to-table.\n";

    unsigned int inSz  = sizeof(text);
    unsigned int outSz;
    unsigned char cBuffer[1024];
    unsigned char dBuffer[1024];

    int ret1, ret2;

    /* dynamic */
    ret1 = CRYPT_HUFFMAN_Compress(cBuffer, sizeof(cBuffer), text, inSz, 0);
    ret2 = Compress(dBuffer, sizeof(dBuffer), text, inSz, 0);

    if (ret1 != ret2 || ret1 < 0) {
        printf("compress dynamic ret failed\n");
        return -1;
    }

    if (memcmp(cBuffer, dBuffer, ret1) != 0) {
        printf("compress dynamic cmp failed\n");
        return -1;
    }

    outSz = ret1;

    ret1 = CRYPT_HUFFMAN_DeCompress(dBuffer, sizeof(dBuffer), cBuffer, outSz);

    if (memcmp(dBuffer, text, inSz) != 0) {
        printf("mcapi decompress dynamic cmp failed\n");
        return -1;
    }

    memset(dBuffer, 0, sizeof(dBuffer));

    ret1 = DeCompress(dBuffer, sizeof(dBuffer), cBuffer, outSz);

    if (memcmp(dBuffer, text, inSz) != 0) {
        printf("decompress dynamic cmp failed\n");
        return -1;
    }

    memset(cBuffer, 0, sizeof(cBuffer));
    memset(dBuffer, 0, sizeof(dBuffer));

    /* static */
    ret1 = CRYPT_HUFFMAN_Compress(cBuffer, sizeof(cBuffer), text, inSz, 1);
    ret2 = Compress(dBuffer, sizeof(dBuffer), text, inSz, 1);

    if (ret1 != ret2 || ret1 < 0) {
        printf("compress static ret failed\n");
        return -1;
    }

    if (memcmp(cBuffer, dBuffer, ret1) != 0) {
        printf("compress static cmp failed\n");
        return -1;
    }

    outSz = ret1;

    ret1 = CRYPT_HUFFMAN_DeCompress(dBuffer, sizeof(dBuffer), cBuffer, outSz);

    if (memcmp(dBuffer, text, inSz) != 0) {
        printf("mcapi decompress static cmp failed\n");
        return -1;
    }

    memset(dBuffer, 0, sizeof(dBuffer));

    ret1 = DeCompress(dBuffer, sizeof(dBuffer), cBuffer, outSz);

    if (memcmp(dBuffer, text, inSz) != 0) {
        printf("decompress static cmp failed\n");
        return -1;
    }

    printf("huffman     mcapi test passed\n");

    return 0;
}


#define RANDOM_BYTE_SZ 32

/* check mcapi rng */
static int check_rng(void)
{
    CRYPT_RNG_CTX rng;
    int           ret;
    int           i; 
    byte          in[RANDOM_BYTE_SZ];
    byte          out[RANDOM_BYTE_SZ];

    for (i = 0; i < RANDOM_BYTE_SZ; i++)
        in[i] = (byte)i;

    for (i = 0; i < RANDOM_BYTE_SZ; i++)
        out[i] = (byte)i;

    ret = CRYPT_RNG_Initialize(&rng);
    if (ret != 0) {
        printf("mcapi rng init failed\n");
        return -1;
    }

    ret = CRYPT_RNG_Get(&rng, &out[0]);
    if (ret != 0) {
        printf("mcapi rng get failed\n");
        return -1;
    }

    ret = CRYPT_RNG_BlockGenerate(&rng, out, RANDOM_BYTE_SZ);
    if (ret != 0) {
        printf("mcapi rng block gen failed\n");
        return -1;
    }

    if (memcmp(in, out, RANDOM_BYTE_SZ) == 0) {
        printf("mcapi rng block gen output failed\n");
        return -1;
    }

    printf("rng         mcapi test passed\n");

    return 0;
}


#define TDES_TEST_SIZE 32

/* check mcapi des3 */
static int check_des3(void)
{
    CRYPT_TDES_CTX mcDes3;
    Des3           defDes3;
    int            ret;
    byte           out1[TDES_TEST_SIZE];
    byte           out2[TDES_TEST_SIZE];

    strncpy((char*)key, "1234567890abcdefghijklmn", 24);
    strncpy((char*)iv,  "12345678", 8);

    /* cbc encrypt */
    ret = CRYPT_TDES_KeySet(&mcDes3, key, iv, CRYPT_TDES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi tdes key set failed\n");
        return -1;
    }
    Des3_SetKey(&defDes3, key, iv, DES_ENCRYPTION);

    ret = CRYPT_TDES_CBC_Encrypt(&mcDes3, out1, ourData, TDES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi tdes cbc encrypt failed\n");
        return -1;
    }
    Des3_CbcEncrypt(&defDes3, out2, ourData, TDES_TEST_SIZE);

    if (memcmp(out1, out2, TDES_TEST_SIZE) != 0) {
        printf("mcapi tdes cbc encrypt cmp failed\n");
        return -1;
    }

    /* cbc decrypt */
    ret = CRYPT_TDES_KeySet(&mcDes3, key, iv, CRYPT_TDES_DECRYPTION);
    if (ret != 0) {
        printf("mcapi tdes key set failed\n");
        return -1;
    }
    Des3_SetKey(&defDes3, key, iv, DES_DECRYPTION);

    ret = CRYPT_TDES_CBC_Decrypt(&mcDes3, out2, out1, TDES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi tdes cbc decrypt failed\n");
        return -1;
    }
    Des3_CbcDecrypt(&defDes3, out1, out1, TDES_TEST_SIZE);

    if (memcmp(out1, out2, TDES_TEST_SIZE) != 0) {
        printf("mcapi tdes cbc decrypt cmp failed\n");
        return -1;
    }

    if (memcmp(out1, ourData, TDES_TEST_SIZE) != 0) {
        printf("mcapi tdes cbc decrypt orig cmp failed\n");
        return -1;
    }

    printf("tdes        mcapi test passed\n");

    return 0;
}


#define AES_TEST_SIZE 32

/* check mcapi aes cbc */
static int check_aescbc(void)
{
    CRYPT_AES_CTX mcAes;
    Aes           defAes;
    int           ret;
    byte          out1[AES_TEST_SIZE];
    byte          out2[AES_TEST_SIZE];

    strncpy((char*)key, "1234567890abcdefghijklmnopqrstuv", 32);
    strncpy((char*)iv,  "1234567890abcdef", 16);

    /* 128 cbc encrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 16, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-128 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 16, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-128 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CBC_Encrypt(&mcAes, out1, ourData, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-128 cbc encrypt failed\n");
        return -1;
    }
    AesCbcEncrypt(&defAes, out2, ourData, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-128 cbc encrypt cmp failed\n");
        return -1;
    }

    /* 128 cbc decrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 16, iv, CRYPT_AES_DECRYPTION);
    if (ret != 0) {
        printf("mcapi aes-128 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 16, iv, DES_DECRYPTION);
    if (ret != 0) {
        printf("default aes-128 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CBC_Decrypt(&mcAes, out2, out1, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-128 cbc decrypt failed\n");
        return -1;
    }
    AesCbcDecrypt(&defAes, out1, out1, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-128 cbc decrypt cmp failed\n");
        return -1;
    }

    if (memcmp(out1, ourData, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-128 cbc decrypt orig cmp failed\n");
        return -1;
    }

    /* 192 cbc encrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 24, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-192 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 24, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-192 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CBC_Encrypt(&mcAes, out1, ourData, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-192 cbc encrypt failed\n");
        return -1;
    }
    AesCbcEncrypt(&defAes, out2, ourData, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-192 cbc encrypt cmp failed\n");
        return -1;
    }

    /* 192 cbc decrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 24, iv, CRYPT_AES_DECRYPTION);
    if (ret != 0) {
        printf("mcapi aes-192 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 24, iv, AES_DECRYPTION);
    if (ret != 0) {
        printf("default aes-192 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CBC_Decrypt(&mcAes, out2, out1, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-192 cbc decrypt failed\n");
        return -1;
    }
    AesCbcDecrypt(&defAes, out1, out1, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-192 cbc decrypt cmp failed\n");
        return -1;
    }

    if (memcmp(out1, ourData, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-192 cbc decrypt orig cmp failed\n");
        return -1;
    }

    /* 256 cbc encrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 32, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-256 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 32, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-256 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CBC_Encrypt(&mcAes, out1, ourData, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-256 cbc encrypt failed\n");
        return -1;
    }
    AesCbcEncrypt(&defAes, out2, ourData, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-256 cbc encrypt cmp failed\n");
        return -1;
    }

    /* 256 cbc decrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 32, iv, CRYPT_AES_DECRYPTION);
    if (ret != 0) {
        printf("mcapi aes-256 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 32, iv, AES_DECRYPTION);
    if (ret != 0) {
        printf("default aes-256 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CBC_Decrypt(&mcAes, out2, out1, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-256 cbc decrypt failed\n");
        return -1;
    }
    AesCbcDecrypt(&defAes, out1, out1, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-256 cbc decrypt cmp failed\n");
        return -1;
    }

    if (memcmp(out1, ourData, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-256 cbc decrypt orig cmp failed\n");
        return -1;
    }

    printf("aes-cbc     mcapi test passed\n");

    return 0;
}


/* check mcapi aes ctr */
static int check_aesctr(void)
{
    CRYPT_AES_CTX mcAes;
    Aes           defAes;
    int           ret;
    byte          out1[AES_TEST_SIZE];
    byte          out2[AES_TEST_SIZE];

    strncpy((char*)key, "1234567890abcdefghijklmnopqrstuv", 32);
    strncpy((char*)iv,  "1234567890abcdef", 16);

    /* 128 ctr encrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 16, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-128 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 16, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-128 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CTR_Encrypt(&mcAes, out1, ourData, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-128 ctr encrypt failed\n");
        return -1;
    }
    AesCtrEncrypt(&defAes, out2, ourData, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-128 ctr encrypt cmp failed\n");
        return -1;
    }

    /* 128 ctr decrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 16, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-128 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 16, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-128 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CTR_Encrypt(&mcAes, out2, out1, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-128 ctr decrypt failed\n");
        return -1;
    }

    if (memcmp(out2, ourData, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-128 ctr decrypt orig cmp failed\n");
        return -1;
    }

    /* 192 ctr encrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 24, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-192 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 24, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-192 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CTR_Encrypt(&mcAes, out1, ourData, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-192 ctr encrypt failed\n");
        return -1;
    }
    AesCtrEncrypt(&defAes, out2, ourData, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-192 ctr encrypt cmp failed\n");
        return -1;
    }

    /* 192 ctr decrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 24, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-192 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 24, iv, AES_DECRYPTION);
    if (ret != 0) {
        printf("default aes-192 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CTR_Encrypt(&mcAes, out2, out1, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-192 ctr decrypt failed\n");
        return -1;
    }

    if (memcmp(out2, ourData, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-192 ctr decrypt orig cmp failed\n");
        return -1;
    }

    /* 256 ctr encrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 32, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-256 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 32, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-256 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CTR_Encrypt(&mcAes, out1, ourData, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-256 ctr encrypt failed\n");
        return -1;
    }
    AesCtrEncrypt(&defAes, out2, ourData, AES_TEST_SIZE);

    if (memcmp(out1, out2, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-256 ctr encrypt cmp failed\n");
        return -1;
    }

    /* 256 ctr decrypt */
    ret = CRYPT_AES_KeySet(&mcAes, key, 32, iv, CRYPT_AES_ENCRYPTION);
    if (ret != 0) {
        printf("mcapi aes-256 key set failed\n");
        return -1;
    }
    ret = AesSetKey(&defAes, key, 32, iv, AES_ENCRYPTION);
    if (ret != 0) {
        printf("default aes-256 key set failed\n");
        return -1;
    }

    ret = CRYPT_AES_CTR_Encrypt(&mcAes, out2, out1, AES_TEST_SIZE);
    if (ret != 0) {
        printf("mcapi aes-256 ctr decrypt failed\n");
        return -1;
    }

    if (memcmp(out2, ourData, AES_TEST_SIZE) != 0) {
        printf("mcapi aes-256 ctr decrypt orig cmp failed\n");
        return -1;
    }

    printf("aes-ctr     mcapi test passed\n");

    return 0;
}



