
void	 p(char *buffer, char *string) {

	char *v;
	char input[4104];

	puts(string);
	read(0, input, 0x4096);
	v = strchr(input, 10);
	*v = '\0';
	strncpy(bufffer, input, 20);
	return ;
}

void	pp(char *buffer) {

	char 	buff_1[20];
	char 	buff_2[20];
	int	lbuff1;

	p(buff_1, " -"); 
	p(buff_2, " -"); 
	strcpy(buffer, buff_1);
	lbuff1 = strlen(buff_1);	
	buffer[lbuff1] = ' ';
	buffer[lbuff1 + 1] = '\0';
	strcat(buffer, buff_2);
	return ;
}

int main() {
	char buffer[54];
	pp(buffer);
	puts(buffer);
	return 0;
}
