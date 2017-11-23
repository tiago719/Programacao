#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "auxiliares.h"


typedef struct jogada historico, *phistorico;

struct jogada
{
    int linhas;  // linhas do tabuleiro atual
    int colunas;  // colunas do tabuleiro atual
    char *tabuleiro;  // ponteiro para o tabuleiro
    char jogador;  // jogador atual
    int linha_jogada;  // linha jogada
    int coluna_jogada; // coluna jogada
    phistorico prox;  // ponteiro para o proximo elemento da lista ligada

};

phistorico novo_ramo(phistorico lista, int linhas, int colunas, char tabuleiro[][colunas],
                     char jogador, int linha_jogada, int coluna_jogada)
{
    phistorico novo=(phistorico)malloc(sizeof(historico));  // alocar memoria para o novo no

    if(novo==NULL)
    {
        printf("Nao foi possivel alocar memoria. Tente novamente mais tarde");
        return lista;
    }

    char temp[linhas][colunas];
    char * novo_tabuleiro = (char*)malloc(sizeof(temp));
     // alocar memoria para um tabuleiro aux, uma vez que na estrutra o tabuleiro e do tipo char *

    if(novo_tabuleiro==NULL)
    {
        printf("Nao foi possivel alocar memoria. Tente novamente mais tarde");
        return;
    }

    int i,j, count;

    for(i=0, count=0; i<linhas; i++)
    {
        for(j=0; j<colunas; j++, count++)
        {
            *(novo_tabuleiro+count)=tabuleiro[i][j];
             //ciclos para passar o conteudo do tabuleiro para um vetor unidimensional
        }
    }

    //copiar a unformacao para o novo no
    novo->tabuleiro=novo_tabuleiro;
    novo->linhas=linhas;
    novo->colunas=colunas;
    novo->coluna_jogada=coluna_jogada;
    novo->linha_jogada=linha_jogada;
    novo->jogador=jogador;
    novo->prox=NULL;

    //free(novo_tabuleiro);   // novo_tabuleiro ja nao e necessario

    if(lista==NULL)
    {
        lista=novo;     // se a lista for NULL, adicionar o no no ao inicio da lista
    }

    else
    {
        phistorico aux=lista;
        while(aux->prox!=NULL)
        {
            aux=aux->prox;
        }
        aux->prox=novo;
        // se a lista nao for NULL, utilizar um aux para percorrer a lista ate ao fim e adicionar o novo nó, no fim da lista
    }

    return lista;

}

char aumentar_tabuleiro(int *linhas, int *colunas, char tabuleiro[][*colunas], char tab_aux[][(*colunas)+1])
{
    char alfabeto[14]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N'};
    (*linhas)++;
    (*colunas)++;

    int i,j;

    for(i=0; i<*linhas; i++)
    {
        for(j=0; j<*colunas; j++)
        {
            tab_aux[i][j]=tabuleiro[i][j]; // copiar o conteudo do tabuleiro para o tab_aux
        }
    }

    for(i=1; i<=*linhas; i++)
    {
        tab_aux[*linhas-1][i]='*'; // inserir comida na ultima coluna
    }

    for(j=1; j<*colunas-1; j++)
    {
        tab_aux[j][*colunas-1]='*';     //inserir comida na ultima linha
    }
    tab_aux[0][*colunas-1]=alfabeto[*colunas-2];
    tab_aux[*linhas-1][0]=*linhas+47;   // introduzir o numero da linha (linhas-1+48)-codigo ascii
    tab_aux[*linhas-1][*colunas-1]='X'; // introduzir o veneno na ultima posicao
    tab_aux[*linhas-2][*colunas-2]='*';

    mostra_tabuleiro(*linhas,*colunas,tab_aux); // onde tinha veneno passa a ter comida
}

