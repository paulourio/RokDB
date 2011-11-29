RokDB
=====

Sistema Gerenciador de Banco de Dados escrito em C++
com foco em concorrência, no modelo Cliente/Servidor.

Bibliotecas (Dependências)
--------------------------

 * `POSIX threads` ou `pthreads` -- Usada para cada conexão e controle
 	de concorrência.  Suportada nativamente em ambiente UNIX, porém
	no Windows é necessário ter `Pthreads-w32`.
 * `ICU 4.8.1` -- International Components for Unicode (ICU) é usada
 	para trabalhar com dados codificados em unicode e para a análise
	da linguagem Rok Query Language (RQL) via expressões regulares.
	Atualmente no Windows a versão é 4.0, portanto incompativel e
	o RokDB não irá rodar em ambientes não-UNIX.


Diretórios
----------

 * `access` -- Classes de acesso ao banco de dados;
 * `bin` -- Pasta com o diretório de produção (diretório de trabalho);
 * `client` -- Exemplo de cliente usando o RokDB;
 * `db` -- Modelo de banco de dados;
 * `doc` -- Documentação do SGBD, como formato da linguagem;
 * `drivers` -- Alguns drivers desenvolvidos para o RokDB;
 * `include` -- Diretório com cabeçalhos globais;
 * `lib` -- Bibliotecas genéricas, como a de Socket;
 * `protocol` -- Classes de análise dos comandos recebidos no servidor;
 * `server` -- Implementação do servidor;
 * `storage` -- Classes de persistência do modelo;
 * `temp` -- Arquivos temporários;



Arquivos
--------

Arquivos principais do projeto.

 * `bin/RokDB` -- Executável do servidor RokDB.
 * `bin/rokdb.conf` -- Arquivo de configuração do RokDB.
 * `bin/rokdb.lock` -- Arquivo de _lock_ (existe apenas quando em execução).

Os bancos de dados ficam na pasta `bin`.

Comandos
--------

Todos os comandos listados irão compilar o projeto automaticamente caso
seja necessário.

 * `make run` -- Executa o servidor RokDB.
 * `make valgrind` -- Executa o servidor RokDB em modo /debugging/.
 * `make cli` -- Executa o cliente da pasta `client`.  A configuração de IP e 
 	Porta deve ser definida em `client/main.cc` antes de executar o 
	comando.
 * `make clean` -- Remove os arquivos gerados pela compilação e o
 	executável `bin/RokDB`.

Para executar o servidor sem utilizar os comandos `make`, vá até o diretório
`bin` e digite `./RokDB`.

