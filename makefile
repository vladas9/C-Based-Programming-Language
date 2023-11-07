myprogram: main.c lexer.c
	gcc -o .exe/myprogram main.c lexer.c -I.
	./.exe/myprogram