void guardar_texto(char *nomefich, phistorico lista)
{
    FILE *f;
    f=fopen(nomefich,"wt");

    if(!f)
    {
        printf("Nao foi possivel abrir o ficheiro. Tente novamente mais tarde");
        return;
    }
    phistorico aux=lista;   // variavel aux para percorrer a lista ligada

    int i, cont=0,j;

    char coluna_jogada, vencedor;

    while(aux!=NULL)
    {
        coluna_jogada=converte_int_char(aux->coluna_jogada);       // funcao para converter numero da coluna em letra
        fprintf(f,"\n___________________________________\n");
        fprintf(f,"%d: Jogador %c \n Linha: %d \t Coluna: %c\n\n", cont, aux->jogador, aux->linha_jogada,coluna_jogada);
        // escreve no fich o numero da jogada, jogador e linha e coluna jogada

        //ciclo para guardar o tabuleiro no ficheiro txt

        for(i=0; i<aux->linhas*aux->colunas; i++)   // enquanto i<linhas*colunas
        {
            fprintf(f," %c |", *(aux->tabuleiro+i));
            if(((i+1)%aux->colunas) == 0 && i!=0)
                // mudar de linha quando o resto da divisao da posicao i+1 pelas colunas for 0 e nao for a primeira posicao
            {
                fprintf(f,"\n");
                for(j=0; j<aux->colunas; j++)
                {
                    fprintf(f,"----");
                }
                fprintf(f,"\n");
            }
        }
        vencedor=aux->jogador;      // o ultimo jogador guardado na variavel vencedor vai ser o vencedor do jogo
        aux=aux->prox;
        cont++; // incrementar o numero de jogadas total
    }
    fprintf(f,"Vencedor: %c ", vencedor);   // escrever o vencedor do jogo no ficheiro

    fclose(f);
}

void sucessao_jogadas_anteriores(phistorico lista)
{
    phistorico aux=lista;   // variavel auxiliar para percorrer a lista

    while(aux!=NULL)    // enquanto nao chegar ao fim da lista
    {
        printf("\nJogador: %c \n", aux->jogador);
        int i;

        mostra_tabuleiro(aux->linhas, aux->colunas, aux->tabuleiro);    //funcao para mostrar o tabuleiro
        printf("\n___________________________________\n");
        aux=aux->prox;
    }
}

void pausa_grava(phistorico lista, char *nomefich, int tipo_jogo)
{
    remove(nomefich);   //remover algum historico que exista anteriormente
    FILE *f;
    f=fopen(nomefich,"wb");
    if(!f)
    {
        printf("Nao foi possivel abrir o ficheiro. Tente novamente mais tarde");
        return;
    }

    fwrite(&tipo_jogo,sizeof(tipo_jogo),1,f);   //escreve no ficheiro o tipo de jogo - pode ser 1vs1 ou 1vsC

    phistorico aux=lista;
    phistorico aux1=lista;
    historico aux_struct;
    int cont=0;

    while(aux1!=NULL)
    {
        cont++;
        aux1=aux1->prox;
    }   // ciclo para contar o numero de estruturas a ser lidas
    fwrite(&cont,sizeof(cont),1,f); // escrever o numero de estruturas a ser lidas
    int i,j;
    while(aux!=NULL)
    {
        //conjunto de instruicoes para guardar o conteudo do nó, numa estrutura auxiliar
        aux_struct.linhas=aux->linhas;
        aux_struct.colunas=aux->colunas;
        aux_struct.jogador=aux->jogador;
        aux_struct.linha_jogada=aux->linha_jogada;
        aux_struct.coluna_jogada=aux->coluna_jogada;
        aux_struct.tabuleiro=aux->tabuleiro;

        fwrite(&aux_struct,sizeof(historico),1,f);  // escrever no ficheiro a estrutura auxiliar

        char tab_aux[aux->linhas][aux->colunas];

        for(i=0,cont=0; i<aux->linhas; i++)
        {
            for(j=0; j<aux->colunas; j++,cont++)
            {
                tab_aux[i][j]=*(aux_struct.tabuleiro+cont); // passar o tabuleiro do tipo char *, para um tabuleiro do tipo tab[l][c]
            }
        }

        putchar('\n');
        fwrite(&tab_aux,sizeof(tab_aux),1,f);   // escrever o tabuleiro no ficheiro
        aux=aux->prox;
    }
    fclose(f);

    system("cls");
}

