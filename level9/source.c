
class N {

	private:
		int 	value;
		char 	*annotation;

	public:
		N(int n): value(n), annotation("") { }
		~N();

		void setAnnotation(char *s) {
			strcpy(this->setAnnotation, s);
		}
}

int main(int argc, char **argv) {
	N* obj;
	N* obj_1;

	if (argc < 2) 
		exit(1);
	obj = new N(5);		
	obj_0 = new N(6);		
	obj->setAnnotation(argv[1]);
	void (*func)() = reinterpret_cast<void (*)()>(obj_0);
	func();
}
