#include "xvm/xvm.h"

#include <sodium.h>

_Static_assert(XVM_DIGEST_BYTES == crypto_generichash_BYTES,
               "XVM digest size must match libsodium crypto_generichash default size");

bool xvm_crypto_init(void)
{
    return sodium_init() >= 0;
}

bool xvm_hash_hex(const unsigned char *data,
                  size_t data_size,
                  char *output,
                  size_t output_size)
{
    static const unsigned char empty_input = 0;
    unsigned char digest[XVM_DIGEST_BYTES];

    if (output == NULL || output_size < XVM_DIGEST_HEX_LENGTH) {
        return false;
    }

    if (data == NULL && data_size != 0) {
        output[0] = '\0';
        return false;
    }

    if (!xvm_crypto_init()) {
        output[0] = '\0';
        return false;
    }

    if (data == NULL) {
        data = &empty_input;
    }

    if (crypto_generichash(digest, sizeof(digest), data, data_size, NULL, 0) != 0) {
        output[0] = '\0';
        return false;
    }

    sodium_bin2hex(output, output_size, digest, sizeof(digest));
    sodium_memzero(digest, sizeof(digest));

    return true;
}
