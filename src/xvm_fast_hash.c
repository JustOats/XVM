#include "xvm/xvm.h"

#include <inttypes.h>
#include <stdio.h>
#include <xxhash.h>

uint64_t xvm_fast_hash64(const unsigned char *data,
                         size_t data_size,
                         uint64_t seed)
{
    static const unsigned char empty_input = 0;

    if (data == NULL && data_size != 0) {
        return 0;
    }

    if (data == NULL) {
        data = &empty_input;
    }

    return (uint64_t)XXH3_64bits_withSeed(data, data_size, seed);
}

bool xvm_fast_hash64_hex(const unsigned char *data,
                         size_t data_size,
                         uint64_t seed,
                         char *output,
                         size_t output_size)
{
    const uint64_t hash = xvm_fast_hash64(data, data_size, seed);
    int written = 0;

    if (output == NULL || output_size < XVM_XXH64_HEX_LENGTH) {
        return false;
    }

    if (data == NULL && data_size != 0) {
        output[0] = '\0';
        return false;
    }

    written = snprintf(output, output_size, "%016" PRIx64, hash);

    return written == (XVM_XXH64_HEX_LENGTH - 1);
}
