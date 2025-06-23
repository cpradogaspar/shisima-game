// Gabriel Inumaru Esteves Nro USP:15453487
// Caio do Prado Gaspar Nro USP:16858643
// Alvaro Ezequiel Barrios C Nro USP:11335088
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Mostra o tabuleiro com as casas atuais e a rodada
void imprimir_tabuleiro(char tabuleiro[3][3]) {
    printf("\n");
    printf(" %c - %c - %c \n", tabuleiro[0][0], tabuleiro[0][1], tabuleiro[0][2]);
    printf(" | \\ | / |\n");
    printf(" %c - %c - %c \n", tabuleiro[1][0], tabuleiro[1][1], tabuleiro[1][2]);
    printf(" | / | \\ |\n");
    printf(" %c - %c - %c \n\n", tabuleiro[2][0], tabuleiro[2][1], tabuleiro[2][2]);
    return;
}

int verificar_vencedor(char tabuleiro[3][3]) {
    // Verifica a linha do meio
    if (tabuleiro[1][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[1][2] && tabuleiro[1][0] != '.') {
        return (tabuleiro[1][0] == 'X') ? 1 : 2;
    }

    // Verifica linhas opostas
    if (tabuleiro[0][0] == tabuleiro[0][1] && tabuleiro[0][1] == tabuleiro[0][2] && tabuleiro[0][0] == 'O') {
        return 2;
    }
    if (tabuleiro[2][0] == tabuleiro[2][1] && tabuleiro[2][1] == tabuleiro[2][2] && tabuleiro[2][0] == 'X') {
        return 1;
    }

    // Verifica colunas
    for (int col = 0; col < 3; col++) {
        if (tabuleiro[0][col] == tabuleiro[1][col] && tabuleiro[1][col] == tabuleiro[2][col] && tabuleiro[0][col] != '.') {
            return (tabuleiro[0][col] == 'X') ? 1 : 2;
        }
    }

    // Verifica diagonais
    if (tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2] && tabuleiro[0][0] != '.') {
        return (tabuleiro[0][0] == 'X') ? 1 : 2;
    }
    if (tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0] && tabuleiro[0][2] != '.') {
        return (tabuleiro[0][2] == 'X') ? 1 : 2;
    }

    return 0;
}

bool movimento_valido(int linha_origem, int col_origem, int linha_dest, int col_dest, char jogador, char tabuleiro[3][3]) {
    // Ajusta indices para 0-2
    linha_origem--; 
    col_origem--; 
    linha_dest--; 
    col_dest--; 

    // Verifica limites
    if (linha_origem < 0 || linha_origem > 2 || col_origem < 0 || col_origem > 2) {
        return false;
    }
    if (linha_dest < 0 || linha_dest > 2 || col_dest < 0 || col_dest > 2){ 
        return false;
    }
    // Verifica peca na origem
    if (tabuleiro[linha_origem][col_origem] != (jogador == 1 ? 'X' : 'O')) {
        return false;
    }
    
    // Verifica destino vazio
    if (tabuleiro[linha_dest][col_dest] != '.') {
        return false;
    }

    // Posicao central pode mover para qualquer lugar
    if (linha_origem == 1 && col_origem == 1) {
        return true;
    }

    // Para posicoes perifericas
    int diff_linha = abs(linha_origem - linha_dest);
    int diff_col = abs(col_origem - col_dest);

    // Movimento valido se:
    // 1. Para o centro
    if (linha_dest == 1 && col_dest == 1) {
        return true;
    }
    
    // 2. Para as 2 posicoes adjacentes especificas
    // Cantos so podem mover para as bordas adjacentes
    if ((linha_origem == 0 && col_origem == 0) && 
        ((linha_dest == 0 && col_dest == 1) || (linha_dest == 1 && col_dest == 0))) {
        return true;
    }
    if ((linha_origem == 0 && col_origem == 2) && 
        ((linha_dest == 0 && col_dest == 1) || (linha_dest == 1 && col_dest == 2))) {
        return true;
    }
    if ((linha_origem == 2 && col_origem == 0) && 
        ((linha_dest == 2 && col_dest == 1) || (linha_dest == 1 && col_dest == 0))) {
        return true;
    }
    if ((linha_origem == 2 && col_origem == 2) && 
        ((linha_dest == 2 && col_dest == 1) || (linha_dest == 1 && col_dest == 2))) {
        return true;
    }
    
    // Bordas do meio
    if ((linha_origem == 0 && col_origem == 1) && 
        ((linha_dest == 0 && col_dest == 0) || (linha_dest == 0 && col_dest == 2))) {
        return true;
    }
    if ((linha_origem == 1 && col_origem == 0) && 
        ((linha_dest == 0 && col_dest == 0) || (linha_dest == 2 && col_dest == 0))) {
        return true;
    }
    if ((linha_origem == 1 && col_origem == 2) && 
        ((linha_dest == 0 && col_dest == 2) || (linha_dest == 2 && col_dest == 2))) {
        return true;
    }
    if ((linha_origem == 2 && col_origem == 1) && 
        ((linha_dest == 2 && col_dest == 0) || (linha_dest == 2 && col_dest == 2))) {
        return true;
    }

    return false;
}

