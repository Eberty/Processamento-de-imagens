/* 		ATIVIDADE 4 - OK
 * Implementar o algoritmo da faixa e testar em imagens coloridas
 * 		– Ler do teclado a banda do fundo, os valores máximo e mínimos
 * 		– Apresentar as imagens originais, o canal alfa e a imagem processada
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

FILE *fpin, *fpin2;
int **imagemR, **imagemG, **imagemB, **imagemR2, **imagemG2, **imagemB2;
int ncol, nlin, grade = 1;
char controle[4];
int fundo, blue_min, blue_max, green_min, green_max, red_min, red_max;

//Funcoes
void abrir_arquivos(int argc, char *argv[]);
void ignorar_comentario(FILE *);
void normalizar_profundidade(void);
void ler_imagem(void);
void ler_cabecalho(void);
void fechar_arquivos(void);
void desenhaOriginal1(void);
void desenhaOriginal2(void);
void desenhaCanalAlfa(void) ;
void desenhaProcessada(void);
void Inicializa(void);
void lerDoTeclado(void);

void abrir_arquivos(int argc, char *argv[]) {
	if (argc  != 3) {
		printf("Modo correto de uso: pdi <imagem_foreground> <imagem_background)>\n");
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


void desenhaCanalAlfa(void) {
	int col, lin, l, c;
	int cor;
	
	glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
		for (col=0, c=0;col<ncol;col++, c++) {
			if (imagemB[l][c] >= blue_min && imagemB[l][c] <= blue_max && imagemG[l][c] >= green_min && imagemG[l][c] <= green_max && imagemR[l][c] >= red_min && imagemR[l][c] <= red_max){
				cor = 0;
			} else{
				cor = 255;
			}
			glColor3ub(cor,cor,cor);
			glVertex2i(col,lin);
		}
	}
	
	glEnd();
	glFlush();
}


void desenhaProcessada(void) {
	int col, lin, l, c;
	int R, G, B;
	
	glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
		for (col=0, c=0;col<ncol;col++, c++) {
			if (imagemB[l][c] >= blue_min && imagemB[l][c] <= blue_max && imagemG[l][c] >= green_min && imagemG[l][c] <= green_max && imagemR[l][c] >= red_min && imagemR[l][c] <= red_max){
				R=imagemR2[l][c];
				G=imagemG2[l][c];
				B=imagemB2[l][c];
			} else{
				R=imagemR[l][c];
				G=imagemG[l][c];
				B=imagemB[l][c];
			}
			
			glColor3ub(R,G,B);
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


void lerDoTeclado(void){
	int min, max;
	
	printf("\n");
	do {
		printf("Indique a banda de fundo - (1)Azul (2)Verde: ");
		scanf("%d", &fundo);
	} while (fundo < 1 || fundo > 2);
	
	printf("\nIndique o valor maximo e minimo para sua cor ecolhida,\n");
	printf("Por padrao, as demais cores estarao entre 0 e o minimo definido\n\n");
	
	do {
		printf("Minimo: ");
		scanf("%d", &min);
	} while (min < 0 || min > 255);

	do {
		printf("Maximo: ");
		scanf("%d", &max);
	} while (max < min || max > 255);
	
	
	if (fundo == 1){ //Azul
		blue_min = min; blue_max = max;
		green_min = 0; green_max = min;
		red_min = 0; red_max = min;
	} else { //Verde
		green_min = min; green_max = max;
		blue_min = 0; blue_max = min;
		red_min = 0; red_max = min;
	}
}


int main(int argc, char *argv[]) {
	abrir_arquivos(argc,argv);
	ler_cabecalho();
	ler_imagem();
	fechar_arquivos();
	
	lerDoTeclado();
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	//----------- Primeira Tela  - Foreground ----------------
	glutInitWindowPosition(10,35);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Original - Foreground");
	glutDisplayFunc(desenhaOriginal1);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM Primeira Tela


	//----------- Segunda Tela  - Background ----------------
	glutInitWindowPosition(10,nlin+60);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Original - Background");
	glutDisplayFunc(desenhaOriginal2);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM Segunda Tela


	//----------- Terceira Tela  - Canal Alfa ----------------
	glutInitWindowPosition(ncol+60,35);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Canal Alfa");
	glutDisplayFunc(desenhaCanalAlfa);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM  Terceira Tela

	//----------- Quarta Tela  - Processada ----------------
	glutInitWindowPosition(ncol+60,nlin+60);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Processada");
	glutDisplayFunc(desenhaProcessada);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM  Quarta Tela
	
	glutMainLoop();

    return 0;
}

