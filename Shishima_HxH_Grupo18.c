/*
    Jogo Pe de Galinha (Shisima) - pt 1
    - Alvaro Ezequiel Barrios Cáceres (11335088)
    - Caio do Prado Gaspar (16858643)
    - Gabriel Inumaru Esteves (15453487)

    github: https://github.com/cpradogaspar/shisima-game.git
*/
#include <stdio.h> 
#include <stdbool.h>


// Mostra o tabuleiro com as casas atuais e a rodada
void imprimir_tabuleiro(char tabuleiro[3][3], int turno) {
    printf("\n");
    printf(" %c - %c - %c \n", tabuleiro[0][0], tabuleiro[0][1], tabuleiro[0][2]);
    printf(" | \\ | / |\n");
    printf(" %c - %c - %c \n", tabuleiro[1][0], tabuleiro[1][1], tabuleiro[1][2]);
    printf(" | / | \\ |\n");
    printf(" %c - %c - %c \n\n", tabuleiro[2][0], tabuleiro[2][1], tabuleiro[2][2]);
    return;
}

// Verifica se o jogador atual venceu, mas ignora vitoria se as pecas estiverem soh na linha inicial
int verificar_vitoria(char tabuleiro[3][3], char jogador) {
    if (jogador == 'O' && tabuleiro[2][0] == 'O' && tabuleiro[2][1] == 'O' && tabuleiro[2][2] == 'O'){
        return 0;
    } 
    if (jogador == 'X' && tabuleiro[0][0] == 'X' && tabuleiro[0][1] == 'X' && tabuleiro[0][2] == 'X'){
        return 0;
    } 

    // checa linhas e colunas
    for (int i = 0; i < 3; i++) {
        if (tabuleiro[i][0] == jogador && tabuleiro[i][1] == jogador && tabuleiro[i][2] == jogador){
            return 1;
        }
        if (tabuleiro[0][i] == jogador && tabuleiro[1][i] == jogador && tabuleiro[2][i] == jogador){
            return 1;
        } 
    }
    // checa as duas diagonais
    if (tabuleiro[0][0] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][2] == jogador){
        return 1;
    } 
    if (tabuleiro[0][2] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][0] == jogador){ 
        return 1;
    }

    return 0;
}

// Checa se o movimento eh valido, considerando as conexoes entre casas
bool movimento_valido(char tabuleiro[3][3], int linha_origem, int col_origem, int linha_dest, int col_dest, char jogador) {
    // Ajusta indices para 0-2
    linha_origem--; 
    col_origem--; 
    linha_dest--; 
    col_dest--; 
    
    // ajusta pra indice de matriz
    if (linha_origem < 0 || linha_origem > 2 || col_origem < 0 || col_origem > 2 || linha_dest < 0 || linha_dest > 2 || col_dest < 0 || col_dest > 2) {
        return false;
    }
    
    if (tabuleiro[linha_origem][col_origem] != jogador || tabuleiro[linha_dest][col_dest] != '.'){
        return false;
    }

    // converte posição 2D para casa de 1 a 9
    int origem = linha_origem * 3 + col_origem + 1;
    int destino = linha_dest * 3 + col_dest + 1;

    // matriz de conexoes entre as casas do tabuleiro
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

    // verifica se destino esta nas conexoes da origem
    for (int i = 0; conexoes[origem - 1][i] != -1; i++) {
        if (conexoes[origem - 1][i] == destino){
            return true;
        } 
    }
    return false;
}

// Mostra instrucoes no comeco do jogo
void mostrar_regras() {
    printf("=== Jogo Pe de Galinha (Shisima) ===\n");
    printf("Jogador 1: O\nJogador 2: X\n\n");
    printf("Tabuleiro inicial:\n X - X - X    (1,1)-(1,2)-(1,3)\n | \\ | / |\n . - . - .    (2,1)-(2,2)-(2,3)\n | / | \\ |\n O - O - O    (3,1)-(3,2)-(3,3)\n\n");
    printf("Objetivo: alinhar suas 3 pecas em linha (horizontal, vertical ou diagonal) e que nao seja a inicial \n");
    printf("Informe as coordenadas da peca (linha coluna) e destino (linha coluna)\n");
    printf("Movimentos validos: apenas entre casas conectadas por linhas e que estejam vazias\n\n");
    return;
}

