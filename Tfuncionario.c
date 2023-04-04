
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Tfuncionario.h"

void menuTabelaHash()
{
    printf("\n------------------------------------------- MENU DE OPCOES TABELA HASH -------------------------------------------\n");
    printf("\n1- Busca funcionario na tabela hash \n");
    printf("2- Deletar funcionario na tabela hash \n");
    printf("3- Imprimir tabela hash \n");
    printf("4- Voltar ao menu principal \n");
    printf("Informe a sua escolha: ");

}

void imprimirTabelaHash (FILE *file, char nomeParticao[])
{

    printf("\ncodigos de funcionarios nesta particao %s: \n ", nomeParticao);

    for (int i = 0; i < tamanhoArquivo(file, 0); ++i)
    {

        fseek(file, i * sizeof(TFunc), SEEK_SET) ;
        TFunc *aux = le(file);

        printf(" %d ", aux->codigo);
    }

    printf("\n");
}
TFunc *buscaTabelaHash(int codigo, int idParticao)
{

    char  nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";
    int comparacao = 0;

    itoa(idParticao, str1, 10);
    strcat(strcpy( nomeParticao, "particaoHash"), str1);
    strcat(strcpy( nomeParticao,  nomeParticao), str2);

    FILE *arquivoParticao = fopen( nomeParticao,"rb+");

    TFunc *auxFunc = buscaSequencial(codigo, arquivoParticao, &comparacao);

    fclose(arquivoParticao);

    return auxFunc;
}

void particaoTabelaHash (int maxParticao)
{

    for (int i = 0; i < maxParticao; ++i)
    {

        char  nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy( nomeParticao, "particaoHash"), str1);
        strcat(strcpy( nomeParticao,  nomeParticao), str2);

        FILE *file = fopen(nomeParticao,"wb+");

        fclose(file);
    }

}

void deletarTabelaHash(int codigo, int idParticao)
{

    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";
    int comparacao = 0;
    int deletado = 999999999;

    itoa(idParticao, str1, 10);
    strcat(strcpy( nomeParticao, "particaoHash"), str1);
    strcat(strcpy( nomeParticao, nomeParticao), str2);

    FILE *arquivoParticao = fopen( nomeParticao,"r+b");

    TFunc *auxFunc = buscaSequencial(codigo, arquivoParticao, &comparacao);

    if (auxFunc != NULL)
    {

        fseek(arquivoParticao, (comparacao - 1) * sizeof(TFunc), SEEK_SET);

        TFunc func;

        func.codigo = deletado;
        sprintf(func.nome, "%s", auxFunc->nome);
        sprintf(func.cpf, "%s", auxFunc->cpf);
        sprintf(func.data_nascimento, "%s", auxFunc->data_nascimento);
        func.salario = auxFunc->salario;

        salvarFuncionario(&func,arquivoParticao);

        printf("\n Funcionario deletado com sucesso !! \n");
    }
    else
    {
        printf("\n Funcionario nao exite na tabela hash \n");
    }

    fclose(arquivoParticao);
}

