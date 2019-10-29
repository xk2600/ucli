/*
 * shell.h - private interface to the shell class
 */
#include "libucli/shell.h"
#include "libucli/pargv.h"
#include "lub/bintree.h"
#include "tinyrl/tinyrl.h"

/*-------------------------------------
 * PRIVATE TYPES 
 *------------------------------------- */
typedef enum
{
    SHELL_STATE_INITIALISING,
    SHELL_STATE_READY,
    SHELL_STATE_HELPING,
    SHELL_STATE_SCRIPT_ERROR,
    SHELL_STATE_CLOSING
} shell_state_t;

/*
 * iterate around commands
 */
typedef struct
{
    const ucli_command_t *last_cmd_local;
    const ucli_command_t *last_cmd_global;
} ucli_shell_iterator_t;

/* this is used to maintain a stack of file handles */
typedef struct ucli_shell_file_s ucli_shell_file_t;
struct ucli_shell_file_s
{
    ucli_shell_file_t *next;
    FILE               *file;
    bool_t              stop_on_error;      /* stop on error for file input  */
};

struct ucli_shell_s
{
    lub_bintree_t        view_tree;         /* Maintain a tree of views      */
    lub_bintree_t        ptype_tree;        /* Maintain a tree of ptypes     */
    const ucli_shell_hooks_t *client_hooks;/* Client callback hooks         */
    void                *client_cookie;     /* Client callback cookie        */
    ucli_view_t        *global;            /* Reference to the global view. */
    ucli_view_t        *view;              /* Reference to the current view.*/
    ucli_command_t     *startup;           /* This is the startup command   */
    ucli_shell_iterator_t iter;            /* used for iterating commands */
    shell_state_t        state;             /* The current state               */
    char                *overview;          /* Overview text for this shell.  */
    char                *viewid;            /* The current view ID string     */
    tinyrl_t            *tinyrl;            /* Tiny readline instance          */
    ucli_shell_file_t  *current_file;      /* file currently in use for input */
};

/**
 * Initialise a command iterator structure
 */
void
    ucli_shell_iterator_init(ucli_shell_iterator_t *iter);

/**
 * get the next command which is an extension of the specified line 
 */
const ucli_command_t *
    ucli_shell_find_next_completion(const ucli_shell_t    *instance,
                                     const char             *line,
                                     ucli_shell_iterator_t *iter);
/** 
 * Push the specified file handle on to the stack of file handles
 * for this shell. The specified file will become the source of 
 * commands, until it is exhausted.
 *
 * \return
 * BOOL_TRUE - the file was successfully associated with the shell.
 * BOOL_FALSE - there was insufficient resource to associate this file.
 */
bool_t 
    ucli_shell_push_file(ucli_shell_t *instance, 
                          FILE          *file,
                          bool_t         stop_on_error);
/** 
 * Pop the current file handle from the stack of file handles, shutting
 * the file down and freeing any associated memory. The next file handle
 * in the stack becomes associated with the input stream for this shell.
 * 
 * \return
 * BOOL_TRUE - the current file handle has been replaced.
 * BOOL_FALSE - there is only one handle on the stack which cannot be replaced.
 */
bool_t
    ucli_shell_pop_file(ucli_shell_t *instance);

ucli_view_t *
    ucli_shell_find_view(ucli_shell_t *instance,
                              const char    *name);
void
    ucli_shell_insert_view(ucli_shell_t *instance,
                            ucli_view_t    *view);
ucli_pargv_status_t
    ucli_shell_parse(const ucli_shell_t    *instance,
                      const char             *line,
                      const ucli_command_t **cmd,
                      ucli_pargv_t         **pargv);
char *
    ucli_shell_word_generator(ucli_shell_t *instance,
                               const char    *line,
                               unsigned       offset,
                               unsigned       state);
const ucli_command_t *
    ucli_shell_resolve_command(const ucli_shell_t *instance,
                                const char          *line);
const ucli_command_t *
    ucli_shell_resolve_prefix(const ucli_shell_t *instance,
                               const char          *line);
const ucli_command_t *
    ucli_shell_getfirst_command(ucli_shell_t *instance,
                                 const char    *line);
const ucli_command_t *
     ucli_shell_getnext_command(ucli_shell_t *instance,
                                 const char    *line);
void
     ucli_shell_insert_ptype(ucli_shell_t *instance,
                              ucli_ptype_t *ptype);
void
    ucli_shell_tinyrl_history(ucli_shell_t *instance,
                               unsigned int  *limit);
tinyrl_t *
    ucli_shell_tinyrl_new(FILE    *instream,
                           FILE    *outstream,
                           unsigned stifle);
void
    ucli_shell_tinyrl_delete(tinyrl_t *instance);

