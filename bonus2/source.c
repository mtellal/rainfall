/**********************/
/*	Ghidra	      */
/**********************/

void	greetuser(int language) {

	char buffer[64];

	if (language == 1) 
		strcpy(buffer, "Hyvää päivää ");	
	else if (language == 2)
		strcpy(buffer, "Goedemiddag! ");	
	else 
		strcpy(buffer, "Hello");
	// strcat(buffer, astack0x004);
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
		// operations stacks
		ret = greetuser(language);
	}
	else
		ret = 1;

	return ret;
}
