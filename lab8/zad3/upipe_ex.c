#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int pipefd[2];
	pid_t cat_pid, sort_pid;

	if (pipe(pipefd) == -1)
	{
		perror("Pipe failed");
		exit(EXIT_FAILURE);
	}

	cat_pid = fork();

	if (cat_pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	else if (cat_pid == 0)
	{
		close(pipefd[0]);						   // Close unused read end
		dup2(pipefd[1], STDOUT_FILENO);			   // Redirect stdout to the pipe
		close(pipefd[1]);						   // Close the write end of the pipe
		execlp("cat", "cat", "/etc/passwd", NULL); // Execute 'cat /etc/passwd'
		perror("Exec failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		sort_pid = fork();
		if (sort_pid == -1)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		else if (sort_pid == 0)
		{
			// child process for sort
			close(pipefd[1]);			   // Close unused write end
			dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the pipe
			close(pipefd[0]);			   // Close the read end of the pipe
			execlp("sort", "sort", NULL);  // Execute 'sort'
			perror("Exec failed");
			exit(EXIT_FAILURE);
		}
		else
		{
			// Parent process
			close(pipefd[0]); // Close both ends of the pipe
			close(pipefd[1]);

			// Wait for both child processes to finish
			waitpid(cat_pid, NULL, 0);
			waitpid(cat_pid, NULL, 0);
		}
	}
	return 0;
}
