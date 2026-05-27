#ifndef XVM_XVM_H
#define XVM_XVM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XVM_VERSION_MAJOR 0
#define XVM_VERSION_MINOR 1
#define XVM_VERSION_PATCH 0
#define XVM_DIGEST_BYTES 32
#define XVM_DIGEST_HEX_LENGTH ((XVM_DIGEST_BYTES * 2) + 1)
#define XVM_XXH64_HEX_LENGTH 17

typedef struct xvm_version {
    int major;
    int minor;
    int patch;
} xvm_version_t;

const char *xvm_name(void);
xvm_version_t xvm_version(void);
bool xvm_self_check(void);
bool xvm_crypto_init(void);
bool xvm_hash_hex(const unsigned char *data,
                  size_t data_size,
                  char *output,
                  size_t output_size);
bool xvm_disassemble_first_x64(const unsigned char *code,
                               size_t code_size,
                               uint64_t runtime_address,
                               char *output,
                               size_t output_size);
uint64_t xvm_fast_hash64(const unsigned char *data,
                         size_t data_size,
                         uint64_t seed);
bool xvm_fast_hash64_hex(const unsigned char *data,
                         size_t data_size,
                         uint64_t seed,
                         char *output,
                         size_t output_size);
bool xvm_yara_init(void);
void xvm_yara_shutdown(void);
bool xvm_yara_scan_memory(const char *rule_source,
                          const unsigned char *data,
                          size_t data_size,
                          unsigned int *match_count);

#ifdef __cplusplus
}
#endif

#endif
