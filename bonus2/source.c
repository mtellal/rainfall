
void	greetuser(int language) {

	char buffer[64];

	if (language == 1) {
		buffer[0] = 'H';
		buffer[1] = 'y';
		buffer[2] = 'v';
		buffer[3] = 'ä';
		buffer[4] = 'ä';
		strncpy(buffer + 5, "päivää ", 11);	
	}
	else if (language == 2) {
		strcnpy(buffer, "Goed", 4);	
		buffer[4] = 'e';	
		buffer[5] = 'm';	
		buffer[6] = 'i';	
		buffer[7] = 'd';	
		strcnpy(buffer + 8, "dag!", 4);	
		buffer[12] = ' ';
		buffer[13] = '\0';
	}
	else 
		strncpy(buffer, "Hello", 4);
	puts(buffer);
	return 0;
}

int main(int argc, char **argv) {

	char 	buff[40];
	char 	buff2[36];
	int	lenv;
	int	language = 0;
	int	ret;

	if (argc == 3) {
		memset(buff, 0, 19);
		strncpy(buff, argv[1], 40);
		strncpy(buff2, argv[2], 32);
		lenv = getenv("LANG");
		if (lenv != 0) {
			if (memcmp(lenv, "fi", 2) == 0)
				language = 1;
			else if (memcmp(lenv, "nl", 2) == 0)
				language = 2;
		}
		ret = greetuser(language);
	}
	else
		ret = 1;

	return ret;
}
