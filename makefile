CC=gcc
CFLAGS=-g -lcrypto -lm
SRC= src/main.c src/file.c src/common.c src/encription.c src/decrypt.c src/itoa.c

visual_sss: $(SRC)
	$(CC) -o sss $(SRC) $(CFLAGS)
