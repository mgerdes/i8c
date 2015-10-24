i8c: main.c i8c.h ast.c environment.c types.c gen.c lex.yy.c parser.tab.h 
	gcc -o i8c main.c ast.c environment.c types.c gen.c lex.yy.c -lfl

parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

test: 
	./i8c tests/test.c
	gcc -m32 tests/test.s -o tests/run_tests
	./tests/run_tests

clean:
	rm -f i8c parser.tab.h lex.yy.c parser.tab.c
