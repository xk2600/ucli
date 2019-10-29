/* FILE shell.h
   
   PACKAGE ucli
   MODULE shell
  
   BRIEF This class represents the top level container for a CLI session.
  
   DETAIL
  
 */
#ifndef _ucli_shell_h
#define _ucli_shell_h

#include <stdio.h>
#include <pthread.h>

#include "lub/c_decl.h"
#include "lub/types.h"
#include "lub/argv.h"

#include "tinyrl/tinyrl.h"

#include "view.h"

_BEGIN_C_DECL

typedef struct ucli_shell_s ucli_shell_t;

/******************************************************************************/ 
/* SHELL PUBLIC INTERFACE -- (implemented in shell.c)
  
   REFERENCE
  
   ELEMENTS
  
   METHODS
  
   ATTRIBUTES
  
   ARGUMENTS
  
 */



    /** ATTRIBUTES ************************************************************/

    /*
       
      ucli_shell_init_fn_t --
     
            A hook function used during the spawning of a new shell.
      
            This will be invoked from the context of the spawned shell's thread
            and will be invoked just after the shell instance is created. This 
            enables the client-specific initialisation of the spawned shell's 
            thread e.g. to map the I/O streams, authenticate a user.
     
            It is possible for a client to have this invoked multiple times 
            if the user is spawning new shells using a commmand which uses the 
            "ucli_spawn" builtin function. Hence the client should remember the
            shell which first calls this function, and only assign resource (e.g. 
            setting up a script interpreter) for that call.
     
      ARGUMENTS
     
            shell          The shell instance which invoked this call
     
      RETURNS
     
            BOOL_TRUE      if everything is OK
            BOOL_FALSE     if the shell should be immediately shut down.
     
            none
     
      SIDE EFFECTS
     
            none
    */
    typedef bool_t          ucli_shell_init_fn_t(
                              const ucli_shell_t *shell);  

    /*
      ucli_shell_fini_fn_t --
     
            A hook function used during the shutting down of a spawned shell
      
            This will be invoked from the context of the spawned shell's thread
            and will be invoked just before the shell is destroyed. This enables 
            the client-specific finalisation to occur. e.g. releasing any resource
            held by the cookie, shutting down telnet connections
     
            This function may be called multiple times if a user is spawning  
            new commands (via the "ucli_spawn" builtin command), hence should use
            the reference to the root shell (remembered by the first call to 
            ucli_shell_init_fn_t callback) to signal when the cleanup should occur.
     
      ARGUMENTS
     
            shell          The shell instance which invoked this call
     
      RETURNS
     
            none
     
      SIDE EFFECTS
     
            none
    */
    typedef void            ucli_shell_fini_fn_t(
                              const ucli_shell_t      *shell);

   /* clisch_shell_cmd_line_fn_t --
     
            A hook function used to indicate a command line has been executed and 
            the shell is about to prompt for the next command.
      
            This will be invoked from the context of the spawned shell's thread
            and will be called once an ACTION has been performed.
      
            A client may use this as a periodic indicator of CLI activity, 
            e.g. to manage session timeouts. In addition any required logging of 
            commands may be performed.
     
      ARGUMENTS
     
            instance       The shell instance which invoked this call
            cmd_line       The text of the command line entered
     
      RETURNS
     
            none
     
      SIDE EFFECTS
     
            none
    */        
    typedef void            ucli_shell_cmd_line_fn_t(
                              const ucli_shell_t      *instance,
                              const char               *cmd_line);

    /*
      ucli_shell_script_fn_t --
     
            A hook function used to invoke the script associated with a command
      
            This will be invoked from the context of the spawned shell's thread
            and will be invoked with the ACTION script which is to be performed.
      
            The ucli component will only pass down a call when a command has 
            been correctly input.
     
            The client may choose to implement invocation of the script in a 
            number of ways, which may include forking a sub-process or thread. 
            It is important that the call doesn't return until the script has 
            been fully evaluated.
      
      ARGUMENTS
     
            ...tbd...
     
      RETURNS
     
            BOOL_TRUE      if the script is executed without issue
            BOOL_FALSE     if the script had an issue with execution.
     
      SIDE EFFECTS
     
            If the script executes successfully then any "view" tag associated 
            with the command will be honored. i.e. the CLI will switch to the
            new view
    */        
    typedef bool_t          ucli_shell_script_fn_t(
                              const ucli_shell_t    *instance,
                              const char             *script);

    /* 
    clang::ctypedef ucli_shell_access_fn_t {

      DESCRIPTION {
       
           "A hook function used to control access for the current user."

           "This will be invoked from the context of the spawned shell's thread
            and will be called during the parsing of the XML files.

            The ucli component will only insert a command into a view if the
            access call is sucessful.

            The client may choose to implement invocation of the script in a 
            number of ways, which may include forking a sub-process or thread. 
            It is important that the call doesn't return until the script has 
            been fully evaluated."
      }
     
      ARGUMENTS {
     
            instance      "The shell instance which invoked this call"
            access        "A textual string which describes a limitation for a
                           command. This string typically may be the name of a
                           user group, of which the current user must be a
                           member to grant access to a command."
      }
     
      RETURNS {
     
            BOOL_TRUE     "the user of the current CLISH session is permitted
                           access."
            BOOL_FALSE    "the user of the current CLISH session is not
                           permitted access."
      }
     
      SIDE_EFFECTS {
     
            If access is granted then the associated command will be inserted 
            into the appropriate view.

      }

    } AS {
            bool_t          ucli_shell_access_fn_t(
                              const ucli_shell_t *instance,
                              const char *access);
    }
    */
    typedef bool_t          ucli_shell_access_fn_t(
                              const ucli_shell_t *instance,
                              const char *access);

   /* ucli_shell_builtin_fn_t --
     
            A hook function used as a built in command callback
      
            This will be invoked from the context of the spawned shell's thread
            and will be called during the execution of a builting command.
      
            A client may register any number of these callbacks in its 
            ucli_shell_builtin_cmds_t structure.
     
      ARGUMENTS
     
            instance       The shell instance which invoked this call
            argv           A vector of textual command line arguments.
     
      RETURN
     
            BOOL_TRUE      if the command completes correctly
            BOOL_FALSE     if the command fails.
     
      SIDE EFFECTS
     
            If access is granted then the associated command will be inserted 
            into the appropriate view.
    */        
    typedef bool_t          ucli_shell_builtin_fn_t(
                              const ucli_shell_t *instance,
                              const lub_argv_t *argv);


   /* struct ucli_shell_builtin_fn_t
     
            A client of libucli may provide some builtin commands which will be
            interpreted by the framework, instead of the client's script engine.
     
      ELEMENTS
      
            name           The textual name to be used in the 'builtin' XML 
                           attribute
            callback       The function to be invoked 
    */
    typedef struct {

        const char                   *name;
        ucli_shell_builtin_fn_t     *callback;

    } ucli_shell_builtin_t;
     
   /* struct ucli_shell_hooks_t
     
           A client of libucli will provide hooks for the control of the CLI 
           within a particular system. They will populate an instance of this 
           structure and pass it into the 
     
      ELEMENTS
     
           init_fn         Initialisation call
           access_fn       Access control call
           cmd_line_fn     Command line logging call
           script_fn       script evaluation call
           fini_fn         Finalisation call
           cmd_list        NULL terminated list
    */
    typedef struct {

        ucli_shell_init_fn_t        *init_fn;
        ucli_shell_access_fn_t      *access_fn;
        ucli_shell_cmd_line_fn_t    *cmd_line_fn;
        ucli_shell_script_fn_t      *script_fn;
        ucli_shell_fini_fn_t        *fini_fn;
        const ucli_shell_builtin_t  *cmd_list;

    } ucli_shell_hooks_t;






    /** LIFECYCLE FUNCTIONS ***************************************************/
    int                      ucli_shell_spawn_and_wait(
                               const ucli_shell_hooks_t *hooks,
                               void                      *cookie);

   /* 
      ucli_shell_spawn --
            This operation causes a separate (POSIX) thread of execution to 
            be spawned. This thread becomes responsible for the CLI session.
      
            This will be invoked from the context of the spawned shell's thread
            and will be called during the execution of a builting command.
      
            A client may register any number of these callbacks in its 
            ucli_shell_builtin_cmds_t structure.
     
      ARGUMENTS
     
            pthread       A POSIX thread reference to fill out. This can be used
                          to later control the spawned thread if necessary.
            attr          A POSIX thread attribute reference which will be used
                          to define the thread which will be lanched. A value of
                          NULL will use the system default.
            hooks         A reference to the clients hooks. These are used to 
                          communicate back the client when client-specific actions
                          are required.
            cookie        A client specific reference which can be obtained during
                          a callback by invoking ucli_shell__get_client_cookie()
     
      RETURN
     
            BOOL_TRUE     if the thread was successfully spawned
            BOOL_FALSE    if the thread failed to be spawned
     
      SIDE EFFECTS
     
            none
    */
    bool_t                  ucli_shell_spawn(
                              pthread_t *pthread,
                              const pthread_attr_t      *attr,
                              const ucli_shell_hooks_t *hooks,
                              void                      *cookie);

    bool_t                  ucli_shell_spawn_from_file(
                              const ucli_shell_hooks_t *hooks,
                              void                      *cookie,
                              const                char *filename);
        
    ucli_shell_t *         ucli_shell_new(
                              const ucli_shell_hooks_t *hooks,
                              void                      *cookie,
                              FILE                      *istream);



    /** METHODS ***************************************************************/

    /* Called to invoke the startup command for this shell */
    bool_t                  ucli_shell_startup(ucli_shell_t *instance);

    void                    ucli_shell_delete(ucli_shell_t *instance);

    ucli_view_t *          ucli_shell_find_create_view(ucli_shell_t *instance,
                              const char                *name,
                              const char                *prompt);

    ucli_ptype_t *         ucli_shell_find_create_ptype(
                              ucli_shell_t             *instance,
                              const char                *name,
                              const char                *text,
                              const char                *pattern,
                              ucli_ptype_method_e       method,
                              ucli_ptype_preprocess_e   preprocess);

    int                     ucli_shell_xml_read(
                              ucli_shell_t             *instance,
                              const char                *filename);
    void                    ucli_shell_help(
                              ucli_shell_t             *instance,
                              const char                *line);
    bool_t                  ucli_shell_execute(
                              ucli_shell_t             *instance,
                              const ucli_command_t     *cmd,
                              ucli_pargv_t            **pargv);

    bool_t                  ucli_shell_readline(
                              ucli_shell_t             *shell,
                              const char                *prompt,
                              const ucli_command_t    **cmd,
                              ucli_pargv_t            **pargv);

    void                    ucli_shell_set_context(
                              ucli_shell_t             *instance,
                              const char                *viewname);

    void                    ucli_shell_dump(
                              ucli_shell_t             *instance);

    void                    ucli_shell_close(
                              ucli_shell_t             *instance);



    /** ATTRIBUTES ************************************************************/
    const ucli_view_t *    ucli_shell__get_view(
                              const ucli_shell_t       *instance);

    const char *            ucli_shell__get_viewid(
                              const ucli_shell_t       *instance);

    const char *            ucli_shell__get_overview(
                              const ucli_shell_t       *instance);

    tinyrl_t *              ucli_shell__get_tinyrl(
                              const ucli_shell_t       *instance);

    void *                  ucli_shell__get_client_cookie(
                              const ucli_shell_t       *instance);

_END_C_DECL
#endif /* _ucli_shell_h */
/** @} ucli_shell */
