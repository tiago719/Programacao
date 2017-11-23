#ifndef AUXILIARES_H_INCLUDED
#define AUXILIARES_H_INCLUDED

void mostra_tabuleiro(int linhas, int colunas,char tabuleiro[][colunas]);

void dimensao(int *linhas, int *colunas);

char monta_tabuleiro(int linhas, int colunas, char tabuleiro[][colunas]);

char muda_tabuleiro(int linha, int coluna,int linhas, int colunas, char tabuleiro[][colunas]);

int converte_char_int(char c);

char converte_int_char(int coluna);

void pede_jogada(int *linha_jogada, int *coluna_jogada, int linhas, int colunas, char tabuleiro[][colunas]);

int sub_menu();


#endif // AUXILIARES_H_INCLUDED
