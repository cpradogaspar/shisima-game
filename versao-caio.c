/*
    Jogo Pe de Galinha (Shisima) - pt 1
    Alunos:
    - Alvaro Ezequiel Barrios Cáceres
    - Caio do Prado Gaspar 16858643
    - Gabriel Inumaru Esteves
*/

#include <stdio.h>

typedef struct {
    int posicao1, posicao2, posicao3;
} Jogador;

void mostrar_regras();
void imprimir_tabuleiro(Jogador jogadores[], int jogada);
int verificar_vitoria(Jogador j);
int movimento_conectado(int origem, int destino);
int casa_ocupada(Jogador jogadores[], int casa);
void realizar_movimento(Jogador jogadores[], int jogador_atual);
int conexoes[9][8];

int main() {
    int boleano_jogar = 1;
    int numero_jogos = 0;
    int vitorias_jogador1 = 0;
    int vitorias_jogador2 = 0;
    do{
        numero_jogos++;
        Jogador jogadores[2];
        jogadores[0].posicao1 = 7;
        jogadores[0].posicao2 = 8;
        jogadores[0].posicao3 = 9;

        jogadores[1].posicao1 = 1;
        jogadores[1].posicao2 = 2;
        jogadores[1].posicao3 = 3;

        int jogada = 0;
        int jogador_atual = 0;
        int vencedor = -1;

        if(numero_jogos == 1){
            mostrar_regras();
        }
        imprimir_tabuleiro(jogadores, jogada);

        while (jogada < 30 && vencedor == -1) {
            realizar_movimento(jogadores, jogador_atual);
            jogada = jogada + 1;
            imprimir_tabuleiro(jogadores, jogada);

            if (verificar_vitoria(jogadores[jogador_atual]) == 1) {
                vencedor = jogador_atual;
            } else {
                jogador_atual = jogador_atual == 0 ? 1 : 0;
            }
        }

        if (vencedor != -1) {
            printf("O vencedor foi o Jogador %d!\n\n\n\n\n", vencedor + 1);
            if (vencedor == 0) {
                vitorias_jogador1++;
            } else {
                vitorias_jogador2++;
            }
            
        } else {
            printf("Empate! 30 jogadas sem vencedor.\n\n");
        }
        printf("Numero de vitorias:\n");
        printf("Jogador 1: %d\n", vitorias_jogador1);
        printf("Jogador 2: %d\n\n\n\n", vitorias_jogador2);
        printf("Deseja jogar novamente? (1 - sim, 0 - nao): ");
        scanf("%d", &boleano_jogar);
        if (boleano_jogar != 0 && boleano_jogar != 1) {
            printf("Opcao invalida. Saindo do jogo.\n");
            boleano_jogar = 0;
        } else if( boleano_jogar == 1) {
            printf("\n\n\n\n\n");
            printf("Reiniciando o jogo...\n");
        }
        
    } while(boleano_jogar==1);
    return 0;
}

int conexoes[9][8] = {
    {2, 4, 5, -1},
    {1, 3, 5, -1},       
    {2, 6, 5, -1},      
    {1, 7, 5, -1},      
    {1, 2, 3, 4, 6, 7, 8, 9},
    {3, 9, 5, -1},
    {4, 8, 5, -1},
    {7, 9, 5, -1},
    {6, 8, 5, -1}
};


void mostrar_regras() {
    printf("=== Jogo Pe de Galinha (Shisima) ===\n");
    printf("Dois jogadores movem suas 3 pecas em um tabuleiro 3x3.\n");
    printf("Jogador 1: pecas O"
           "\nJogador 2: pecas X\n");
    printf("Tabuleiro:\n");
    printf(" 1 - 2 - 3\n"
        " | \\ | / |\n"
        " 4 - 5 - 6\n"
        " | / | \\ |\n"
        " 7 - 8 - 9\n");                                                 
    printf("Objetivo: alinhar suas 3 pecas em linha (horizontal, vertical ou diagonal)\n");
    printf("Voce deve informar a casa onde esta sua peca e a casa de destino\n");
    printf("Movimentos so podem ser feitos para casas conectadas por linha e que estejam vazias\n");
    printf("O jogo termina com vitoria ou apos 30 jogadas (empate)\n\n");
    return;
}

