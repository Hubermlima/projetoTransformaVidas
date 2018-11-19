/*
     Trabalho de Tecnicas de Programacao - Linguagem C
     Nome do aluno: Huber Martins Lima - 18103448
                         
     Professora: Larissa
     Data de entrega: 
	 
	 Status: Sistema funcionando perfeitamente no COMPUTADOR DE TESTE...
	 
	 EXPLICACAO GERAL DO PROJETO
	 
	 1) Foi criado o arquivo principal-texto chamado projetos.txt que abrigará todos os projetos cadastrados da ONG Projetos Comunitários Transforma Vidas
	    e "n" outros auxiliaares. Exemplo, se o usuario cadastrar um novo projeto com a identificação 123456, então, será criado um arquivo texto 
		auxiliar vazio chamado 123456_s.txt. Este conterá as movimentações daquele projeto, a saber, data, descricao, motivo e valor.
	 
	 2) Se o projeto 123456 for excluido do sistema, então, o arquivo auxiliar 123456_s.txt será deletado também.
	 	
	 3) Foram criados tabelas-clones em memoria RAM dos arquivos gravados em disco (.txt). Os arquivos .txt são atualizados pelas tabelas-clones 
	    sempre que ocorrer alguma operação de inclusao, alteração ou exclusão. 
	
	 P.S.: Para a renderização das telas foi utilizada constantemente o arquivo de cabeçalho windows.h que contém as funções que manipulam a
	    API do Windows.
*/

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>    
#include <ctype.h>
#include <unistd.h> // incluido apenas para usar o sleep

#define COR FOREGROUND_GREEN

// Prototipos de funcoes API WINDOWS
void gotoxy(int x, int y);
void verde();
void corPrimeiroPlanoIntensa();
void fundoVerde();
void oculto();
void erro();

// Prototipo de funcoes 
void menuPrincipal();
void efetuarCadastro();
void efetuarAlteracao();
void gravarAlteracao();
void telaInclusao();
void telaAlteracao();
void gravarAlteracaoProjeto();
void efetuarPesquisa();
void telaPesquisa();
void movimentacao();
void telaMovimentacao();
void incluirMovimentacao();
void excluirProjeto();
void excluirItemMov();

// funcoes utilitarias
char validacao();
void popularGrid(); // popular grid de projetos
void popularGridM(); // popular grid de movimentacao de saidas do projeto
int idExiste(); // verificar se existe o projeto, depois recupa a identificacao e o valor Alocado
void abrirArquivo();
void erroAbrirArquivo();
void apagarTelaInclusaoMovimentacao();
int validarNumerico();
int validarTamanhoString();
int validarData();
void criarProjetosMemoria();
void copiarTabelaM();

// Variaveis globais	
typedef struct {
	char identificacao[10];
	char nome[30];
    char dataCriacao[11];
    char previsaoTermino[11];
    char valorAlocado[15];
    char origemRecurso[25];
    char coordenador[30];  
	} registro;

// 		
typedef struct {
    char item[2];
	char data[11];
    char descricao[25];
    char motivo[25];
    char valor[12];
	} registroM;

registro registroNovo;
registroM registroMov;
registro registros[] = {};
registroM registrosMov[] = {};
int x,y,linhasTabela, linhasTabelaMov,indiceAlteracao;
char nomeArquivo[]="projetos.txt";
char nomeArquivoM[50];
char recuperaValorAlocado[100];
char recuperaId[100];

FILE *pf, *pfM;
	
int main(void){
	
	setlocale(LC_ALL,"Portuguese");
	
	char op;      
    while (1){
    	
       // Renderizar tela de menu principal
        menuPrincipal();
	   op=validacao();
	   if (op=='S')
		   exit(1);
		   
	   if (op=='C'){
	        fflush(stdin);
	        efetuarCadastro();
		}    
	   else if (op=='P')
	        efetuarPesquisa();		
	     
     }

return 0;
}

// Funcoes auxiliares de interface API WINDOWS

void gotoxy(int x, int y){
    COORD c = { x, y };  
    SetConsoleCursorPosition(  GetStdHandle(STD_OUTPUT_HANDLE) , c);
}
void verde(){ // verde
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COR) ;  
}
void branco(){ // branco
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED) ;  
}
void erro(){ // vermelho
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED) ;  
}
void oculto(){ // oculto
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COMMON_LVB_REVERSE_VIDEO) ;  
}

void comentario(){ // magenta
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;  
}

void primeiroPlanoIntenso(){ // intenso
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COR | FOREGROUND_INTENSITY);
}
void fundoVerde(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
}

// Tela do menu principal