// Funco principal onde o jogo roda
int main() {
    /*
    O jogo tem placar dos jogadores cumulativo. Entao o programa so sera interrompido se os jogadores quiserem.
    As seguintes variaveis que controlam se o jogo deve continuar (jogar), 
    o numero de jogos que se passaram e numero de vitorias de cada jogador
    */
    int jogo_ativo = true;
    int numero_jogos = 0;
    int vitorias_jogador1 = 0; 
    int vitorias_jogador2 = 0;
    while (jogo_ativo) {
        numero_jogos++;
        char jogador_atual = 'O'; // Jogador 1 começa
        int turno = 0;
        bool vitoria = false;

        // inicializa o tabuleiro com as peças nas posições iniciais
        char tabuleiro[3][3] = {
            {'X', 'X', 'X'},
            {'.', '.', '.'},
            {'O', 'O', 'O'}
        };

        if (numero_jogos == 1){
            mostrar_regras();
        } else {
            printf("\n\n\n================ REINICIANDO ====================\n\n\n");
        }

        //exibe o tabuleiro antes da primeira jogada. Depois soh vai mostrar novamente se a jogada inserida for valida
        imprimir_tabuleiro(tabuleiro, turno);
        while (turno < 30 && !vitoria) {
            //comeca novo turno
            turno++;
            //mostra o turno atual
            printf("================== TURNO NUMERO %d ==================\n", turno);
            //comeca o trecho de leitura da jogada
            printf("TURNO %d: Jogador %c (%c), sua vez!\n", turno, (jogador_atual == 'O' ? '1' : '2'), jogador_atual);
            //define variaveis que controlam as coordenadas iniciais e desejadas
            int linha_origem, col_origem, linha_dest, col_dest;
            int valido = false;
            while (!valido) {
                // Lê movimento do jogador
                printf("Mover de (linha coluna): ");
                scanf("%d %d", &linha_origem, &col_origem);
                printf("Para (linha coluna): ");
                scanf("%d %d", &linha_dest, &col_dest);
                /*
                Se o movimento for valido, muda a variavel tabuleiro, valido para verdadeiro e imprime, entao procesegue. 
                Senao repete o while
                */
                if (movimento_valido(tabuleiro, linha_origem, col_origem, linha_dest, col_dest, jogador_atual)) {
                    tabuleiro[linha_dest-1][col_dest-1] = jogador_atual;
                    tabuleiro[linha_origem-1][col_origem-1] = '.';
                    valido = true;
                    imprimir_tabuleiro(tabuleiro, turno);
                } else {
                    printf("Movimento invalido. Tente novamente.\n");
                    while (getchar() != '\n'); // limpa buffer
                }
            }

            /*
            Verifica se alguem ganhou. Em caso positivo imprime a mensagem, adiciona a vitoria no placar 
                e muda a variavel vitoria para true (que faz sair do loop)
            */
            if (verificar_vitoria(tabuleiro, jogador_atual)) {
                printf("Jogador %c (%c) venceu!\n", (jogador_atual == 'O' ? '1' : '2'), jogador_atual);
                if (jogador_atual == 'O') vitorias_jogador1++;
                else vitorias_jogador2++;
                vitoria=true;
            }
            jogador_atual = (jogador_atual == 'O') ? 'X' : 'O'; // troca jogador
        }

        if (turno == 30) {
            printf("Empate! Limite de turnos atingido.\n");
        }

        // mostra placar acumulado
        printf("\nNumero de vitorias:\nJogador 1 (O): %d\nJogador 2 (X): %d\n", vitorias_jogador1, vitorias_jogador2);
        
        //recebe entrada do usuario para continuar ou parar o jogo
        printf("Deseja jogar novamente? (1 - sim, 0 - nao): ");
        int entrada;
        scanf("%d", &entrada);

        //muda a variavel que mantem o loop while
        switch(entrada){
            case 0:
                jogo_ativo = false;
            case 1:
                jogo_ativo = true;
            default:
                // saída segura caso o usuario nao digite um valor valido
                jogo_ativo = false;  
        }
    }
    return 0;
}
