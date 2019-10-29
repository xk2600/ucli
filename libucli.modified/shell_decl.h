const char *default_path = "/etc/ucli;~/.ucli";

typedef enum ;


typedef struct ;


typedef struct ucli_shell_file_s ucli_shell_file_t;
struct ucli_shell_file_s ;


struct ucli_shell_s ;


typedef struct _context context_t;
struct _context ;


void ucli_shell_iterator_init(ucli_shell_iterator_t *iter);

/* get the next command which is an extension of the specified line */
const ucli_command_t *     ucli_shell_find_next_completion(
                              const ucli_shell_t    *instance, 
                              const char             *line, 
                              ucli_shell_iterator_t *iter);

/* Push the specified file handle on to the stack of file handles for this 
 * shell. The specified file will become the source of commands, until it is 
 * exhausted.
 *
 * Returns
 *
 *      BOOL_TRUE - the file was successfully associated with the shell.
 *      BOOL_FALSE - there was insufficient resource to associate this file. */
bool_t                      ucli_shell_push_file(
                              ucli_shell_t          *instance, 
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
bool_t ucli_shell_pop_file(ucli_shell_t *instance);

      ucli_view_t *        ucli_shell_find_view(
                              ucli_shell_t          *instance, 
                              const char             *name);

void                        ucli_shell_insert_view(
                              ucli_shell_t          *instance, 
                              ucli_view_t           *view);

      ucli_pargv_status_t  ucli_shell_parse(
                              const ucli_shell_t    *instance, 
                              const char             *line, 
                              const ucli_command_t **cmd,
                              ucli_pargv_t         **pargv);

char *                      ucli_shell_word_generator(
                              ucli_shell_t          *instance, 
                              const char             *line, 
                              unsigned int            offset, 
                              unsigned int            state);

const ucli_command_t *     ucli_shell_resolve_command(
                              const ucli_shell_t    *instance, 
                              const char             *line);

const ucli_command_t *     ucli_shell_resolve_prefix(
                              const ucli_shell_t    *instance,
                              const char             *line);

const ucli_command_t *     ucli_shell_getfirst_command(
                              ucli_shell_t          *instance,
                              const char             *line);

const ucli_command_t *     ucli_shell_getnext_command(
                              ucli_shell_t          *instance,
                              const char             *line);

void                        ucli_shell_insert_ptype(
                              ucli_shell_t          *instance,
                              ucli_ptype_t          *ptype);

void                        ucli_shell_tinyrl_history(
                              ucli_shell_t          *instance,
                              unsigned int           *limit);

tinyrl_t *                  ucli_shell_tinyrl_new(
                              FILE                   *instream,
                              FILE                   *outstream,
                              unsigned int            stifle);

void                        ucli_shell_tinyrl_delete(
                              tinyrl_t               *instance);


void                        ucli_shell_cleanup(
                              context_t              *context);





/******************************************************************************/
/** PUBLIC ATTRIBUTES *********************************************************/

/* shell__get_client_cookie.c */

void *
ucli_shell__get_client_cookie(const ucli_shell_t *this) ;


tinyrl_t *
ucli_shell__get_tinyrl(const ucli_shell_t *this) ;


const ucli_view_t *
ucli_shell__get_view(const ucli_shell_t *this) ;


const char *
ucli_shell__get_viewid(const ucli_shell_t *this) ;


void
ucli_shell_iterator_init(ucli_shell_iterator_t *iter) ;


const ucli_command_t *
ucli_shell_find_next_completion(const ucli_shell_t    *this,
                                 const char             *line,
                                 ucli_shell_iterator_t *iter) ;


static char *
ucli_shell_param_generator(const ucli_shell_t   *this,
                            const ucli_command_t *cmd,
                            const char            *line,
                            unsigned               offset,
                            unsigned               state) ;


static char *
ucli_shell_command_generator(ucli_shell_t *this,
                              const char    *line,
                              unsigned       offset,
                              unsigned       state) ;


char *
ucli_shell_word_generator(ucli_shell_t *this,
                           const char    *line,
                           unsigned       offset,
                           unsigned       state) ;


static ucli_shell_builtin_fn_t 
    ucli_close,
    ucli_overview,
    ucli_source,
    ucli_source_nostop,
    ucli_history;

static ucli_shell_builtin_t ucli_cmd_list[] = ;


static bool_t
ucli_close(const ucli_shell_t *shell,
            const lub_argv_t    *argv) ;


static bool_t
ucli_source_internal(const ucli_shell_t *shell,
		      const lub_argv_t    *argv,
                      bool_t              stop_on_error) ;


static bool_t
ucli_source(const ucli_shell_t *shell,
             const lub_argv_t    *argv) ;


static bool_t
ucli_source_nostop(const ucli_shell_t *shell,
             const lub_argv_t    *argv) ;


static bool_t
ucli_overview(const ucli_shell_t *this,
               const lub_argv_t    *argv) ;


static bool_t
ucli_history(const ucli_shell_t *this,
              const lub_argv_t    *argv) ;


static ucli_shell_builtin_fn_t *
find_builtin_callback(const ucli_shell_builtin_t *cmd_list,
                      const char                  *name) ;


void
ucli_shell_cleanup_script(void *script) ;


bool_t
ucli_shell_execute(ucli_shell_t         *this,
                    const ucli_command_t *cmd,
                    ucli_pargv_t        **pargv) ;


ucli_ptype_t *
ucli_shell_find_create_ptype(ucli_shell_t           *this,
                              const char              *name,
                              const char              *text,
                              const char              *pattern,
                              ucli_ptype_method_e     method,
                              ucli_ptype_preprocess_e preprocess) ;


ucli_view_t *
ucli_shell_find_create_view(ucli_shell_t *this,
                             const char    *name,
                             const char    *prompt) ;


ucli_view_t *
ucli_shell_find_view(ucli_shell_t *this,
                      const char    *name) ;


const ucli_command_t *
ucli_shell_getfirst_command(ucli_shell_t *this,
                             const char    *line) ;


const ucli_command_t *
ucli_shell_getnext_command(ucli_shell_t *this,
                            const char    *line) ;


static void
available_commands(ucli_shell_t         *this,
                   const char            *line,
                   bool_t                 full) ;


void
ucli_shell_help(ucli_shell_t *this,
                 const char    *line) ;


void
ucli_shell_insert_ptype(ucli_shell_t *this,
                         ucli_ptype_t *ptype) ;


void
ucli_shell_insert_view(ucli_shell_t *this,
                        ucli_view_t  *view) ;


static void
ucli_shell_init(ucli_shell_t             *this,
                 const ucli_shell_hooks_t *hooks,
                 void                      *cookie,
                 FILE                      *istream) ;


ucli_shell_t *
ucli_shell_new(const ucli_shell_hooks_t *hooks,
                void                      *cookie,
                FILE                      *istream) ;


ucli_pargv_status_t
ucli_shell_parse(const ucli_shell_t    *this,
                  const char             *line,
                  const ucli_command_t **cmd,
                  ucli_pargv_t         **pargv) ;


bool_t
ucli_shell_pop_file(ucli_shell_t *this) ;


bool_t 
ucli_shell_push_file(ucli_shell_t *this,
                      FILE          *file,
                      bool_t        stop_on_error) ;


const ucli_command_t *
ucli_shell_resolve_command(const ucli_shell_t *this,
                            const char          *line) ;


const ucli_command_t *
ucli_shell_resolve_prefix(const ucli_shell_t *this,
                           const char          *line) ;


void
ucli_shell_set_context(ucli_shell_t *this,
                        const char    *viewname) ;


static char *
ucli_shell_tilde_expand(const char *path) ;


void 
ucli_shell_load_files(ucli_shell_t *this) ;


static bool_t
ucli_shell_tinyrl_key_help(tinyrl_t *this,
                            int       key) ;


static char *
ucli_shell_tinyrl_word_generator (tinyrl_t   *this,
                                   const char *line,
                                   unsigned    offset,
                                   unsigned    state) ;


static ucli_pargv_status_t
ucli_shell_tinyrl_expand(tinyrl_t *this) ;


static tinyrl_match_e
ucli_shell_tinyrl_complete(tinyrl_t *this) ;


static bool_t
ucli_shell_tinyrl_key_space(tinyrl_t *this,
                             int       key) ;


static bool_t
ucli_shell_tinyrl_key_enter(tinyrl_t *this,
                             int       key) ;


static tinyrl_completion_func_t ucli_shell_tinyrl_completion;
static char **
ucli_shell_tinyrl_completion(tinyrl_t   *this,
                              const char *line,
                              unsigned    start,
                              unsigned    end) ;


static void
ucli_shell_tinyrl_init(tinyrl_t *this) ;


tinyrl_t *
ucli_shell_tinyrl_new(FILE    *istream,
                       FILE    *ostream,
                       unsigned stifle) ;


void
ucli_shell_tinyrl_fini(tinyrl_t *this) ;


void
ucli_shell_tinyrl_delete(tinyrl_t *this) ;


bool_t
ucli_shell_readline(ucli_shell_t          *this,
                     const char             *prompt,
                     const ucli_command_t **cmd,
                     ucli_pargv_t         **pargv) ;


static void *
ucli_shell_thread(void *arg) ;


static context_t *
_ucli_shell_spawn(const pthread_attr_t      *attr,
                   const ucli_shell_hooks_t *hooks,
                   void                      *cookie,
                   FILE                      *istream) ;


static int
_ucli_shell_spawn_and_wait(const ucli_shell_hooks_t *hooks,
                            void                      *cookie,
                            FILE                      *file) ;


int 
ucli_shell_spawn_and_wait(const ucli_shell_hooks_t *hooks,
                           void                      *cookie) ;


bool_t
ucli_shell_spawn(pthread_t                 *pthread,
                  const pthread_attr_t      *attr,
                  const ucli_shell_hooks_t *hooks,
                  void                      *cookie) ;


bool_t
ucli_shell_spawn_from_file(const ucli_shell_hooks_t *hooks,
                            void                      *cookie,
                            const char                *filename) ;


bool_t
ucli_shell_startup(ucli_shell_t *this) ;


static void
ucli_shell_cleanup(context_t *context) ;


static void
ucli_shell_fini(ucli_shell_t *this) ;


void
ucli_shell_delete(ucli_shell_t *this) ;


void
ucli_shell_dump(ucli_shell_t *this) ;