void menuPrincipal(){
   
   system("cls");
   
   // dimensionar janela   
   HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
   SMALL_RECT sr;
   COORD consoleSize;
   consoleSize.X = 100; consoleSize.Y = 25;
   sr.Top=sr.Left=0;
   sr.Right=consoleSize.X-1; sr.Bottom=consoleSize.Y-1;
   SetConsoleWindowInfo(console, TRUE, &sr);
   SetConsoleScreenBufferSize(console, consoleSize);
    
    char titulo[] = " PROJETOS Comunitarios >> TRANSFORMA VIDAS ";
    x = (consoleSize.X-strlen(titulo))/2;
    y = 5;
   	gotoxy(x,y);
	primeiroPlanoIntenso();
	fundoVerde();
	printf(" PROJETOS Comunitarios >> ");
	printf("TRANSFORMA VIDAS ");
	oculto();
    printf(";");
    
    y+=2;
	gotoxy(x,y);
	branco();
    printf("["); 
	fundoVerde();
	printf("Cc");
	branco();
	printf("]ADASTRAR");
	y++;
	gotoxy(x,y);
    comentario();
	printf("Esta opcao cadastra um novo projeto;");
	
	y+=2;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Pp");
	branco();
	printf("]ESQUISAR");
	y++;
	gotoxy(x,y);
	comentario();
	printf("Esta opcao pesquisa um projeto cadastrado;");
	
	y+=2;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Ss");
	branco();
	printf("]AIR");
	y++;
	gotoxy(x,y);
	comentario();
	printf("Esta opcao sai do sistema;");
	
	y+=2;
	gotoxy(x,y);
	comentario();
	printf("Digite uma das alternativas: "); 	
}

// Funcoes de abertura de arquivos

void abrirArquivo(char *nomeArquivo, char *tipo){
	  pf = fopen (nomeArquivo, tipo);
	  if (pf==NULL){
	    erroAbrirArquivo(7,25);
	    exit(1);}	
}

void abrirArquivoM(char *nomeArquivoM, char *tipo){
	  pfM = fopen (nomeArquivoM, tipo);
	  if (pfM==NULL){
	    erroAbrirArquivo(7,25);
	    exit(1);}	
}

void erroAbrirArquivo(int x, int y) {
	       gotoxy(x,y);
           fundoVerde();
	       printf("Erro na abertura do arquivo... <Tecle para retornar!>");
	       getch();
		   oculto();
           printf(";");
	       branco();
}

// Obtem os dados dos projetos e grava em arquivo

void efetuarCadastro(){
	
	  // efetuar abertura de arquivo
	  system("cls");
	   
      char op;
	  char dataCriacao[10], dataTermino[10];   
	  while(1){
    	
		abrirArquivo(nomeArquivo,"a");
	     
		// Renderizar tela inclusao
		telaInclusao();
			
	    verde();
	    x=46;
	    y=5;
	    while (1){
			gotoxy(x,y);
			fflush(stdin);
			gets(registroNovo.identificacao);
			if (!validarTamanhoString(registroNovo.identificacao,5,46,74,y))
					continue;
	        else
	                break;
         }
		y++;
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.nome);
					if (!validarTamanhoString(registroNovo.nome,25,46,74,y))
					continue;
	        else
	                break;
         }
		y++;

		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.dataCriacao);
					if (!validarTamanhoString(registroNovo.dataCriacao,10,46,74,y) || 
					    !validarData(registroNovo.dataCriacao,10,46,74,y)) 
				           	continue;
	                else
	                        break;
     	}
		y++;
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.previsaoTermino);
						if (!validarTamanhoString(registroNovo.previsaoTermino,10,46,74,y) || 
						    !validarData(registroNovo.previsaoTermino,10,46,74,y))
					continue;
	        else
	                break;
         }
		
		y++;
		while (1){
		
			gotoxy(x,y);
			fflush(stdin);
			gets(registroNovo.valorAlocado);
			if (!validarNumerico(registroNovo.valorAlocado,46,74,y))
					continue;
	        else
	                break;
  	}
		y++;
		
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.origemRecurso);
					if (!validarTamanhoString(registroNovo.origemRecurso,20,46,74,y))
					continue;
	        else
	                break;
         }
		
		y++;
		gotoxy(x,y);
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.coordenador);
					if (!validarTamanhoString(registroNovo.coordenador,25,46,74,y))
					continue;
	        else
	                break;
         }
	    fflush(stdin);
	    
	    // gravar
	    gotoxy(7,13); printf("Deseja efetuar a gravacao (S/N)? "); 
	    op=validacao();
		if (op=='N')
		   break;
		   
	   if (op=='S'){
	        //efetuarGravacao 
			fprintf(pf, "%s\n", registroNovo.identificacao);
	        fprintf(pf, "%s\n", registroNovo.nome);
	        fprintf(pf, "%s\n", registroNovo.dataCriacao);
	        fprintf(pf, "%s\n", registroNovo.previsaoTermino);
	        fprintf(pf, "%s\n", registroNovo.valorAlocado);
	        fprintf(pf, "%s\n", registroNovo.origemRecurso);
	        fprintf(pf, "%s\n", registroNovo.coordenador); 
	        fclose(pf);
		        
		    strcat(registroNovo.identificacao,"_s.txt");
	        strcpy(nomeArquivoM,registroNovo.identificacao); // gerar nome arquivo auxiliar de movimentacao
	        abrirArquivo (nomeArquivoM, "a");
	        fclose(pf);
	        
		    gotoxy(7,13); printf("Gravacao realizada com sucesso!          ");
			fflush(stdin);
			gotoxy(7,14); printf("Deseja efetuar novo cadastro (S/N)? ");
			op=validacao();
            if (op=='N')
	            break;      
		} // if
      } // while
} // fim arquivo