phistorico pausa_le(phistorico lista, char *nomefich, int *tipo_jogo)
{
    FILE *f;
    f=fopen(nomefich,"rb");

    if(!f)
    {
        printf("\nNao existe nenhum jogo gravado previamente\n");
        return 0;
    }

    int tipo;

    fread(&tipo,sizeof(tipo_jogo),1,f); //ler o tipo de jogo, pode ser 1vs1 ou 1vsC
    *tipo_jogo=tipo;

    int num_estruturas;
    fread(&num_estruturas,sizeof(num_estruturas),1,f);
    // ler o numero de estruturas gravdas no ficheiro

    historico aux;

    int i;

    printf("\nJOGADAS ANTERIORES\n");

    for(i=0; i<num_estruturas; i++)
    {
        fread(&aux,sizeof(historico),1,f); // guardar a estrutura lida numa estrutura aux
        char tab_aux[aux.linhas][aux.colunas];
        fread(&tab_aux,sizeof(tab_aux),1,f);    // guardar o tabulerio lido num tab:aux
        lista=novo_ramo(lista,aux.linhas,aux.colunas,tab_aux,aux.jogador,aux.linha_jogada,aux.coluna_jogada);
         // adicionar novo nó na lista ligada, com a informacao lida
        printf("\nJogador: %c\n", aux.jogador);
        mostra_tabuleiro(aux.linhas,aux.colunas,tab_aux);   // funcao para mostrar o tabuleiro
        putchar('\n');
    }

    fclose(f);
    remove(nomefich);   // remover o ficheiro depois de ter a informacao toda lida
    return lista;
}

phistorico jogo_normal(phistorico lista)
{
    int l,c;
    dimensao(&l,&c);    // funcao para perguntar as dimensoes do tabuleiro

    char jogador='B';
    char tabuleiro[l][c];

    tabuleiro[l][c]=monta_tabuleiro(l, c, tabuleiro);  // funcao que cria o tabuleiro
    lista=novo_ramo(lista,l,c,tabuleiro,NULL,0,0);  // funcao para adicionar uma nova estrutura a lista ligada
    putchar('\n');
    mostra_tabuleiro(l,c,tabuleiro); // funcao que mostra o tabuleiro

    int lin, col_num, nova_acao,j;

    do
    {
        // trocar o jogador
        if(jogador=='A')
        {
            jogador='B';
        }
        else
        {
            jogador='A';
        }

        printf("\n\nJogador: %c\n", jogador);

        char tab_aux[l+1][c+1];
        int i,j;

        nova_acao=sub_menu();
        switch(nova_acao)
        {
        case 1:
            break;
        case 2:
            aumentar_tabuleiro(&l,&c,tabuleiro,&tab_aux);

             for(i=0;i<l-1;i++)
            {
                for(j=0;j<c-1;j++)
                {
                    tabuleiro[i][j]=tab_aux[i][j];
                }
            }
            system("pause");
            return;
        case 3:
            pausa_grava(lista,"pausar.dat",1); // funcao que grava o jogo ate ao momento
            return lista;
        case 4:
            sucessao_jogadas_anteriores(lista); // funcao para mostrar as jogadas anteriores
            printf("\nJogador %c", jogador);
        }

        pede_jogada(&lin,&col_num,l,c,tabuleiro);   // funcao para pedir a nova jogada ao utilizador

        tabuleiro[l][c]=muda_tabuleiro(lin, col_num,l,c, tabuleiro);  // funcao para alterar o tabuleiro de acordo com a jogada
        putchar('\n');
        mostra_tabuleiro(l, c, tabuleiro);  // mostrar o tabuleiro alterado

        lista=novo_ramo(lista,l,c,tabuleiro,jogador,lin,col_num);   // funcao para adicionar novo no a lista ligada
    }
    while(tabuleiro[l-2][c-1]!=' ' || tabuleiro[l-1][c-2]!=' ');   // Continuar jogo enquanto nas 2 posicoes ao lado do tabuleiro existir comido

    guardar_texto("historico.txt", lista);  // funcao para guardar o historico do jogo

    printf("\n\nO vencedor e o jogador %c\n\n", jogador);

    return lista;
}

