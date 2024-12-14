#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    void hal_setup(void);
    void hal_loop(void);
    void log_number(int i);

#ifdef __cplusplus
} /* extern "C" */
#endif
