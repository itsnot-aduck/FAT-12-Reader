all:
	gcc main.c fat12-err.c fat12-parser.c fat12-fattable.c fat12-entry.c fat12-cli-table.c fat12-path.c -o main && ./main

clean:
	rm -rf ./main