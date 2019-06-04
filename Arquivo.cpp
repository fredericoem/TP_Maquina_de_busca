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
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Arquivo.h"


using std::vector;
using std::string;
using std::map;

namespace TpFinal
{
	map<string, vector<string>> Arquivo::indiceInvertido;

	Arquivo::Arquivo()
	{

	}


    Arquivo::Arquivo(string nomeArquivo)
	{
		this->nomeArquivo = nomeArquivo;

		nullFile = 0;

		// Abre arquivo
		// To do: exception IO
		std::ifstream arquivoEntrada;
		// arquivoEntrada.exceptions(std::ifstream::failbit);
		// try
		// {
			arquivoEntrada.open(this->nomeArquivo);

			if(arquivoEntrada.fail())
            {
                nullFile = 1;
            }
		// }
		// catch(std::exception& e)
		// {
		// 	std::cout << "Erro ao abrir o arquivo especificado. Nenhuma palavra adicionada." << std::endl;
		// 	this->nullFile = true;
		// 	return;
		// }

		// Para cada linha, formata, separa em palavras e adiciona ao dict
		for(string linha; getline(arquivoEntrada, linha);)
		{
		    vector<string> palavras = this->formataLinha(linha);

            for(vector<string>::iterator palavra = palavras.begin(); palavra != palavras.end(); ++palavra)
			{
				map<string, vector<string>>::iterator chave = Arquivo::indiceInvertido.find(*palavra);
				if(chave != Arquivo::indiceInvertido.end())
				{
					if(std::find(chave->second.begin(), chave->second.end(), this->nomeArquivo) == chave->second.end())
					{
						// Adiciona novo nome ao vector
						// To do : ordem alfabética
						vector<string>::iterator vecArquivos = chave->second.end();
						chave->second.insert(vecArquivos, this->nomeArquivo);
					}
				}
				else
				{
					// Adiciona nova palavra
					vector<string> novoValor {this->nomeArquivo};
					Arquivo::indiceInvertido.insert(std::pair<string, vector<string>>(*palavra, novoValor));
				}
			}
		}

		arquivoEntrada.close();
	}

	vector<string> Arquivo::formataLinha(string linha)
	{
		// Deixa todas os caracteres minusculos
		for(int i = 0; linha[i]; i++)
		{
			linha[i] = tolower(linha[i]);
		}

		// Exclui os caracteres diferentes de: letras minusculas, numeros e o caractere ' '
		for(int i = 0; linha[i]; i++)
		{
			if(!((linha[i] >= 97 && linha[i] <= 122) || (linha[i] >= 30 && linha[i] <= 39) || linha[i] == 32))
            linha.erase(i,1);

        }




        // Separa em palavras
		string buffer;
		std::stringstream ss(linha);
		vector<string> palavras;
		while (ss >> buffer)
		{
			palavras.push_back(buffer);
			//std::cout << buffer << std::endl;
		}
		//std::cout << std::endl;

		return palavras;
	}



	bool Arquivo::consultaPalavra(vector<string>& resultado, const string& palavra)
	{
	    std::map<string, vector<string>>::iterator busca;
		busca = Arquivo::indiceInvertido.find(palavra);
		std::cout << "Pesquisando por: '" << palavra << "'" << std::endl;
		if(busca != Arquivo::indiceInvertido.end())
		{
			resultado = busca->second;
			return true;
		}
		else
		{
			return false;
		}
	}

    int Arquivo::contaPalavra(string palavra, string nomedoArquivo)
	{
        int i, cont=0;
	    vector<string> palavras;

	    std::ifstream arquivoEntrada;

        arquivoEntrada.open(nomedoArquivo);
            if(arquivoEntrada.fail())
            {
                std::cout<<"FALHA na leitura do arquivo!!!!"<<std::endl;
                nullFile = 1;

                return 0;
            }

		for(string linha; getline(arquivoEntrada, linha);)
		{
			vector<string> palavras = this->formataLinha(linha);

            for(i=0; i<palavras.size(); i++)
            {
                    if(palavra.compare(palavras[i])==0)
                    {
                        cont++;
                    }
            }
		}

		arquivoEntrada.close();

        return cont;

	}

	int Arquivo::indiceInvertidosize()
	{
	    return indiceInvertido.size();
	}

    map<string, vector<string>>  Arquivo::getindiceInvertido()
    {
        return indiceInvertido;
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
            }

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

	


}
