
yacc -d -y 2005087.y
g++ -w -c -o y.o y.tab.c
flex 2005087.l
g++ -w -c -o l.o lex.yy.c
g++ y.o l.o -lfl -o out
./out 2005087.c
