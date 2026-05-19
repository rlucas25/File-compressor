# ==========================================================
# Makefile - Huffman
# =========================================================

# 1️⃣ Variáveis (para facilitar manutenção)
CFLAGS=-O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -fsanitize=address,undefined,leak 
CLIBS=-lm
LDFLAGS = -fsanitize=address,undefined,leak # Flags usadas no linking (geram o executável)
PKG = `pkg-config --cflags --libs gtk+-3.0`
EXECUTAVEL=Huffman.exe # nome do executável final

# ==========================================================
# 2️⃣ Alvo principal (primeiro alvo → padrão ao rodar "make")
# ==========================================================

all: $(EXECUTAVEL)

$(EXECUTAVEL): main.o Huffman.o Heap.o
	@echo "🔧 Ligando objetos e gerando executável..."
	@gcc main.o Huffman.o Heap.o $(CLIBS) $(LDFLAGS) $(PKG) -o $(EXECUTAVEL)
	@echo "✅ Compilação concluída."

# ==========================================================
# 3️⃣ Regras para gerar os arquivos objeto
#===========================================================

main.o: main.c
	@echo "🧩 Compilando main.c"
	@gcc $(CFLAGS) $(PKG) -c main.c


Huffman.o: Huffman.c
	@echo "🧩 Compilando Huffman.c"
	@gcc $(CFLAGS) $(PKG) -c Huffman.c


Heap.o: Heap.c
	@echo "🧩 Compilando Heap.c"
	@gcc $(CFLAGS) $(PKG) -c Heap.c


# ==========================================================
# 4️⃣ Alvos auxiliares (não geram arquivos → PHONY)
# ==========================================================

.PHONY: clean run

# Limpa os arquivos temporários
clean: 
	@echo "🧹 Limpando arquivos compilados..."
	@rm -rf *.o $(EXECUTAVEL)
	@echo "🧼 Diretório limpo!"
	
# Executa o programa (compila antes se necessário)
run: $(EXECUTAVEL)
	@echo "🚀 Executando o programa"
	@ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=gtk.supp:print_suppressions=0 ./$(EXECUTAVEL)
# 	@LSAN_OPTIONS=suppressions=gtk.supp ./$(EXECUTAVEL)