void criarTabelaHash(FILE *file, int maxParticao, int numFunc)
{

    rewind(file);

    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";

    for (int i = 0; i < numFunc; ++i)
    {

        TFunc *auxFunc = le(file);

        int particao = auxFunc->codigo % maxParticao;

        itoa(particao, str1, 10);
        strcat(strcpy(nomeParticao, "particaoHash"), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);


        FILE *arquivoParticao = fopen(nomeParticao,"ab+");

        salvarFuncionario(auxFunc, arquivoParticao);


        fclose(arquivoParticao);

    }

    for (int i = 0; i < maxParticao; ++i)
    {

        itoa(i, str1, 10);
        strcat(strcpy(nomeParticao, "particaoHash"), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *arquivoParticao = fopen(nomeParticao,"rb+");

        imprimirTabelaHash(arquivoParticao, nomeParticao);

        fclose(arquivoParticao);
    }
}


void salvar_no(TNo *no, FILE *arquivo)
{
    if (no == NULL || arquivo == NULL) return;

    fwrite(&no->m, sizeof(int), 1, arquivo);
    fwrite(&no->func, sizeof(TFunc *), 1, arquivo);
    fwrite(&no->pai, sizeof(TNo *), 1, arquivo);

    for (int i = 0; i < no->m; i++)
    {
        fwrite(&no->p[i], sizeof(TNo *), 1, arquivo);
    }

    fwrite(&no->p[no->m], sizeof(TNo *), 1, arquivo);

}


TNo *ler_no(FILE *arquivo)
{

    if (arquivo == NULL) return NULL;

    TNo *no = (TNo *)malloc(sizeof(TNo));

    fread(&no->m, sizeof(int), 1, arquivo);
    fread(&no->func, sizeof(TFunc *), 1, arquivo);
    fread(&no->pai, sizeof(TNo *), 1, arquivo);

    no->p = (TNo **)malloc((no->m + 1) * sizeof(TNo *));

    for (int i = 0; i <= no->m; i++)
    {
        no->p[i] = (TNo *)malloc(sizeof(TNo));
        fread(&no->p[i], sizeof(TNo *), 1, arquivo);
    }

    return no;
}


TNo *cria(int d)
{
    TNo *novo = (TNo *) malloc(sizeof(TNo));
    novo->m = 0;
    novo->pai = NULL;
    novo->func = (TFunc *) malloc(sizeof(TFunc) * (d * 2));
    novo->p = (TNo **) malloc(sizeof(TNo *) * (d * 2) + 1);
    for (int i = 0; i < (d * 2 + 1); i++)
    {
        novo->p[i] = NULL;
    }
    return novo;
}

// Função para remover um funcionário de um nó
void removerFuncionario(TNo* raiz, int codigo)
{

    if (raiz == NULL)
    {
        return NULL;
    }

    int i;
    for (i = 0; i < raiz->m && raiz->func[i].codigo < codigo; i++);

    if (i < raiz->m && raiz->func[i].codigo == codigo)
    {
        raiz->func[i] = raiz->func[i+1];
        printf("\n Funcionario com o codigo %d foi excluido com sucesso da arvore B \n",codigo);

    }
    else if (raiz->p[i] != NULL)
    {
        return removerFuncionario(raiz->p[i], codigo);
    }
    else
    {
        printf("\n Funcionario com o codigo %d nao foi encontrado para exclusao na arvore b \n", codigo);
        return NULL;
    }
}

TFunc* buscarFuncionario(TNo* raiz, int codigo)
{
    // Verifica se o nó raiz é NULL ou se o código é inválido
    if (raiz == NULL || codigo < 0 )
    {
        return NULL;
    }

    int i = 0;

    while (i < raiz->m && raiz->func[i].codigo < codigo)
    {
        i++;
    }

    if (i < raiz->m && raiz->func[i].codigo == codigo)
    {
        // O funcionário foi encontrado
        return &(raiz->func[i]);
    }
    else if (raiz->p[i] != NULL)
    {
        // O funcionário pode estar em um nó filho
        return buscarFuncionario(raiz->p[i], codigo);
    }
    else
    {
        // O funcionário não foi encontrado
        return NULL;
    }
}

TNo *libera(TNo *a, int d)
{
    if (a != NULL)
    {
        for (int i = 0; i <= d * 2 + 1; i++)
        {
            libera(a->p[i], d);
        }
        free(a->func);
        free(a->p);
        free(a);
    }
    return NULL;
}



TNo *busca(TNo *no, int codigo)
{
    if (no != NULL)
    {
        int i = 0;
        while (i < no->m && codigo > no->func[i].codigo)
        {
            i++;
        }
        if (i < no->m && codigo == no->func[i].codigo)
        {
            return no; // encontrou chave
        }
        else if (no->p[i] != NULL)
        {
            return busca(no->p[i], codigo);
        }
        else
        {
            return no; // nó era folha -- não existem mais nós a buscar, então retorna o nó onde a chave deveria estar
        }
    }
    else
    {
        return NULL; // nó é NULL, não há como buscar
    }
}




TNo *insere_folha(TNo *raiz, int d, TFunc *func)
{
    if (raiz != NULL)
    {
        TNo *no = busca(raiz, func->codigo);
        int pos = posicao(func->codigo, no);
        if (pos == no->m || func->codigo != no->func[pos].codigo)   // chave não existe no nó
        {
            insere(no, d, pos, func, NULL);
            if (raiz->pai != NULL)   // Inserção criou uma nova raiz
            {
                raiz = raiz->pai;
            }
        }
    }
    else
    {
        raiz = cria(d);
        raiz->func[0] = *func;
        raiz->m = 1;
    }

    return raiz;
}

void insere(TNo *no, int d, int pos, TFunc *func, TNo *pt)
{
    if (pos == -1)
    {
        pos = posicao(func->codigo, no);
    }

    if (no->m < 2 * d)   // Tem espaço no nó, então pode inserir diretamente
    {
        for (int i = no->m; i > pos; i--)   // Abre espaço para a inserção
        {
            no->func[i] = no->func[i - 1];
            no->p[i + 1] = no->p[i];
        }
        no->func[pos] = *func;
        no->p[pos + 1] = pt;
        if (pt != NULL)
        {
            pt->pai = no;
        }
        no->m++;
    }
    else     // Nó está cheio -- é necessário particionar
    {
        particiona(no, d, pos, func, pt);
    }


}

void particiona(TNo *P, int d, int pos, TFunc *func, TNo *pt)
{
    // Prepara Q
    TNo *Q = cria(d);
    int i = d * 2 - 1;
    int j = d - 1;
    while (j >= 0)
    {
        if (i != pos - 1)   // Move chave de P para Q
        {
            Q->func[j] = P->func[i];
            Q->p[j + 1] = P->p[i + 1];
            if (Q->p[j + 1] != NULL)
            {
                Q->p[j + 1]->pai = Q;
            }
            i--;
        }
        else     // Insere chave em Q
        {
            Q->func[j] = *func;
            Q->p[j + 1] = pt;
            if (Q->p[j + 1] != NULL)
            {
                Q->p[j + 1]->pai = Q;
            }
        }
        j--;
    }
    Q->m = d;

    // Processa P
    P->m = d;
    if (pos <= d)
    {
        insere(P, d, pos, func, pt); // inserção vai fazer o número de chaves aumentar para d+1, e isso é tratado no final dessa função
    }

    // Processa W
    if (P->pai == NULL)   // Caso o nó seja raiz, cria o nó superior
    {
        TNo *W = cria(d);
        W->p[0] = P;
        P->pai = W;
    }
    // Chave d de P sobe para o pai W
    insere(P->pai, d, -1, &P->func[d], Q);
    // Ajusta o ponteiro da esquerda de Q (recebe ponteiro da direita da chave que subiu para o pai W)
    Q->p[0] = P->p[d+1];
    if (Q->p[0] != NULL)
    {
        Q->p[0]->pai = Q;
    }
    // A inserção da chave d em W fez o número de chaves de P diminuir para D
    P->m = d;
}


int posicao(int codigo, TNo *no)
{
    int inicio = 0;
    int fim = no->m;
    int pos = (fim + inicio) / 2;
    while (pos != no->m && codigo != no->func[pos].codigo && inicio < fim)
    {
        if (codigo > no->func[pos].codigo)
        {
            inicio = pos + 1;
        }
        else
        {
            fim = pos;
        }
        pos = (fim + inicio) / 2;
    }
    return pos;
}


void imprime_arvore(TNo *a, int nivel)
{
    if (a != NULL)
    {
        for (int i = a->m; i > 0; i--)
        {
            imprime_arvore(a->p[i], nivel + 1);
            printf("Codigo: %d\n", a->func[i - 1].codigo);
            printf("Nome: %s\n", a->func[i - 1].nome);
            printf("Data de nascimento: %s\n", a->func[i - 1].data_nascimento);
            printf("CPF: %s\n", a->func[i - 1].cpf);
            printf("Salario: %.2f\n\n", a->func[i - 1].salario);

        }
        imprime_arvore(a->p[0], nivel + 1);
    }
}

void imprime_no(TNo *a)
{
    if (a != NULL)
    {
        for (int i = 0; i < a->m; i++)
        {
            printf("%s ",a->func[i].nome);
        }
        printf("\n");
    }
    else
    {
        printf("deu erro ao imprimir passou null");
    }
}


void salvarDadosInsertionSortEmDisco(int numeroDeComparacoes, double tempoGasto,FILE *arquivo)
{
    fprintf(arquivo, "\nTempo gasto na execucao do metodo: %f\n",tempoGasto);
    fprintf(arquivo, "Numero de comparacoes: %d\n",numeroDeComparacoes);
}

void salvarDadosOrdenacaoExterna(FILE *arquivo, double tempoSelecao, double tempoIntercalacao)
{
    fprintf(arquivo,"\n Tempo gasto na execucao do metodo de selecao por substituicao: %f",tempoSelecao);
    fprintf(arquivo,"\n Tempo gasto na execucao do metodo de intercalacao otima: %f",tempoIntercalacao);

}

void salvarResultados(TFunc *func, FILE *arquivo, int maxComparacoes, double tempoDeBusca)
{
    int i = 0 ;
    fprintf(arquivo, "\ncodigo: %d\n", func->codigo);
    fprintf(arquivo, "Nome: %s\n", func->nome);
    fprintf(arquivo, "salario: %.2f\n", func->salario);
    fprintf(arquivo, "cpf: %s\n", func->cpf);
    fprintf(arquivo, "data de nascimento: %s\n", func->data_nascimento);
    fprintf(arquivo, "numero de comparacoes: %d \n", maxComparacoes);
    fprintf(arquivo, "tempo de busca: %f \n", tempoDeBusca);
    fprintf(arquivo, "------------------------------------------------------------------------------------------------- ");

}


int selecaoPorSubstituicao (FILE *file, char nameFilePartition[])
{
    // Declaração e inicialização de variáveis
    int numeroDeParticoes = 0;
    int contadorTamanhoArquivo = 0;
    int posicao = 6 ;
    int posicaoMenorElemento = 0;
    int menorElemento = 999999;

    // Declaração e inicialização de vetores
    TFunc func[6];
    int auxiliarVetorFuncionarios[6] = {0, 0, 0, 0, 0, 0}, i;

    // Posiciona o cursor no início do arquivo
    rewind(file);

    // Obtém o tamanho do arquivo
    int tamanhoArquivoAux = tamanhoArquivo(file,contadorTamanhoArquivo);

    // Lê os primeiros 6 funcionários do arquivo e armazena em um vetor
    for (i = 0; i < 6; ++i)
    {
        fseek(file, i * sizeof(TFunc), SEEK_SET);

        TFunc *aux = le(file);
        func[i] = *aux;

    }

    // Copia os códigos dos funcionários do vetor "func" para o vetor "auxiliarVetorFuncionarios"
    for (int i = 0; i < 6; ++i)
    {
        auxiliarVetorFuncionarios[i] = func[i].codigo;
    }

    // Realiza a ordenação externa por seleção por substituição
    while (posicao != tamanhoArquivoAux)
    {
        // Cria um arquivo para a partição atual
        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(numeroDeParticoes, str1, 10);
        strcat(strcpy(nomeParticao, nameFilePartition), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao, "wb+");

        // Declaração e inicialização de variáveis e vetores
        int auxVetFrozen[6] = {0, 0, 0, 0, 0, 0,};

        // Realiza a ordenação interna para a partição atual
        while (posicao != tamanhoArquivoAux)
        {
            // Encontra o menor elemento no vetor "auxiliarVetorFuncionarios"
            menorElemento = 9999999;

            for (i = 0; i < 6; ++i)
            {

                if (menorElemento > auxiliarVetorFuncionarios[i] && auxVetFrozen[i] != 1)
                {
                    menorElemento = auxiliarVetorFuncionarios[i];
                    posicaoMenorElemento = i;
                }
            }
            // Salva o menor elemento no arquivo da partição atual
            salvarFuncionario(&func[posicaoMenorElemento], filePartition);

            fseek(file, posicao * sizeof(TFunc), SEEK_SET);
            // Lê o próximo funcionário do arquivo e atualiza as variáveis e vetores
            TFunc *aux = le(file);

            posicao++;

            auxiliarVetorFuncionarios[posicaoMenorElemento] = aux->codigo;
            func[posicaoMenorElemento] = *aux;

            // Marca a posição do vetor "auxiliarVetorFuncionarios" correspondente ao menor elemento como congelada
            if (aux->codigo < menorElemento)
            {
                auxVetFrozen[posicaoMenorElemento] = 1;
            }

            if(allVetFrozen(auxVetFrozen) == 1)
            {
                numeroDeParticoes++;
                break;
            }
        }

        fclose(filePartition);

        if (posicao >= tamanhoArquivoAux)
        {
            break;
        }

    }

    char nomeParticao[100]; //  definido um nome para o arquivo de saída
    char str1[100];
    char str2[100] = ".dat";

    itoa(numeroDeParticoes, str1, 10);
    strcat(strcpy(nomeParticao, nameFilePartition), str1);
    strcat(strcpy(nomeParticao, nomeParticao), str2);

    FILE *filePartitionFinal = fopen(nomeParticao, "ab+");

    int k, j;

    TFunc funcAux;
    int numComparacoes = 0;


    for (k = 1; k < 6; k++)  // algoritmo Bubble Sort para a ordenação
    {

        for (j = 0; j < 6 - 1; j++)
        {

            if (func[j].codigo> func[j + 1].codigo)
            {
                numComparacoes++;
                funcAux = func[j];
                func[j] = func[j + 1];
                func[j + 1] = funcAux;
            }

        }
    }

    for (i = 0; i < 6; ++i)
    {
        salvarFuncionario(&func[i], filePartitionFinal); //os registros de cada funcionário são gravados no arquivo
    }

    fclose(filePartitionFinal);

    for (i = 0; i <= numeroDeParticoes; ++i) //  nomeParticao é atualizada para conter o nome de cada partição gravada
    {


        itoa(i, str1, 10); // conversão de inteiro para string
        strcat(strcpy(nomeParticao, nameFilePartition), str1); // cria o nome do arquivo de partição a partir da string
        strcat(strcpy(nomeParticao, nomeParticao), str2); // / adiciona a extensão .dat ao nome do arquivo de partição

    }

    return numeroDeParticoes;  // retorno o numero de particao gerada

}


void intercalacaoOtima(int numberOfPartition, char nameFilePartition[] )
{

    int *vetSizePartition = (int *) malloc(numberOfPartition * sizeof(int));
    int *vetFinalPartition = (int *) malloc(numberOfPartition * sizeof(int));
    int *vetPositionPartition = (int *) malloc(numberOfPartition * sizeof(int));
    int *vetValueEmployeePartition = (int *) malloc(numberOfPartition * sizeof(int));
    int flagAuxFinal = 0, count, smallElement = INT_MAX, smallElementPosition = 0, i;

    FILE *arquivoFinal = fopen("BaseDeDadosFinal.dat", "wb+");

    for (i = 0; i <= numberOfPartition; ++i)
    {

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(i, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "rb+");

        rewind(filePartition);

        vetSizePartition[i] = tamanhoArquivo(filePartition, 0);
        vetFinalPartition[i] = 0;
        vetPositionPartition[i] = 0;

        fseek(filePartition, vetPositionPartition[i] * sizeof(TFunc), SEEK_SET);
        TFunc *auxFunc = le(filePartition);

        vetValueEmployeePartition[i] = auxFunc->codigo;

        fclose(filePartition);
    }


    while (flagAuxFinal != 1)
    {

        count = 0;

        for (i = 0; i <= numberOfPartition; ++i)
        {

            if (vetFinalPartition[i] == 1)
            {
                count++;
            }

            if (count == numberOfPartition)
            {
                flagAuxFinal = 1;
            }
        }

        for (i = 0; i <= numberOfPartition; ++i)
        {

            if (vetValueEmployeePartition[i] < smallElement && vetFinalPartition[i] != 1)
            {
                smallElement = vetValueEmployeePartition[i];
                smallElementPosition = i;
            }
        }

        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        itoa(smallElementPosition, str1, 10);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(strcpy(partitionName, partitionName), str2);

        FILE *filePartition = fopen(partitionName, "rb+");

        rewind(filePartition);
        fseek(filePartition, vetPositionPartition[smallElementPosition] * sizeof(TFunc), SEEK_SET);
        TFunc *auxFunc = le(filePartition);
        salvarFuncionario(auxFunc, arquivoFinal);
        vetPositionPartition[smallElementPosition]++;

        rewind(filePartition);

        if (vetPositionPartition[smallElementPosition] >= vetSizePartition[smallElementPosition])
        {
            vetFinalPartition[smallElementPosition] = 1;
        }
        else
        {
            fseek(filePartition, vetPositionPartition[smallElementPosition] * sizeof(TFunc), SEEK_SET);
            TFunc *auxFunc2 = le(filePartition);
            vetValueEmployeePartition[smallElementPosition] = auxFunc2->codigo;
        }

        fclose(filePartition);

        smallElement = INT_MAX;

    }


    fclose(arquivoFinal);
    free(vetFinalPartition);
    free(vetSizePartition);
    free(vetPositionPartition);
    free(vetValueEmployeePartition);
}


int tamanhoArquivo(FILE *file, int contSizeFile)
{

    int bytesAUX = 0;

    while(!feof(file))
    {

        fseek(file, bytesAUX * sizeof(TFunc), SEEK_SET);

        TFunc *aux = le(file);
        if(aux != NULL)
        {
            contSizeFile++;
        }

        bytesAUX++;
    }

    return contSizeFile;
}

int allVetFrozen (int vet[6])
{

    int cont = 0, i;

    for (i = 0; i < 6; ++i)
    {
        if (vet[i] == 1)
        {
            cont++;
        }
    }

    if (cont == 6)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void CodigoAleatorio (int *vet, int maxFun)
{
    for (int i = 0; i < maxFun; i++)
    {
        vet[i] = i + 1;
    }

    for (int i = 0; i < maxFun; i++)
    {
        int aleatorio = rand() % maxFun;
        int aux = vet[i];
        vet[i] = vet[aleatorio];
        vet[aleatorio] = aux;
    }

}


TFunc *criarfuncionario(int codigo, char *nome, char *cpf, char *data_nascimento, double salario)
{
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (func) memset(func, 0, sizeof(TFunc));
    func->codigo = codigo;
    strcpy(func->nome, nome);
    strcpy(func->cpf, cpf);
    strcpy(func->data_nascimento, data_nascimento);
    func->salario = salario;
    return func;
}

void salvarFuncionario(TFunc *func, FILE *arquivo)
{
    fwrite(&func->codigo, sizeof(int), 1, arquivo);
    fwrite(func->nome, sizeof(char), sizeof(func->nome), arquivo);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), arquivo);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), arquivo);
    fwrite(&func->salario, sizeof(double), 1, arquivo);
}

