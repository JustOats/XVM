#include "xvm/xvm.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    const xvm_version_t version = xvm_version();
    const unsigned char sample_code[] = { 0x55, 0x48, 0x89, 0xE5, 0x5D, 0xC3 };
    const char sample_rule[] = "rule xvm_probe { strings: $name = \"XVM\" condition: $name }";
    char digest[XVM_DIGEST_HEX_LENGTH];
    char instruction[128];
    char fast_hash[XVM_XXH64_HEX_LENGTH];
    unsigned int yara_matches = 0;

    printf("%s %d.%d.%d\n",
           xvm_name(),
           version.major,
           version.minor,
           version.patch);

    if (!xvm_self_check()) {
        fputs("XVM startup check failed.\n", stderr);
        return 1;
    }

    puts("XVM startup check passed.");

    if (xvm_hash_hex((const unsigned char *)xvm_name(),
                     strlen(xvm_name()),
                     digest,
                     sizeof(digest))) {
        printf("libsodium hash probe: %s\n", digest);
    }

    if (xvm_disassemble_first_x64(sample_code,
                                  sizeof(sample_code),
                                  0x1000,
                                  instruction,
                                  sizeof(instruction))) {
        printf("zydis x64 probe: 0x1000 %s\n", instruction);
    }

    if (xvm_fast_hash64_hex((const unsigned char *)xvm_name(),
                            strlen(xvm_name()),
                            0,
                            fast_hash,
                            sizeof(fast_hash))) {
        printf("xxHash probe: %s\n", fast_hash);
    }

    if (xvm_yara_scan_memory(sample_rule,
                             (const unsigned char *)xvm_name(),
                             strlen(xvm_name()),
                             &yara_matches)) {
        printf("YARA probe: %u matching rule(s)\n", yara_matches);
    }

    xvm_yara_shutdown();

    return 0;
}
