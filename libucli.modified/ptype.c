/* ptype.c --
 *
 *
 */
#include "libucli/pargv.h"
#include "lub/argv.h"
#include "lub/bintree.h"
#include "lub/string.h"
#include "lub/ctype.h"
#include "lub/dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/types.h>
#include <regex.h>


/** PRIVATE STRUCTURES ********************************************************/

typedef struct ucli_ptype_integer_s ucli_ptype_integer_t;
struct ucli_ptype_integer_s {

    int min;
    int max;
};

typedef struct ucli_ptype_select_s ucli_ptype_select_t;
struct ucli_ptype_select_s {

    lub_argv_t *items;
};

struct ucli_ptype_s {

    lub_bintree_node_t         bt_node;
    char                      *name;
    char                      *text;
    /* TODO: replace pattern and range with 'constraint' */
    char                    *pattern;
    char                    *range;
    /* ************************************************* */
    ucli_ptype_method_e       method;
    ucli_ptype_preprocess_e   preprocess;
    unsigned                   last_name; /* index used for auto-completion */
    union {    

        regex_t               regexp;
        ucli_ptype_integer_t integer;
        ucli_ptype_select_t  select;
    } u;
};


/** PRIVATE METHODS ***********************************************************/

static char *
ucli_ptype_select__get_name(const ucli_ptype_t *this,
                             unsigned             index) {

    char       *result = NULL;
    const char *arg    = lub_argv__get_arg(this->u.select.items,index);
    if(NULL != arg) {    

        size_t      name_len = 0;
        const char *lbrk     = strchr(arg,'(');
        if(NULL != lbrk) {        

            name_len = (size_t)( lbrk - arg);
        }
        assert(name_len < strlen(arg)); /* check for syntax error */
        result = lub_string_dupn(arg,name_len);
    }
    return result;
}

static char *
ucli_ptype_select__get_value(const ucli_ptype_t *this,
                              unsigned             index) {

    char       *result = NULL;
    const char *arg    = lub_argv__get_arg(this->u.select.items,index);
    if(NULL != arg) {    

        const char *lbrk      = strchr(arg,'(');
        const char *rbrk      = strchr(arg,')');
        const char *value     = NULL;
        size_t      value_len = 0;
        if(lbrk) {        

            value = lbrk +1;
            if(rbrk) {            

                value_len = (size_t)(rbrk - value );
            }
        }
        assert(value_len < strlen(arg)); /* check for syntax error */
        result = lub_string_dupn(value,value_len);
    }
    return result;
}

static void
ucli_ptype__set_range(ucli_ptype_t *this) {

    char tmp[80];
    
    /* now set up the range values */
    switch(this->method) {    

        
        case CLISH_PTYPE_REGEXP: {        

            /* nothing more to do  */
            break;
        }
        
        case CLISH_PTYPE_INTEGER: {        

            /* Setup the integer range */
            sprintf(tmp,
                    "%d..%d",
                    this->u.integer.min,
                    this->u.integer.max);
            this->range = lub_string_dup(tmp);
            break;
        }
        
        case CLISH_PTYPE_UNSIGNEDINTEGER: {        

            /* Setup the unsigned integer range */
            sprintf(tmp,
                    "%u..%u",
                    (unsigned int)this->u.integer.min,
                    (unsigned int)this->u.integer.max);
            this->range = lub_string_dup(tmp);
            break;
        }
        
        case CLISH_PTYPE_SELECT: {        

            /* Setup the selection values to the help text */
            unsigned    i;
            
            for(i=0;
                i < lub_argv__get_count(this->u.select.items);
                i++) {            

                char *p    = tmp;
                char *name = ucli_ptype_select__get_name(this,i);
                
                if(i > 0) {                

                    p += sprintf(p,"/");
                }
                p += sprintf(p,"%s",name);
                lub_string_cat(&this->range,tmp);
                lub_string_free(name);
            }
            break;
        }
    }
}



/** PUBLIC META FUNCTIONS *****************************************************/

int
ucli_ptype_bt_compare(const void *clientnode,
                       const void *clientkey) {

        const ucli_ptype_t *this = clientnode;
        const char          *key  = clientkey;

        return strcmp(this->name,key);
}

