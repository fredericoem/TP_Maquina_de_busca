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











// g++ -o build Arquivo.h Arquivo.cpp main.cpp
