#include <threads.h>
#include <assert.h>
#include <stdbool.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>
#include <pdclib/_PDCLIB_xbox_tss.h>

int tss_create (tss_t *key, tss_dtor_t dtor)
{
    *key = TlsAlloc();
    if (*key == TLS_OUT_OF_INDEXES) {
        return thrd_error;
    }

    tss_dtors[*key] = dtor;
    return thrd_success;
}

static __stdcall void _PDCLIB_xbox_CreateThreadNotifyRoutine (PETHREAD Thread, HANDLE ThreadId, BOOLEAN Create)
{
    if (Create) return;

    for (int dtor_i = 0; dtor_i < TSS_DTOR_ITERATIONS; dtor_i++) {
        for (int i = 0; i < FLS_MAXIMUM_AVAILABLE; i++) {
            if (!tss_dtors[i]) continue;
            if (!TlsGetValue(i)) continue;

            void *val = TlsGetValue(i);
            TlsSetValue(i, NULL);
            tss_dtors[i](val);
        }

        bool done = true;

        for (int i = 0; i < FLS_MAXIMUM_AVAILABLE; i++) {
            if (tss_dtors[i] && TlsGetValue(i)) {
                done = false;
                break;
            }
        }

        if (done) break;
    }
}

__attribute__((constructor)) void _PDCLIB_register_tss_thread_notification (void)
{
    NTSTATUS status;
    status = PsSetCreateThreadNotifyRoutine(_PDCLIB_xbox_CreateThreadNotifyRoutine);
    assert(NT_SUCCESS(status));
}
