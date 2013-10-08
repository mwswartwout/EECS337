#
#	book.sh
#
lex first.l
yacc second.y
cc y.tab.c -ly -ll
./a.out < math0.txt
