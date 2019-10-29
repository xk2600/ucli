/*
 * module.h
 */
 
#include "../libucli/lub/c_decl.h"
#include "../libucli/shell.h"

//_BEGIN_C_DECL

#define MODULE_PATH  "./module/mod_"
#define MODULE_EXT   ".plugin"
#define MODULE_HOOKS "module_hooks"

const char modulePath[]           = MODULE_PATH;
const char moduleExt[]            = MODULE_EXT;
const char moduleHookStructName[] = MODULE_HOOKS;

/* utility functions */
struct ucli_shell_hooks_t module_hooks;

/* storage */
extern struct termios ucli_default_tty_termios;




/* module_init interface */

int loadShellModule(const char* shellName) {

  char * dlFilePath;

  /* build filePath */
  if(
    (dlFilePath = 
      malloc(strlen(modulePath + shellName + moduleExt + 1))
    ) != NULL) {
      dlFilePath[0] = '\0';   // ensures the memory is an empty string
      strcat(dlFilePath,modulePath);
      strcat(dlFilePath,shellName);
      strcat(dlFilePath,moduleExt);
  } else {
      fprintf(STDERR,"malloc failed!\n");
      exit
  } 

  /* attempt to open "./module/mod_<shellName>.plugin" */
  void *handle = dlopen(dlFilePath, RTLD_NOW);

  if (handle) {
    moduleHooks = dlsym(handle, moduleHooksStructName);
    if (moduleHooks) ->init_fn();
  }
  if (dlerror() != NULL) {
    dlclose(handle);
    return 2;
  }
 
  printf("succcessfully loaded %s module.\n\n", shellName);
 
  dlclose(handle);
  return 0;

}

//_END_C_DECL
