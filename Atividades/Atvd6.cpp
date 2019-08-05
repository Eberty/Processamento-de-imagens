/* 		ATIVIDADE 6
 * Implementar um programa de melhora de contraste por equalização de histograma
 * 		– O programa tem que exibir ambas imagens: original e equalizada
 * 		– Exibir também ambos histogramas, original e equalizado
 * 		– Funcionar para imagem monocromáticas e coloridas
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#define MAX(a,b) (((a)>(b))?(a):(b))

FILE *fpin;
int **imagemR, **imagemG, **imagemB, ncol, nlin, grade;
char controle[4];
int histR[4][256], histG[4][256], histB[4][256];
int maiorR, maiorG, maiorB;
int process = 0;


//Funcoes
void abrir_arquivos(int argc, char *argv[]);
void ignorar_comentario(void);
void normalizar_profundidade(void);
void ler_imagem(void);
void ler_cabecalho(void);
void fechar_arquivos(void);
void desenhaOriginal(void);
void desenhaProcessada(void);
void Inicializa(void);
void MenuPrincipal(int op);
void CriaMenu();
void GerenciaMouse(int button, int state, int x, int y);
void calcula_histograma(void);
void desenhaHistogramaOriginal(void) ;
void desenhaHistogramaEqualizado(void);


void abrir_arquivos(int argc, char *argv[]) {
	if (argc  <= 1) {
		printf("Modo correto de uso: pdi <imagem> <arq_saida>\n");
	}
	
	if ((fpin = fopen(argv[1], "r"))==NULL) {
		printf("Nao foi possivel abrir arquivo de imagem %s\n", argv[1]);
		exit(1);
	}
} // Fim: abrir_arquivos


void ignorar_comentario(void) {
	char comentario = getc(fpin);
	while (comentario == '#'){
		while (comentario != '\n')
			comentario = getc(fpin);
		comentario = getc(fpin);
	}
	fseek(fpin, -sizeof(char), SEEK_CUR);
}


void normalizar_profundidade(void) {
	int lin, col, aux;
	
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
}


void ler_imagem(void) {
	int cont, col, lin, aux;
	//printf("TIPO: %s\n", controle);
		
	if ((strcmp(controle, "P2") == 0) || (strcmp(controle, "P3") == 0)){
		fscanf(fpin,"%d\n",&grade);
		ignorar_comentario();
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
	
	normalizar_profundidade();
	//printf("Fim: ler_imagem\n");
} // Fim: ler_imagem


void ler_cabecalho(void) {
	fscanf(fpin,"%s\n",controle);
	ignorar_comentario();
	fscanf(fpin,"%d %d\n",&ncol, &nlin);
	ignorar_comentario();
} // Fim: ler_cabecalho


void fechar_arquivos(void) {
	fclose(fpin);
} // Fim: fechar_arquivos


void desenhaOriginal(void) {
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


void desenhaProcessada(void) {
	int col, lin, l, c;
	int R, G, B;
	glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
		for (col=0, c=0;col<ncol;col++, c++) {
			R=histR[2][imagemR[l][c]];
			G=histG[2][imagemG[l][c]];
			B=histB[2][imagemB[l][c]];
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


void MenuPrincipal(int op) {
	if ((strcmp(controle, "P2") == 0) || (strcmp(controle, "P1") == 0)){
		process = 4;
	} else{
		process = op;
	}
    glutPostRedisplay();
}


void CriaMenu() {
	glutCreateMenu(MenuPrincipal);
	glutAddMenuEntry("Red",0);
	glutAddMenuEntry("Green",1);
	glutAddMenuEntry("Blue",2);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}


void GerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN)
            CriaMenu();

    glutPostRedisplay();
}


void calcula_histograma(void) {
	float I = (ncol * nlin) / 256.0;
	int somaR=0, somaG=0, somaB=0;
	int aux;
	
	if ((strcmp(controle, "P2") == 0) || (strcmp(controle, "P1") == 0)){
		process = 4;
	}
	
	for (int i=0; i<4; i++){
		for (int j=0; j<256; j++){
			histR[i][j] = 0;
			histG[i][j] = 0;
			histB[i][j] = 0;
		}
	}
	
	for (int i=0; i<nlin; i++){
		for (int j=0; j<ncol; j++){
			histR[0][imagemR[i][j]]++;
			histG[0][imagemG[i][j]]++;
			histB[0][imagemB[i][j]]++;
		}
	}
	
	for (int i=0; i<256; i++){
		somaR+=histR[0][i];
		histR[1][i] = somaR;
		somaG+=histG[0][i];
		histG[1][i] = somaG;
		somaB+=histB[0][i];
		histB[1][i] = somaB;
	}
	
	
	for (int i=0; i<256; i++){
		aux = (int)(((histR[1][i] / I) - 1) + 0.5);
		histR[2][i] = MAX(0,aux);
		aux = (int)(((histG[1][i] / I) - 1) + 0.5);
		histG[2][i] = MAX(0,aux);
		aux = (int)(((histB[1][i] / I) - 1) + 0.5);
		histB[2][i] = MAX(0,aux);
	}
	
	for (int i=0; i<256; i++){
		aux = histR[2][i];
		histR[3][aux]+=histR[0][i];
		histG[3][aux]+=histG[0][i];
		histB[3][aux]+=histB[0][i];
	}
	
	maiorR = histR[0][0];
	maiorG = histG[0][0];
	maiorB = histB[0][0];
	//maiorEqR = histR[3][0];
	//maiorEqG = histG[3][0];
	//maiorEqB = histB[3][0];
	for (int i=1; i<256; i++){
		if (histR[0][i] > maiorR)
			maiorR = histR[0][i];
		if (histG[0][i] > maiorG)
			maiorG = histG[0][i];
		if (histB[0][i] > maiorB)
			maiorB = histB[0][i];
		/*if (histR[3][i] > maiorEqR)
			maiorEqR = histR[3][i];
		if (histG[3][i] > maiorEqG)
			maiorEqG = histG[3][i];
		if (histB[3][i] > maiorEqB)
			maiorEqB = histB[3][i];*/
	}
	
	//for (int i=0; i<256; i++)
	//	printf("[%3d] %10d %10d %10d %10d\n", i, histR[0][i], histR[1][i], histR[2][i], histR[3][i]);
}


