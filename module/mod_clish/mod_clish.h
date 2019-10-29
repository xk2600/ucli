/*
 * standalone.h
 */
#include "lub/c_decl.h"
#include "lib/shell.h"

_BEGIN_C_DECL

/* utility functions */
extern void clish_startup(int argc, const char **argv);
extern void clish_shutdown(void);


/* storage */
extern struct termios ucli_default_tty_termios;

/* clish callback functions */
extern clish_shell_access_fn_t   clish_access_callback;
extern clish_shell_script_fn_t   clish_script_callback;

_END_C_DECL
