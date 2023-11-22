/* shell.h : function prototypes for all functions in shell exercise */

#define FALSE 0
#define TRUE (!FALSE)

char ** parse(char *);		/* tokenise a string */
void free_argv(char **);	/* free argv array */
void run_command(char **);	/* do the fork, exec stuff */
int is_background(char **); 	/* check for & at end */
int redirect_in(char **);	/* check for < */
int redirect_out(char **);	/* check for > */
int is_builtin(char *);		/* check for in-built keywords */
int do_builtin(char **);	/* and then do them */
int pipe_present(char **);	/* check for | */
void pipe_command(char **);	/* deal with pipes recursively */