void
ucli_ptype_bt_getkey(const void         *clientnode,
                      lub_bintree_key_t *key) {

        const ucli_ptype_t *this = clientnode;

        /* fill out the opaque key */
    strcpy((char *)key,this->name);
}

size_t
ucli_ptype_bt_offset(void) {

      return offsetof(ucli_ptype_t,bt_node);
}

static const char *method_names[] =  {

    "regexp",
    "integer",
    "unsignedInteger",
    "select"
};

const char *
ucli_ptype_method__get_name(ucli_ptype_method_e method) {

    return method_names[method];
}

ucli_ptype_method_e
ucli_ptype_method_resolve(const char *name) {

    ucli_ptype_method_e result = CLISH_PTYPE_REGEXP;
    if(NULL != name) {    

        unsigned i;
        for(i  = 0;
            i < CLISH_PTYPE_SELECT + 1;
            i++) {        

            if(0 == strcmp(name,method_names[i])) {            

                result = (ucli_ptype_method_e)i;
                break;
            }
        }
        /* error for incorrect type spec */
        assert(i <= CLISH_PTYPE_SELECT);
    }
    return result;
}

static const char *preprocess_names[] =  {

    "none",
    "toupper",
    "tolower"
};

const char *
ucli_ptype_preprocess__get_name(ucli_ptype_preprocess_e preprocess) {

    return preprocess_names[preprocess];
}

ucli_ptype_preprocess_e
ucli_ptype_preprocess_resolve(const char *name) {

    ucli_ptype_preprocess_e result = CLISH_PTYPE_NONE;
    if(NULL != name) {    

        unsigned i;
        for(i  = 0;
            i < CLISH_PTYPE_TOLOWER + 1;
            i++) {        

            if(0 == strcmp(name,preprocess_names[i])) {            

                result = (ucli_ptype_preprocess_e)i;
                break;
            }
        }
        /* error for incorrect type spec */
        assert((ucli_ptype_preprocess_e)i <= CLISH_PTYPE_TOLOWER);
    }
    return result;
}



/** PUBLIC METHODS ************************************************************/

char *
ucli_ptype_word_generator(ucli_ptype_t *this,
                           const char    *text,
                           unsigned       state) {

    char                  *result = NULL;
    if (0 == state) {    

        /* first of all simply try to validate the result */
        result = ucli_ptype_validate(this,text);
    }
    if(NULL == result) {    

        switch(this->method) {        

            case CLISH_PTYPE_SELECT: {            

                if(0 == state) {                

                    this->last_name = 0;
                }
                while((result = ucli_ptype_select__get_name(this,this->last_name++))) {                

                    /* get the next item and check if it is a completion */
                    if(result == lub_string_nocasestr(result,text)) {                    

                        /* found the next completion */
                        break; 
                    }
                    lub_string_free(result);
                }
                break;
            }
            
            case CLISH_PTYPE_INTEGER:
            case CLISH_PTYPE_UNSIGNEDINTEGER:
            case CLISH_PTYPE_REGEXP: {            

                /* do nothing */
                break;
            }
        }
    }
    return result;
}

