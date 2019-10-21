/* clish_module.c --
 * 
 *       This module provides the supporting hooks for the clish executable.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "private.h"


#include <assert.h>
#include <string.h>

#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef DMALLOC
#  include <dmalloc.h>
#endif /* DMALLOC */

#define MAX_GROUPS 10


/* usage --
 *
 *      Prints clish usage help text.
 *
 * ARGUMENTS
 *
 *      filename  name of executable file ($0 or argv0)
 *
 * EFFECTS
 *
 *      none
 *
 * RETURNS
 *      
 *      nothing
 */
static void
usage(const char *filename)
{
    printf("%s [-help] [scriptname]\n",filename);
    printf("  -help      : display this usage\n");
    printf("  scriptname : run the commands in the specified file\n");
    printf("\n");
    printf("VERSION %s\n\n",PACKAGE_VERSION);
    printf("ENVIRONMENT\n");
    printf("  CLISH_PATH : Set to a semicolon separated list of directories\n");
    printf("               which should be searched for XML definition files.\n");
    printf("               Current Value: '%s'\n",getenv("CLISH_PATH"));
    printf("               If undefined then '/etc/clish;~/.clish' will be used.\n");
}


/* clish_startup --
 *
 *       Processes arguments and run startup hook, printing usage if -help is
 *       provided as an argument
 * 
 * ARGUMENTS
 *
 *       argc     argument count
 *       argv     array of char arrarys containing arguments
 *
 * EFFECTS
 *
 *       none.
 *
 * RETURNS
 *
 *       nothing.
 */
void 
clish_startup(int argc, const char **argv) {

# ifdef DMALLOC
  /* Get environ variable DMALLOC_OPTIONS and pass the settings string
   * on to dmalloc_debug_setup to setup the dmalloc debugging flags. */
  dmalloc_debug_setup(getenv("DMALLOC_OPTIONS"));
# endif

    if(argc > 1) {

        const char *help_switch = "-help";
        if(strstr(help_switch,argv[1]) == help_switch) {

            usage(argv[0]);
            exit(1);
        }
    }
    /* TODO: VERIFY AN ARGUMENT WAS SUBMITTED OR CLISH_PATH ENVIRONMENT VARIABLE 
     *       EXISTS. */

    if(argc == 0) {

    }
}




/* clish_shutdown.c 
 *
 *       Cleanup as we shutdown clish.
 *
 * ARGUMENTS
 *
 *       none
 *
 * EFFECTS
 *
 *       none
 *
 * RETURNS
 *
 *       nothing
 */
void 
clish_shutdown(void) {

# ifdef DMALLOC
  dmalloc_shutdown();
# endif

}



/* clish_access_callback --
 *
 *       Callback hook to check whether the current user is a member of the 
 *       specified group (access string)
 *
 * ARGUMENTS
 *
 *       shell      current shell
 *       access     group/access string
 *
 * EFFECTS
 *
 *       none
 *
 * RETURNS
 *
 *       nothing
 */
bool_t
clish_access_callback(const clish_shell_t *shell,
                      const char          *access)
{
	bool_t allowed = BOOL_FALSE;

    assert(access);
    /* There is an access restricion on this element */
	int   num_groups;
	gid_t group_list[MAX_GROUPS];
	int   i;
		
	/* assume the worst */
	allowed = BOOL_FALSE;
		
	/* get the groups for the current user */
	num_groups = getgroups(MAX_GROUPS,group_list);
	assert(num_groups != -1);

	/* now check these against the access provided */
	for(i = 0;
	    i < num_groups;
	    i++)
	{
		struct group *ptr = getgrgid(group_list[i]);
		if(0 == strcmp(ptr->gr_name,access))
        {
			/* The current user is permitted to use this command*/
			allowed = BOOL_TRUE;
			break;
		}
	}
	return allowed;
}



/* clish_script_callback.c --
 *
 *       Callback hook to action a shell script.
 */
bool_t
clish_script_callback(const clish_shell_t *shell,
                      const char          *script)
{
#ifdef DEBUG
    printf("CLI-EXEC: %s\n",script);
#endif /* DEBUG */

    return (0 == system(script)) ? BOOL_TRUE : BOOL_FALSE;
}