TFunc *le(FILE *arquivo)
{

    TFunc *func = (TFunc*) malloc (sizeof(TFunc));

    if ( 0 >= fread(&func->codigo, sizeof(int), 1, arquivo))
    {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), arquivo);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), arquivo);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), arquivo);
    fread(&func->salario, sizeof(double), 1, arquivo);

    return func;
}

TFunc *buscaSequencial(int codigo, FILE *arquivo, int *contador)
{

    int posicao = 0;

    while (!feof(arquivo))
    {

        fseek(arquivo, posicao * sizeof(TFunc), SEEK_SET) ;
        TFunc *func = le(arquivo);

        posicao++;

        if (func == NULL)
        {
            *contador += 1;
            return NULL;
        }
        else if (codigo == func->codigo)
        {
            *contador += 1;
            return func;
        }
        else
        {
            *contador+= 1;
        }

    }

    return NULL;
}

void menu()
{

    printf("\n------------------------------------------- MENU DE OPCOES -------------------------------------------\n\n");
    printf("Digite -1 para sair do Loop \n");
    printf("1- Busca Sequencial (Arquivo nao ordenado)\n");
    printf("2- Ordenacao pelo metodo insertion sort em disco\n");
    printf("3- ordenacao pelo metodo de selecao por substituicao e intercalacao otima\n");
    printf("4- Busca Binaria (Arquivo Ordenado)\n");
    printf("5- Criar Arvore b e Carregar base de dados\n");
    printf("6- Busca Funcionario na arvore b\n");
    printf("7- Exclusao de Funcionario da arvore b\n");
    printf("8- Criar Tabela Hash e Carregar base de dados  \n");

    printf("Informe a sua escolha: ");

}

