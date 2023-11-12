myprogram: main.c lexer.c
	gcc -g main.c lexer.c parser.c interpreter.c -o .exe/myprogram
	./.exe/myprogram.exe