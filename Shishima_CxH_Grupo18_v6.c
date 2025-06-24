#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// Definições de parâmetros da IA
#define MAX_DEPTH 5               // Profundidade máxima da árvore do minimax
#define TEMPO_MAXIMO 1            // Tempo máximo por jogada da IA (em segundos)
#define TAMANHO_TABELA 10007      // Tamanho da tabela de transposição (para otimização)

// Estrutura para representar uma jogada
typedef struct {
    int linha_origem;
    int col_origem;
    int linha_dest;
    int col_dest;
    int valor;
} Jogada;

// Entrada para a tabela de transposição (cache do Minimax)
typedef struct {
    char tabuleiro[3][3];
    int valor;
    int profundidade;
} EntradaTabela;

// Armazena o contexto do jogo entre partidas
typedef struct {
    int numero_partidas;
    int vitorias_jogador1;
    int vitorias_jogador2;
} ContextoJogo;

// Tabela de transposição global
EntradaTabela tabelaTransposicao[TAMANHO_TABELA];

// === UTILITÁRIOS ===

// Obtém o tempo atual
time_t obter_tempo_atual() {
    return time(NULL);
}

// Gera hash para o tabuleiro atual (usado para a tabela de transposição)
unsigned int hashTabuleiro(char tabuleiro[3][3]) {
    unsigned int hash = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            hash = hash * 31 + tabuleiro[i][j];
    return hash % TAMANHO_TABELA;
}

// Imprime o tabuleiro no terminal
void imprimir_tabuleiro(char tabuleiro[3][3]) {
    printf("\n");
    printf(" %c - %c - %c \n", tabuleiro[0][0], tabuleiro[0][1], tabuleiro[0][2]);
    printf(" | \\ | / |\n");
    printf(" %c - %c - %c \n", tabuleiro[1][0], tabuleiro[1][1], tabuleiro[1][2]);
    printf(" | / | \\ |\n");
    printf(" %c - %c - %c \n\n", tabuleiro[2][0], tabuleiro[2][1], tabuleiro[2][2]);
}

// Mostra as regras do jogo
void mostrar_regras() {
    printf("=== Jogo Pé de Galinha (Shisima) ===\n");
    printf("Jogador 1 IA: X\nJogador 2: O\n\n");
    printf("Objetivo: alinhar suas 3 peças em linha (horizontal, vertical ou diagonal) e que não seja a posição inicial.\n");
    printf("Movimentos válidos: apenas entre casas conectadas e livres.\n\n");
}

// Limpa tela e mostra cabeçalho da partida
void imprimir_partida(int partida, bool continuar) {
    if (continuar) {
        for (int i = 0; i < 20; i++) printf("\n");  // Limpa a tela
        printf("================= PARTIDA %d ============\n", partida);
    }
}

// Copia um tabuleiro para outro
void copiar_tabuleiro(char origem[3][3], char destino[3][3]) {
    memcpy(destino, origem, sizeof(char) * 9);
}

// Verifica se há um vencedor no tabuleiro atual
int verificar_vencedor(char tabuleiro[3][3]) {
    // Linhas centrais ou de base específicas para cada jogador
    if (tabuleiro[1][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[1][2] && tabuleiro[1][0] != '.')
        return (tabuleiro[1][0] == 'X') ? 1 : 2;

    if (tabuleiro[0][0] == tabuleiro[0][1] && tabuleiro[0][1] == tabuleiro[0][2] && tabuleiro[0][0] == 'O')
        return 2;

    if (tabuleiro[2][0] == tabuleiro[2][1] && tabuleiro[2][1] == tabuleiro[2][2] && tabuleiro[2][0] == 'X')
        return 1;

    // Colunas
    for (int col = 0; col < 3; col++)
        if (tabuleiro[0][col] == tabuleiro[1][col] && tabuleiro[1][col] == tabuleiro[2][col] && tabuleiro[0][col] != '.')
            return (tabuleiro[0][col] == 'X') ? 1 : 2;

    // Diagonais
    if (tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2] && tabuleiro[0][0] != '.')
        return (tabuleiro[0][0] == 'X') ? 1 : 2;

    if (tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0] && tabuleiro[0][2] != '.')
        return (tabuleiro[0][2] == 'X') ? 1 : 2;

    return 0; // Sem vencedor
}

