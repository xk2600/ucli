/* FILE ptype.h
 *
 * PACKAGE ucli
 * MODULE ptype
 *
 * BRIEF This class represents a parameter type.
 *
 * DETAIL Types are a syntatical template which parameters reference.
 *
 */

#ifndef _ucli_ptype_h
#define _ucli_ptype_h

typedef struct ucli_ptype_s ucli_ptype_t;

#include "lub/types.h"
#include "lub/bintree.h"

#include <stddef.h>



/* The means by which the pattern is interpreted and validated. */
typedef enum {

    /* [default] - A POSIX regular expression. */
    CLISH_PTYPE_REGEXP,

    /* A numeric definition "min..max" signed and unsigned versions */
    CLISH_PTYPE_INTEGER,
    CLISH_PTYPE_UNSIGNEDINTEGER,

    /* A list of possible values, in the form of: 
     *     "valueOne(ONE) valueTwo(TWO) valueThree(THREE)" */
    CLISH_PTYPE_SELECT
    
} ucli_ptype_method_e;

/* This defines the pre processing which is to be
 * performed before a string is validated. */
typedef enum {

    /* [default] - do nothing */
    CLISH_PTYPE_NONE,

    /* before validation convert to UPPERCASE. */
    CLISH_PTYPE_TOUPPER,

    /* before validation convert to lowercase. */
    CLISH_PTYPE_TOLOWER,

    /* before validation convert to CamelCase. (NOT IMPLEMENTED) */
    CLISH_PTYPE_TOCAMELCASE

} ucli_ptype_preprocess_e;



/******************************************************************************/
/* PTYPE PUBLIC INTERFACE -- (implmented in ptype.h)
 *
 * REFERENCE
 *
 *        ## PTYPE REPRESENTATION ######
 *
 *        struct ucli_ptype_s { ... }
 *
 * ELEMENTS
 *
 *        .....         ....
 *
 * METHODS
 *
 *        .....         ....
 *
 * ATTRIBUTES
 *
 *        .....         ....
 *
 * ARGUMENTS
 *
 *        .....         ....
 * 
 */



int                         ucli_ptype_bt_compare(
                              const void        *clientnode,
                              const void        *clientkey);

void                        ucli_ptype_bt_getkey(
                              const void        *clientnode,
                              lub_bintree_key_t *key);

size_t                      ucli_ptype_bt_offset(void);

const char *    ucli_ptype_method__get_name(ucli_ptype_method_e method);

ucli_ptype_method_e  ucli_ptype_method_resolve(const char *method_name);

const char *    ucli_ptype_preprocess__get_name(ucli_ptype_preprocess_e preprocess);

ucli_ptype_preprocess_e 
    ucli_ptype_preprocess_resolve(const char *preprocess_name);

ucli_ptype_t *    ucli_ptype_new(const char              *name,
                    const char              *text,
                    const char              *pattern,
                    ucli_ptype_method_e     method,
                    ucli_ptype_preprocess_e preprocess);
/*-----------------
 * methods
 *----------------- */
void    ucli_ptype_delete(ucli_ptype_t *instance);
/**
 * This is the validation method for the specified type.
 * \return 
 * - NULL if the validation is negative.
 * - A pointer to a string containing the validated text. NB. this
 *   may not be identical to that passed in. e.g. it may have been
 *   a case-modified "select" or a preprocessed value.
 */
char *    ucli_ptype_validate(const ucli_ptype_t *instance,
                         const char          *text);
/**
 * This is the translation method for the specified type. The text is
 * first validated then translated into the form which should be used
 * for variable substitutions in ACTION or VIEW_ID fields.
 * \return 
 * - NULL if the validation is negative.
 * - A pointer to a string containing the translated text. NB. this
 *   may not be identical to that passed in. e.g. it may have been
 *   a translated "select" value.
 */
char *    ucli_ptype_translate(const ucli_ptype_t *instance,
                          const char          *text);
/**
 * This is used to perform parameter auto-completion
 */
char *    ucli_ptype_word_generator(ucli_ptype_t *instance,
                               const char    *text,
                               unsigned       state);
void    ucli_ptype_dump(ucli_ptype_t *instance);
/*-----------------
 * attributes 
 *----------------- */
const char *    ucli_ptype__get_name(const ucli_ptype_t *instance);
const char *    ucli_ptype__get_text(const ucli_ptype_t *instance);
const char *    ucli_ptype__get_range(const ucli_ptype_t *instance);
void    ucli_ptype__set_preprocess(ucli_ptype_t            *instance,
                                ucli_ptype_preprocess_e  preprocess);
void    ucli_ptype__set_pattern(ucli_ptype_t       *instance,
                             const char          *pattern,
                             ucli_ptype_method_e method);
void    ucli_ptype__set_text(ucli_ptype_t *instance,
                          const char    *text);
#endif /* _ucli_ptype_h */
/** @} ucli_ptype */
