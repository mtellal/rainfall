
void m() {
	time_t tvar;

	tvar = time(0);
	printf("%s - %d\n", c, tvar);
	return ;
}

int main() {

	char *buff1;
	char *buff2;
	char *buff3;
	FILE *stream;

	buff1 = (char*)malloc(8);
	*buff1 = 1;

	buff2 = (char*)malloc(8);
	buff1[1] = buff2;

	buff3 = (char*)malloc(8);
	*buff3 = 2;

	buff2 = malloc(8);
	buff3[1] = buff2;
	strcpy((char *)buff1[1], argv[1]);
	strcpy((char *)buff3[1], argv[2]);
	stream = fopen("/home/user/level8/.pass");
	fgets(c, 0x44, strean);
	puts("~~");
	return 0;
}
