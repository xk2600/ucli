/*
 * @file mod_tclsh.h --
 */

#ifndef __MOD_TCLSH_H
#define __MOD_TCLSH_H

#include "lub/c_decl.h"
#include "lub/c_dconcat.h"
#include "lub/shell.h"
#include "module/module.h"

_BEGIN_C_DECL

/* Generate fully qualified namespace */
#define TCL_FQNS(...) \
  _DCONCAT(::,__VA_ARGS__)

/* Generate ensemble command */
#define TCL_ENSEMBLE(...) \
  _DCONCAT(" ",__VA_ARGS__)

/* TCL Command Signature */
#define TclCmd(...)                  \
  static int                         \
  _DCONCAT(_,TclshCmd_,__VA_ARGS__)( \
        ClientData       dummy,      \
        Tcl_Interp *     interp,     \
        int              objc,       \
        Tcl_Obj * const  objv[] )

/* REGISTER TCL COMMAND FOR EXECUTION */
#define Tcl_Register(i,...,cd,dp)   \
  if (Tcl_CreateObjCommand( i                      /* interpreter */ \
        _DCONCAT(::,__VA_ARGS__)                       /* command */ \
        _DCONCAT(_, TclshCmd_,__VA_ARGS__),     /* C-function ptr */ \
        cd,                                         /* clientData */ \
        dp)                                     /* deleteProc ptr */ \
       != TCL_OK) {                                                  \
     /* notify interpreter of error */                               \
     Tcl_AddErrorInfo(i,                                             \
         "builtin command registration failed for "                  \
         "\"" _DCONCAT(_,TclshCmd_,__VA_ARGS__)                      \
         "\" at line " __LINE__                                      \
         " in " __FILE__ );                                          \
     return TCL_ERROR;                                               \
  }

/* USAGE:

TCL_COMMAND(ucli,ptype,create) {
   ... implementation ...
   return TCL_OK;
}

*/

/* structures */

struct Tcl_Interp;

typedef struct tclsh_cookie_s tclsh_cookie_t;
struct tclsh_cookie_s {
    struct Tcl_Interp   *interp;
};

/* tclsh callback functions */
extern ucli_shell_init_fn_t   tclsh_init_callback;
extern ucli_shell_access_fn_t tclsh_access_callback;
extern ucli_shell_script_fn_t tclsh_script_callback;
extern ucli_shell_fini_fn_t   tclsh_fini_callback;

/* hooks -- array of function pointers used as hooks for callbacks into 
   this module.
                                                                              */
static ucli_shell_hooks_t module_hooks = {
  
  /* ucli_shell_init_fn_t:      */ tclsh_init_callback,
  /* ucli_shell_access_fn_t:    */ tclsh_access_callback,
  /* ucli_shell_cmd_line_fn_t:  */ NULL,
  /* ucli_shell_script_fn_t:    */ tclsh_script_callback,
  /* ucli_shell_fini_fn_t:      */ tclsh_fini_callback,
  /* const ucli_shell_builtin_t:   builtin_callbacks, ... */

  /* NULL POINTER TO END VECTOR  */ NULL
};


/* utility functions */
extern void                 tclsh_show_result(
  struct Tcl_Interp           *interp);

static void                 tclsh_cookie_init(
  tclsh_cookie_t             *cookie,
  const char                  *argv0);

static tclsh_cookie_t *    tclsh_cookie_new(
  const char                  *argv0);






/* tcl commands */

/* NOTE: Need to figure out how to create a namespace unkown proc */

TclCmd(ucli,ptype,create);
TclCmd(ucli,command,create);
TclCmd(ucli,view,create);
TclCmd(ucli,overview);
TclCmd(ucli,startup);


/* shell spawn --
 *
 *       spawns instance of shell
 *
 * ARGUMENTS
 *
 *       wait - spawn and await input (default just proceeds)
 *       file - uses spawn_from_file to load a file into ucli.
 */

/* shell new --
 *
 *       creates instance of shell
 *
 * ARGUMENTS
 * 
 *       hooks   - structure containing pointers to callbacks
 *       cookie  - object storing session data that gets tossed
 *                 around.
 *       istream - file attached to stdin 
 * 
 * RETURNS
 *
 *       shell   - shell instance.
 */

/* shell startup --
 *
 * ARGUMENTS
 *
 *       
 */

/* shell delete */

/* shell view create */

/* shell ptype create */

/* shell help */

/* shell execute */

/* shell readline */

/* shell context set */

/* shell readline */

/* shell close */

/* shell dump */

/& shell set
static int                  cmd_shell_find(ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

_END_C_DECL

#endif