// Funcao que faz alteracao de um registro de projeto ja gravado em arquivo
 
void efetuarAlteracao(){
	   
        char op;   
	    telaAlteracao();
		verde();
	    x=46;
	    y=15;
	    
	    while(1){
			gotoxy(x,y);
			fflush(stdin);
			gets(registroNovo.identificacao);
			if (!validarTamanhoString(registroNovo.identificacao,5,46,74,y))
					continue;
	        else
	                break;
         }
		y++;
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.nome);
					if (!validarTamanhoString(registroNovo.nome,25,46,74,y))
					continue;
	        else
	                break;
         }
		y++;

		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.dataCriacao);
					if (!validarTamanhoString(registroNovo.dataCriacao,10,46,74,y) || 
					    !validarData(registroNovo.dataCriacao,10,46,74,y)) 
				           	continue;
	                else
	                        break;
     	}
		y++;
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.previsaoTermino);
						if (!validarTamanhoString(registroNovo.previsaoTermino,10,46,74,y) || 
						    !validarData(registroNovo.previsaoTermino,10,46,74,y))
					continue;
	        else
	                break;
         }
		
		y++;
		while (1){
		
			gotoxy(x,y);
			fflush(stdin);
			gets(registroNovo.valorAlocado);
			if (!validarNumerico(registroNovo.valorAlocado,46,74,y))
					continue;
	        else
	                break;
  	}
		y++;
		
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.origemRecurso);
					if (!validarTamanhoString(registroNovo.origemRecurso,20,46,74,y))
					continue;
	        else
	                break;
         }
		
		y++;
		gotoxy(x,y);
		while (1){
				gotoxy(x,y);
				fflush(stdin);
				gets(registroNovo.coordenador);
					if (!validarTamanhoString(registroNovo.coordenador,25,46,74,y))
					continue;
	        else
	                break;
         }
	    fflush(stdin);
	    
	    // gravar
	    gotoxy(5,23); printf("Deseja efetuar a alteracao (S/N)? "); 
	    op=validacao();
	    if (op=='S')
	        gravarAlteracaoProjeto();
		   
} // fim arquivo

// Funcao que faz a validacao das opcoes do menu

char validacao(){
	fflush(stdin);
	char op;
	scanf("%c",&op);
	op=toupper(op);
	return op;    
}

// Funcao que verifica se existe um projeto ja cadastrado

int idExiste(char *id){ 
    char linha[100];
    char *result; 
    int flag = 0, cont=0;
    abrirArquivo(nomeArquivo,"r");
   	  
	 while(!feof(pf)){
            fgets(linha, 100, pf);
            linha[strcspn(linha, "\n")] = 0; //retira o enter da string
            if (strcmp(id, linha)==0) {
			     flag = 1;
				 strcpy(recuperaId,linha);} 	 
                                   
		    if (flag == 1) {
		    	cont++; // ate chegar no campo desejado
		    	if (cont==5) {
					 strcpy(recuperaValorAlocado,linha);
					 return 1;}
			}
				     
	   }  	
	   
	 return 0;
}

// Funcao que renderiza a tela de inclusao de um projeto

void telaInclusao(){ 
	system("cls");
	
    x = 3;
    y = 1;
   	gotoxy(x,y);
	primeiroPlanoIntenso();
	fundoVerde();
	printf(" PROJETOS Comunitarios >> TRANSFORMA VIDAS ");
	oculto();
    printf(";");
	branco();
	x=5;
	y=3;
    gotoxy(x,y);
	printf("- Cadastrar Projeto -");
    
    x=7;
    y=5;
    gotoxy(x,y); printf("Id do projeto [Alfa-5]: ");
    y++;
    gotoxy(x,y); printf("Nome do projeto [Alfa-25]: ");
    y++;
    gotoxy(x,y); printf("Data de criacao [dd/mm/aaaa]: ");
    y++;
    gotoxy(x,y); printf("Previsao termino [dd/mm/aaaa]: ");
    y++;
    gotoxy(x,y); printf("Valor alocado R$ [9.999.999,99]: ");
    y++;
    gotoxy(x,y); printf("Origem do recurso [Alfa-20]: ");
    y++;
    gotoxy(x,y); printf("Coordenador(a) do projeto [Alfa-25]: ");   
}

