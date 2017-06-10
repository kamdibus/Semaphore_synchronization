all: producer readerA readerB consumer main remove_mem

producer:
	gcc -pthread -Wall -o p producer.c -lrt

readerA:
	gcc -pthread -Wall -o ra readerA.c -lrt

readerB:
	gcc -pthread -Wall -o rb readerB.c -lrt

consumer:
	gcc -pthread -Wall -o c consumer.c -lrt

main:
	gcc -pthread -Wall -o run main.c -lrt

remove_mem:
	gcc -pthread -Wall -o rmem remove_mem.c -lrt

clean:
	rm ra rb c p run rmem
