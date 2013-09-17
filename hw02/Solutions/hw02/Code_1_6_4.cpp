# 1 "Code_1_6_4.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "Code_1_6_4.c"

# 14 "Code_1_6_4.c"

//	enter the sample code from 1.6.4

int x = 2;

void b()
{
	x = (x+1);
	printf( "%d\n", x);
}

void c()
{
	int x = 1;
	printf( "%d\n", (x+1));
}

int main()
{
	b();
	c();
	return 0;
}