void desenhaHistogramaOriginal(void) {
	int col, lin, aux;
	
	glClearColor(1.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	if (process == 0)
		glColor3ub(255,0,0);
	else if (process == 1)
		glColor3ub(0,255,0);
	else if (process == 2)
		glColor3ub(0,0,255);
	else
		glColor3ub(0,0,0);
		
	for (col=0; col<256; col++){
		if (process == 0)
			aux = (histR[0][col]*250)/maiorR;
		else if (process == 1)
			aux = (histG[0][col]*250)/maiorG;
		else
			aux = (histB[0][col]*250)/maiorB;
			
		for (lin=0; lin<aux; lin++)
			glVertex2i(col,lin);
	}
	glEnd();
	glFlush();
}


void desenhaHistogramaEqualizado(void) {
	int col, lin, aux;
	
	glClearColor(1.0,1.0,1.0,0); //Especifica um cor para o fundo
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	if (process == 0)
		glColor3ub(255,0,0);
	else if (process == 1)
		glColor3ub(0,255,0);
	else if (process == 2)
		glColor3ub(0,0,255);
	else
		glColor3ub(0,0,0);
		
	for (col=0; col<256; col++){
		if (process == 0)
			aux = (histR[3][col]*250)/maiorR;
		else if (process == 1)
			aux = (histG[3][col]*250)/maiorG;
		else
			aux = (histB[3][col]*250)/maiorB;
			
		for (lin=0; lin<aux; lin++)
			glVertex2i(col,lin);
	}
	glEnd();
	glFlush();
}


int main(int argc, char *argv[]) {
	abrir_arquivos(argc,argv);
	ler_cabecalho();
	ler_imagem();
	fechar_arquivos();
	
	calcula_histograma();
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	//----------- Primeira Tela  - Original ----------------
	glutInitWindowPosition(10,35);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Original");
	glutDisplayFunc(desenhaOriginal);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM Primeira Tela


	//----------- Segunda Tela  - Processada ----------------
	glutInitWindowPosition(ncol+60,35);
	glutInitWindowSize(ncol,nlin);
	glutCreateWindow("Foto Processada");
	glutDisplayFunc(desenhaProcessada);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM  Segunda Tela


	//----------- Terceira Tela  - Histograma Original ----------------
	glutInitWindowPosition(10,nlin+60);
	glutInitWindowSize(256,256);
	glutCreateWindow("Histograma Original");
	glutDisplayFunc(desenhaHistogramaOriginal);
    glutMouseFunc(GerenciaMouse);
	gluOrtho2D(0, 256, 0, 256);
	//FIM  Terceira Tela
	
	
	//----------- Quarta Tela  - Histograma Equalizado ----------------
	glutInitWindowPosition(ncol+60,nlin+60);
	glutInitWindowSize(256,256);
	glutCreateWindow("Histograma Equalizado");
	glutDisplayFunc(desenhaHistogramaEqualizado);
    glutMouseFunc(GerenciaMouse);
	gluOrtho2D(0, 256, 0, 256);
	//FIM  Quarta Tela
	
	
	glutMainLoop();

    return 0;
}

