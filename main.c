#include "libft/includes/libft.h"

int	main(void)
{
	int father;
	int  tabl[2];

	char		*opt1[2];
	char		*opt2[3];

	opt1[0] = "ls";
	opt1[1] = NULL;
	opt2[0] = "cat";
	opt2[1] = "-e";
	opt2[2] = NULL;
	pipe(tabl);
	father = fork();
	if (father == 0)
	{
		dup2(tabl[1], STDOUT_FILENO);
		close(tabl[0]);	
		execve("/bin/ls", opt1, NULL);
	}
	dup2(tabl[0], STDIN_FILENO);
	close(tabl[1]);
	wait(0);
	execve("/bin/cat", opt2, NULL);
	return (0);
}