phistorico jogo_automatico(phistorico lista)
{
    int l,c;
    char jogador='A';

    dimensao(&l, &c);   // pede ao utilizador a dimensao do tabuleiro
    char tabuleiro[l][c];

    tabuleiro[l][c]=monta_tabuleiro(l, c, tabuleiro);  // funcao que cria o tabuleiro
    putchar('\n');
    mostra_tabuleiro(l,c, tabuleiro); // funcao que mostra o tabuleiro

    lista=novo_ramo(lista,l,c,tabuleiro,jogador,0,0);   // funcao para adicionar um novo nó á lista ligada

    int lin, col_num, nova_acao,j,i;
    char col, tab_aux[l+1][c+1];

    do
    {
        if(jogador=='A')
        {
            printf("\n\nJogador: %c", jogador);

            nova_acao=sub_menu();
            switch(nova_acao)
            {
            case 1:
                break;  // caso seja nova acao, continuar a jogar
            case 2:
                aumentar_tabuleiro(&l,&c,tabuleiro,&tab_aux);

             for(i=0;i<l-1;i++)
            {
                for(j=0;j<c-1;j++)
                {
                    tabuleiro[i][j]=tab_aux[i][j];
                }
            }
            system("pause");
            return;

            case 3:
                pausa_grava(lista,"pausar.dat",0);  // funcao para guardar o jogo
                return lista;
            case 4:
                sucessao_jogadas_anteriores(lista);    //funcao para mostrar a sucessao de jogadas anteriores
                printf("Jogador: %c", jogador);
            }

            pede_jogada(&lin,&col_num,l,c,tabuleiro);   // funcao para pedir nova jogada ao utilizador

            muda_tabuleiro(lin, col_num,l,c, tabuleiro);  // funcao para alterar o tabuleiro de acordo com a jogada
            putchar('\n');
            mostra_tabuleiro(l, c, tabuleiro);  // mostrar o tabuleiro alterado

            lista=novo_ramo(lista,l,c,tabuleiro,jogador,lin,col_num);   // funcao para adicionar novo nó na lista ligada

            jogador='C';    // jogador passa a ser C
        }

        else    // se o jogador nao foi o A, entao é a vez do computador jogar
        {
            printf("\n\nJogador: %c\n", jogador);

            do
            {
                lin = rand() % (l-1) + 1;
                  // atribuir um numero aleatorio para a linha jogada entre 1 e as linhas totais do tabuleiro -1
                col_num = rand() % (c-1) + 1;
                 // atribuir um numero aleatorio para a coluna jogada entre 1 e as colunas totais do tabuleiro -1

            }
            while(tabuleiro[lin][col_num]==' ' || tabuleiro[lin][col_num]=='X');
            // voltar a fazer se a posicao aleatoria nao tiver comida ou for veneno

            col=converte_int_char(col_num); // converte o numero da coluna num caracter

            printf("Linha %d\n", lin);
            printf("Coluna %c\n", col);

            muda_tabuleiro(lin, col_num,l,c, tabuleiro);  // funcao para alterar o tabuleiro de acordo com a jogada
            putchar('\n');
            mostra_tabuleiro(l, c, tabuleiro);  // mostrar o tabuleiro alterado

            lista=novo_ramo(lista,l,c,tabuleiro,jogador,lin,col_num);   // funcao para adicionar um novo nó na lista ligada

            jogador='A';    // o jogador passa a ser o A
        }
    }
    while(tabuleiro[l-2][c-1]!=' ' || tabuleiro[l-1][c-2]!=' ');

    guardar_texto("historico.txt", lista);  // funcao para guardar o historico do jogo

    //trocar o jogador para anunciar o vencedor
    if(jogador=='A')
    {
        jogador='C';
    }
    else
    {
        jogador='A';
    }

    printf("\n\n\nO vencedor e o jogador: %c\n\n\n", jogador);

    return lista;
}

