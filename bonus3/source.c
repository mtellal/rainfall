

int main(int argc, char **argv) {

	char	buff[65];
	char	buff2[66];
	FILE	*file;
	int	ret;	
	int	value;

	file = fopen("/home/user/end/.pass", "r");
	memset(buff, 0, 65);
	if (file == 0 || argc != 2)
		return 1;
	else {
		fread(buff2, 66, file);

		value = atoi(argv[1]);	
		buff[value] = 0;		

		fread(buff, 65, file);		
		fclose(file);

		if (!strcmp(buffer, argv[1])) 
			execl("/bin/bash", "sh", 0);
		else
			puts(buff2);

		return 0;
	}
	return 1;	
}
