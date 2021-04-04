/*
 * Copyright 2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef HEADER_KDF_H
# define HEADER_KDF_H

#ifdef __cplusplus
extern "C" {
#endif

# define EVP_PKEY_CTRL_TLS_MD       (EVP_PKEY_ALG_CTRL)
# define EVP_PKEY_CTRL_TLS_SECRET   (EVP_PKEY_ALG_CTRL + 1)
# define EVP_PKEY_CTRL_TLS_SEED     (EVP_PKEY_ALG_CTRL + 2)
# define EVP_PKEY_CTRL_HKDF_MD      (EVP_PKEY_ALG_CTRL + 3)
# define EVP_PKEY_CTRL_HKDF_SALT    (EVP_PKEY_ALG_CTRL + 4)
# define EVP_PKEY_CTRL_HKDF_KEY     (EVP_PKEY_ALG_CTRL + 5)
# define EVP_PKEY_CTRL_HKDF_INFO    (EVP_PKEY_ALG_CTRL + 6)

# define EVP_PKEY_CTX_set_tls1_prf_md(pctx, md) \
            EVP_PKEY_CTX_ctrl(pctx, -1, EVP_PKEY_OP_DERIVE, \
                              EVP_PKEY_CTRL_TLS_MD, 0, (void *)md)

# define EVP_PKEY_CTX_set1_tls1_prf_secret(pctx, sec, seclen) \
            EVP_PKEY_CTX_ctrl(pctx, -1, EVP_PKEY_OP_DERIVE, \
                              EVP_PKEY_CTRL_TLS_SECRET, seclen, (void *)sec)

# define EVP_PKEY_CTX_add1_tls1_prf_seed(pctx, seed, seedlen) \
            EVP_PKEY_CTX_ctrl(pctx, -1, EVP_PKEY_OP_DERIVE, \
                              EVP_PKEY_CTRL_TLS_SEED, seedlen, (void *)seed)

# define EVP_PKEY_CTX_set_hkdf_md(pctx, md) \
            EVP_PKEY_CTX_ctrl(pctx, -1, EVP_PKEY_OP_DERIVE, \
                              EVP_PKEY_CTRL_HKDF_MD, 0, (void *)md)

# define EVP_PKEY_CTX_set1_hkdf_salt(pctx, salt, saltlen) \
            EVP_PKEY_CTX_ctrl(pctx, -1, EVP_PKEY_OP_DERIVE, \
                              EVP_PKEY_CTRL_HKDF_SALT, saltlen, (void *)salt)

# define EVP_PKEY_CTX_set1_hkdf_key(pctx, key, keylen) \
            EVP_PKEY_CTX_ctrl(pctx, -1, EVP_PKEY_OP_DERIVE, \
                              EVP_PKEY_CTRL_HKDF_KEY, keylen, (void *)key)

# define EVP_PKEY_CTX_add1_hkdf_info(pctx, info, infolen) \
            EVP_PKEY_CTX_ctrl(pctx, -1, EVP_PKEY_OP_DERIVE, \
                              EVP_PKEY_CTRL_HKDF_INFO, infolen, (void *)info)

/* BEGIN ERROR CODES */
/*
 * The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */

int ERR_load_KDF_strings(void);

/* Error codes for the KDF functions. */

/* Function codes. */
# define KDF_F_PKEY_TLS1_PRF_CTRL_STR                     100
# define KDF_F_PKEY_TLS1_PRF_DERIVE                       101

/* Reason codes. */
# define KDF_R_INVALID_DIGEST                             100
# define KDF_R_MISSING_PARAMETER                          101
# define KDF_R_VALUE_MISSING                              102

# ifdef  __cplusplus
}
# endif
#endif
