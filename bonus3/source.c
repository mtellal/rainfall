

int main(int argc, char **argv) {

	char	buff[65];
	char	buff2[66];
	FILE	*file;
	int	ret;	

	file = fopen("/home/user/end/.pass", "r");
	memset(buff, 0, 33);
	if (file == 0 || argc != 2)
		ret = 1;
	else {
		fread(buff, 66, file);
		
	}

	return ret;	
}
