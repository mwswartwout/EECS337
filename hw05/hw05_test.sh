#
#	hw05_test.sh
#
make clean
make
#
#	test the NFA grammar
#
./hw05testNFA +yydebug test1.c
./hw05testNFA test2.c
./hw05testNFA test3.c
#
#	test the DFA grammar
#
./hw05testDFA +yydebug test1.c
./hw05testDFA test2.c
./hw05testDFA test3.c
