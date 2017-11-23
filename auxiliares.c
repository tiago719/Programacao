#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "auxiliares.h"

void mostra_tabuleiro(int linhas, int colunas,char tabuleiro[][colunas])
{
    int i, j;

    for(i=0; i<linhas; i++)
    {
        putchar('\n');
        for(j=0; j<colunas; j++)
        {
            printf(" %c ", tabuleiro[i][j]);  // Mostra o conteudo de cada posicao do tabuleiro
            putchar('|');
        }
        putchar('\n');
        for(j=0; j<colunas; j++)
        {
            printf("----");
        }
    }
}
void dimensao(int *linhas, int *colunas)
{
    int l,c;
    do
    {
        do
        {
            printf("Insira o numero de linhas: \t");
            scanf("%d", &l);
        }
        while(l<4 || l>8); // continuar a perguntar enquanto o numero de linhas nao estiver compreendido entre 4 e 8

        do
        {
            printf("Insira o numero de colunas: \t");
            scanf("%d", &c);
        }
        while(c<6 || c>10); // continuar a perguntar enquanto o numero de colunas nao estiver compreendido entre 6 e 10
    }
    while(l>=c);    // continuar a perguntar enquanto o numero de linhas for superior ao numero de colunas

    *linhas=1+l;    //linhas incrementam para ser possivel ter o titulo das colunas no tabuleiro
    *colunas=1+c;   //colunas incrementam para ser possivel ter o tirulo das linhas no tabuleiro
}

char monta_tabuleiro(int linhas, int colunas, char tabuleiro[][colunas])
{
    char alfabeto[14]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N'};
    int i,j,a=0;

    for(j=1; j<colunas; j++)
    {
        tabuleiro[0][j]=alfabeto[a];  // Introduzir no tabuleiro o titulo das colunas, recorrendo ao array global
        a++;
    }

    for(i=1; i<linhas; i++)
    {
        tabuleiro[i][0]= i+48;   // Introduzir no tabuleiro o titulo das linhas (i+48=numero da linha em linguagem ascii)
    }

    for(i=1; i<linhas; i++)
    {
        for(j=1; j<colunas; j++)
        {
            tabuleiro[i][j]='*';  // Introduzir no tabuleiro as comidas
        }
    }
    tabuleiro[0][0]=' ';    // espaço em branco
    tabuleiro[linhas-1][colunas-1]='X';  // Introduzir na ultima posicao do tabuleiro o veneno

    return tabuleiro;
}

char muda_tabuleiro(int linha, int coluna,int linhas, int colunas, char tabuleiro[][colunas])
{
    int i,j;

    for(i=1; i<=linha; i++)
    {
        for(j=1; j<=coluna; j++)
        {
            tabuleiro[i][j]=' ';    // Retirar a comida ate as linhas e colunas passadas como argumento
        }
    }
    return tabuleiro;
}

int converte_char_int(char c)
{
    char alfabeto[14]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N'};
    int i;

    for(i=0; i<10; i++)
    {
        if(c==alfabeto[i] || toupper(c)==alfabeto[i])   // comparar o carater com os caracteres do array glocal alfabeto
        {
            return i+1;
        }
    }

    return 15;  // 15, de forma a que na funcao main, nao seja possivel sair do while, uma vez que nao e possivel que existe coluna correspondente a letra introduzida
}

char converte_int_char(int coluna)
{
    char alfabeto[14]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N'};
    if(coluna==0)
    {
            return 0;
    }
    else
    {
            return alfabeto[coluna-1];
    }
}

int menu()
{
    int op;

    printf("\n1 - 1vs1");
    printf("\n2 - 1vsCOM(C)");
    printf("\n3 - Continuar jogo gravado");
    printf("\n4 - Sair");

    do
    {
        printf("\n\nOpcao: ");
        scanf(" %d", &op);
    }
    while(op<1 || op>4);
    return op;
}

void pede_jogada(int *linha_jogada, int *coluna_jogada, int linhas, int colunas, char tabuleiro[][colunas])
{
    int lin, col_num;
    char col;
    do
    {
        do
        {
            printf("\nLinha:");
            scanf("%d", &lin);
        }
        while(lin>linhas-1 || lin<1);  // a linha a comer nao pode ser maior que o numero de linhas do tabuleiro, nem pode ser menor que 1

        do
        {
            printf("Coluna:");
            scanf(" %c", &col);
            col_num=converte_char_int(col); // funcao para converter o char da coluna num int
        }
        while(col_num>colunas-1);  // a coluna a comer nao pode ser maior que o numero de colunas do tabuleiro
    }
    while(tabuleiro[lin][col_num]==' ' || tabuleiro[lin][col_num]=='X');    // a linha e coluna a serem comidas tem de ter comida e nao podem ter veneno

    *linha_jogada=lin;
    *coluna_jogada=col_num;
}

int sub_menu()
{
    int op;

    printf("\n1 - Nova Jogada");
    printf("\n2 - Aumentar Tabuleiro");
    printf("\n3 - Guardar jogo");
    printf("\n4 - Mostrar sucessao de jogadas anteriores");

    do
    {
        printf("\n\nOpcao: ");
        scanf(" %d", &op);
    }
    while(op<1 || op>4);
    return op;
}
