#include "xvm/xvm.h"

#include <Zydis/Zydis.h>

#include <stdio.h>

bool xvm_disassemble_first_x64(const unsigned char *code,
                               size_t code_size,
                               uint64_t runtime_address,
                               char *output,
                               size_t output_size)
{
    ZydisDisassembledInstruction instruction;
    int written = 0;

    if (output == NULL || output_size == 0) {
        return false;
    }

    output[0] = '\0';

    if (code == NULL || code_size == 0) {
        return false;
    }

    if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64,
                                            runtime_address,
                                            code,
                                            code_size,
                                            &instruction))) {
        return false;
    }

    written = snprintf(output, output_size, "%s", instruction.text);

    return written >= 0 && (size_t)written < output_size;
}
