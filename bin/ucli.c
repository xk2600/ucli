/* ucli --
 
   Implements the base libucli shell for the Universal CLI. (ucli)

   This binary provides the base executive to implement shell modules
   as dynamically loadable plugins to implement on top of the libucli
   framework. It's only purpose is to decide which shell dialect the
   user wishes to execute script in, load that module, and spawn a 
   libucli shell process with the callbacks bound to the selected
   modules function pointer hooks.

   Note, if no module is specified, it defaults to which ever module
   is specified in 'ucli_default_module' in 'config.h'.

 */


/* TODO:
 
   * Generalize the following from the tclish implementation to allow
     us to provide the base implementation of args handling, module
     loading, failure, etc... 

   * Move any modular bits this calls into libucli.

     The only bits that should be in module.h are the required
     structures that allow the callback system to work, as well as exposure
     of the public interfaces of libucli to the interpreter.

     Each module will need to implement:
       
     Callback hooks, to allow a shell to handle lifecycle events.

       init      initialization of the module's interpeter.
       access    handling of permissions for a command.
       cmd_line  command has completed, prompt is about to be displayed.
       script    execution of the script as result of the completion of a 
                 command line.
       fini      safe destruction of interpreter at exit.
       builtin   any number of builtin comands to be added to libucli's 
                 dialect.

     ucli configuration processing: (similar to tinyxml implem.)

       process_* : These functions configure the ucli command interface.

         ucli_module  creates global view
         startup      creates command with 'null' help called startup, dfine view
                      this command changes to, reference next view
         view         creates an additional view (if it doesn't exist)
         command      auth user via access hook, check for duplicates, create
                      new command in view, sets escape chars, sets rest of line
                      arg, opt. sets view or viewId command changes to.
         param        create a parameter type, createa param, set prefix and
                      set default value
         action       sets action on command and/or builtin on command.
         ptype        creates a parameter type
         overview     sets overview text for this view
         detail       sets detail on command


     libucli API:

       clish_shell_* :

         spawn_and_wait      spawn               spawn_from_file
         new                 startup             delete   
         find_create_view    find_create_ptype   
         help                execute             close
         readline            set_context         dump

         _get_view           _get_viewid         _get_overview   
         _get_tinyrl         _get_client_cookie


       clish_view_* :
         
         new                 delete              dump
         bt_compare          bt_getkey           bt_offset
         new_command         find_command        find_next_completion
         resolve_command     resolve_prefix

         _get_name           _set_prompt         _get_prompt

       
       clish_command_* : command / action

         new                 delete
         bt_compare          bt_getkey           bt_offset
         choose_longest      diff                help
         insert_param        find_option         next_non_option
         dump

         _get_name           _get_suffix         _get_text
         _set_detail         _get_detail         _get_param
         _set_builtin        _get_builtin        _get_param_count
         _set_escape_chars   _get_escape_chars
         _set_args           _get_args           
         _set_action         _get_action         _set_viewid
         _set_view           _get_view           _get_viewid
       

       clish_ptype_* : parameter types

         new                 delete              validate
         translate           word_generator      dump
         bt_compare          bt_getkey           bt_offset

         _get_name           _get_text           _get_range
         _set_preprocess     _set_text           _set_pattern


         method_* :

           get_name          resolve      
         

         preprocess_* :

           get_name          resolve


       clish_param_* : command parameters
         
         new                 delete              help
         validate            dump

         _get_name           _get_text           _get_range
         _get_prefix         _get_default        _get_ptype
         _set_prefix         _set_default

   * Build plugin architecture around: 
        https://hackaday.com/2018/07/12/its-all-in-the-libs-building-a-plugin-system-using-dynamic-loading/ 

 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_LIBTCL
#include <assert.h>
#include <stdlib.h>

#include <unistd.h>

#include "tcl.h"

#include "clish/private.h"


static clish_shell_hooks_t my_hooks = {
  
    tclish_init_callback,
    clish_access_callback,
    NULL, /* don't worry about cmd_line callback */
    tclish_script_callback,
    tclish_fini_callback,
    NULL /* no builtins */
};


static void
tclish_cookie_init(tclish_cookie_t *cookie,
                   const char      *argv0) {

    /* make sure that the platform specific details are set up */
    Tcl_FindExecutable(argv0);

    cookie->interp     = NULL;
}


static tclish_cookie_t *
tclish_cookie_new(const char *argv0) {

    tclish_cookie_t *cookie = (tclish_cookie_t *)malloc(sizeof(tclish_cookie_t));
    
    if(NULL != cookie) {
        tclish_cookie_init(cookie,argv0);
    }
    return cookie;
}


int 
main(int argc, const char **argv) {
    int              status = 0;
	tclish_cookie_t *cookie; 
    
    clish_startup(argc,argv);
    
    if(argc > 1) {

        int i = 1;
        while((0 == status) && argc--) {

            cookie = tclish_cookie_new(argv[0]);
            /* run the commands in the file */
            status = clish_shell_spawn_from_file(&my_hooks,cookie,argv[i++]);
        }

    } else {

        pthread_t pthread;
        void     *dummy;
        
        cookie = tclish_cookie_new(argv[0]);
        /* spawn the shell */
        status = clish_shell_spawn(&pthread,NULL,&my_hooks,cookie);

        if(-1 != status) {
            pthread_join(pthread,&dummy);
        }
    }

    if(-1 == status) {

        /* something went wrong */
        free(cookie);
    }
    
    (void)Tcl_FinalizeThread();
    clish_shutdown();

    return status;
}


#else /* not HAVE_LIBTCL */
#include <stdio.h>
int main(void) {
    printf("TCL not compiled in...\n");
}
#endif /* not HAVE_LIBTCL */
