# 1 "Code_1_6_4.c"
# 1 "<command-line>"
# 1 "Code_1_6_4.c"
# 16 "Code_1_6_4.c"
int x = 2;
void b() { x = (x+1); printf("%d\n", x); }
void c() { int x = 1; printf("%d\n", (x+1)); }
void main() { b(); c(); }
