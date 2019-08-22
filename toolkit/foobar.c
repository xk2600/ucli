/*
 * PSEUDO-CODE to outline how non-UNIX based authentication could be done
 */
#define MAX_LOGIN_ATTEMPTS 3
#define LOGIN_DELAY        10
/*--------------------------------------------------------- */
bool_t
foobar_access_callback(const clish_shell_t *shell,
                      const char          *access)
{
    foobar_account_t *account = clish_shell__get_cookie(shell);
    
    /* check whether the current account has the necessary permissions */
    return foobar_validate_access(account,access);
}
/*--------------------------------------------------------- */
bool_t
foobar_init_callback(const clish_shell_t *shell)
{
    bool_t    result = BOOL_TRUE;
    tinyrl_t *tinyrl = NULL;
    int       last_state;

    /* create a tinyrl instance */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&last_state);
    tinyrl = tinyrl_new(stdin,stdout,0,NULL);

    /* limit login line length */
    tinyrl_limit_line_length(tinyrl, CLI_MAX_LINE_LENGTH);
    pthread_cleanup_push(login_cleanup,(void*)tinyrl);
    pthread_setcancelstate(last_state,&last_state);

    tinyrl_crlf(tinyrl);
    
    if (tinyrl) 
    {
        foobar_account_t *account = foobar_login(tinyrl);

        if(NULL != account)
        {
            /* store this information for later use */
            clish_shell__set_cookie(shell,account);
        
            result = BOOL_TRUE;
        }
    }
    /* always clean up the Tiny RL using for login */
    pthread_cleanup_pop(1); 
    
    return result;
}
/*--------------------------------------------------------- */
static foobar_account_t *
foobar_login(tinyrl_t  *tinyrl)
{
    unsigned int attempts=0;
    char *username, *password;
    foobar_account_t *result = NULL;
    
    tinyrl_crlf(tinyrl);
    
    while (attempts < MAX_LOGIN_ATTEMPTS) 
    {
        /* get the username from the terminal */
        username = tinyrl_readline(tinyrl,"login: ",NULL);
        if(!username )
        {
            /* end of input stream */
            break;
        }
        if(!username[0])
        {
            /* only prompt for password once a user has been entered */
            lub_string_free(username);
            continue;
        }
        /* switch off the echoing and replace with '' character */
        tinyrl_disable_echo(tinyrl,'\0');
        
        /* get the password from the terminal */
        password = tinyrl_readline(tinyrl,"password: ",NULL);
        tinyrl_crlf(tinyrl);

        /* re-enable the echoing */
        tinyrl_enable_echo(tinyrl);
        
        /* get my system to validate this combination */
        result = foobar_validate(username,password);
       
        /* now free resources */
        lub_string_free(username);
        lub_string_free(password);

        if(NULL == result)
        {
            /* all done */
            break;
        }
        else
        {
            attempts++;
        }    
        // don't make life too easy
        sleep(LOGIN_DELAY);
        
        // opportunity to canccel
        pthread_testcancel();
        
    }  /* end while */

    return result;
}
/*--------------------------------------------------------- */