// Mostra instrucoes no comeco do jogo
void mostrar_regras() {
    printf("=== Jogo Pe de Galinha (Shisima) ===\n");
    printf("Jogador 1: X\nJogador 2: O\n\n");
    printf("Tabuleiro inicial:\n X - X - X    (1,1)-(1,2)-(1,3)\n | \\ | / |\n . - . - .    (2,1)-(2,2)-(2,3)\n | / | \\ |\n O - O - O    (3,1)-(3,2)-(3,3)\n\n");
    printf("Objetivo: alinhar suas 3 pecas em linha (horizontal, vertical ou diagonal) e que nao seja a inicial \n");
    printf("Informe as coordenadas da peca (linha coluna) e destino (linha coluna)\n");
    printf("Movimentos validos: apenas entre casas conectadas por linhas e que estejam vazias\n\n");
    return;
}

// Funcao principal do jogo
int main() {
    // Tabuleiro inicial
    char tabuleiro[3][3] = {
        {'X', 'X', 'X'},
        {'.', '.', '.'},
        {'O', 'O', 'O'}
    };
    // Variaveis de controle do jogo
    int jogador_atual = 1;
    int vencedor = 0;
    bool jogo_ativo = true;
    int movimentos_jogador1 = 0;
    int movimentos_jogador2 = 0;
    const int MAX_MOVIMENTOS = 30;
    
    mostrar_regras();
    
    printf("Digite coordenadas de 1 a 3 (linha coluna)\n");
    // Loop principal do jogo
    while (jogo_ativo) {
        imprimir_tabuleiro(tabuleiro);
        printf("Jogador %d (%c), sua vez!\n", jogador_atual, (jogador_atual == 1 ? 'X' : 'O'));

        int linha_origem, col_origem, linha_dest, col_dest;
        bool movimento_ok = false;
        // Obter movimento
        while (!movimento_ok) {
            printf("Mover peca de (linha coluna): ");
            if (scanf("%d %d", &linha_origem, &col_origem) == 2 &&
                linha_origem >= 1 && linha_origem <= 3 &&
                col_origem >= 1 && col_origem <= 3) {
                
                printf("Para (linha coluna): ");
                if (scanf("%d %d", &linha_dest, &col_dest) == 2 &&
                    linha_dest >= 1 && linha_dest <= 3 &&
                    col_dest >= 1 && col_dest <= 3) {
                    
                    movimento_ok = movimento_valido(linha_origem, col_origem, linha_dest, col_dest, jogador_atual, tabuleiro);
                }
            }
            
            if (!movimento_ok) {
                printf("Movimento invalido! Tente novamente.\n");
                while (getchar() != '\n'); // Limpa buffer
            }
        }

        // Executa movimento
        tabuleiro[linha_dest-1][col_dest-1] = (jogador_atual == 1 ? 'X' : 'O');
        tabuleiro[linha_origem-1][col_origem-1] = '.';

        // Atualiza contador
        if (jogador_atual == 1) movimentos_jogador1++;
        else movimentos_jogador2++;

        // Verifica vitoria
        vencedor = verificar_vencedor(tabuleiro);
        if (vencedor != 0) {
            imprimir_tabuleiro(tabuleiro);
            printf("Jogador %d (%c) venceu!\n", vencedor, (vencedor == 1 ? 'X' : 'O'));
            jogo_ativo = false;
        } 
        // Verifica empate
        else if (movimentos_jogador1 >= MAX_MOVIMENTOS && movimentos_jogador2 >= MAX_MOVIMENTOS) {
            imprimir_tabuleiro(tabuleiro);
            printf("Empate! Limite de movimentos atingido.\n");
            jogo_ativo = false;
        }
        // Troca jogador
        else {
            jogador_atual = (jogador_atual == 1) ? 2 : 1;
        }
    }

    return 0;
}
