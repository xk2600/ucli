const char *default_path = "/etc/clish;~/.clish";

typedef enum ;


typedef struct ;


typedef struct clish_shell_file_s clish_shell_file_t;
struct clish_shell_file_s ;


struct clish_shell_s ;


typedef struct _context context_t;
struct _context ;


void clish_shell_iterator_init(clish_shell_iterator_t *iter);

/* get the next command which is an extension of the specified line */
const clish_command_t *     clish_shell_find_next_completion(
                              const clish_shell_t    *instance, 
                              const char             *line, 
                              clish_shell_iterator_t *iter);

/* Push the specified file handle on to the stack of file handles for this 
 * shell. The specified file will become the source of commands, until it is 
 * exhausted.
 *
 * Returns
 *
 *      BOOL_TRUE - the file was successfully associated with the shell.
 *      BOOL_FALSE - there was insufficient resource to associate this file. */
bool_t                      clish_shell_push_file(
                              clish_shell_t          *instance, 
                              FILE                   *file, 
                              bool_t                  stop_on_error);

/* Pop the current file handle from the stack of file handles, shutting the 
 * file down and freeing any associated memory. The next file handle in the 
 * stack becomes associated with the input stream for this shell.
 * 
 * Returns
 *
 *      BOOL_TRUE - the current file handle has been replaced.
 *      BOOL_FALSE - there is only one handle on the stack which cannot be replaced. */
bool_t clish_shell_pop_file(clish_shell_t *instance);

      clish_view_t *        clish_shell_find_view(
                              clish_shell_t          *instance, 
                              const char             *name);

void                        clish_shell_insert_view(
                              clish_shell_t          *instance, 
                              clish_view_t           *view);

      clish_pargv_status_t  clish_shell_parse(
                              const clish_shell_t    *instance, 
                              const char             *line, 
                              const clish_command_t **cmd,
                              clish_pargv_t         **pargv);

char *                      clish_shell_word_generator(
                              clish_shell_t          *instance, 
                              const char             *line, 
                              unsigned int            offset, 
                              unsigned int            state);

const clish_command_t *     clish_shell_resolve_command(
                              const clish_shell_t    *instance, 
                              const char             *line);

const clish_command_t *     clish_shell_resolve_prefix(
                              const clish_shell_t    *instance,
                              const char             *line);

const clish_command_t *     clish_shell_getfirst_command(
                              clish_shell_t          *instance,
                              const char             *line);

const clish_command_t *     clish_shell_getnext_command(
                              clish_shell_t          *instance,
                              const char             *line);

void                        clish_shell_insert_ptype(
                              clish_shell_t          *instance,
                              clish_ptype_t          *ptype);

void                        clish_shell_tinyrl_history(
                              clish_shell_t          *instance,
                              unsigned int           *limit);

tinyrl_t *                  clish_shell_tinyrl_new(
                              FILE                   *instream,
                              FILE                   *outstream,
                              unsigned int            stifle);

void                        clish_shell_tinyrl_delete(
                              tinyrl_t               *instance);


void                        clish_shell_cleanup(
                              context_t              *context);





/******************************************************************************/
/** PUBLIC ATTRIBUTES *********************************************************/

/* shell__get_client_cookie.c */

void *
clish_shell__get_client_cookie(const clish_shell_t *this) ;


tinyrl_t *
clish_shell__get_tinyrl(const clish_shell_t *this) ;


const clish_view_t *
clish_shell__get_view(const clish_shell_t *this) ;


const char *
clish_shell__get_viewid(const clish_shell_t *this) ;


void
clish_shell_iterator_init(clish_shell_iterator_t *iter) ;


const clish_command_t *
clish_shell_find_next_completion(const clish_shell_t    *this,
                                 const char             *line,
                                 clish_shell_iterator_t *iter) ;


static char *
clish_shell_param_generator(const clish_shell_t   *this,
                            const clish_command_t *cmd,
                            const char            *line,
                            unsigned               offset,
                            unsigned               state) ;


static char *
clish_shell_command_generator(clish_shell_t *this,
                              const char    *line,
                              unsigned       offset,
                              unsigned       state) ;


char *
clish_shell_word_generator(clish_shell_t *this,
                           const char    *line,
                           unsigned       offset,
                           unsigned       state) ;


static clish_shell_builtin_fn_t 
    clish_close,
    clish_overview,
    clish_source,
    clish_source_nostop,
    clish_history;

static clish_shell_builtin_t clish_cmd_list[] = ;


static bool_t
clish_close(const clish_shell_t *shell,
            const lub_argv_t    *argv) ;


static bool_t
clish_source_internal(const clish_shell_t *shell,
		      const lub_argv_t    *argv,
                      bool_t              stop_on_error) ;


static bool_t
clish_source(const clish_shell_t *shell,
             const lub_argv_t    *argv) ;


static bool_t
clish_source_nostop(const clish_shell_t *shell,
             const lub_argv_t    *argv) ;


static bool_t
clish_overview(const clish_shell_t *this,
               const lub_argv_t    *argv) ;


static bool_t
clish_history(const clish_shell_t *this,
              const lub_argv_t    *argv) ;


