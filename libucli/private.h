/*
 * standalone.h
 */
#include "lub/c_decl.h"
#include "shell.h"

_BEGIN_C_DECL

struct Tcl_Interp;

typedef struct ucli_cookie_s ucli_cookie_t;
struct ucli_cookie_s
{
    struct Tcl_Interp   *interp;
};

/* utility functions */
extern void
    ucli_startup(int argc, const char **argv);
extern void
    ucli_shutdown(void);
extern void
    ucli_show_result(struct Tcl_Interp *interp);

/* storage */
extern struct termios ucli_default_tty_termios;

/* ucli callback functions */
extern ucli_shell_access_fn_t   ucli_access_callback;
extern ucli_shell_script_fn_t   ucli_script_callback;

/* ucli callback functions */
extern ucli_shell_init_fn_t   ucli_init_callback;
extern ucli_shell_access_fn_t ucli_access_callback;
extern ucli_shell_script_fn_t ucli_script_callback;
extern ucli_shell_fini_fn_t   ucli_fini_callback;
 
_END_C_DECL
