#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

	int number;
	char *cmd = NULL;

	number = atoi(argv[1]);
	if (number == 0x1a7) { // 0x1a7 = 423
		cmd = strdup("/bin/sh");
		uid_t uid = geteuid();
		git_t gid = getegid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);
		execve("bin/sh", &cmd);
	}
	else
		fwrite("No !\n", 1, 5, (FILE *)2);
	return 0;
}
