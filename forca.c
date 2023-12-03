#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "forca.h"

char palavrasecreta[TAMANHO_PALAVRA];
char chutes[26];
int chutesdados = 0;
int maxChutes = 0;

typedef struct {
    char nome[50];
    int pontuacao;
} Jogador;

void adicionaaoRanking(char nome[], int pontuacao) {
    FILE* ranking = fopen("ranking.txt", "a");

    if (ranking == NULL) {
        printf("Erro ao abrir o arquivo de ranking.\n");
        exit(1);
    }

    fprintf(ranking, "%s %d\n", nome, pontuacao);

    fclose(ranking);
}

void mostraRanking() {
    FILE* ranking = fopen("ranking.txt", "r");

    if (ranking == NULL) {
        printf("Ainda não há nenhum ranking disponível.\n");
        return;
    }

    printf("\nRanking:\n");

    Jogador jogador;

    while (fscanf(ranking, "%s %d", jogador.nome, &jogador.pontuacao) != EOF) {
        printf("%s - Pontuação: %d\n", jogador.nome, jogador.pontuacao);
    }

    fclose(ranking);
}

int letraexiste(char letra) {

	for(int j = 0; j < strlen(palavrasecreta); j++) {
		if(letra == palavrasecreta[j]) {
			return 1;
		}
	}

	return 0;
}

int chuteserrados() {
	int erros = 0;

	for(int i = 0; i < chutesdados; i++) {
		
		if(!letraexiste(chutes[i])) {
			erros++;
		}
	}

	return erros;
}

int enforcou() {
	return chuteserrados() >= 5;
}

int ganhou() {
	for(int i = 0; i < strlen(palavrasecreta); i++) {
		if(!jachutou(palavrasecreta[i])) {
			return 0;
		}
	}

	return 1;
}


void abertura() {
	printf("/****************/\n");
	printf("/ Jogo de Forca */\n");
	printf("/****************/\n\n");
}

int main() {
    abertura();

    char nome[50];
    printf("Digite seu nome: ");
    scanf("%s", nome);

void escolheniveldificuldade() {
    printf("Escolha o nível de dificuldade:\n");
    printf("1 - Fácil\n");
    printf("2 - Médio\n");
    printf("3 - Difícil\n");

    int escolha;
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            maxChutes = 15; 
            break;
        case 2:
            maxChutes = 10;
            break;
        case 3:
            maxChutes = 5; 
            break;
        default:
            printf("Escolha inválida. Configurando para nível médio.\n");
            maxChutes = 10;
    }
}

void chuta() {
    char chute;
    printf("Qual letra? ");
    scanf(" %c", &chute);

    if (chute < 'A' || chute > 'Z') {
        printf("Por favor, digite uma letra maiúscula válida.\n\n");
        return;
    }

    if (letraexiste(chute)) {
        printf("Você acertou: a palavra tem a letra %c\n\n", chute);
    } else {
        printf("\nVocê errou: a palavra NÃO tem a letra %c\n\n", chute);
    }

    chutes[chutesdados] = chute;
    chutesdados++;
}


int jachutou(char letra) {
	int achou = 0;
	for(int j = 0; j < chutesdados; j++) {
		if(chutes[j] == letra) {
			achou = 1;
			break;
		}
	}

	return achou;
}

void desenhaforca() {

	int erros = chuteserrados();

	printf("  _______       \n");
	printf(" |/      |      \n");
	printf(" |      %c%c%c  \n", (erros>=1?'(':' '), (erros>=1?'_':' '), (erros>=1?')':' '));
	printf(" |      %c%c%c  \n", (erros>=3?'\\':' '), (erros>=2?'|':' '), (erros>=3?'/': ' '));
	printf(" |       %c     \n", (erros>=2?'|':' '));
	printf(" |      %c %c   \n", (erros>=4?'/':' '), (erros>=4?'\\':' '));
	printf(" |              \n");
	printf("_|___           \n");
	printf("\n\n");

	for(int i = 0; i < strlen(palavrasecreta); i++) {

		if(jachutou(palavrasecreta[i])) {
			printf("%c ", palavrasecreta[i]);
		} else {
			printf("_ ");
		}

	}
	printf("\n");

}

int main() {
    abertura();
    escolheniveldificuldade();
}

void escolhepalavra() {
	FILE* f;

	f = fopen("palavras.txt", "r");
	if(f == 0) {
		printf("Banco de dados de palavras não disponível\n\n");
		exit(1);
	}

	int qtddepalavras;
	fscanf(f, "%d", &qtddepalavras);

	srand(time(0));
	int randomico = rand() % qtddepalavras;

	for(int i = 0; i <= randomico; i++) {
		fscanf(f, "%s", palavrasecreta);
	}

	fclose(f);
}


void adicionapalavra() {
    if (ganhou()) {
        char quer;

        printf("Você deseja adicionar uma nova palavra no jogo (S/N)? ");
        scanf(" %c", &quer);

        if (quer == 'S' || quer == 's') {
            char novapalavra[TAMANHO_PALAVRA];

            printf("Digite a nova palavra, em letras maiúsculas: ");
            scanf("%s", novapalavra);

            FILE* f;
            f = fopen("palavras.txt", "r+");

            if (f == NULL) {
                printf("Banco de dados de palavras não disponível\n\n");
                exit(1);
            }

            int qtd;
            fscanf(f, "%d", &qtd);

            for (int i = 0; i < qtd; i++) {
                char palavraExistente[TAMANHO_PALAVRA];
                fscanf(f, "%s", palavraExistente);

                if (strcmp(novapalavra, palavraExistente) == 0) {
                    printf("Esta palavra já existe. Tente outra.\n");
                    fclose(f);
                    return;
                }
            }

            qtd++;
            fseek(f, 0, SEEK_SET);
            fprintf(f, "%d", qtd);

            fseek(f, 0, SEEK_END);
            fprintf(f, "\n%s", novapalavra);

            fclose(f);
        }
    }
}


int main() {

	abertura();
	escolhepalavra();

	do {

		desenhaforca();
		chuta();

	} while (!ganhou() && !enforcou());

	if(ganhou()) {
		printf("\nParabéns, você ganhou!\n\n");

		printf("       ___________      \n");
		printf("      '._==_==_=_.'     \n");
		printf("      .-\\:      /-.    \n");
		printf("     | (|:.     |) |    \n");
		printf("      '-|:.     |-'     \n");
		printf("        \\::.    /      \n");
		printf("         '::. .'        \n");
		printf("           ) (          \n");
		printf("         _.' '._        \n");
		printf("        '-------'       \n\n");

	} else {
		printf("\nPuxa, você foi enforcado!\n");
		printf("A palavra era **%s**\n\n", palavrasecreta);

		printf("    _______________         \n");
		printf("   /               \\       \n"); 
		printf("  /                 \\      \n");
		printf("//                   \\/\\  \n");
		printf("\\|   XXXX     XXXX   | /   \n");
		printf(" |   XXXX     XXXX   |/     \n");
		printf(" |   XXX       XXX   |      \n");
		printf(" |                   |      \n");
		printf(" \\__      XXX      __/     \n");
		printf("   |\\     XXX     /|       \n");
		printf("   | |           | |        \n");
		printf("   | I I I I I I I |        \n");
		printf("   |  I I I I I I  |        \n");
		printf("   \\_             _/       \n");
		printf("     \\_         _/         \n");
		printf("       \\_______/           \n");
	}

	adicionapalavra();
}
