/* mod_tclsh.c --
 *
 *       This module provides the necessary procs and hooks to implement ucli
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <tcl.h>

#include "mod_tclsh.h"
#endif



/* 

ARG:

         shell          current instance of ucli 
         script         script to evaluate
         interp         TCL interpreter for current shell
         clientData     1-word representing internal representation
         objc           count of arguments submitted during execution of TclCmd
         objv           vector of arguments submitted during execution of TclCmd

 */



/* ucli_init_callback -- Callback to set the initialise the terminal

RET:  Boolean evaluation result signifying if this executed without error.
EFX:  Set raw mode (block for a single character)
                                                                              */
bool_t
tclsh_init_callback(const ucli_shell_t *shell) {

    int              result;
    ucli_cookie_t    *this = ucli_shell__get_client_cookie(shell);
    
    /* initialise the TCL interpreter */
    this->interp = Tcl_CreateInterp();
    assert(NULL != this->interp);
    /* initialise the memory */
    Tcl_InitMemory(interp);
    result = Tcl_Init(this->interp);
    if(TCL_OK != result) {
        ucli_show_result(this->interp);
    }
    Tcl_Preserve(this->interp);

    return BOOL_TRUE;
}


/* tclsh_fini_callback -- Callback to restore the terminal settings
 
RET:  nothing  
EFX:  Destroys TCL interpreter
                                                                               */
void
tclsh_fini_callback(const ucli_shell_t *shell)
{
    ucli_cookie_t *this = ucli_shell__get_client_cookie(shell);
    
    /* cleanup the TCL interpreter */
    (void)Tcl_Release(this->interp);
    (void)Tcl_DeleteInterp(this->interp);

    /* free up memory */
    free(this);
}




/* tclsh_script_callback -- Callback hook to action a TCL script; evaulates 
   script printing.
 
RET:  Boolean result of evaluation
EFX:  Pretty much anything as this allows a user supplied script to evaluate
      modifying the interpreter.
                                                                              */
bool_t
tclsh_script_callback(const ucli_shell_t *shell,
                      const char          *script)
{
    bool_t           result = BOOL_TRUE;
    ucli_cookie_t *this   = ucli_shell__get_client_cookie(shell);

    if (Tcl_CommandComplete(script)) {
        if(TCL_OK != Tcl_EvalEx(this->interp,
                                script,
                                strlen(script),
                                TCL_EVAL_GLOBAL)) {
            result = BOOL_FALSE;
        }
        ucli_show_result(this->interp);
    }
    return result;
}



/* tclsh_show_result -- Display the last result for the specified interpreter.

RET:  nothing
EFX:  none
                                                                              */
void
tclsh_show_result(Tcl_Interp *interp) {
    Tcl_Obj *obj = Tcl_GetObjResult(interp);
    int      length;
    if (NULL != obj) {
        char *string = Tcl_GetStringFromObj(obj, &length);
        if (NULL != string) {
            printf("%s",string);
        }
    }         
}

static void
tclsh_cookie_init(tclish_cookie_t *cookie,
                   const char      *argv0) {

    /* make sure that the platform specific details are set up */
    Tcl_FindExecutable(argv0);

    cookie->interp     = NULL;
}


static tclsh_cookie_t *
tclsh_cookie_new(const char *argv0) {

    tclish_cookie_t *cookie = (ucli_cookie_t *)malloc(sizeof(tclish_cookie_t));
    
    if(NULL != cookie) {
        tclish_cookie_init(cookie,argv0);
    }
    return cookie;
}





/******************************************************************************/
/** TCL COMMANDS **************************************************************/


TclCmd(ucli,ptype,create) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);


  return TCL_OK;
}


/* */
TclCmd(ucli,spawn) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliNew(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliStartup(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliDelete(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliViewCreate(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliPtypeCreate(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliHelp(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliExecute(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliReadline(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliContextSet(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}

static int
TclCmd_ucliClose(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr)

  return TCL_OK;
}

static int
TclCmd_ucliDump(
        ClientData        clientData,
        Tcl_Interp *      interp,
        int               objc,
        Tcl_Obj * const   objv[]) {

  objResultPtr = TclDuplicateObj(objResultPtr);
  Tcl_SetObjResult(interp, objResultPtr);

  return TCL_OK;
}




/* utility function to add commands notated in this source file */
int
tclEmbedCommands(Tcl_Interp *interp) {
  

  if (Tcl_CreateObjCommand(
        interp, 
        "hello",
        cmd_Hello, 
        NULL,
        NULL
     ) != TCL_OK) {
       return TCL_ERROR;
     }
     
  return TCL_OK;
  
}

