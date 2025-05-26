# Jogo Pé de Galinha (Shisima)

Este é um projeto em linguagem C que implementa o jogo tradicional africano **Shisima**, também conhecido como **Pé de Galinha**. O jogo foi desenvolvido como parte da disciplina de Introdução à Programação de Computadores.

## 🎮 Sobre o Jogo

- Dois jogadores participam (Jogador 1: `O`, Jogador 2: `X`).
- O objetivo é alinhar as 3 peças na vertical, horizontal ou diagonal.
- A linha inicial (linha de onde o jogador começa) **não conta como vitória**.
- As peças só podem se mover para casas **conectadas por linhas no tabuleiro**.

O tabuleiro é representado assim:

```
 X - X - X
 | \ | / |
 . - . - .
 | / | \ |
 O - O - O
```

## ▶️ Como Jogar

1. Compile o código:
   ```bash
   gcc shisima.c -o shisima
   ```

2. Execute o jogo:
   ```bash
   ./shisima
   ```

3. Siga as instruções no terminal para mover suas peças digitando coordenadas no formato `linha coluna` (por exemplo, `3 1`).

## ⚙️ Regras de Movimento

- As peças podem se mover apenas entre casas ligadas por linhas visuais no tabuleiro.
- A peça central (2,2) pode se conectar com todas as casas.
- Cada jogador pode mover **uma peça por turno**.

## 🛠️ Requisitos

- Compilador C (GCC, Clang, etc)

## 📁 Arquivos

- `Shishima_HxH_Grupo18.c`: código-fonte principal do jogo
- `README.txt`: este arquivo

## ✏️ Observações

- O código foi feito com foco em clareza e legibilidade.
- Alguns comentários e lógica foram mantidos simples para facilitar a compreensão.
- Não há uso de ponteiros nem estruturas complexas para facilitar o entendimento inicial.

---

Desenvolvido para fins educacionais 🤓
