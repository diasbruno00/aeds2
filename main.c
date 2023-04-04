#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Tfuncionario.h"


int main()
{
    // criando array de nomes para ser inserido aletoriamente em cada funcionario
    char nomes[100][20] = { "Bruno", "Lucas", "Pedro", "Jose", "Mariana", "luana", "Luiz", "lorena", "maria", "Matheus",
                            "Margarida", "Sara", "Raquel", "Fernando", "Thiago", "luiz", "nayara", "maite", "Aurora", "Helena",
                            "Cecilia", "Alice", "Livia", "Eleo", "liz", "Julia", "Beatriz", "Maya", "Ayla", "laura", "Antonella",
                            "laura", "Luna", "Noah", "Miguel", "gael", "Ravi", "Gabriel", "lucas", "Benicio", "joao", "samuel",
                            "davi", "Levi", "guilerme", "Artur", "lavinia", "ines", "liam", "Alex ", "sandro", "Alex", "Artur",
                            "fabio", "Raabe", "Ester", "Camila", "Vanessa", "luan", "Bianca", "Priscila", "Marcos vinicis ",
                            "Marcos vinicius", "vinicius", "Francisco", "Alberto", "Alexandre", "Alfredo", "guga", "Michel", "Luan",
                            "Tardeli", "Ronaldo", "Messi", "Zidane", "cristiano", "beto", "romario", "caio ribeiro", "galvao",
                            "marta", "Apolo", "Pele", "Maradona", "Augusto", "Breno", "Caetano", "celso", "charles", "caio",
                            "danilo", "fabricio", "Roger guedes ", "fagner", "tulio", "cassio", "yuri alberto ", "heitor", "nelio",
                            "jose"
                          };

    int MaxFuncionario = 0;
    printf("Digite a quantidade de  funcionarios deseja que seja criados na base de dados:  ");
    scanf("%d",&MaxFuncionario);

    int array[MaxFuncionario];

    CodigoAleatorio(array,MaxFuncionario);

    FILE *arquivo = fopen("BaseDeDadosInicial.dat","wb+");

    criarBaseDeDados(arquivo,MaxFuncionario,array,nomes);

    int opcao = 0;
    TNo *raiz = NULL;
    int ordem = 5;

    do
    {
        menu();
        scanf("%d",&opcao);

        if(opcao == 1)
        {

            int idBusca = 0;

            srand(time(NULL));

            for(int i = 0 ; i <  50; i++)
            {
                int totalComparacoes = 0;

                double tempoDeBusca = 0.0;

                clock_t inicio = clock();

                idBusca = rand() % MaxFuncionario ;

                TFunc *func = buscaSequencial(idBusca,arquivo,&totalComparacoes);

                clock_t fim = clock();

                tempoDeBusca += (double)(fim - inicio) / CLOCKS_PER_SEC;

                if (func == NULL)
                {
                    printf("\nFuncionario nao encontrado\n");
                }
                else
                {
                    printf("\nResultado da busca sequencial: \n");

                    imprimirFuncionario(func);

                    printf("Quantidade de comparacoes realizadas: %d \n",totalComparacoes);

                    printf("Tempo gasto para localizar o funcionario: %f\n", tempoDeBusca);

                    FILE *arquivoResultadoBuscaSequencial = fopen("arquivoResultadosBuscasSequencial.txt","a");

                    salvarResultados(func,arquivoResultadoBuscaSequencial,totalComparacoes,tempoDeBusca);

                    fclose(arquivoResultadoBuscaSequencial);
                }

            }

        }

        else if( opcao == 2)
        {
            int idBusca = 0;
            int contador =  0 ;

            double tempoDeExecucao = 0 ;

            clock_t comeco = clock();

            contador = insertion_sort_disco(arquivo,MaxFuncionario);
            printf("numero de comparacoes para ordenar o metodo: %d \n",contador);
            clock_t termino = clock();

            tempoDeExecucao += (double)(termino - comeco) / CLOCKS_PER_SEC;
            printf("tempo gasto na execucao do metodo insertion Sort: %.3f \n",tempoDeExecucao);

            printf("Arquivo Ordenado com sucesso \n");

            FILE *arquivo= fopen("ArquivoResultadoInsertionSortEmDisco.txt","a");
            salvarDadosInsertionSortEmDisco(contador,tempoDeExecucao,arquivo);

            fclose(arquivo);

        }
        else if(opcao == 3)
        {
            double tempoDeExecucaoIntercalacao = 0.0;
            double tempoDeExecucaoSelecao = 0.0;

            clock_t a = clock();
            int numeroParticoes = selecaoPorSubstituicao (arquivo,"particao");
            clock_t b = clock();
            tempoDeExecucaoSelecao += (double)(b- a) / CLOCKS_PER_SEC;

            printf("Numero de particoes geradas: %d \n",numeroParticoes);

            clock_t x = clock();
            intercalacaoOtima(numeroParticoes,"particao");
            clock_t y = clock();

            tempoDeExecucaoIntercalacao += (double)(y - x) / CLOCKS_PER_SEC;
            printf("Tempo de execucao do metodo intercalacao otima: %.5f \n", tempoDeExecucaoIntercalacao);

            printf("Arquivo Ordenado com sucesso \n");

            FILE *arquivo = fopen("ArquivoResultadosOrdenacaoExterna.txt", "a");

            salvarDadosOrdenacaoExterna(arquivo,tempoDeExecucaoSelecao,tempoDeExecucaoIntercalacao);
            printf("Dados salvos no arquivo \n");

            fclose(arquivo);


        }
        else if(opcao == 4)
        {
            FILE *arquivoFinal = fopen("BaseDeDadosFinal.dat", "rb+");

            if(arquivoFinal != NULL)
            {

                for(int i = 0 ; i < 50; i++)
                {
                    int totalComparacoes = 0;
                    int idBusca = rand() % MaxFuncionario ;
                    double tempoGasto = 0 ;

                    clock_t x = clock();

                    TFunc *func = buscaBinaria(idBusca,arquivoFinal,MaxFuncionario,&totalComparacoes);

                    clock_t y = clock();

                    tempoGasto += (double)(y - x) / CLOCKS_PER_SEC;

                    if (func == NULL)
                    {
                        printf("\nFuncionario nao encontrado.\n");
                    }
                    else
                    {
                        printf("\n Resultado da Busca Binaria: \n");

                        imprimirFuncionario(func);

                        FILE *arquivoResultadoBuscaBinaria = fopen("arquivoResultadosBuscasBinaria.txt","a");

                        salvarResultados(func,arquivoResultadoBuscaBinaria,totalComparacoes,tempoGasto);

                        fclose(arquivoResultadoBuscaBinaria);
                    }

                }
                fclose(arquivoFinal);
            }
            else
            {
                printf("Arquivo nao encontrado,e necessario primeiro gerar o arquivo ordenado, escolha a opcao 3 do menu e tente novamente \n");
            }

        }
        else if (opcao == 5)
        {
            TFunc *func;
            FILE *arquivo = fopen("BaseDeDadosInicial.dat","rb+");

            FILE *arquivoArvoreB = fopen("NoArvoreB.dat","wb+");

            if (arquivo == NULL)
            {
                printf("Erro ao abrir o arquivo\n");
            }
            else
            {
                func = malloc(sizeof(TFunc));

                int contador = 0;
                int x = 0 ;

                printf("\n Digite o numero de funcionarios que deseja carregar da base de dados e inserir na arvore b: ");
                scanf("%d",&x);

                if(x <= MaxFuncionario)
                {

                    while(fread(func, sizeof(TFunc), 1, arquivo) == 1 && contador < x)
                    {
                        printf("funcionario de codigo %d foi inseriondo na arvore b\n",func->codigo);
                        raiz = insere_folha(raiz,ordem,func);
                        contador++;
                    }

                    salvar_no(raiz, arquivoArvoreB);
                    printf("\n Arvore B gravada com %d funcionarios com sucesso no arquivo\n",x);

                    free(func);
                    fclose(arquivo);
                    fclose(arquivoArvoreB);

                }
                else
                {
                    printf("A base de dados contem %d funcionarios, voce solicitou %d nao foi possivel realizar a insercao \n",MaxFuncionario,x);
                }


            }

        }
        else if(opcao == 6)
        {

            FILE *arquivoArvoreB = fopen("NoArvoreB.dat","rb+");

            if (arquivoArvoreB == NULL)
            {
                printf("Erro ao abrir o arquivo\n");
            }
            else
            {


                TNo *no_lido = ler_no(arquivoArvoreB);

                int idBusca ;
                printf("\nDigite o codigo do funcionario que deseja realizar a busca na arvore b: ");
                scanf("%d",&idBusca);

                TFunc *funcionario = buscarFuncionario(no_lido, idBusca);

                if (funcionario != NULL)
                {
                    imprimirFuncionario(funcionario);
                }
                else
                {
                    printf("\n Funcionario com o codigo %d nao encontrado na arvore B\n",idBusca);
                }

                fclose(arquivoArvoreB);
            }

        }
        else if(opcao == 7)
        {
            FILE *arquivoArvoreB = fopen("NoArvoreB.dat","rb+");

            if (arquivoArvoreB == NULL)
            {
                printf("Erro ao abrir o arquivo\n");
            }
            else
            {

                TNo* no_lido = ler_no(arquivoArvoreB);

                int idBusca ;

                printf("\nDigite o codigo do funcionario que deseja realizar a exclusao na arvore b: ");
                scanf("%d",&idBusca);

                removerFuncionario(no_lido,idBusca);

                fclose(arquivoArvoreB);
            }

        }
        else
        {
            int escolha = 0 ;
            int maxParticao;
            int numFunc ;

            do
            {
                printf("\n Digite o numero de funcionarios que deseja carregar da base de dados e inserir na tabela hash: ");
                scanf("%d",&numFunc);

                if(numFunc >MaxFuncionario)
                {
                    printf("\n A base de dados contem %d funcionarios, voce soliticou a insercao %d funcionarios, tente novamente \n",MaxFuncionario,numFunc);
                }
            }
            while(numFunc > MaxFuncionario);

            char nomeParticao[100];
            char str1[100];
            char str2[100] = ".dat";

            printf("\nNumero da funcao de hash: ");
            scanf("%d", & maxParticao);

            particaoTabelaHash( maxParticao);

            criarTabelaHash(arquivo,  maxParticao, numFunc);

            printf("\n Tabela hash criado com sucesso \n");


            do
            {
                menuTabelaHash();
                scanf("%d",&escolha);

                if (escolha == 1)
                {
                    int cod;
                    int idParticao;

                    printf("\n Digite o codigo do funcionario: ");
                    scanf("%d", &cod);

                    if(cod < 0)
                    {
                        printf("\n codigo de funcionario invalido \n");

                    }
                    else
                    {

                        idParticao = cod %  maxParticao;

                        TFunc *func = buscaTabelaHash(cod, idParticao);

                        if (func != NULL)
                        {
                            printf("\n Funcionario encontrado \n");
                            imprimirFuncionario(func);
                        }
                        else
                        {
                            printf("\nFuncionario nao encontrado na tabela hash");
                        }
                    }

                }
                else if (escolha == 2)
                {

                    int cod;
                    int idParticao;

                    printf("\n Digite o codigo do funcionario: ");
                    scanf("%d", &cod);

                    if(cod < 0)
                    {
                        printf("\n codigo de funcionario invalido \n");

                    }
                    else
                    {
                        idParticao = cod % maxParticao;

                        deletarTabelaHash(cod, idParticao);
                    }

                }
                else if (escolha == 3)
                {

                    for (int i = 0; i < maxParticao; ++i)
                    {

                        itoa(i, str1, 10);
                        strcat(strcpy(nomeParticao, "particaoHash"), str1);
                        strcat(strcpy(nomeParticao, nomeParticao), str2);

                        FILE *arquivoParticao = fopen(nomeParticao,"rb+");

                        imprimirTabelaHash(arquivoParticao,nomeParticao);

                        fclose(arquivoParticao);
                    }

                }
            }
            while(escolha != 4);
            {
                printf("voltando ao menu principal \n");
            }
        }

    }
    while(opcao != -1);
    {
        printf("Programa finalizado \n\n");
    }


    fclose(arquivo);


    return 0;
}

