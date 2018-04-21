keyblock: main.c
	clang -Wall -o keyblock main.c deps/parson/parson.c -framework ApplicationServices
