
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int codigo;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;

} TFunc;

typedef struct No
{
    int m;
    struct No *pai;
    TFunc *func;
    struct No **p;
} TNo;

void menuTabelaHash();

TFunc *buscaTabelaHash(int codigo, int idParticao) ;

void particaoTabelaHash (int maxParticao) ;

void deletarTabelaHash(int codigo, int idParticao);

void criarTabelaHash(FILE *file, int maxParticao, int numFunc);

void imprimirTabelaHash(FILE *file, char nomeParticao[]);

void salvar_no(TNo *no, FILE *arquivo) ;

TNo *ler_no(FILE *arquivo);

void removerFuncionario(TNo* raiz, int codigo);

TFunc* buscarFuncionario(TNo* raiz, int codigo) ;

void insere(TNo *no, int d, int pos, TFunc *func, TNo *pt);

TNo *insere_folha(TNo *raiz, int d, TFunc *func);

void particiona(TNo *P, int d, int pos, TFunc *func, TNo *pt);

void imprime_arvore(TNo *a, int nivel);

void imprime_no(TNo *a);

int posicao(int codigo, TNo *no);

int allVetFrozen (int vet[6]);

int tamanhoArquivo(FILE *file, int contSizeFile) ;

int selecaoPorSubstituicao (FILE *file, char nameFilePartition[]) ;

void intercalacaoOtima (int numberOfPartition, char nameFilePartition[]) ;

void imprime_arquivo(FILE *arq) ;

int tamanho_registro();

int insertion_sort_disco(FILE *arq, int tam) ;

void menu();

void CodigoAleatorio(int *vet, int maxFun);

void criarBaseDeDados (FILE *arq, int nFunc, int *vet, char nomes[][20]) ;

TFunc *buscaBinaria(int codigo, FILE *arquivo, int maxFun, int *contador) ;

void imprimirFuncionario (TFunc *func) ;

TFunc *buscaSequencial(int codigo, FILE *arquivo, int *contador);

TFunc *criarfuncionario(int codigo, char *nome, char *cpf, char *data_nascimento, double salario);

void salvarFuncionario(TFunc *func, FILE *arquivo);

void salvarResultados(TFunc *func, FILE *arquivo, int maxComparacoes, double tempoDeBusca);

TFunc *le(FILE *arquivo);

int verificarAberturaArquivo(FILE *arquivo);

void salvarDadosInsertionSortEmDisco(int numeroDeComparacoes, double tempoGasto,FILE *arquivo);