// Avalia heurística do tabuleiro para a IA
int avaliar_tabuleiro(char tabuleiro[3][3], char computador) {
    char humano = (computador == 'O') ? 'X' : 'O';
    int vencedor = verificar_vencedor(tabuleiro);

    if (vencedor != 0) return (vencedor == (computador == 'O' ? 2 : 1)) ? 1000 : -1000;

    int score = 0;
    // Pontuação baseada em controle do centro e ameaças
    score += (tabuleiro[1][1] == computador) ? 2 : 0;
    score -= (tabuleiro[1][1] == humano) ? 3 : 0;

    // Avaliação por linhas e colunas com 2 peças
    for (int i = 0; i < 3; i++) {
        int linha_comp = 0, linha_humano = 0, col_comp = 0, col_humano = 0;
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == computador) linha_comp++;
            if (tabuleiro[i][j] == humano) linha_humano++;
            if (tabuleiro[j][i] == computador) col_comp++;
            if (tabuleiro[j][i] == humano) col_humano++;
        }
        if (linha_comp == 2) score += 10;
        if (linha_humano == 2) score -= 15;
        if (col_comp == 2) score += 10;
        if (col_humano == 2) score -= 15;
    }

    // Pontos por diagonais
    if (tabuleiro[0][0] == computador && tabuleiro[1][1] == computador) score += 5;
    if (tabuleiro[1][1] == computador && tabuleiro[2][2] == computador) score += 5;
    if (tabuleiro[0][2] == computador && tabuleiro[1][1] == computador) score += 5;
    if (tabuleiro[1][1] == computador && tabuleiro[2][0] == computador) score += 5;

    return score;
}

// Verifica se uma jogada é válida segundo as regras do jogo
bool movimento_valido(int linha_origem, int col_origem, int linha_dest, int col_dest, int jogador, char tabuleiro[3][3]) {
    linha_origem--; col_origem--; linha_dest--; col_dest--;

    if (linha_origem < 0 || linha_origem > 2 || col_origem < 0 || col_origem > 2) return false;
    if (linha_dest < 0 || linha_dest > 2 || col_dest < 0 || col_dest > 2) return false;

    char peca = (jogador == 1) ? 'X' : 'O';
    if (tabuleiro[linha_origem][col_origem] != peca || tabuleiro[linha_dest][col_dest] != '.') return false;

    if (linha_dest == 1 && col_dest == 1) return true;
    if (linha_origem == 1 && col_origem == 1) return true;

    int dx = abs(linha_origem - linha_dest);
    int dy = abs(col_origem - col_dest);

    return (dx + dy == 1); // Movimento ortogonal adjacente
}
// === IA ===

