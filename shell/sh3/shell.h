/* shell.h : function prototypes for all functions in shell exercise */

char ** parse(char *);		/* tokenise a string */
void free_argv(char **);	/* free argv array. */
void run_command(char **);	/* do the fork, exec stuff */
int is_builtin(char *);		/* check for in-built keywords */
int do_builtin(char **);	/* and then do them */
