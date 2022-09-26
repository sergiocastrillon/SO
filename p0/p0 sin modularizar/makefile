all: clean executable execute  #valgrind # Modificadores para el comando make
# valgrind ejecuta el programa por si mismo, si eliminas la orden, recuerda
# añadir "execute" para que el programa se ejecute tras compilarlo

executable: p0.o lista.o # Crea el ejecutable
	gcc -Wall -o exec p0.o lista.o

p0.o: # Solo crea el archivo de salida de p0.c
	gcc -c p0.c 

lista.o: # """" de lista.c
	gcc -c lista.c

execute: # Permite correr el ejecutable, solo si antes existe (make executable execute)
	./exec

valgrind: # Generación automática de un informe de valgrind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./exec
	# valgrind --show-reachable=yes --leak-check=full --log-file=valgrind-report.txt ./exec

clean: # Borra todos los .o, .txt y el ejecutable del programa
	rm -f *.o *.txt p0
    # CUIDADO: NO dejar ningún .txt en la misma carpeta, será borrado al ejecutar el make
    # Para evitar esto cambiar el asterico por el nombre del log de valgrind
    # En Linux el editor de textos guarda por defecto en plain text.