phistorico continua_jogo_gravado(phistorico lista, int tipo_jogo)
{
    int nova_acao;
    phistorico aux=lista;

    while(aux->prox!=NULL)
    {
        aux=aux->prox;  // utilizar um aux para pecorrer a lista ligada enquando o proximo nó nao for nulo
    }

    int c=aux->colunas,l=aux->linhas,lin, col_num,i,j, count;
    char *tab_aux=aux->tabuleiro;
    char tabuleiro[l][c];

    //ciclo para transformar o tabuleiro do tipo char* num tabuleiro do tipo tab[linhas][colunas]
    for(i=0,count=0; i<l; i++)
    {
        for(j=0; j<c; j++,count++)
        {
            tabuleiro[i][j]=*(tab_aux+count);
        }
    }

    char tab_aux1[l+1][c+1];

    char jogador=aux->jogador;
    char col;

    if(tipo_jogo==1) // se for um jogo 1vs1
    {
        do
        {
            //trocar jogador
            if(jogador=='A')
            {
                jogador='B';
            }
            else
            {
                jogador='A';
            }
            printf("\n\nJogador: %c\n", jogador);

            nova_acao=sub_menu();
            switch(nova_acao)
            {
            case 1:
                break;
            case 2:
                aumentar_tabuleiro(&l,&c,tabuleiro,&tab_aux1);

             for(i=0;i<l-1;i++)
            {
                for(j=0;j<c-1;j++)
                {
                    tabuleiro[i][j]=tab_aux1[i][j];
                }
            }
            system("pause");
            return;
            case 3:
                pausa_grava(lista,"pausar.dat",1);  // funcao para guardar o jogo no estado atual
                return lista;
            case 4:
                sucessao_jogadas_anteriores(lista); // funncao para mostrar a sucessao de jogadas anteriores
                printf("\nJogador %c", jogador);
            }

            pede_jogada(&lin,&col_num,l,c,tabuleiro);   // funcao para pedir os dados para a nova jogada

            tabuleiro[l][c]=muda_tabuleiro(lin, col_num,l,c, tabuleiro);  // funcao para alterar o tabuleiro de acordo com a jogada
            putchar('\n');
            mostra_tabuleiro(l, c, tabuleiro);  // mostrar o tabuleiro alterado
            lista=novo_ramo(lista,l,c,tabuleiro,jogador,lin,col_num);   // fncao para adicionar um novo nó na lista ligada

        }while(tabuleiro[l-2][c-1]!=' ' || tabuleiro[l-1][c-2]!=' ');   // continuar jogo enquanto as posicoes ao lado da ultima nao estiverem vazias

        printf("\n\n\nO vencedor e o jogador %c\n\n\n", jogador);
    }

    else    //se for um jogador 1vsC
    {
        jogador='A';
        do
        {
            if(jogador=='A')
            {
                printf("\n\nJogador: %c", jogador);

                nova_acao=sub_menu();
                switch(nova_acao)
                {
                case 1:
                    break;
                case 2:
                    aumentar_tabuleiro(&l,&c,tabuleiro,&tab_aux1);

             for(i=0;i<l-1;i++)
            {
                for(j=0;j<c-1;j++)
                {
                    tabuleiro[i][j]=tab_aux1[i][j];
                }
            }
            system("pause");
            return;

                case 3:
                    pausa_grava(lista,"pausar.dat",0);  // funcao para guardar jogo atual
                    return NULL;
                case 4:
                    sucessao_jogadas_anteriores(lista); // funcao para mostrar sucessao de ogadas anteriores
                    printf("Jogador: %c", jogador);
                }

                pede_jogada(&lin,&col_num,l,c,tabuleiro);   // funcao para pedir jogada ao utilizador

                muda_tabuleiro(lin, col_num,l,c, tabuleiro);  // funcao para alterar o tabuleiro de acordo com a jogada
                putchar('\n');
                mostra_tabuleiro(l, c, tabuleiro);  // mostrar o tabuleiro alterado

                lista=novo_ramo(lista,l,c,tabuleiro,jogador,lin,col_num);   // adicionar novo nó na lista ligada

                jogador='C';    // jogador passa a ser C
            }

            else    // se o jogador for C
            {
                printf("\n\nJogador: %c\n", jogador);

                do
            {
                lin = rand() % (l-1) + 1;   // atribuir um numero aleatorio para a linha jogada entre 1 e as linhas totais do tabuleiro -1
                col_num = rand() % (c-1) + 1;   // atribuir um numero aleatorio para a coluna jogada entre 1 e as colunas totais do tabuleiro -1

            }
            while(tabuleiro[lin][col_num]==' ' || tabuleiro[lin][col_num]=='X');    // voltar a fazer se a posicao aleatoria nao tiver comida ou for veneno

                col=converte_int_char(col_num); // funcao para connverter o numero da coluna num carcter

                printf("Linha %d\n", lin);
                printf("Coluna %c\n", col);

                muda_tabuleiro(lin, col_num,l,c, tabuleiro);  // funcao para alterar o tabuleiro de acordo com a jogada
                putchar('\n');
                mostra_tabuleiro(l, c, tabuleiro);  // mostrar o tabuleiro alterado

                lista=novo_ramo(lista,l,c,tabuleiro,jogador,lin,col_num);   //funcao para adicionar um novo nó na lista ligada

                jogador='A';
            }
        }while(tabuleiro[l-2][c-1]!=' ' || tabuleiro[l-1][c-2]!=' ');   // continuar jogo enquanto as posicoes ao lado da ultima nao estiverem vazias

        //trocar jogador para aunciar o vencedor
        if(jogador=='A')
        {
            jogador='C';
        }
        else
        {
            jogador='A';
        }
        printf("\n\n\nO vencedor e o jogador %c\n\n\n", jogador);
    }

}

