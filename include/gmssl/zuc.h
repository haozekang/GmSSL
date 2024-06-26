/*
 *  Copyright 2014-2024 The GmSSL Project. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the License); you may
 *  not use this file except in compliance with the License.
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef GMSSL_ZUC_H
#define GMSSL_ZUC_H


#include <stdlib.h>
#include <stdint.h>
#include <gmssl/api.h>


#ifdef __cplusplus
extern "C" {
#endif


# define ZUC_KEY_SIZE	16
# define ZUC_IV_SIZE	16
# define ZUC_MAC_SIZE	4

typedef uint32_t ZUC_BIT;
typedef uint32_t ZUC_UINT5;
typedef uint8_t  ZUC_UINT6;
typedef uint32_t ZUC_UINT15;
typedef uint32_t ZUC_UINT31;
typedef uint32_t ZUC_UINT32;

typedef struct {
	ZUC_UINT31 LFSR[16];
	ZUC_UINT32 R1;
	ZUC_UINT32 R2;
} ZUC_STATE;

void zuc_init(ZUC_STATE *state, const uint8_t key[ZUC_KEY_SIZE], const uint8_t iv[ZUC_IV_SIZE]);
void zuc_generate_keystream(ZUC_STATE *state, size_t nwords, ZUC_UINT32 *words);
ZUC_UINT32 zuc_generate_keyword(ZUC_STATE *state);
void zuc_encrypt(ZUC_STATE *state, const uint8_t *in, size_t inlen, uint8_t *out);

typedef struct ZUC_MAC_CTX_st {
	ZUC_UINT31 LFSR[16];
	ZUC_UINT32 R1;
	ZUC_UINT32 R2;
	ZUC_UINT32 T;
	ZUC_UINT32 K0;
	uint8_t buf[4];
	size_t buflen;
} ZUC_MAC_CTX;

void zuc_mac_init(ZUC_MAC_CTX *ctx, const uint8_t key[ZUC_KEY_SIZE], const uint8_t iv[ZUC_IV_SIZE]);
void zuc_mac_update(ZUC_MAC_CTX *ctx, const uint8_t *data, size_t len);
void zuc_mac_finish(ZUC_MAC_CTX *ctx, const uint8_t *data, size_t nbits, uint8_t mac[ZUC_MAC_SIZE]);

#define ZUC_EEA_ENCRYPT_NWORDS(nbits) ((nbits + 31)/32)
#define ZUC_EEA_ENCRYPT_NBYTES(nbits) (ZUC_EEA_ENCRYPT_NWORDS(nbits)*4)
void zuc_eea_encrypt(const ZUC_UINT32 *in, ZUC_UINT32 *out, size_t nbits,
	const uint8_t key[ZUC_KEY_SIZE], ZUC_UINT32 count, ZUC_UINT5 bearer,
	ZUC_BIT direction);
ZUC_UINT32 zuc_eia_generate_mac(const ZUC_UINT32 *data, size_t nbits,
	const uint8_t key[ZUC_KEY_SIZE], ZUC_UINT32 count, ZUC_UINT5 bearer,
	ZUC_BIT direction);


# define ZUC256_KEY_SIZE	32
# define ZUC256_IV_SIZE		23
# define ZUC256_MAC32_SIZE	4
# define ZUC256_MAC64_SIZE	8
# define ZUC256_MAC128_SIZE	16
# define ZUC256_MIN_MAC_SIZE	ZUC256_MAC32_SIZE
# define ZUC256_MAX_MAC_SIZE	ZUC256_MAC128_SIZE

typedef ZUC_STATE ZUC256_STATE;

void zuc256_init(ZUC256_STATE *state, const uint8_t key[ZUC256_KEY_SIZE], const uint8_t iv[ZUC256_IV_SIZE]);
void zuc256_generate_keystream(ZUC_STATE *state, size_t nwords, ZUC_UINT32 *words);
ZUC_UINT32 zuc256_generate_keyword(ZUC_STATE *state);


typedef struct ZUC256_MAC_CTX_st {
	ZUC_UINT31 LFSR[16];
	ZUC_UINT32 R1;
	ZUC_UINT32 R2;
	ZUC_UINT32 T[4];
	ZUC_UINT32 K0[4];
	uint8_t buf[4];
	size_t buflen;
	int macbits;
} ZUC256_MAC_CTX;

void zuc256_mac_init(ZUC256_MAC_CTX *ctx, const uint8_t key[ZUC256_KEY_SIZE],
	const uint8_t iv[ZUC256_IV_SIZE], int macbits);
void zuc256_mac_update(ZUC256_MAC_CTX *ctx, const uint8_t *data, size_t len);
void zuc256_mac_finish(ZUC256_MAC_CTX *ctx, const uint8_t *data, size_t nbits, uint8_t mac[ZUC_MAC_SIZE]);


typedef struct {
	ZUC_STATE zuc_state;
	uint8_t block[4];
	size_t block_nbytes;
} ZUC_CTX;

_gmssl_export int zuc_encrypt_init(ZUC_CTX *ctx, const uint8_t key[ZUC_KEY_SIZE], const uint8_t iv[ZUC_IV_SIZE]);
_gmssl_export int zuc_encrypt_update(ZUC_CTX *ctx, const uint8_t *in, size_t inlen, uint8_t *out, size_t *outlen);
_gmssl_export int zuc_encrypt_finish(ZUC_CTX *ctx, uint8_t *out, size_t *outlen);


#ifdef __cplusplus
}
#endif
#endif
