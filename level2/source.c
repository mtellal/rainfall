

void p() {

	unsigned int 	retaddr;
	char 		buffer[76];

	fflush(stdout);
	gets(buffer);
	if ((retaddr & & 0xb0000000) == 0xb0000000) {
		printf("(%p)\n", retaddr);
		exit(1);
	}
	puts(buffer);
	stdup(buffer);
	return ;
}


int main() {
	p();
	return 0;
}
