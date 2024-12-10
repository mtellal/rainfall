

int main() {

	char *auth;
	char *service;

	char cmd[5];
	char buff2[2];
	char buff3[125];

	while (true) {
		printf("%p, %p \n", auth, service);
		if (fgets(cmd, sizeof(cmd), stdin) == NULL)
			return 0;
		if (!strncmp(cmd, "auth ", 5)) {
			auth = (char*)malloc(4);
			if (!auth)
				return 0;
			memset(auth, 0, 4);
			if (strlen(buff2) < 31)
				strcpy(auth, buff2);
		}
		else if (!strncmp(cmd, "reset", 5)) {
			free(auth);
			auth = NULL;	
		}
		else if (!strncmp(cmd, "service", 7)) {
			service = strdup(buff3);
			if (!service)
				return 1;
		}
		else if (!strncmp(cmd, "login", 5)) {
			if (!auth) {
				if (*(int*)(auth + 32) == 0)
					fwrite("Password:\n", 1, 10, stdout);	
				else 
					system("/bin/sh");
			}
		}
	}
}
