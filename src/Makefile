flex:
	flex BASIC.l
bison:
	bison -d BASIC.y
compile:
	gcc -o BASIC BASIC.tab.c lex.yy.c Runtime.c Data.c HashTable.c LinkedList.c ListBuffer.c Stack.c -ly -lfl
clean:
	rm BASIC.tab.c BASIC.tab.h BASIC lex.yy.c