// Tenta encontrar jogadas críticas como vitórias imediatas, bloqueios ou centro vazio
Jogada jogada_especial(char tabuleiro[3][3], char computador) {
    char humano = (computador == 'X') ? 'O' : 'X';
    Jogada melhor = {-1, -1, -1, -1, INT_MIN};

    // 1. Verifica se há uma jogada que leva à vitória direta
    for (int l1 = 0; l1 < 3; l1++) {
        for (int c1 = 0; c1 < 3; c1++) {
            if (tabuleiro[l1][c1] != computador) continue;

            for (int l2 = 0; l2 < 3; l2++) {
                for (int c2 = 0; c2 < 3; c2++) {
                    if (l1 == l2 && c1 == c2) continue;

                    if (movimento_valido(l1 + 1, c1 + 1, l2 + 1, c2 + 1, (computador == 'X' ? 1 : 2), tabuleiro)) {
                        char copia[3][3];
                        copiar_tabuleiro(tabuleiro, copia);
                        copia[l2][c2] = computador;
                        copia[l1][c1] = '.';

                        if (verificar_vencedor(copia) == ((computador == 'X') ? 1 : 2)) {
                            return (Jogada){l1 + 1, c1 + 1, l2 + 1, c2 + 1, 1000};
                        }
                    }
                }
            }
        }
    }

    // 2. Tenta bloquear uma vitória do oponente
    for (int l1 = 0; l1 < 3; l1++) {
        for (int c1 = 0; c1 < 3; c1++) {
            if (tabuleiro[l1][c1] != computador) continue;

            for (int l2 = 0; l2 < 3; l2++) {
                for (int c2 = 0; c2 < 3; c2++) {
                    if (l1 == l2 && c1 == c2) continue;

                    if (movimento_valido(l1 + 1, c1 + 1, l2 + 1, c2 + 1, (computador == 'X' ? 1 : 2), tabuleiro)) {
                        char copia[3][3];
                        copiar_tabuleiro(tabuleiro, copia);
                        copia[l2][c2] = computador;
                        copia[l1][c1] = '.';

                        if (verificar_vencedor(copia) == ((computador == 'X') ? 2 : 1)) {
                            return (Jogada){l1 + 1, c1 + 1, l2 + 1, c2 + 1, 950};
                        }
                    }
                }
            }
        }
    }

    // 3. Tenta ocupar o centro se estiver livre
    if (tabuleiro[1][1] == '.') {
        for (int l = 0; l < 3; l++) {
            for (int c = 0; c < 3; c++) {
                if (tabuleiro[l][c] == computador &&
                    movimento_valido(l + 1, c + 1, 2, 2, (computador == 'X' ? 1 : 2), tabuleiro)) {
                    return (Jogada){l + 1, c + 1, 2, 2, 920};
                }
            }
        }
    }

    return melhor; // Nenhuma jogada especial encontrada
}
Jogada minimax(char tabuleiro[3][3], int profundidade, int alpha, int beta, bool maximizando, char jogador_atual, time_t inicio_turno) {
    Jogada melhor_jogada = {0,0,0,0, maximizando ? INT_MIN : INT_MAX};
    char oponente = (jogador_atual == 'X') ? 'O' : 'X';

    // Verifica se tabuleiro atual já está na tabela (cache)
    unsigned int hash = hashTabuleiro(tabuleiro);
    EntradaTabela entrada = tabelaTransposicao[hash];
    if (entrada.profundidade >= profundidade &&
        memcmp(entrada.tabuleiro, tabuleiro, sizeof(char[3][3])) == 0) {
        melhor_jogada.valor = entrada.valor;
        return melhor_jogada;
    }

    // Caso base do minimax
    int pontuacao = avaliar_tabuleiro(tabuleiro, jogador_atual);
    if (profundidade == 0 || abs(pontuacao) >= 900 || difftime(obter_tempo_atual(), inicio_turno) > TEMPO_MAXIMO) {
        melhor_jogada.valor = pontuacao;
        return melhor_jogada;
    }

    // Explora todos os movimentos possíveis
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == (maximizando ? jogador_atual : oponente)) {
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue;

                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < 3 && nj >= 0 && nj < 3 &&
                            tabuleiro[ni][nj] == '.' &&
                            movimento_valido(i + 1, j + 1, ni + 1, nj + 1, maximizando ? 1 : 2, tabuleiro)) {

                            tabuleiro[ni][nj] = maximizando ? jogador_atual : oponente;
                            tabuleiro[i][j] = '.';

                            Jogada atual = minimax(tabuleiro, profundidade - 1, alpha, beta, !maximizando, oponente, inicio_turno);

                            // Desfaz jogada
                            tabuleiro[ni][nj] = '.';
                            tabuleiro[i][j] = maximizando ? jogador_atual : oponente;

                            if (maximizando) {
                                if (atual.valor > melhor_jogada.valor) {
                                    melhor_jogada = (Jogada){i + 1, j + 1, ni + 1, nj + 1, atual.valor};
                                }
                                alpha = fmax(alpha, atual.valor);
                            } else {
                                if (atual.valor < melhor_jogada.valor) {
                                    melhor_jogada = (Jogada){i + 1, j + 1, ni + 1, nj + 1, atual.valor};
                                }
                                beta = fmin(beta, atual.valor);
                            }

                            if (beta <= alpha) goto podado;
                        }
                    }
                }
            }
        }
    }