// Tela que renderiza a tela de alteracao de um projeto ja cadastrado

void telaAlteracao(){ 
	system("cls");
    x = 3;
    y = 1;
   	gotoxy(x,y);
	primeiroPlanoIntenso();
	fundoVerde();
	printf(" PROJETOS Comunitarios >> TRANSFORMA VIDAS ");
	oculto();
    printf(";");
	branco();
	x=5;
	y=3;
    gotoxy(x,y);
	printf("- Alterar Projeto -");
    
    x=5;
    y=5;
    verde();
    gotoxy(x,y); printf("Projeto original");
    y=6;
    branco();
    gotoxy(x,y); printf("Id do projeto [Alfa-5]: "); 
    y++;
    gotoxy(x,y); printf("Nome do projeto [Alfa-25]: ");
    y++;
    gotoxy(x,y); printf("Data de criacao [dd/mm/aaaa]: ");
    y++;
    gotoxy(x,y); printf("Previsao termino [dd/mm/aaaa]: ");
    y++;
    gotoxy(x,y); printf("Valor alocado R$ [9.999.999,99]: ");
    y++;
    gotoxy(x,y); printf("Origem do recurso [Alfa-20]: ");
    y++;
    gotoxy(x,y); printf("Coordenador(a) do projeto [Alfa-25]: ");
     
    // Localizar registro em memoria
    for(indiceAlteracao=0; indiceAlteracao<linhasTabela; indiceAlteracao++)
        if (strcmp(registros[indiceAlteracao].identificacao, recuperaId) == 0){
    	  // mostra registro na tela
    	  y=6;
		  verde();
    	  gotoxy(46,y); printf(registros[indiceAlteracao].identificacao);
    	  y++;
    	  gotoxy(46,y); printf(registros[indiceAlteracao].nome);
    	  y++;
    	  gotoxy(46,y); printf(registros[indiceAlteracao].dataCriacao);
    	  y++;
    	  gotoxy(46,y); printf(registros[indiceAlteracao].previsaoTermino);
    	  y++;
    	  gotoxy(46,y); printf(registros[indiceAlteracao].valorAlocado);
    	  y++;
    	  gotoxy(46,y); printf(registros[indiceAlteracao].origemRecurso);
    	  y++;
    	  gotoxy(46,y); printf(registros[indiceAlteracao].coordenador);
    	  branco();
		}
	y=14;
    verde();
    gotoxy(x,y); printf("Alterar registro");
    y=15;
    branco();
    gotoxy(x,y); printf("Id do projeto [Alfa-5]: "); 
    y++;
    gotoxy(x,y); printf("Nome do projeto [Alfa-25]: ");
    y++;
    gotoxy(x,y); printf("Data de criacao [dd/mm/aaaa]: ");
    y++;
    gotoxy(x,y); printf("Previsao termino [dd/mm/aaaa]: ");
    y++;
    gotoxy(x,y); printf("Valor alocado R$ [9.999.999,99]: ");
    y++;
    gotoxy(x,y); printf("Origem do recurso [Alfa-20]: ");
    y++;
    gotoxy(x,y); printf("Coordenador(a) do projeto [Alfa-25]: ");
    
}
// Funcao que renderiza a tela de pesquisa de um projeto ja cadastrado com a finalidade de pesquisa, exclusao, alteracao

void telaPesquisa(){
	system("cls");
    
    x = 30;
    y = 1;
   	gotoxy(x,y);
	primeiroPlanoIntenso();
	fundoVerde();
	printf(" PROJETOS Comunitarios >> TRANSFORMA VIDAS ");
	oculto();
    printf(";");
	branco();
	x=5;
	y=3;
    gotoxy(x,y);
	printf("- Pesquisar Projetos -");
    
    // cabeÃƒÂ§alho do grid
    x=5;
	y=5;
    gotoxy(x,y);
    primeiroPlanoIntenso();
	fundoVerde();
	printf("Identificacao   Nome Projeto                            Coordenador                         ");
    oculto();
    printf(";");
	branco();
	
	y=20;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Ee");
	branco();
	printf("]xcluir projeto | ");
	
	x=26;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Aa");
	branco();
	printf("]lterar projeto | ");
	
	x=47;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Mm");
	branco();
	printf("]ovimentar projeto | ");
	
	x=70;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Ss");
	branco();
	printf("]air");
	
	x=5;
	y++;
	gotoxy(x,y);
	comentario();
	printf("Selecione uma alternativa: ");	
    

}

