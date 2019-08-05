/* 		ATIVIDADE 3
 * Utilizando como base o seu visualizador, e o programa menu.c, implementar as seguintes funcionalidades:
 * 		– Clarear ou escurecer imagens (de 20 em 20)
 * 		– Calcular e apresentar a imagem negativa
 * 		– Voltar ao original
 * 		– Obs1: Apresentar sempre as imagens, original e processada (no início do programa ambas serão iguais) 
 * 		– Obs2: Testes com umagens binárias, monocromáticas e coloridas
 * 
 * A imagem processada será uma escolha do usuário feita via mouse, desta forma:
 * 		–Ao clicarmos com o botão esquerdo sobre imagem original aparecerá menu com opções: clarear,  escurecer, voltar ao original e negativo (em laço e cumulativo),
 * 			ex:
 * 				Escolhendo clarear e escurecer em seguida, a imagem processada ficará igual a original Podemos escurecer/clarear uma imagem negativa e a recíproca é verdadeira
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

// Variáveis
FILE *fpin;
int **imagemR, **imagemG, **imagemB, ncol, nlin, grade;
char controle[4], texto[30];
int process = 0, neg = 0;


//Funcoes
void abrir_arquivos(int argc, char *argv[]);
void ler_cabecalho(void);
void ler_imagem(void);
void fechar_arquivos(void);
void desenhaOriginal(void);
void desenhaProcessada(void);
void Inicializa(void);
void ignorar_comentario(void);
void normalizar_profundidade(void);


void abrir_arquivos(int argc, char *argv[]) {
	if (argc  <= 1) {
		printf("Modo correto de uso: pdi <imagem> <arq_saida>\n");
	}
	
	if ((fpin = fopen(argv[1], "r"))==NULL) {
		printf("Nao foi possivel abrir arquivo de imagem %s\n", argv[1]);
		exit(1);
	}
} // Fim: abrir_arquivos


void ignorar_comentario(void){
	char comentario = getc(fpin);
	while (comentario == '#'){
		while (comentario != '\n')
			comentario = getc(fpin);
		comentario = getc(fpin);
	}
	fseek(fpin, -sizeof(char), SEEK_CUR);
}


void normalizar_profundidade(void){
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
			R=imagemR[l][c] + process;
			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
				
			G=imagemG[l][c] + process;
			if (G < 0)
				G = 0;
			else if (G > 255)
				G = 255;
			
			B=imagemB[l][c] + process;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;
				
			if (neg)
				glColor3ub(255-R,255-G,255-B);
			else
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


// Gerenciamento do menu principal
void MenuPrincipal(int op) {
	switch(op) {
		case 0:
			if (neg)
				process -= 20;
			else
				process += 20;
			break;
			
		case 1:
			if (neg)
				process += 20;
			else
				process -= 20;
			break;
			
		case 2:
			process = 0;
			neg = 0;
			break;
			
		case 3:
			neg = !neg;
			break;
	}
	
    glutPostRedisplay();
}


// Criacao do Menu
void CriaMenu() {
	glutCreateMenu(MenuPrincipal);
	glutAddMenuEntry("Clarear",0);
	glutAddMenuEntry("Escurecer",1);
	glutAddMenuEntry("Voltar ao original",2);
	glutAddMenuEntry("Negativo",3);

	glutAttachMenu(GLUT_LEFT_BUTTON);
}


// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN)
            CriaMenu();

    glutPostRedisplay();
}


int main(int argc, char *argv[]) {
	abrir_arquivos(argc,argv);
	ler_cabecalho();
	ler_imagem();
	fechar_arquivos();

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
    glutMouseFunc(GerenciaMouse);
	// Chama a função responsável por fazer as inicializações
	Inicializa();
	//FIM  Segunda Tela

	glutMainLoop();

    return 0;
}

