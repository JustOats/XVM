#include "xvm/xvm.h"

#include <limits.h>
#include <yara.h>

typedef struct xvm_yara_scan_state {
    unsigned int match_count;
} xvm_yara_scan_state_t;

static bool xvm_yara_initialized = false;

static int xvm_yara_scan_callback(YR_SCAN_CONTEXT *context,
                                  int message,
                                  void *message_data,
                                  void *user_data)
{
    xvm_yara_scan_state_t *state = (xvm_yara_scan_state_t *)user_data;

    (void)context;
    (void)message_data;

    if (message == CALLBACK_MSG_RULE_MATCHING && state != NULL) {
        if (state->match_count < UINT_MAX) {
            ++state->match_count;
        }
    }

    return CALLBACK_CONTINUE;
}

bool xvm_yara_init(void)
{
    if (xvm_yara_initialized) {
        return true;
    }

    if (yr_initialize() != ERROR_SUCCESS) {
        return false;
    }

    xvm_yara_initialized = true;
    return true;
}

void xvm_yara_shutdown(void)
{
    if (xvm_yara_initialized) {
        yr_finalize();
        xvm_yara_initialized = false;
    }
}

bool xvm_yara_scan_memory(const char *rule_source,
                          const unsigned char *data,
                          size_t data_size,
                          unsigned int *match_count)
{
    static const unsigned char empty_input = 0;
    YR_COMPILER *compiler = NULL;
    YR_RULES *rules = NULL;
    xvm_yara_scan_state_t state = { 0 };
    bool ok = false;

    if (match_count == NULL) {
        return false;
    }

    *match_count = 0;

    if (rule_source == NULL || (data == NULL && data_size != 0)) {
        return false;
    }

    if (data == NULL) {
        data = &empty_input;
    }

    if (!xvm_yara_init()) {
        return false;
    }

    if (yr_compiler_create(&compiler) != ERROR_SUCCESS) {
        return false;
    }

    yr_compiler_set_include_callback(compiler, NULL, NULL, NULL);

    if (yr_compiler_add_string(compiler, rule_source, NULL) != 0) {
        goto cleanup;
    }

    if (yr_compiler_get_rules(compiler, &rules) != ERROR_SUCCESS) {
        goto cleanup;
    }

    if (yr_rules_scan_mem(rules,
                          data,
                          data_size,
                          SCAN_FLAGS_REPORT_RULES_MATCHING,
                          xvm_yara_scan_callback,
                          &state,
                          0) != ERROR_SUCCESS) {
        goto cleanup;
    }

    *match_count = state.match_count;
    ok = true;

cleanup:
    if (rules != NULL) {
        yr_rules_destroy(rules);
    }

    if (compiler != NULL) {
        yr_compiler_destroy(compiler);
    }

    return ok;
}