// Funcao que renderiza a tela de movimentacao de projeto: data, descricao, motivo, valor

void telaMovimentacao(){
	char recuperaIdNovo[100];
	strcpy(recuperaIdNovo,recuperaId);
    x = 30;
    y = 1;
   	gotoxy(x,y);
	primeiroPlanoIntenso();
	fundoVerde();
	printf(" PROJETOS Comunitarios >> TRANSFORMA VIDAS ");
	oculto();
    printf(";");
	branco();
	
	x=5;
	y=3;
    gotoxy(x,y);
	printf("Ind. projeto:             Alocado R$ original:          Fonte");
	gotoxy(5,4);
	printf(recuperaId);
	
	gotoxy(31,4);
	printf(recuperaValorAlocado);
	
	strcat(recuperaIdNovo,"_s.txt");
	gotoxy(61,4);
	printf(recuperaIdNovo);
    strcpy(nomeArquivoM,recuperaIdNovo); // gerar nome arquivo completo
    
	// cabecalho do grid
    x=5;
	y=5;
    gotoxy(x,y);
    primeiroPlanoIntenso();
	fundoVerde();
	printf("It. Data         Descricao                            Motivo                    Valor R$     ");
    oculto();
    printf(";");
	branco();
	
	y=22;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Ii");
	branco();
	printf("]ncluir item | ");
	
	x=23;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Ee");
	branco();
	printf("]xcluir item | ");
	
	x=41;
	gotoxy(x,y);
	branco();
    printf("[");
	fundoVerde();
	printf("Ss");
	branco();
	printf("]air");
	
	x=5;
	y++;
	gotoxy(x,y);
	comentario();
	printf("Selecione uma alternativa: ");	

}

// Funcao que verifica qual a opcao escolhida: Exclusao, Alteracao e movimentacao de projeto

void efetuarPesquisa(){
	char op;		
	char *id; 
	while(1){
	
	    system("cls");	   
        // Renderizar tela
		telaPesquisa();
		popularGrid();
		criarProjetosMemoria();
		while (1){
			x=32;
			y=21;
			gotoxy(x,y);	
			op=validacao();
			if (op=='S') break;
			
			if (op=='M' || op == 'E' || op == 'A') {
			     comentario();
				 gotoxy(5,22);
				 printf("Digite o numero identificador: "); scanf("%s",id);
				 fflush(stdin);
				 
				 if (!idExiste(id)){ // recupera tambem o valor alocado
				     gotoxy(50,22); erro();   printf("Erro: Nao existe!"); sleep(5); 
					 gotoxy(50,22); oculto(); printf("Erro: Nao existe!"); 
					 gotoxy(5,22);
				     oculto(); printf("Digite o numero identificador:              ");
					 branco(); }
				  else {// existe 
		     	     if (op == 'M'){ movimentacao(); fclose(pf); break;}
		     	        else if (op == 'A') { efetuarAlteracao(); fclose(pf); break;}
					       else { excluirProjeto(id); fclose(pf); break;}
					  }     
			}
			 
		} // end while
        if (op=='S'){ break;fclose(pfM);}		   
     } // fim while
} // fim funcao

// Funcao que exclui um projeto selecionado pela identificacao

void excluirProjeto(char *id){
    
    int i;
    int ret;
       
	abrirArquivoM("projetos.txt","w"); // Regravar o array da memoria com opcao "w" (apaga o conteudo anterior)				
	for(i=0;i<linhasTabela;i++){
		if (strcmp(id, registros[i].identificacao)!=0) {
	                 
			fprintf(pfM, "%s\n", registros[i].identificacao);
	        fprintf(pfM, "%s\n", registros[i].nome);
	        fprintf(pfM, "%s\n", registros[i].dataCriacao);
	        fprintf(pfM, "%s\n", registros[i].previsaoTermino);
	        fprintf(pfM, "%s\n", registros[i].valorAlocado);
	        fprintf(pfM, "%s\n", registros[i].origemRecurso);
	        fprintf(pfM, "%s\n", registros[i].coordenador); }
		        	
      }
      fclose(pfM);
      strcat(recuperaId,"_s.txt");
      ret = remove(recuperaId);  // deletar arquivo auxiliar de movimentacao
      
       gotoxy(50,22); erro();   printf("Sucesso: projeto excluido!");
	   sleep(5);  
	   gotoxy(50,22); oculto(); printf("Sucesso: projeto excluido!"); 
	   branco(); 			  
      
}

