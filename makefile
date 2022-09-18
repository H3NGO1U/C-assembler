Main: main.o first_transition.o helping.o code_table.o defFuncs.o hash.o code_into_table.o text_func.o macro.o second_transition.o 
	gcc -ansi -Wall -pedantic main.o first_transition.o helping.o code_table.o defFuncs.o hash.o code_into_table.o text_func.o macro.o second_transition.o -o Main -lm
main.o: main.c first_transition.h helping.h macro.h
	gcc -c -ansi -Wall -pedantic main.c
first_transition.o: first_transition.c first_transition.h helping.h code_table.h defFuncs.h second_transition.h
	gcc -c -ansi -Wall -pedantic first_transition.c
helping.o: helping.c helping.h
	gcc -c -ansi -Wall -pedantic helping.c
defFuncs.o: defFuncs.c defFuncs.h helping.h hash.h code_into_table.h
	gcc -c -ansi -Wall -pedantic defFuncs.c
code_table.o: code_table.c code_table.h text_func.h
	gcc -c -ansi -Wall -pedantic code_table.c
text_func.o: text_func.c text_func.h
	gcc -c -ansi -Wall -pedantic text_func.c -lm
hash.o: hash.c hash.h
	gcc -c -ansi -Wall -pedantic hash.c
code_into_table.o: code_into_table.c code_into_table.h helping.h hash.h code_table.h
	gcc -c -ansi -Wall -pedantic code_into_table.c
second_transition.o: second_transition.c second_transition.h helping.h code_table.h defFuncs.h
	gcc -c -ansi -Wall -pedantic second_transition.c
macro.o: macro.c macro.h hash.h defFuncs.h
	gcc -c -ansi -Wall -pedantic macro.c
clean: 
	rm *.o
