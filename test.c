#include <stdio.h>
void main()
{
printf("%lu\n", sizeof(char));
printf("%lu\n", sizeof(short));
printf("%lu\n", sizeof(int));
printf("%lu\n", sizeof(long));
printf("%lu\n", sizeof(long long) );
printf("%lu\n", sizeof(signed));
printf("%lu\n", sizeof(unsigned));
printf("%lu\n", sizeof(float));
printf("%lu\n", sizeof(double));
printf("%lu\n", sizeof(long double));
printf("%lu\n", sizeof(const));
printf("%lu\n", sizeof(volatile));
//printf("%lu\n", sizeof(struct));
//printf("%lu\n", sizeof(union));
//printf("%lu\n", sizeof(enum));
printf("%lu\n", sizeof(unsigned int));
printf("%lu\n", sizeof(0xf));
printf("%lu\n", sizeof(0xf1));
printf("%lu\n", sizeof(0xfu));
printf("%lu\n", sizeof(0xful));
printf("%lu\n", sizeof(0123));
printf("%lu\n", sizeof(0123l));
printf("%lu\n", sizeof(0123u));
printf("%lu\n", sizeof(0123ul));
printf("%lu\n", sizeof(123));
printf("%lu\n", sizeof(123l));
printf("%lu\n", sizeof(123u));
printf("%lu\n", sizeof(123ul));
printf("%lu\n", sizeof('a'));
printf("%lu\n", sizeof('\x61'));
printf("%lu\n", sizeof('\141'));
//printf("%lu\n", sizeof('aa'));
//printf("%lu\n", sizeof('aaa'));
//printf("%lu\n", sizeof('aaaa'));
printf("%lu\n", sizeof(1.23e3f));
printf("%lu\n", sizeof(1.2e3));
printf("%lu\n", sizeof(1.2e31));
}