void imprimir_tabuleiro(Jogador jogadores[], int jogada) {
    if (jogada != 0) {
        printf("\n************** Jogada N %d **************\n", jogada);
    }

    char casa_char(int pos){
        for (int j = 0; j < 2; j++) {
            if (pos == jogadores[j].posicao1 || pos == jogadores[j].posicao2 || pos == jogadores[j].posicao3) {
                return (j == 0) ? 'O' : 'X';
            }
        }
        return '0' + pos;  
    }

    printf(" %c - %c - %c\n", casa_char(1), casa_char(2), casa_char(3));
    printf(" | \\ | / |\n");
    printf(" %c - %c - %c\n", casa_char(4), casa_char(5), casa_char(6));
    printf(" | / | \\ |\n");
    printf(" %c - %c - %c\n\n", casa_char(7), casa_char(8), casa_char(9));
    return;
}


int verificar_vitoria(Jogador j) {
    int p[3] = {j.posicao1, j.posicao2, j.posicao3};
    int vitorias[8][3] = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9},
        {1, 4, 7}, {2, 5, 8}, {3, 6, 9},
        {1, 5, 9}, {3, 5, 7}
    };

    for (int i = 0; i < 8; i++) {
        int tem1 = 0, tem2 = 0, tem3 = 0;
        for (int j = 0; j < 3; j++) {
            if (p[0] == vitorias[i][j]) tem1 = 1;
            if (p[1] == vitorias[i][j]) tem2 = 1;
            if (p[2] == vitorias[i][j]) tem3 = 1;
        }
        if (tem1 && tem2 && tem3) {
            if (!(p[0] == 7 && p[1] == 8 && p[2] == 9) && !(p[0] == 1 && p[1] == 2 && p[2] == 3)) {
                return 1;
            }
        }
    }
    return 0;
}

int movimento_conectado(int origem, int destino) {
    int i = 0;
    while (conexoes[origem - 1][i] != -1 && i < 8) {
        if (conexoes[origem - 1][i] == destino) return 1;
        i = i + 1;
    }
    return 0;
}

int casa_ocupada(Jogador jogadores[], int casa) {
    int i = 0;
    while (i < 2) {
        if (jogadores[i].posicao1 == casa || jogadores[i].posicao2 == casa || jogadores[i].posicao3 == casa) {
            return 1;
        }
        i++;
    }
    return 0;
}

void realizar_movimento(Jogador jogadores[], int jogador_atual) {
    int origem = 0, destino = 0;
    int valido = 0;

    while (valido == 0) {
        printf("Jogador %d, digite a casa da peca e a casa de destino: ", jogador_atual + 1);
        scanf("%d %d", &origem, &destino);

        int *posicao = 0;
        if (jogadores[jogador_atual].posicao1 == origem) posicao = &jogadores[jogador_atual].posicao1;
        else if (jogadores[jogador_atual].posicao2 == origem) posicao = &jogadores[jogador_atual].posicao2;
        else if (jogadores[jogador_atual].posicao3 == origem) posicao = &jogadores[jogador_atual].posicao3;
        else {
            printf("Erro: Voce nao possui uma peca na casa %d.\n", origem);
            continue;
        }

        if (!movimento_conectado(origem, destino)) {
            printf("Erro: As casas nao estao conectadas.\n");
            continue;
        }

        if (casa_ocupada(jogadores, destino)) {
            printf("Erro: A casa de destino esta ocupada.\n");
            continue;
        }

        *posicao = destino;
        valido = 1;
    }
}