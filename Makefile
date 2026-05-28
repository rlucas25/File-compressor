CFLAGS=-O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -fsanitize=address,undefined,leak 
CLIBS=-lm
LDFLAGS = -fsanitize=address,undefined,leak # Flags usadas no linking (geram o executável)
PKG = `pkg-config --cflags --libs gtk+-3.0`
EXECUTAVEL=Huffman.exe # nome do executável final

all: $(EXECUTAVEL)

$(EXECUTAVEL): main.o Huffman.o Heap.o
	@gcc main.o Huffman.o Heap.o $(CLIBS) $(LDFLAGS) $(PKG) -o $(EXECUTAVEL)
	@echo "Compilação concluída com sucesso!"

main.o: main.c
	@gcc $(CFLAGS) $(PKG) -c main.c


Huffman.o: Huffman.c
	@gcc $(CFLAGS) $(PKG) -c Huffman.c


Heap.o: Heap.c
	@gcc $(CFLAGS) $(PKG) -c Heap.c

.PHONY: clean run

clean: 
	@rm -rf *.o $(EXECUTAVEL)
	@echo "Diretório limpo!"
	
run: $(EXECUTAVEL)
	@echo "Executando o programa"
	@ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=gtk.supp:print_suppressions=0 ./$(EXECUTAVEL)
# 	@LSAN_OPTIONS=suppressions=gtk.supp ./$(EXECUTAVEL)