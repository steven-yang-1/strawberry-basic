flex:
	flex BASIC.l
bison:
	bison -d BASIC.y
debug:
	make flex
	make bison
	gcc -o BASIC BASIC.tab.c lex.yy.c Runtime.c Data.c HashTable.c LinkedList.c ListBuffer.c Stack.c ./itoa/itoa.c -ly -lfl -Wall -Wextra -g -D__WITH_MURMUR -L./lib/ -lqlibc -lhashtable -pthread
production:
	gcc -o BASIC BASIC.tab.c lex.yy.c Runtime.c Data.c HashTable.c LinkedList.c ListBuffer.c Stack.c ./itoa/itoa.c -ly -lfl -Wall -Wextra -D__WITH_MURMUR -L./lib/ -lqlibc -lhashtable -pthread
clean:
	rm BASIC.tab.c BASIC.tab.h BASIC lex.yy.c
