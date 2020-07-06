#include <threads.h>
#include <assert.h>
#include <stdlib.h>
#include <windows.h>

struct thread_info_t {
    thrd_start_t func;
    void *arg;
};

static __stdcall DWORD thread_wrapper (LPVOID lpThreadParameter)
{
    debugPrint("%s: %d\n", __FILE__, __LINE__);
    struct thread_info_t *thread_info = (struct thread_info_t *)lpThreadParameter;

    thrd_start_t func = thread_info->func;
    void *arg = thread_info->arg;

    free(lpThreadParameter);

    return (DWORD)func(arg);
}

int thrd_create (thrd_t *thr, thrd_start_t func, void *arg)
{
    if (thr == NULL) {
        return thrd_error;
    }

    struct thread_info_t *thread_info = malloc(sizeof(struct thread_info_t));
    if (!thread_info) {
        return thrd_nomem;
    }

    thread_info->func = func;
    thread_info->arg = arg;

    *thr = CreateThread(NULL, 0, thread_wrapper, (void *)thread_info, 0, NULL);
    if (!*thr) {
        free(thread_info);
        return thrd_error;
    }

    return thrd_success;
}
