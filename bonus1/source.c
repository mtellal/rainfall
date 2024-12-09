
int main(int argc, char **argv) {

	int	ret;
	char	buffer[40];
	int	n;

	num = atoi(argv[2]);
	if (num < 10) {
		memcpy(buffer, argv[3], n * 4);
		if (n == 0x574f4c46)
			execl("/bin/sh", "sh", 0);
		ret = 0;
	}	
	else
		ret = 1;
	return ret
}
