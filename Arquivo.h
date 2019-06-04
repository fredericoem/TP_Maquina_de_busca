/*++++++++++++++++++++++++++++++++++++++++++++++
+   	 PDS2- PRIMEIRO SEMESTRE 2019          +
+        ALUNOS: ANDRE RABELO AGUIAR		   +
+		 FREDERICO ESTEVES MARTINS 	           +
+     		 GUSTAVO LUIS GONCALVES FERREIRA   +
+						                       +
+           ENGENHARIA  DE  SISTEMAS           +
+   	     TP - MAQUINA DE BUSCA             +
++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef ARQUIVO_H
#define ARQUIVO_H
#include <iostream>
#include <vector>
#include <map>
#include <string>


using std::vector;
using std::string;
using std::map;

namespace TpFinal
{
	class Arquivo
	{
		public:
			//
            Arquivo();

			//
			Arquivo(string nomeArquivo);

			//
			static bool consultaPalavra(vector<string>& resultado, const string& palavra);

			//
			bool nullFile;

			//

			// metodo auxiliar para retornar o tf quantidade de vezes que uma palavra aparece dentro de um determinado documento
			int contaPalavra(string palavra, string nomeArquivo);

			//

			map<string, vector<string>>  getindiceInvertido();

			//

			int indiceInvertidosize();

		private:
			//
			static map<string, vector<string>> indiceInvertido;

			//
			static vector<string> formataLinha(string linha);

			//
			string nomeArquivo;

			friend class Teste;

	};
}

#endif  // ARQUIVO_H
