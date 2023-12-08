myprogram: 
	gcc -g main.c lexer.c parser.c interpreter.c -o myprogram -lm
	./myprogram.exe