static char *
ucli_ptype_validate_or_translate(const ucli_ptype_t *this,
                                  const char          *text,
                                  bool_t               translate) {

    char *result = lub_string_dup(text);
    assert(this->pattern);    
    
    switch(this->preprocess) {    

        case CLISH_PTYPE_NONE: {        

            break;
        }
        
        case CLISH_PTYPE_TOUPPER: {        

            unsigned char *p = result;
            while(*p) {            

                /*lint -e155 Ignoring { }'ed sequence within an expression, 0 assumed 
                 * MACRO implementation uses braces to prevent multiple increments
                 * when called.
                 */
                *p = lub_ctype_toupper(*p);
                p++;
            }
            break;
        }
        
        case CLISH_PTYPE_TOLOWER: {        

            unsigned char *p = result;
            while(*p) {            

                *p = lub_ctype_tolower(*p);
                p++;
            }
            break;
        }
    }

    /* now validate according the specified method */
    switch(this->method) {    

        case CLISH_PTYPE_REGEXP: {        

            /* test the regular expression against the string */
             /*lint -e64 Type mismatch (arg. no. 4) */
            /*
             * lint seems to equate regmatch_t[] as being of type regmatch_t !!!
             */
            if(0 != regexec(&this->u.regexp,result,0,NULL,0)) {            

                lub_string_free(result);
                result = NULL;
            }
            /*lint +e64 */
            break;
        }
        
        case CLISH_PTYPE_INTEGER: {        

            /* first of all check that this is a number */

            bool_t      ok = BOOL_TRUE;
            const char *p  = result;

            if (*p == '-') {            

                p++;
            }
            /* loop until \0 (aka NULL) is found. */
            while (*p) {            

                if (!lub_ctype_isdigit(*p++)) {                

                    ok = BOOL_FALSE;
                    break;
                }
            }
            if(BOOL_TRUE == ok) {            

                /* convert and check the range */
                int value = atoi(result); // cstring to integer.
                if(    (value < this->u.integer.min)
                    || (value > this->u.integer.max) ) {                    

                        lub_string_free(result);
                        result = NULL;
                    }
            }
            else {            

                lub_string_free(result);
                result = NULL;
            }
            break;
        }
        
        case CLISH_PTYPE_UNSIGNEDINTEGER: {        

            /* first of all check that this is a number */
            bool_t      ok = BOOL_TRUE;
            const char *p  = result;
            if (*p == '-') {            

                /* unsigned integers don't have signs */
                ok = BOOL_FALSE;
                break;
            }
            /* loop until \0 (aka NULL) is found. */
            while (*p) {            

                if (!lub_ctype_isdigit(*p++)) {                

                    ok = BOOL_FALSE;
                    break;
                }
            }
            if(BOOL_TRUE == ok) {            

                /* convert and check the range */
                unsigned int value = (unsigned int)atoi(result); // cstring to integer.
                if(    (value < (unsigned)this->u.integer.min)
                    || (value > (unsigned)this->u.integer.max) ) {                    

                        lub_string_free(result);
                        result = NULL;
                    }
            }
            else {            

                lub_string_free(result);
                result = NULL;
            }
            break;
        }
        
        case CLISH_PTYPE_SELECT: {        

            unsigned i;
            for(i = 0;
                i < lub_argv__get_count(this->u.select.items);
                i++) {            

                unsigned char *name  = ucli_ptype_select__get_name(this,i);
                unsigned char *value = ucli_ptype_select__get_value(this,i);
                int   tmp  = lub_string_nocasecmp(result,name);
                lub_string_free((BOOL_TRUE == translate) ? name : value);
                if(0 == tmp) {                

                    lub_string_free(result);
                    result = ((BOOL_TRUE == translate) ? value : name);
                    break;
                }
                else {                

                    lub_string_free((BOOL_TRUE == translate) ? value : name);
                }
            }
            if(i == lub_argv__get_count(this->u.select.items)) {            

                /* failed to find a match */
                lub_string_free(result);
                result=NULL;
            }
            break;
        }
    }
    return (char*)result;
}

static void
ucli_ptype_init(ucli_ptype_t           *this,
                 const char              *name,
                 const char              *text,
                 const char              *pattern,
                 ucli_ptype_method_e     method,
                 ucli_ptype_preprocess_e preprocess) {

    assert(name);
    this->name       = lub_string_dup(name);
    this->text       = NULL;
    this->pattern    = NULL;
    this->preprocess = preprocess;
    this->range      = NULL;

    /* Be a good binary tree citizen */
    lub_bintree_node_init(&this->bt_node);

    if(NULL != pattern) {    

        /* set the pattern for this type */
        ucli_ptype__set_pattern(this,pattern,method);
    }
    if(NULL != text) {    

        /* set the help text for this type */
        ucli_ptype__set_text(this,text);
    }
}

char *
ucli_ptype_validate(const ucli_ptype_t *this,
                     const char          *text) {

    return ucli_ptype_validate_or_translate(this,text,BOOL_FALSE);
}

char *
ucli_ptype_translate(const ucli_ptype_t *this,
                      const char          *text) {

    return ucli_ptype_validate_or_translate(this,text,BOOL_TRUE);
}

ucli_ptype_t *
ucli_ptype_new(const char              *name,
                const char              *help,
                const char              *pattern,
                ucli_ptype_method_e     method,
                ucli_ptype_preprocess_e preprocess) {

    ucli_ptype_t *this = malloc(sizeof(ucli_ptype_t));

    if(NULL != this) {    

        ucli_ptype_init(this,name,help,pattern,method,preprocess);
    }
    return this;
}