// Funcao que exclui uma item de um arquivo de movimentacao de um projeto

void excluirItemMov(){
    
    int i, novoItem = 1;
    int ret;
    char it[2];
	
	gotoxy(5,24);
	comentario();
    printf("Digite o item da linha: "); scanf("%s",it);
	fflush(stdin); 
	branco();
	it[strcspn(it, "\n")] = 0; //retira o enter da string
	  
	abrirArquivoM(nomeArquivoM,"w"); // Regravar o array da memoria com opcao "w" (apaga o conteudo anterior)				
	for(i=0;i<linhasTabelaMov;i++){
		if (strcmp(it, registrosMov[i].item) != 0) {
	  
	        sprintf(registrosMov[i].item, "%i", novoItem);
			               
			fprintf(pfM, "%s\n", registrosMov[i].item);
	        fprintf(pfM, "%s\n", registrosMov[i].data);
	        fprintf(pfM, "%s\n", registrosMov[i].descricao);
	        fprintf(pfM, "%s\n", registrosMov[i].motivo);
	        fprintf(pfM, "%s\n", registrosMov[i].valor); 
	        novoItem++;}
			    	
      }
      fclose(pfM);
       
}

// Funcao que grava a alteracao de um projeto

void gravarAlteracaoProjeto(){
    
    int i;
    int ret;
       
	abrirArquivoM("projetos.txt","w"); // Regravar o array da memoria com opcao "w" (apaga o conteudo anterior)				
	for(i=0;i<linhasTabela;i++){
	                 
	        if (strcmp(recuperaId, registros[i].identificacao) == 0) {   
			    fprintf(pfM, "%s\n", registroNovo.identificacao);
		        fprintf(pfM, "%s\n", registroNovo.nome);
		        fprintf(pfM, "%s\n", registroNovo.dataCriacao);
		        fprintf(pfM, "%s\n", registroNovo.previsaoTermino);
		        fprintf(pfM, "%s\n", registroNovo.valorAlocado);
		        fprintf(pfM, "%s\n", registroNovo.origemRecurso);
		        fprintf(pfM, "%s\n", registroNovo.coordenador);}
		     else{
				fprintf(pfM, "%s\n", registros[i].identificacao);
		        fprintf(pfM, "%s\n", registros[i].nome);
		        fprintf(pfM, "%s\n", registros[i].dataCriacao);
		        fprintf(pfM, "%s\n", registros[i].previsaoTermino);
		        fprintf(pfM, "%s\n", registros[i].valorAlocado);
		        fprintf(pfM, "%s\n", registros[i].origemRecurso);
		        fprintf(pfM, "%s\n", registros[i].coordenador); }
	        	
      }
      fclose(pfM);
      
       gotoxy(5,24); erro();   printf("Sucesso: projeto alterado!");
	   sleep(5);  
	   gotoxy(5,24); oculto(); printf("Sucesso: projeto alterado!"); 
	   branco(); 			  
      
}

// Funcao que valida um dado como numerico digitado pelo usuario

int validarNumerico(char *input, int po, int x, int y){
      if (strtod(input,NULL) == 0){
           gotoxy(po,y);
           printf("               ");
		   gotoxy(x,y);
           erro();
           printf("Valor invalido!");
           verde();
           return 0;}
	   else{
	       gotoxy(x,y);
           printf("                ");
           return 1; }         
}

// Funcao que valida o tamanho de um dado string digitado pelo usuario

int validarTamanhoString(char *input, int tam, int po, int x, int y){
      if (strlen(input) > tam || strlen(input) == 0 ){
           gotoxy(po,y);
           printf("                              ");
		   gotoxy(x,y);
           erro();
           printf("Tamanho de campo invalido!");
           verde();
           return 0;}
	   else {
		   gotoxy(x,y);
           printf("                          ");
           return 1;}          
}

// Funcao que valida datas digitadas pelo usuario

int validarData(char *data, int tam, int po, int x, int y){
	 char dia[2];
     char mes[2];
	 char ano[2];
	 int flag = 0;
	 int i;
	 // dia
	 for(i=0; i<2; i++)
	        dia[i]=data[i];
	 if (atoi(dia) == 0) flag = 1;

	 // mes
	 for(i=3; i<5; i++)
	        mes[i-3]=data[i];
	 if (atoi(mes) == 0) flag = 1;	
	            
	 // ano
	 for(i=6; i<10; i++)
	        ano[i-6]=data[i];
	  if (atoi(dia) == 0) flag = 1;
	  
	  if (flag==1){
           gotoxy(po,y);
           printf("                              ");
		   gotoxy(x,y);
           erro();
           printf("Data invalida!");
           verde();
           return 0;}
	  else {
           gotoxy(x,y);
           printf("                          ");
           return 1; }
}

