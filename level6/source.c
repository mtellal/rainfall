
void	n() {
	system("/bin/cat /home/user/level7/.pass");
	return ;
}

void	m() {
	puts("Nope");
	return ;
}

int main() {

	char *buffer;
	int *p;

	buffer = (char*)malloc(64);
	p = (int*)malloc(4);
	*p = m;
	strcpy(buffer, argv[1]);
	((void (*)(void))*p)();
	return 0;
}
