all:
	gcc main.c fat12-parser.c fat12-fattable.c fat12-entry.c -o main

clean:
	rm -rf ./main