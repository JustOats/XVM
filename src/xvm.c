#include "xvm/xvm.h"

const char *xvm_name(void)
{
    return "XVM";
}

xvm_version_t xvm_version(void)
{
    const xvm_version_t version = {
        .major = XVM_VERSION_MAJOR,
        .minor = XVM_VERSION_MINOR,
        .patch = XVM_VERSION_PATCH,
    };

    return version;
}

bool xvm_self_check(void)
{
    const xvm_version_t version = xvm_version();

    return xvm_name()[0] != '\0'
        && version.major == XVM_VERSION_MAJOR
        && version.minor == XVM_VERSION_MINOR
        && version.patch == XVM_VERSION_PATCH
        && xvm_crypto_init()
        && xvm_yara_init();
}
