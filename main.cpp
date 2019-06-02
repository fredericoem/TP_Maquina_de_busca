/*++++++++++++++++++++++++++++++++++++++++++++++
+   	 PDS2- PRIMEIRO SEMESTRE 2019          +
+        ALUNOS: ANDRE RABELO AGUIAR		   +
+		 FREDERICO ESTEVES MARTINS 	           +
+     		 GUSTAVO LUIS GONCALVES FERREIRA   +
+						                       +
+           ENGENHARIA  DE  SISTEMAS           +
+   	     TP - MAQUINA DE BUSCA             +
++++++++++++++++++++++++++++++++++++++++++++++*/




#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "Arquivo.h"
#include "Arquivo.cpp"

using std::map;
using std::string;
using std::vector;
using TpFinal::Arquivo;

void mostraResultado(vector<string> resultado);
void cosineRanking(int argc, int num, char *argv[], string *palavraPesquisada, Arquivo arquivo);



int main(int argc, char *argv[])
{
	// validar os argumentos de entrada

    Arquivo arquivo = Arquivo();

	int i, num=1;
	for(i = 1; i < argc; i++)
	{
        std::cout << "Lendo arquivo " << i << std::endl;
		arquivo = Arquivo(argv[i]);

		if(arquivo.nullFile == 1)
		{
			std::cout << "Arquivo nao foi lido, terminando a execucao"<<std::endl;
			return 0;
		}

    }




    // consulta de palavras
    while(num!=0)
    {
    std::cout<<std::endl<<"Digite quantas palavras voce deseja pesquisar ou Zero para sair: ";
    std::cin>> num;
    string palavraPesquisada[num];
    vector<string> resultado;

        if(num == 0)
        {
            return 0;
        }


        for(i=0; i<num; i++)
        {
        std::cout << "Insira a palavra "<< i+1 << std::endl;
		std::cin >> palavraPesquisada[i];

		bool existe = Arquivo::consultaPalavra(resultado, palavraPesquisada[i]);
		if(existe)
		{
			std::cout << "Palavra existe nos seguintes arquivos: " << std::endl;
			mostraResultado(resultado);
		}
		else
		{
			std::cout << "Palavra pesquisada NAO EXISTE" << std::endl << std::endl;
		}

        }

        cosineRanking(argc, num, argv, palavraPesquisada, arquivo);


    }


}

void mostraResultado(vector<string> resultado)
{
	for (vector<string>::iterator res = resultado.begin() ; res != resultado.end(); ++res)
	{
		std::cout << *res << std::endl;
	}
	std::cout << std::endl;
}

void cosineRanking(int argc, int num, char *argv[], string *palavraPesquisada, Arquivo arquivo)
{
    // matrizes determinam as coordenadas dos documentos e palavras pesquisadas pelo usuario
    float vetorDocumento[argc-1][arquivo.indiceInvertidosize()] = {{0}};
    float vetorPalavrasPesquisadas[num][arquivo.indiceInvertidosize()] = {{0}};

    float similaridade[argc], idf=0;

    float aux1=0, aux2=0, aux3=0, id=0, n=0;

    int i, j=0, k;

    string auxstring;
    map<string, vector<string>> auxmap = arquivo.getindiceInvertido();
    vector<string> auxvetor;
    map<string, vector<string>>::iterator it;


    // laço que preenche as coordenadas da matriz documentos
    for(i=1; i<argc; i++)
    {
        for(it = auxmap.begin(); it != auxmap.end(); it++)
        {
            auxvetor = it->second;
            n = auxvetor.size();
            id = arquivo.contaPalavra(it->first, argv[i]);
            idf = log((argc-1)/n);
            vetorDocumento[i-1][j] = id*idf;
            j++;
        }
        j=0;
    }


    n=0, j=0;


    //laço que preenche as coordenadas da matriz pesquisa
    for(i=0; i<num; i++)
    {
            for(it = auxmap.begin(); it != auxmap.end(); it++)
            {
                if(palavraPesquisada[i].compare(it->first)==0)
                {
                    auxvetor = it->second;
                    n = auxvetor.size();
                    idf = log((argc-1)/n);
                    vetorPalavrasPesquisadas[i][j] = idf*1;
                }
                else
                {
                    vetorPalavrasPesquisadas[i][j] = 0;
                }
                j++;
            }https://www.youtube.com/watch?v=v8-4rI0cJXE

            j=0;
    }


    // laço auxiliar para calcular a similaridade a soma dos quadrados da matriz palavras pesquisada
    for(k=0; k<num; k++)
    {
            for(j=0; j<arquivo.indiceInvertidosize();j++)
            {
                aux2 = aux2 + pow(vetorPalavrasPesquisadas[k][j],2);
            }
    }

    for(i=0; i<argc-1; i++)
    {

        // laço auxiliar para calcular a similaridade a soma dos quadrados da matriz documentos
        for(j=0;j<arquivo.indiceInvertidosize();j++)
        {
            aux1 = aux1 + pow(vetorDocumento[i][j], 2);
        }


        // laço auxiliar para calcular a similaridade a soma dos produtos entre as matrizes documentos e palavras pesquisada
        for(k=0;k<num;k++)
        {
            for(j=0; j<arquivo.indiceInvertidosize();j++)
            {
                aux3 = aux3 + vetorDocumento[i][j]*vetorPalavrasPesquisadas[k][j];
            }
        }


        // equação da similaridade
        similaridade[i] = aux3/(sqrt(aux1) * sqrt(aux2));

        aux1 = 0;
        aux3 = 0;
    }

    std::cout<<"Ranking Consine: "<< std::endl;

    map<float, string> ranking;


    //impressão da ordem de importancia em um documento no consine ranking
    for(i=0; i<argc-1; i++)
    {
        ranking.insert(std::pair<float,string>(similaridade[i]*-1, argv[i+1]));
    }

    i=1;
    map<float, string>::iterator it2;

    for(it2 = ranking.begin(); it2 != ranking.end(); it2++)
    {
        std::cout<<i<<". "<<it2->second<<std::endl;
        i++;
    }

}











// g++ -o build Arquivo.h Arquivo.cpp main.cpp