// Funcao que preenche as linhas do grid projetos cadastrados

void popularGrid(){
    int i=0;
	y=6; 
	char linha[100];
    char *result; 
    abrirArquivo(nomeArquivo,"r");
    while(!feof(pf)){
            result = fgets(linha, 100, pf);
            if (!result) {break;}
	        switch(i){
		   	    case 0: 
				   gotoxy(5,y);
                   primeiroPlanoIntenso();
				   printf("%s",linha);
				   branco();
				   break;
				case 1: 
				     gotoxy(21,y);
				     printf("%s",linha);
	                 break; 
				case 6: 
				     gotoxy(61,y);
				     printf("%s",linha);
				     break;
              }			
            if (i==6){
			   i=0;
			   y++;}
     	    else 
			  i++; 		
						
	    } // end while
	    fclose(pf);
}

// Funcao que preenche as linhas do grid movimentacao de um projeto

void popularGridM(){
    int i=0;
    int lin = 0;
    double total = 0;
   	char linha[50];
    char *result; 
    y=6;
	// abrir arquivo
	pfM = fopen (nomeArquivoM, "r");
	if (pfM==NULL){
	        erroAbrirArquivo(7,25); return;}
	while(!feof(pfM)){
            result = fgets(linha, 50, pfM);
            if (!result) {break;}
	        switch(i){
	        	 case 0: // item 
		   	       gotoxy(5,y);
				   branco();
				   printf("%s",linha);
				   break;
			
		   	    case 1: // data 
				   gotoxy(9,y);
				   printf("%s",linha);
				   break;
				case 2: //descricao 
				     gotoxy(22,y);
				     printf("%s",linha);
	                 break; 
				case 3: // motivo
				     gotoxy(59,y);
				     printf("%s",linha);
				     break;
				case 4: // valor
				     gotoxy(85,y);
				     primeiroPlanoIntenso();
				     printf("%s",linha);
				     total+=strtod(linha,NULL);
				     break;}			
            if (i==4){
			   i=0;
			   y++;
			   lin++;}
     	    else 
			  i++; 		
						
	    } // end while
	    // Gerar rodape do grid
		branco();  
	    gotoxy(83,y);
	    printf("------------");
		gotoxy(86,y+1);
		// Imprimir total geral de saidas
		branco();
		gotoxy(69,y+1);
	    printf("Total R$ ->");
		fundoVerde();
		gotoxy(85,y+1);
		printf("%.2lf",total);
		oculto();
		printf(";");
		branco();
		gotoxy(83,y+2);
	    printf("------------");
	    // Imprimir saldo 
		gotoxy(69,y+3);
	    printf("Saldo R$ ->");
		gotoxy(85,y+3);
	    fundoVerde();
		printf("%.2f\n",(strtod(recuperaValorAlocado,NULL) - total));
		gotoxy(98,y+3);
	    oculto();
		printf(";");
		branco();		
		fclose(pfM); 
} // end function

// Funcao que carrega todos os projetos cadastrados na memoria

void criarProjetosMemoria(){
	int i=0;
	int y=0; 
	char linha[100];
    char *result; 
    abrirArquivo(nomeArquivo,"r");
    while(!feof(pf)){
            result = fgets(linha, 100, pf);
            linha[strcspn(linha, "\n")] = 0; //retira o enter da string
            if (!result) {break;}
	        switch(i){
		   	    case 0: 
				   strcpy(registros[y].identificacao,linha);
				   break;
				case 1: 
				     strcpy(registros[y].nome,linha);
	                 break; 
				case 2:
				     strcpy(registros[y].dataCriacao,linha);
				     break;
				case 3: 
			       strcpy(registros[y].previsaoTermino,linha);
				   break;
				case 4: 
				     strcpy(registros[y].valorAlocado,linha);
	                 break; 
				case 5: 
				     strcpy(registros[y].origemRecurso,linha);
				     break; 
				case 6: 
				     strcpy(registros[y].coordenador,linha);
				     break;	     
              }			
            if (i==6){ i=0; y++;}
     	      else i++; 		
						
	    } // end while
	    
	    linhasTabela = y;
		fclose(pf);
}

// Funcao que carrega todos os movimentos de um dado projeto na memoria

