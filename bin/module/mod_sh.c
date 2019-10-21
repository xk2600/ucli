/* modsh.c

   Implements a POSIX (bourne) shell client for the Universal CLI (ucli) using libucli.

   Include builtins for the following commands:

     !           .           :           [           {           }
     alias       bg          bind        break       builtin     case
     cd          chdir       command     continue    do          done
     echo        elif        else        esac        eval        exec
     exit        export      false       fc          fg          fi
     for         getopts     hash        if          jobid       jobs
     kill        local       printf      pwd         read        readonly
     return      set         setvar      shift       test        then
     times       trap        true        type        ulimit      umask
     unalias     unset       until       wait        while
           
 */