podado:
    // Salva resultado no cache
    memcpy(tabelaTransposicao[hash].tabuleiro, tabuleiro, sizeof(char[3][3]));
    tabelaTransposicao[hash].valor = melhor_jogada.valor;
    tabelaTransposicao[hash].profundidade = profundidade;

    return melhor_jogada;
}
void jogada_IA(char tabuleiro[3][3], char computador) {
    // Verifica se há jogada especial
    Jogada especial = jogada_especial(tabuleiro, computador);
    if (especial.valor >= 900) {
        printf("IA moveu de (%d %d) para (%d %d)\n", especial.linha_origem, especial.col_origem, especial.linha_dest, especial.col_dest);
        tabuleiro[especial.linha_dest - 1][especial.col_dest - 1] = computador;
        tabuleiro[especial.linha_origem - 1][especial.col_origem - 1] = '.';
        return;
    }

    // Caso não haja, usa heurística simples (avaliação direta)
    Jogada melhor = {-1, -1, -1, -1, INT_MIN};
    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++) {
            if (tabuleiro[l][c] == computador) {
                for (int dl = 0; dl < 3; dl++) {
                    for (int dc = 0; dc < 3; dc++) {
                        if ((l == dl && c == dc) || !movimento_valido(l + 1, c + 1, dl + 1, dc + 1, 1, tabuleiro)) continue;

                        char copia[3][3];
                        copiar_tabuleiro(tabuleiro, copia);
                        copia[dl][dc] = computador;
                        copia[l][c] = '.';

                        int valor = avaliar_tabuleiro(copia, computador);
                        if (valor > melhor.valor) {
                            melhor = (Jogada){l + 1, c + 1, dl + 1, dc + 1, valor};
                        }
                    }
                }
            }
        }
    }

    if (melhor.valor != INT_MIN) {
        printf("IA moveu de (%d %d) para (%d %d)\n", melhor.linha_origem, melhor.col_origem, melhor.linha_dest, melhor.col_dest);
        tabuleiro[melhor.linha_dest - 1][melhor.col_dest - 1] = computador;
        tabuleiro[melhor.linha_origem - 1][melhor.col_origem - 1] = '.';
    } else {
        printf("IA não encontrou jogada válida.\n");
    }
}
void jogada_humano(char tabuleiro[3][3], char jogador) {
    int linha_origem, col_origem, linha_dest, col_dest;
    bool valido = false;
    int numero_jogador = (jogador == 'X') ? 1 : 2;

    do {
        printf("Jogador %d, informe linha e coluna da peça para mover (ex: 3 1): ", numero_jogador);
        if (scanf("%d %d", &linha_origem, &col_origem) != 2) {
            printf("Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n'); // limpa buffer
            continue;
        }

        printf("Jogador %d, informe linha e coluna destino (ex: 2 2): ", numero_jogador);
        if (scanf("%d %d", &linha_dest, &col_dest) != 2) {
            printf("Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        valido = movimento_valido(linha_origem, col_origem, linha_dest, col_dest, numero_jogador, tabuleiro);
        if (!valido) printf("Movimento inválido, tente novamente.\n");

    } while (!valido);

    // Realiza jogada
    tabuleiro[linha_dest - 1][col_dest - 1] = jogador;
    tabuleiro[linha_origem - 1][col_origem - 1] = '.';
}
void jogo() {
    char tabuleiro[3][3] = { {'X', 'X', 'X'}, {'.', '.', '.'}, {'O', 'O', 'O'} };
    ContextoJogo contexto = {0, 0, 0};
    int jogador_atual = 2;
    int vencedor = 0, partida = 1, modo_jogo = 0;
    bool continuar = true;
    int jogadas_X = 0, jogadas_O = 0;

    mostrar_regras();
    printf("Escolha o modo de jogo:\n1 - Jogar contra IA\n2 - Jogar contra outro humano\n> ");
    while (modo_jogo != 1 && modo_jogo != 2) {
        scanf("%d", &modo_jogo);
        if (modo_jogo != 1 && modo_jogo != 2) printf("Modo inválido. Escolha 1 ou 2:\n> ");
    }

    while (continuar) {
        imprimir_partida(partida, true);
        imprimir_tabuleiro(tabuleiro);

        if ((jogador_atual == 1 && jogadas_X >= 30) || (jogador_atual == 2 && jogadas_O >= 30)) {
            printf("Jogador %d excedeu o limite de 30 jogadas. Fim da partida.\n", jogador_atual);
            vencedor = (jogador_atual == 1) ? 2 : 1;
        } else {
            if (modo_jogo == 1 && jogador_atual == 1) {
                jogada_IA(tabuleiro, 'X');
                jogadas_X++;
            } else {
                char simbolo = (jogador_atual == 1) ? 'X' : 'O';
                jogada_humano(tabuleiro, simbolo);
                if (jogador_atual == 1) jogadas_X++; else jogadas_O++;
            }

            vencedor = verificar_vencedor(tabuleiro);
        }

        if (vencedor != 0 || jogadas_X >= 30 || jogadas_O >= 30) {
            imprimir_tabuleiro(tabuleiro);
            if (vencedor != 0)
                printf("Jogador %d venceu!\n", vencedor);
            else
                printf("Limite de jogadas atingido. Empate!\n");

            contexto.numero_partidas++;
            if (vencedor == 1) contexto.vitorias_jogador1++;
            else if (vencedor == 2) contexto.vitorias_jogador2++;

            printf("Deseja jogar novamente? (s/n): ");
            char resposta = 'n';
            scanf(" %c", &resposta);
            if (resposta == 's' || resposta == 'S') {
                memcpy(tabuleiro, (char[3][3]){{'X','X','X'},{'.','.','.'},{'O','O','O'}}, sizeof(tabuleiro));
                jogador_atual = 1;
                partida++;
                jogadas_X = 0;
                jogadas_O = 0;
                vencedor = 0;
            } else {
                continuar = false;
            }
        } else {
            jogador_atual = (jogador_atual == 1) ? 2 : 1;
        }
    }

    printf("\nResumo final:\n");
    printf("Partidas jogadas: %d\n", contexto.numero_partidas);
    printf("Vitórias Jogador 1 (X): %d\n", contexto.vitorias_jogador1);
    printf("Vitórias Jogador 2 (O): %d\n", contexto.vitorias_jogador2);
}
int main() {
    jogo(); // Inicia o jogo
    return 0;
}
