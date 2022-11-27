all: clean executable valgrind #execute  # Modificadores para el comando make
# valgrind ejecuta el programa por si mismo, si eliminas la orden, recuerda
# añadir "execute" para que el programa se ejecute tras compilarlo

executable: p3.o lista.o funciones.o # Crea el ejecutable
	gcc -Wall -o p3 p3.o lista.o listam.o funciones.o funciones1.o funciones2.c

p0.o: # Solo crea el archivo de salida de p3.c
	gcc -c p3.c
	
funciones.o:
	 gcc -c funciones.c funciones1.c funciones2.c

lista.o: # """" de lista.c
	gcc -c lista.c listam.c

execute: # Permite correr el ejecutable, solo si antes existe (make executable execute)
	./p3

valgrind: # Generación automática de un informe de valgrind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p3
	# valgrind --show-reachable=yes --leak-check=full --log-file=valgrind-report.txt ./exec

clean: # Borra todos los .o, .txt y el ejecutable del programa
	rm -f *.o valgrind-report.txt p3 # *.txt
    # CUIDADO: NO dejar ningún .txt en la misma carpeta, será borrado al ejecutar el make
    # Para evitar esto cambiar el asterico por el nombre del log de valgrind
    # En Linux el editor de textos guarda por defecto en plain text.
