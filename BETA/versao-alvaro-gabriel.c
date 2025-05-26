// Gabriel Inumaru Esteves Nro USP:15453487
// Caio do Prado Gaspar Nro USP:16858643
// Alvaro Ezequiel Barrios C Nro USP:11335088
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void imprimir_tabuleiro(char tabuleiro[3][3]) {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) printf("---+---+---\n");
    }
    printf("\n");
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
    // Ajusta índices para 0-2
    linha_origem--; col_origem--; linha_dest--; col_dest--;

    // Verifica limites
    if (linha_origem < 0 || linha_origem > 2 || col_origem < 0 || col_origem > 2) return false;
    if (linha_dest < 0 || linha_dest > 2 || col_dest < 0 || col_dest > 2) return false;

    // Verifica peça na origem
    if (tabuleiro[linha_origem][col_origem] != (jogador == 1 ? 'X' : 'O')) return false;
    
    // Verifica destino vazio
    if (tabuleiro[linha_dest][col_dest] != '.') return false;

    // Posição central pode mover para qualquer lugar
    if (linha_origem == 1 && col_origem == 1) {
        return true;
    }

    // Para posições periféricas
    int diff_linha = abs(linha_origem - linha_dest);
    int diff_col = abs(col_origem - col_dest);

    // Movimento válido se:
    // 1. Para o centro
    if (linha_dest == 1 && col_dest == 1) {
        return true;
    }
    
    // 2. Para as 2 posições adjacentes específicas
    // Cantos só podem mover para as bordas adjacentes
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

// Função principal do jogo
int main() {
    // Tabuleiro inicial
    char tabuleiro[3][3] = {
        {'X', 'X', 'X'},
        {'.', '.', '.'},
        {'O', 'O', 'O'}
    };
    // Variáveis de controle do jogo
    int jogador_atual = 1;
    int vencedor = 0;
    bool jogo_ativo = true;
    int movimentos_jogador1 = 0;
    int movimentos_jogador2 = 0;
    const int MAX_MOVIMENTOS = 30;
    // Instruções do jogo
    printf("Jogo Shisima - Regras de Movimentação:\n");
    printf("1. Posições periféricas podem mover para:\n");
    printf("   - Posição central (2,2)\n");
    printf("   - 2 posições adjacentes específicas\n");
    printf("2. Posição central pode mover para qualquer posição\n\n");
    printf("Jogador 1: X\nJogador 2: O\n");
    printf("Digite coordenadas de 1 a 3 (linha coluna)\n");
    // Loop principal do jogo
    while (jogo_ativo) {
        imprimir_tabuleiro(tabuleiro);
        printf("Jogador %d (%c), sua vez!\n", jogador_atual, (jogador_atual == 1 ? 'X' : 'O'));

        int linha_origem, col_origem, linha_dest, col_dest;
        bool movimento_ok = false;
        // Obter movimento
        while (!movimento_ok) {
            printf("Mover peça de (linha coluna): ");
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
                printf("Movimento inválido! Tente novamente.\n");
                while (getchar() != '\n'); // Limpa buffer
            }
        }

        // Executa movimento
        tabuleiro[linha_dest-1][col_dest-1] = (jogador_atual == 1 ? 'X' : 'O');
        tabuleiro[linha_origem-1][col_origem-1] = '.';

        // Atualiza contador
        if (jogador_atual == 1) movimentos_jogador1++;
        else movimentos_jogador2++;

        // Verifica vitória
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