static void
ucli_ptype_fini(ucli_ptype_t *this) {

    lub_string_free(this->name);
    this->name = NULL;
    lub_string_free(this->text);
    this->text = NULL;
    lub_string_free(this->pattern);
    this->pattern = NULL;
    lub_string_free(this->range);
    this->range = NULL;

    switch(this->method) {    

        case CLISH_PTYPE_REGEXP: {        

            regfree(&this->u.regexp);
            break;
        }
        
        case CLISH_PTYPE_INTEGER:
        case CLISH_PTYPE_UNSIGNEDINTEGER: {        

            break;
        }
        
        case CLISH_PTYPE_SELECT: {        

            lub_argv_delete(this->u.select.items);
            break;
        }
    }
}

void
ucli_ptype_delete(ucli_ptype_t *this) {

    ucli_ptype_fini(this);
    free(this);
}

const char *
ucli_ptype__get_name(const ucli_ptype_t *this) {

    return (const char*)this->name; 
}

const char *
ucli_ptype__get_text(const ucli_ptype_t *this) {

    return (const char *)this->text; 
}

void
ucli_ptype__set_pattern(ucli_ptype_t       *this,
                         const char          *pattern,
                         ucli_ptype_method_e method) {

    assert(NULL == this->pattern);
    this->method = method;
    
    switch(this->method) {    

        case CLISH_PTYPE_REGEXP: {        

            int result;
    
            /* only the expression is allowed */
            lub_string_cat(&this->pattern,"^");
            lub_string_cat(&this->pattern,pattern);
            lub_string_cat(&this->pattern,"$");

            /* compile the regular expression for later use */
            result = regcomp(&this->u.regexp,this->pattern,REG_NOSUB | REG_EXTENDED);
            assert(0 == result);
            break;
        }
        
        case CLISH_PTYPE_INTEGER: {        

            /* default the range to that of an integer */
            this->u.integer.min = INT_MIN;
            this->u.integer.max = INT_MAX;
            this->pattern = lub_string_dup(pattern);
            /* now try and read the specified range */
            sscanf(this->pattern,
                   "%d..%d",
                   &this->u.integer.min,
                   &this->u.integer.max);
            break;
        }
        
        case CLISH_PTYPE_UNSIGNEDINTEGER: {        

            /* default the range to that of an unsigned integer */
            this->u.integer.min = 0;
            this->u.integer.max = (int)UINT_MAX;
            this->pattern = lub_string_dup(pattern);
            /* now try and read the specified range */
            sscanf(this->pattern,
                   "%u..%u",
                   (unsigned int *)&this->u.integer.min,
                   (unsigned int *)&this->u.integer.max);
            break;
        }
        
        case CLISH_PTYPE_SELECT: {        

            this->pattern = lub_string_dup(pattern);
            /* store a vector of item descriptors */
            this->u.select.items = lub_argv_new(this->pattern,0);
            break;
        }
    }
    /* now set up the range details */
    ucli_ptype__set_range(this);
}

void
ucli_ptype__set_text(ucli_ptype_t *this,
                      const char    *text) {

    assert(NULL == this->text);
    this->text = lub_string_dup(text);

}

void
ucli_ptype__set_preprocess(ucli_ptype_t            *this,
                             ucli_ptype_preprocess_e preprocess) {

    this->preprocess = preprocess;
}

const char *
ucli_ptype__get_range(const ucli_ptype_t *this) {

    return (const char*)this->range;
}



/** DEBUGGING *****************************************************************/

void
ucli_ptype_dump(ucli_ptype_t *this) {

    lub_dump_printf("ptype(%p)\n",this);
    lub_dump_indent();
    lub_dump_printf("name       : %s\n",
                    ucli_ptype__get_name(this));
    lub_dump_printf("text       : %s\n",
                    ucli_ptype__get_text(this));
    lub_dump_printf("pattern    : %s\n",
                    this->pattern            );
    lub_dump_printf("method     : %s\n",
                    ucli_ptype_method__get_name(this->method));
    lub_dump_printf("postprocess: %s\n",
                    ucli_ptype_preprocess__get_name(this->preprocess));
    lub_dump_undent();
}
