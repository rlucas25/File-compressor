# ==========================================================
# Makefile - FotoXop
# =========================================================

# 1️⃣ Variáveis (para facilitar manutenção)
CFLAGS=-O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -fsanitize=address,undefined,leak 
CLIBS=-lm
LDFLAGS = -fsanitize=address,undefined,leak # Flags usadas no linking (geram o executável)
PKG = `pkg-config --cflags --libs gtk+-3.0`
EXECUTAVEL=FotoXop.exe # nome do executável final

# ==========================================================
# 2️⃣ Alvo principal (primeiro alvo → padrão ao rodar "make")
# ==========================================================

all: $(EXECUTAVEL)

$(EXECUTAVEL): main.o huffman.o heap.o
	@echo "🔧 Ligando objetos e gerando executável..."
	@gcc main.o huffman.o heap.o $(CLIBS) $(LDFLAGS) $(PKG) -o $(EXECUTAVEL)
	@echo "✅ Compilação concluída."
	@echo "🚀 Para executar o programa digite ./$(EXECUTAVEL) no terminal."

# ==========================================================
# 3️⃣ Regras para gerar os arquivos objeto
#===========================================================

main.o: main.c
	@echo "🧩 Compilando main.c"
	@gcc $(CFLAGS) $(PKG) -c main.c

huffman.o: huffman.c
	@echo "🧩 Compilando huffman.c"
	@gcc $(CFLAGS) $(PKG) -c huffman.c

heap.o: heap.c
	@echo "🧩 Compilando heap.c"
	@gcc $(CFLAGS) $(PKG) -c heap.c


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