static clish_shell_builtin_fn_t *
find_builtin_callback(const clish_shell_builtin_t *cmd_list,
                      const char                  *name) ;


void
clish_shell_cleanup_script(void *script) ;


bool_t
clish_shell_execute(clish_shell_t         *this,
                    const clish_command_t *cmd,
                    clish_pargv_t        **pargv) ;


clish_ptype_t *
clish_shell_find_create_ptype(clish_shell_t           *this,
                              const char              *name,
                              const char              *text,
                              const char              *pattern,
                              clish_ptype_method_e     method,
                              clish_ptype_preprocess_e preprocess) ;


clish_view_t *
clish_shell_find_create_view(clish_shell_t *this,
                             const char    *name,
                             const char    *prompt) ;


clish_view_t *
clish_shell_find_view(clish_shell_t *this,
                      const char    *name) ;


const clish_command_t *
clish_shell_getfirst_command(clish_shell_t *this,
                             const char    *line) ;


const clish_command_t *
clish_shell_getnext_command(clish_shell_t *this,
                            const char    *line) ;


static void
available_commands(clish_shell_t         *this,
                   const char            *line,
                   bool_t                 full) ;


void
clish_shell_help(clish_shell_t *this,
                 const char    *line) ;


void
clish_shell_insert_ptype(clish_shell_t *this,
                         clish_ptype_t *ptype) ;


void
clish_shell_insert_view(clish_shell_t *this,
                        clish_view_t  *view) ;


static void
clish_shell_init(clish_shell_t             *this,
                 const clish_shell_hooks_t *hooks,
                 void                      *cookie,
                 FILE                      *istream) ;


clish_shell_t *
clish_shell_new(const clish_shell_hooks_t *hooks,
                void                      *cookie,
                FILE                      *istream) ;


clish_pargv_status_t
clish_shell_parse(const clish_shell_t    *this,
                  const char             *line,
                  const clish_command_t **cmd,
                  clish_pargv_t         **pargv) ;


bool_t
clish_shell_pop_file(clish_shell_t *this) ;


bool_t 
clish_shell_push_file(clish_shell_t *this,
                      FILE          *file,
                      bool_t        stop_on_error) ;


const clish_command_t *
clish_shell_resolve_command(const clish_shell_t *this,
                            const char          *line) ;


const clish_command_t *
clish_shell_resolve_prefix(const clish_shell_t *this,
                           const char          *line) ;


void
clish_shell_set_context(clish_shell_t *this,
                        const char    *viewname) ;


static char *
clish_shell_tilde_expand(const char *path) ;


void 
clish_shell_load_files(clish_shell_t *this) ;


static bool_t
clish_shell_tinyrl_key_help(tinyrl_t *this,
                            int       key) ;


static char *
clish_shell_tinyrl_word_generator (tinyrl_t   *this,
                                   const char *line,
                                   unsigned    offset,
                                   unsigned    state) ;


static clish_pargv_status_t
clish_shell_tinyrl_expand(tinyrl_t *this) ;


static tinyrl_match_e
clish_shell_tinyrl_complete(tinyrl_t *this) ;


static bool_t
clish_shell_tinyrl_key_space(tinyrl_t *this,
                             int       key) ;


static bool_t
clish_shell_tinyrl_key_enter(tinyrl_t *this,
                             int       key) ;


static tinyrl_completion_func_t clish_shell_tinyrl_completion;
static char **
clish_shell_tinyrl_completion(tinyrl_t   *this,
                              const char *line,
                              unsigned    start,
                              unsigned    end) ;


static void
clish_shell_tinyrl_init(tinyrl_t *this) ;


tinyrl_t *
clish_shell_tinyrl_new(FILE    *istream,
                       FILE    *ostream,
                       unsigned stifle) ;


void
clish_shell_tinyrl_fini(tinyrl_t *this) ;


void
clish_shell_tinyrl_delete(tinyrl_t *this) ;


bool_t
clish_shell_readline(clish_shell_t          *this,
                     const char             *prompt,
                     const clish_command_t **cmd,
                     clish_pargv_t         **pargv) ;


static void *
clish_shell_thread(void *arg) ;


static context_t *
_clish_shell_spawn(const pthread_attr_t      *attr,
                   const clish_shell_hooks_t *hooks,
                   void                      *cookie,
                   FILE                      *istream) ;


static int
_clish_shell_spawn_and_wait(const clish_shell_hooks_t *hooks,
                            void                      *cookie,
                            FILE                      *file) ;


int 
clish_shell_spawn_and_wait(const clish_shell_hooks_t *hooks,
                           void                      *cookie) ;


bool_t
clish_shell_spawn(pthread_t                 *pthread,
                  const pthread_attr_t      *attr,
                  const clish_shell_hooks_t *hooks,
                  void                      *cookie) ;


bool_t
clish_shell_spawn_from_file(const clish_shell_hooks_t *hooks,
                            void                      *cookie,
                            const char                *filename) ;


bool_t
clish_shell_startup(clish_shell_t *this) ;


static void
clish_shell_cleanup(context_t *context) ;


static void
clish_shell_fini(clish_shell_t *this) ;


void
clish_shell_delete(clish_shell_t *this) ;


void
clish_shell_dump(clish_shell_t *this) ;


