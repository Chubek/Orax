CC = gcc
LEX = flex
YACC = bison
CFLAGS = -Wall

all: parser

parser: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -o $@ y.tab.c lex.yy.c -lfl

lex.yy.c: orax-munch.l
	$(LEX) $^

y.tab.c: orax-munch.y
	$(YACC) -d $^

clean:
	rm -f lex.yy.c y.tab.c y.tab.h parser

