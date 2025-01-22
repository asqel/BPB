
int var[12] = {0};
extern int x;;

int main(int argc, char **argv) {
	var[9 - argc] = (int)argv;
	return 42;
}