void liberta_lista(phistorico lista)
{
    phistorico aux; // aux para guardar no

    while(lista=!NULL)
    {
        aux=lista;  // guardar no aux o nó atual para o qual a lista aponta
        lista=lista->prox;  // lista fica a apontar para o próximo
        free(aux);  // fazer free do aux
    }
}

void liberta_tabuleiro(phistorico lista)
{
    phistorico aux=lista;

    while(aux!=NULL)
    {
        free(aux->tabuleiro);
    }
}
int main()
{
    printf("Sejam bem-vindos ao jogo do Comilao.\n");

    phistorico lista=NULL;  //criacao de um ponteiro lista

    int i, tipo_jogo, aux=0;

    do
    {
        phistorico lista=NULL;
        i=menu();
        switch(i)
        {
        case 1:
            system("cls");
            lista=jogo_normal(lista); // funcao para jogar o jogo normal
            break;
        case 2:
            system("cls");
            lista=jogo_automatico(lista);// funcao para jogar o jogo automatico
            break;
        case 3:
            system("cls");
            lista=pausa_le(lista,"pausar.dat",&tipo_jogo);  // funcao para ler o conteudo do ficheiro binario para ser possivel retomar o jogo gravado
            if (lista==0)
            {
                break;              // se a lista for igual a 0 significa que nao foi possivel ler o ficheiro binário, então sair do case
            }
            lista=continua_jogo_gravado(lista, tipo_jogo);   // funcao para continuar a jogar o jogo gravado
            break;

        case 4:
            //sair
            break;
        }

    }while(i!=4);

    liberta_tabuleiro(lista);
    liberta_lista(lista);// funcao que liberta o espaco em memoria alocado anteriormente

}