int verificarAberturaArquivo(FILE *arquivo)
{

    if(arquivo == NULL)
    {
        printf("Erro na abertura do arquivo \n");
        return 0 ;
    }
    else
    {
        return 1 ;
    }

}


TFunc *buscaBinaria(int codigo, FILE *arquivo, int maxFunc, int *contador)
{

    int esquerda = 0, direita = maxFunc - 1;

    rewind(arquivo);

    while (esquerda <= direita)
    {

        int meio = (esquerda+direita) / 2;

        fseek(arquivo, meio * sizeof(TFunc), SEEK_SET);

        TFunc *func = le(arquivo);

        if (codigo == func ->codigo)
        {
            *contador += 1;
            return func;
        }
        else if (func->codigo < codigo)
        {
            *contador+= 1;
            esquerda = meio+1;
        }
        else
        {
            *contador += 1;
            direita = meio-1;
        }
    }

    return NULL;
}

int tamanho_registro()
{
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(double) //salario
           + sizeof(char) * 11
           + sizeof(char) * 15;
}

void imprime_arquivo(FILE *arq)
{
    rewind(arq);
    TFunc *f = le(arq);
    int i = 0;
    while (!feof(arq))
    {
        imprimirFuncionario(f);
        f = le(arq);
    }
}


int insertion_sort_disco(FILE *arq, int tam)
{
    printf("\n Aguarde esta sendo realizada a ordenacao no arquivo... \n");
    int i;
    int numeroComparacoes = 0;
    for (int j = 2; j <= tam; j++)
    {
        fseek(arq, (j-1) * tamanho_registro(), SEEK_SET);
        TFunc *fj = le(arq);
        i = j - 1;
        fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
        TFunc *fi = le(arq);
        while ((i > 0) && (fi->codigo > fj->codigo))
        {
            numeroComparacoes++;
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            salvarFuncionario(fi, arq);
            i = i - 1;
            fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
            fi = le(arq);
        }


        fseek(arq, (i) * tamanho_registro(), SEEK_SET);
        salvarFuncionario(fj, arq);
    }
    return numeroComparacoes ;
    fflush(arq);
}

void criarBaseDeDados( FILE *arq, int maxFun, int *vet,  char nomes[][20])
{
    srand(time(NULL));
    for(int i = 1; i <= maxFun; i++)
    {
        int aleatorio = rand() % 100 ; // 100 = quantidade de nomes no array de nomes
        TFunc func;
        func.codigo = vet[i - 1];
        sprintf(func.nome, "%s", nomes[aleatorio]);
        sprintf(func.cpf, "111.111.111-11");
        sprintf(func.data_nascimento, "02/03/1999");
        func.salario = 1000 * i;

        fseek(arq, (i-1)* sizeof(TFunc), SEEK_SET);
        salvarFuncionario(&func, arq);
    }
    printf("Base de dados com %d funcionarios criado com sucesso \n\n",maxFun);
}


void imprimirFuncionario (TFunc *func)
{
    printf("\n");
    printf("Codigo: %d\n", func->codigo);
    printf("Nome: %s\n", func->nome);
    printf("CPF: %s\n", func->cpf);
    printf("Data de nascimento: %s\n", func->data_nascimento);
    printf("Salario: %.1f\n", func->salario);
}