void copiarTabelaM(){
	int i=0;
	int y=0; 
	char linha[100];
    char *result; 
    abrirArquivo(nomeArquivoM,"r");
    while(!feof(pfM)){
            result = fgets(linha, 100, pfM);
            linha[strcspn(linha, "\n")] = 0; //retira o enter da string
            if (!result) {break;}
	        switch(i){
		   	    case 0: 
				   strcpy(registrosMov[y].item,linha);
				   break;
				case 1: 
				     strcpy(registrosMov[y].data,linha);
	                 break; 
				case 2:
				     strcpy(registrosMov[y].descricao,linha);
				     break;
				case 3: 
			       strcpy(registrosMov[y].motivo,linha);
				   break;
				case 4: 
				     strcpy(registrosMov[y].valor,linha);
	                 break; 	     
              }			
            if (i == 4) { i = 0; y++;}
     	      else i++; 		
						
	    } // end while
	    
	    linhasTabelaMov = y;
		fclose(pf);
}

// Funcao principal de controle das movimentacoes de um projeto
	
void movimentacao(){
  
  while(1){
  	    char op;
	    system("cls");      	  
	    // Renderizar tela    
		telaMovimentacao();
		popularGridM(); // abre, pesquisa e fecha o arquivo
		copiarTabelaM();
		while (1){
			x=32;
			y=23;
			gotoxy(x,y);	
			op=validacao();
			if (op=='S') break;
			if (op == 'I'){
                incluirMovimentacao();	// abre, inclui e fecha o arquivo	        
				break;}
			else
			    { excluirItemMov(); break; }    				
					 
		} // end while
        if (op=='S') {
           fclose(pfM);
	        break;	}		   
  } // end while
} // fim funcao

// Funcao que inclui um item de movimentacao de um projeto: data, descricao, motivo, valor

void incluirMovimentacao(){
    char op;
	fflush(stdin);
	pfM = fopen (nomeArquivoM, "a");
	if (pfM==NULL){
	         erroAbrirArquivo(7,25); return;}
	
	// inclusao
	x=5;
    y=15;
    branco();
    gotoxy(x,y); printf("Item:");
    verde();
    gotoxy(10,y); printf("%2d", linhasTabelaMov+1);
    branco();
    y++;
    gotoxy(x,y); printf("Data [dd/mm/aaaa]: ");
    y++;
    gotoxy(x,y); printf("Descricao [Alfa-20]: ");
    y++;
    gotoxy(x,y); printf("Motivo [Alfa-20]: ");
    y++;
    gotoxy(x,y); printf("Valor alocado R$: ");
    
	verde();

    x=28;
    y=16;
	while (1){
 		gotoxy(x,y);
        fflush(stdin);
    	gets(registroMov.data);
	    if (!validarTamanhoString(registroMov.data,10,28,74,y) || !validarData(registroMov.data,10,28,74,y)) 
           	continue;
        else
            break;
	}
    
	y++;
	while (1){
			gotoxy(x,y);
			fflush(stdin);
			gets(registroMov.descricao);
				if (!validarTamanhoString(registroMov.descricao,20,28,74,y))
				continue;
             else
                break;
     }
	
	y++;
	while (1){
		gotoxy(x,y);
		fflush(stdin);
		gets(registroMov.motivo);
			if (!validarTamanhoString(registroMov.motivo,20,28,74,y))
			     continue;
	        else
                 break;
    }
	
	y++;
		while (1){

			gotoxy(x,y);
			fflush(stdin);
			gets(registroMov.valor);
			if (!validarNumerico(registroMov.valor,28,74,y))
					continue;
	        else
	                break;
  	    }
	
	gotoxy(5,20); printf("Deseja efetuar a gravacao (S/N)? "); 
    op=validacao();
	   
   if (op=='S'){
	    //efetuarGravacao
		sprintf(registroMov.item, "%i", linhasTabelaMov+1);
        fprintf(pfM, "%s\n", registroMov.item);
		fprintf(pfM, "%s\n", registroMov.data);
	    fprintf(pfM, "%s\n", registroMov.descricao);
	    fprintf(pfM, "%s\n", registroMov.motivo);
	    fprintf(pfM, "%s\n", registroMov.valor);
	    gotoxy(5,21); printf("Gravacao realizada com sucesso! <Tecle algo>"); getch();
	    fclose(pfM);
	        
	   } // if
	   apagarTelaInclusaoMovimentacao();
				
} // end incluir movimentacao

// Funcao que apaga a tela de inclusao de movimentacao de itens de um projeto

void apagarTelaInclusaoMovimentacao(){
	x=5;
    y=15;
    oculto();
    gotoxy(x,y); printf("Item:                                                                                ");
    y++;
    gotoxy(x,y); printf("Data [dd/mm/aaaa]:                                                                   ");
    y++;
    gotoxy(x,y); printf("Descricao [Alfa-25]:                                                                  ");
    y++;
    gotoxy(x,y); printf("Motivo [Alfa-25]:                                                                     ");
    y++;
    gotoxy(x,y); printf("Valor alocado R$:                                                                      ");
    branco();
}

// THE END
