/* 		ATIVIDADE 1 - OK
 * Estudar o código pdi.c e modificá-lo para abrir
 * e apresentar qualquer imagem da família PNM
 * no modo texto: P1, P2 e P3
 * 		– Identificar a extensão automáticamente
 * 		– Ignorar comentários quando presentes
 * 		– Normalizar a profundidade caso seja necessário
 * */
 
/* 		ATIVIDADE 2 - OK
 * Implementar algorítmo de subtração de imagens para detecção de movimento
 * 		– Tirar fotos com câmera fixa: cenário sem um determinado objeto (foto 1), simular 
 * 			o “aparecimento” de um objeto na cena (foto 2), e posteriormente, seu movimento (foto 3)
 * 		– O programa deve apresentar as 3 imagens simultâneamente
 * 		– Ler da entrada padrão o percentual de erro
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

FILE *fpin, *fpin2;
int **imagemR, **imagemG, **imagemB, **imagemR2, **imagemG2, **imagemB2;
int ncol, nlin, grade = 1, erro;
char controle[4];


//Funcoes
void abrir_arquivos(int argc, char *argv[]);
void ler_cabecalho(void);
void ler_imagem(void);
void fechar_arquivos(void);
void desenhaOriginal(void);
void desenhaProcessada(void);
void Inicializa(void);
void ignorar_comentario(FILE *);
void normalizar_profundidade(void);


void abrir_arquivos(int argc, char *argv[]) {
	if (argc  != 3) {
		printf("Modo correto de uso: pdi <imagem> <arq_saida>\n");
	}
	
	if ((fpin = fopen(argv[1], "r"))==NULL) {
		printf("Nao foi possivel abrir arquivo de imagem %s\n", argv[1]);
		exit(1);
	}

	if ((fpin2 = fopen(argv[2], "r"))==NULL) {
		printf("Nao foi possivel abrir arquivo de imagem %s\n", argv[2]);
		exit(1);
	}
} // Fim: abrir_arquivos


void ignorar_comentario(FILE *f){
	char comentario = getc(f);
	while (comentario == '#'){
		while (comentario != '\n')
			comentario = getc(f);
		comentario = getc(f);
	}
	fseek(f, -sizeof(char), SEEK_CUR);
}


void normalizar_profundidade(void){
	int lin, col, aux;
	//IMG 1
	if ((strcmp(controle, "P1") == 0)){
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				if (imagemG[lin][col] == 0)
					imagemG[lin][col] = imagemB[lin][col] = imagemR[lin][col] = 255;
				else
					imagemG[lin][col] = imagemB[lin][col] = imagemR[lin][col] = 0;
			}
		}
		
	} else if (strcmp(controle, "P2") == 0){
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				aux = (imagemG[lin][col]*255) / grade;
				imagemG[lin][col] = imagemB[lin][col] = imagemR[lin][col] = aux;
			}
		}
		
	} else {
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				imagemG[lin][col] = (imagemG[lin][col] * 255) / grade;
				imagemB[lin][col] = (imagemB[lin][col] * 255) / grade;
				imagemR[lin][col] = (imagemR[lin][col] * 255) / grade;
			}
		}
	} 
	
	//IMG 2
	if ((strcmp(controle, "P1") == 0)){
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				if (imagemG2[lin][col] == 0)
					imagemG2[lin][col] = imagemB2[lin][col] = imagemR2[lin][col] = 255;
				else
					imagemG2[lin][col] = imagemB2[lin][col] = imagemR2[lin][col] = 0;
			}
		}
		
	} else if (strcmp(controle, "P2") == 0){
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				aux = (imagemG2[lin][col]*255) / grade;
				imagemG2[lin][col] = imagemB2[lin][col] = imagemR2[lin][col] = aux;
			}
		}
		
	} else {
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				imagemG2[lin][col] = (imagemG2[lin][col] * 255) / grade;
				imagemB2[lin][col] = (imagemB2[lin][col] * 255) / grade;
				imagemR2[lin][col] = (imagemR2[lin][col] * 255) / grade;
			}
		}
	} 
}


void ler_imagem(void) {
	int cont, col, lin, aux;
	//printf("TIPO: %s\n", controle);
	
	//IMG 1
	if ((strcmp(controle, "P2") == 0) || (strcmp(controle, "P3") == 0)){
		fscanf(fpin,"%d\n",&grade);
		ignorar_comentario(fpin);
	}
	
	imagemR = (int **) malloc((nlin+1)*sizeof(int *));
	imagemG = (int **) malloc((nlin+1)*sizeof(int *));
	imagemB = (int **) malloc((nlin+1)*sizeof(int *));
	
	for (cont=0;cont<nlin;cont++) {
		imagemR[cont]=(int *)malloc((ncol+1)*sizeof(int));
		if (imagemR[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
			printf("Falha na alocacao de memoria - 1\n");
			exit(1);
		}
		
		imagemG[cont]=(int *)malloc((ncol+1)*sizeof(int));
		if (imagemG[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
			printf("Falha na alocacao de memoria - 1\n");
			exit(1);
		}
		
		imagemB[cont]=(int *)malloc((ncol+1)*sizeof(int));
		if (imagemR[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
			printf("Falha na alocacao de memoria - 1\n");
			exit(1);
		}
	}
	
	if (strcmp(controle, "P3") == 0){
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				fscanf(fpin,"%d ",&imagemR[lin][col]);
				fscanf(fpin,"%d ",&imagemG[lin][col]);
				fscanf(fpin,"%d ",&imagemB[lin][col]);
			}
		}
	} else {
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				fscanf(fpin,"%d ",&aux);
				imagemG[lin][col] = imagemB[lin][col] = imagemR[lin][col] = aux;
			}
		}
	}
	//printf("Fim: ler_imagem 1\n");
	
	//IMG 2
	if ((strcmp(controle, "P2") == 0) || (strcmp(controle, "P3") == 0)){
		fscanf(fpin2,"%d\n",&grade);
		ignorar_comentario(fpin2);
	}
	
	imagemR2 = (int **) malloc((nlin+1)*sizeof(int *));
	imagemG2 = (int **) malloc((nlin+1)*sizeof(int *));
	imagemB2 = (int **) malloc((nlin+1)*sizeof(int *));
	
	for (cont=0;cont<nlin;cont++) {
		imagemR2[cont]=(int *)malloc((ncol+1)*sizeof(int));
		if (imagemR2[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
			printf("Falha na alocacao de memoria - 1\n");
			exit(1);
		}
		
		imagemG2[cont]=(int *)malloc((ncol+1)*sizeof(int));
		if (imagemG2[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
			printf("Falha na alocacao de memoria - 1\n");
			exit(1);
		}
		
		imagemB2[cont]=(int *)malloc((ncol+1)*sizeof(int));
		if (imagemR2[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
			printf("Falha na alocacao de memoria - 1\n");
			exit(1);
		}
	}
	
	if (strcmp(controle, "P3") == 0){
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				fscanf(fpin2,"%d ",&imagemR2[lin][col]);
				fscanf(fpin2,"%d ",&imagemG2[lin][col]);
				fscanf(fpin2,"%d ",&imagemB2[lin][col]);
			}
		}
	} else {
		for (lin=0;lin<nlin;lin++) {
			for (col=0;col<ncol;col++) {
				fscanf(fpin2,"%d ",&aux);
				imagemG2[lin][col] = imagemB2[lin][col] = imagemR2[lin][col] = aux;
			}
		}
	}
	//printf("Fim: ler_imagem 2\n");
	
	normalizar_profundidade();
} // Fim: ler_imagem


void ler_cabecalho(void) {
	//IMG 1
	fscanf(fpin,"%s\n",controle);
	ignorar_comentario(fpin);
	fscanf(fpin,"%d %d\n",&ncol, &nlin);
	ignorar_comentario(fpin);

	//IMG 2
	fscanf(fpin2,"%s\n",controle);
	ignorar_comentario(fpin2);
	fscanf(fpin2,"%d %d\n",&ncol, &nlin);
	ignorar_comentario(fpin2);
} // Fim: ler_cabecalho


void fechar_arquivos(void) {
	fclose(fpin);
	fclose(fpin2);
} // Fim: fechar_arquivos


void desenhaOriginal1(void) {
	int col, lin, l, c;
	int R, G, B;
	glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
		for (col=0, c=0;col<ncol;col++, c++) {
			R=imagemR[l][c];
			G=imagemG[l][c];
			B=imagemB[l][c];
			glColor3ub(R,G,B);
			glVertex2i(col,lin);
		}
	}
	
	glEnd();
	glFlush();
}


void desenhaOriginal2(void) {
	int col, lin, l, c;
	int R, G, B;
	glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
		for (col=0, c=0;col<ncol;col++, c++) {
			R=imagemR2[l][c];
			G=imagemG2[l][c];
			B=imagemB2[l][c];
			glColor3ub(R,G,B);
			glVertex2i(col,lin);
		}
	}
	
	glEnd();
	glFlush();
}


void desenhaProcessada(void) {
	int col, lin, l, c;
	int cor1, cor2, cor;
	int porcentagem = (255 * erro) / 100;
	//printf("\nPorcentagem: %d\n", porcentagem);
	glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
		for (col=0, c=0;col<ncol;col++, c++) {
			cor1=(imagemR[l][c]+imagemG[l][c]+imagemB[l][c])/3;
			cor2=(imagemR2[l][c]+imagemG2[l][c]+imagemB2[l][c])/3;
			
			cor = abs(cor1-cor2);
			if (cor <= porcentagem){
				cor = 255;//cor = 0;
			} else{
				cor = 255 - cor;
			}
			
			glColor3ub(cor,cor,cor);
			glVertex2i(col,lin);
		}
	}
	
	glEnd();
	glFlush();
}


void Inicializa(void) {
	// Exibição Bidimensional
	gluOrtho2D(0, ncol, 0, nlin);
}//fim da funcao inicializa


int main(int argc, char *argv[]) {
	abrir_arquivos(argc,argv);
	ler_cabecalho();
	ler_imagem();
	fechar_arquivos();
	
	do {
		printf("Apresente a porcentagem inteira do erro: ");
		scanf("%d", &erro);
	} while (erro < 0 || erro > 100);
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	//----------- Primeira Tela  - Original 1 ----------------
	glutInitWindowPosition(10,35);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Original 1");
	glutDisplayFunc(desenhaOriginal1);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM Primeira Tela


	//----------- Segunda Tela  - Original 2 ----------------
	glutInitWindowPosition(10,35);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Original 2");
	glutDisplayFunc(desenhaOriginal2);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM Segunda Tela


	//----------- Terceira Tela  - Processada ----------------
	glutInitWindowPosition(ncol+60,35);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Processada");
	glutDisplayFunc(desenhaProcessada);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM  Segunda Tela

	glutMainLoop();

    return 0;
}

