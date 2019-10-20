/* tclish_module.c --
 *
 *       This module provides the necessary procs and hooks to implement tclish
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <tcl.h>
 
#include "private.h" 



/* tclish_init_callback --
 *
 *       Callback to set the initialise the terminal
 *
 * ARGUMENTS
 *
 *       shell          current instance of tclish
 *
 * RETURNS
 *
 *       Boolean evaluation result signifying if this executed without error.
 *
 * SIDE EFFECTS
 *
 *       Set raw mode (block for a single character)
 */
bool_t
tclish_init_callback(const clish_shell_t *shell) {

    int              result;
    tclish_cookie_t *this = clish_shell__get_client_cookie(shell);
    
    /* initialise the TCL interpreter */
    this->interp = Tcl_CreateInterp();
    assert(NULL != this->interp);
    /* initialise the memory */
    Tcl_InitMemory(interp);
    result = Tcl_Init(this->interp);
    if(TCL_OK != result) {
        tclish_show_result(this->interp);
    }
    Tcl_Preserve(this->interp);

    return BOOL_TRUE;
}


/* tclish_fini_callback --
 *
 *       Callback to restore the terminal settings
 *
 * ARGUMENTS
 *
 *       shell          current instance of tclish
 *
 * RETURNS
 *
 *       nothing
 *
 * SIDE EFFECTS
 *
 *       destroys TCL interpreter
 */
void
tclish_fini_callback(const clish_shell_t *shell)
{
    tclish_cookie_t *this = clish_shell__get_client_cookie(shell);
    
    /* cleanup the TCL interpreter */
    (void)Tcl_Release(this->interp);
    (void)Tcl_DeleteInterp(this->interp);

    /* free up memory */
    free(this);
}




/* tclish_script_callback --
 *
 *       Callback hook to action a TCL script; evaulates script printing 
 *
 * ARGUMENTS
 *
 *       shell          current instance of tclish
 *       script         script to evaluate
 *
 * RETURNS
 *
 *       boolean result of evaluation
 *
 * SIDE EFFECTS
 *
 *       Pretty much anything as this allows a user supplied script to evaluate
 *       modifying the interpreter.
 */
bool_t
tclish_script_callback(const clish_shell_t *shell,
                       const char          *script)
{
    bool_t           result = BOOL_TRUE;
    tclish_cookie_t *this   = clish_shell__get_client_cookie(shell);

    if (Tcl_CommandComplete(script)) {
        if(TCL_OK != Tcl_EvalEx(this->interp,
                                script,
                                strlen(script),
                                TCL_EVAL_GLOBAL)) {
            result = BOOL_FALSE;
        }
        tclish_show_result(this->interp);
    }
    return result;
}



/*
 * tclish_show_result.c
 *
 *       Display the last result for the specified interpreter.
 *
 * ARGUMENTS
 *
 *       interp         TCL interpreter for current shell
 *
 * RETURNS
 *
 *       nothing
 *
 * SIDE EFFECTS
 *
 *       none
 *
 */
void
tclish_show_result(Tcl_Interp *interp) {
    Tcl_Obj *obj = Tcl_GetObjResult(interp);
    int      length;
    if (NULL != obj) {
        char *string = Tcl_GetStringFromObj(obj, &length);
        if (NULL != string) {
            printf("%s",string);
        }
    }         
}

