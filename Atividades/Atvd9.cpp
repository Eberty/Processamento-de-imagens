/* 		ATIVIDADE 9
 * Trabalhar com imagens em niveis de cinza
 * Utlizar o algorimo Quadtree, e mostrar 5 imagesns, original, as divididas (como no exmplo Atvd9.png), e as reconstruidas (a mesma cor para to quadrante)
 * 		- Limiar (lido pelo teclado, quantidade de variacao de pixel) fixo e variavel (Lk = Li / 2^(n-k)) onde Li = Limiar inicial, n a profundidade e k o nivel, com 1<=k<n
 * 		- Procurara valores de max e min no quadrante pesquisado
 * 		- Cada no deve possuir: xmin, xmax, ymin, ymax, valor de nivel de cinza para a resconstrucao (media)
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>


typedef struct arvore {
    int xmin, xmax, ymin, ymax;
    int cor;
    struct arvore *no, *ne, *so, *se;
} quadTree;


FILE *fpin;
int **imagemR, **imagemG, **imagemB, ncol, nlin, grade;
int **imagem, **matrizLimiarFixo, **matrizLimiarVariavel;
char controle[4];
int Limiar;
quadTree *arvoreLimiarFixo, *arvoreLimiarVariavel;

//Funcoes
void abrir_arquivos(int argc, char *argv[]);
void ignorar_comentario(void);
void normalizar_profundidade(void);
void ler_imagem(void);
void ler_cabecalho(void);
void fechar_arquivos(void);
void desenhaOriginal(void);
void desenhaProcessada1(void);
void desenhaProcessada2(void);
void desenhaProcessada3(void);
void desenhaProcessada4(void);
void Inicializa(void);
void preProcessamento(void);
void contruirArvoreLimiarFixo(quadTree *);
void contruirArvoreLimiarVariavel(quadTree *, int);
void preencherMatriz(int **, quadTree *);
void libera (quadTree *);


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
    while (comentario == '#') {
        while (comentario != '\n')
            comentario = getc(fpin);
        comentario = getc(fpin);
    }
    fseek(fpin, -sizeof(char), SEEK_CUR);
}


void normalizar_profundidade(void) {
    int lin, col, aux;

    if ((strcmp(controle, "P1") == 0)) {
        for (lin=0; lin<nlin; lin++) {
            for (col=0; col<ncol; col++) {
                if (imagemG[lin][col] == 0)
                    imagemG[lin][col] = imagemB[lin][col] = imagemR[lin][col] = 255;
                else
                    imagemG[lin][col] = imagemB[lin][col] = imagemR[lin][col] = 0;
            }
        }

    } else if (strcmp(controle, "P2") == 0) {
        for (lin=0; lin<nlin; lin++) {
            for (col=0; col<ncol; col++) {
                aux = (imagemG[lin][col]*255) / grade;
                imagemG[lin][col] = imagemB[lin][col] = imagemR[lin][col] = aux;
            }
        }

    } else {
        for (lin=0; lin<nlin; lin++) {
            for (col=0; col<ncol; col++) {
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

    if ((strcmp(controle, "P2") == 0) || (strcmp(controle, "P3") == 0)) {
        fscanf(fpin,"%d\n",&grade);
        ignorar_comentario();
    }

    imagemR = (int **) malloc((nlin+1)*sizeof(int *));
    imagemG = (int **) malloc((nlin+1)*sizeof(int *));
    imagemB = (int **) malloc((nlin+1)*sizeof(int *));

    for (cont=0; cont<nlin; cont++) {
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

    if (strcmp(controle, "P3") == 0) {
        //for (lin=0;lin<nlin;lin++) {
        for (lin=nlin-1; lin>=0; lin--) {
            for (col=0; col<ncol; col++) {
                fscanf(fpin,"%d ",&imagemR[lin][col]);
                fscanf(fpin,"%d ",&imagemG[lin][col]);
                fscanf(fpin,"%d ",&imagemB[lin][col]);
            }
        }
    } else {
        //for (lin=0;lin<nlin;lin++) {
        for (lin=nlin-1; lin>=0; lin--) {
            for (col=0; col<ncol; col++) {
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

    for (lin=0, l=0; lin<nlin; lin++,l++) {
        for (col=0, c=0; col<ncol; col++, c++) {
            //for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
            //	for (col=0, c=0;col<ncol;col++, c++) {
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


void desenhaProcessada1(void) {
    int cor, col, lin, l, c;
    glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (lin=0, l=0; lin<nlin; lin++,l++) {
        for (col=0, c=0; col<ncol; col++, c++) {
            //for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
            //	for (col=0, c=0;col<ncol;col++, c++) {
            if (matrizLimiarFixo[l][c] < 0){
				glColor3ub(255,0,0);
			} else {
				cor = imagem[l][c];
				glColor3ub(cor,cor,cor);
			}
			glVertex2i(col,lin);
        }
    }

    glEnd();
    glFlush();
}


void desenhaProcessada2(void) {
    int cor, col, lin, l, c;
    glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (lin=0, l=0; lin<nlin; lin++,l++) {
        for (col=0, c=0; col<ncol; col++, c++) {
            //for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
            //	for (col=0, c=0;col<ncol;col++, c++) {
            if (matrizLimiarVariavel[l][c] < 0){
				glColor3ub(255,0,0);
			} else {
				cor = imagem[l][c];
				glColor3ub(cor,cor,cor);
			}
			glVertex2i(col,lin);
        }
    }

    glEnd();
    glFlush();
}


void desenhaProcessada3(void) {
    int cor, col, lin, l, c;
    glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (lin=0, l=0; lin<nlin; lin++,l++) {
        for (col=0, c=0; col<ncol; col++, c++) {
            //for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
            //	for (col=0, c=0;col<ncol;col++, c++) {
            cor = abs(matrizLimiarFixo[l][c]);
            if (cor == 256)
				cor = 0;
			glColor3ub(cor,cor,cor);
			glVertex2i(col,lin);
        }
    }

    glEnd();
    glFlush();
}


void desenhaProcessada4(void) {
    int cor, col, lin, l, c;
    glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (lin=0, l=0; lin<nlin; lin++,l++) {
        for (col=0, c=0; col<ncol; col++, c++) {
            //for (lin=nlin-1, l=0;lin>=0;lin--,l++) {
            //	for (col=0, c=0;col<ncol;col++, c++) {
            cor = abs(matrizLimiarVariavel[l][c]);
            if (cor == 256)
				cor = 0;
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


void preProcessamento(void) {
    printf("Indique o Limiar (0-256): \n");
    do
        scanf("%d", &Limiar);
    while (Limiar < 0 || Limiar > 256);

    //Alocar matriz Imagem
    imagem = (int **) malloc((nlin+1)*sizeof(int *));
    for (int cont=0; cont<nlin; cont++) {
        imagem[cont]=(int *)malloc((ncol+1)*sizeof(int));
        if (imagem[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
            printf("Falha na alocacao de memoria - 1\n");
            exit(1);
        }
    }

    matrizLimiarFixo = (int **) malloc((nlin+1)*sizeof(int *));
    for (int cont=0; cont<nlin; cont++) {
        matrizLimiarFixo[cont]=(int *)malloc((ncol+1)*sizeof(int));
        if (matrizLimiarFixo[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
            printf("Falha na alocacao de memoria - 1\n");
            exit(1);
        }
    }

    matrizLimiarVariavel = (int **) malloc((nlin+1)*sizeof(int *));
    for (int cont=0; cont<nlin; cont++) {
        matrizLimiarVariavel[cont]=(int *)malloc((ncol+1)*sizeof(int));
        if (matrizLimiarVariavel[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
            printf("Falha na alocacao de memoria - 1\n");
            exit(1);
        }
    }

    //Prencher matrizes
    for (int l=0; l<nlin; l++) {
        for (int c=0; c<ncol; c++) {
            imagem[l][c]=(imagemR[l][c]+imagemG[l][c]+imagemB[l][c])/3;
            matrizLimiarVariavel[l][c] = matrizLimiarFixo[l][c] = 0;
        }
    }

    //Alocar arvores
    /*arvoreLimiarFixo = (quadTree*) malloc(sizeof(quadTree));
    if(arvoreLimiarFixo == NULL) {
        printf("Falha na alocacao de memoria - 1\n");
        exit(1);
    }*/
    arvoreLimiarFixo = new quadTree[1];
    arvoreLimiarVariavel = new quadTree[1];

    //Construir arvores
    arvoreLimiarFixo->xmin = 0;
    arvoreLimiarFixo->xmax = ncol-1;
    arvoreLimiarFixo->ymin = 0;
    arvoreLimiarFixo->ymax = nlin-1;
    contruirArvoreLimiarFixo(arvoreLimiarFixo);

    arvoreLimiarVariavel->xmin = 0;
    arvoreLimiarVariavel->xmax = ncol-1;
    arvoreLimiarVariavel->ymin = 0;
    arvoreLimiarVariavel->ymax = nlin-1;
    contruirArvoreLimiarVariavel(arvoreLimiarVariavel, 1);

    preencherMatriz(matrizLimiarFixo, arvoreLimiarFixo);
    preencherMatriz(matrizLimiarVariavel, arvoreLimiarVariavel);
    
    libera(arvoreLimiarFixo);
    libera(arvoreLimiarVariavel);
}


void contruirArvoreLimiarFixo(quadTree *regiao) {
    int corMax, corMin, tot=0;
    regiao->cor = 0;
    corMin = corMax = imagem[regiao->ymin][regiao->xmin];

    for (int l=(regiao->ymin); l<(regiao->ymax); l++) {
        for (int c=(regiao->xmin); c<(regiao->xmax); c++) {
            regiao->cor += imagem[l][c];
			tot++;

            if (imagem[l][c] < corMin)
                corMin = imagem[l][c];
            else if (imagem[l][c] > corMax)
                corMax = imagem[l][c];
        }
    }
    if (tot != 0)
		regiao->cor /= tot;
	
    //printf("xmax=%3d   xmin=%3d   ymax=%3d   ymin=%3d   corMin=%3d   corMax=%3d\n", regiao->xmax, regiao->xmin, regiao->ymax, regiao->ymin, corMin, corMax);
    //Define Filhos como Null
    regiao->no = regiao->ne = regiao->so = regiao->se = NULL;

    if ((corMax-corMin) > Limiar) {
        //printf("no\n");
        regiao->no = new quadTree[1];
        regiao->no->xmin = regiao->xmin;
        regiao->no->xmax = (regiao->xmax + regiao->xmin)/2;
        regiao->no->ymin = ((regiao->ymax + regiao->ymin)/2);
        regiao->no->ymax = regiao->ymax;
        contruirArvoreLimiarFixo(regiao->no);

        //printf("ne\n");
        regiao->ne = new quadTree[1];
        regiao->ne->xmin = ((regiao->xmax + regiao->xmin)/2);
        regiao->ne->xmax = regiao->xmax;
        regiao->ne->ymin = ((regiao->ymax + regiao->ymin)/2);
        regiao->ne->ymax = regiao->ymax;
        contruirArvoreLimiarFixo(regiao->ne);

        //printf("so\n");
        regiao->so = new quadTree[1];
        regiao->so->xmin = regiao->xmin;
        regiao->so->xmax = (regiao->xmax + regiao->xmin)/2;
        regiao->so->ymin = regiao->ymin;
        regiao->so->ymax = (regiao->ymax + regiao->ymin)/2;
        contruirArvoreLimiarFixo(regiao->so);

        //printf("se\n");
        regiao->se = new quadTree[1];
        regiao->se->xmin = ((regiao->xmax + regiao->xmin)/2);
        regiao->se->xmax = regiao->xmax;
        regiao->se->ymin = regiao->ymin;
        regiao->se->ymax = (regiao->ymax + regiao->ymin)/2;
        contruirArvoreLimiarFixo(regiao->se);
    }
}


void contruirArvoreLimiarVariavel(quadTree *regiao, int profundidade) {
    int corMax, corMin, tot=0;
    regiao->cor = 0;
    corMin = corMax = imagem[regiao->ymin][regiao->xmin];

    for (int l=(regiao->ymin); l<(regiao->ymax); l++) {
        for (int c=(regiao->xmin); c<(regiao->xmax); c++) {
            regiao->cor += imagem[l][c];
			tot++;
            if (imagem[l][c] < corMin)
                corMin = imagem[l][c];
            else if (imagem[l][c] > corMax)
                corMax = imagem[l][c];
        }
    }
    if (tot != 0)
		regiao->cor /= tot;

    //printf("xmax=%3d   xmin=%3d   ymax=%3d   ymin=%3d   corMin=%3d   corMax=%3d\n", regiao->xmax, regiao->xmin, regiao->ymax, regiao->ymin, corMin, corMax);
    //Define Filhos como Null
    regiao->no = regiao->ne = regiao->so = regiao->se = NULL;
	int k = profundidade / 2;
	if (k<1) k =1;
    if ((corMax-corMin) > (Limiar/(int)pow(2,profundidade-k))) {
        profundidade++;

        //printf("no\n");
        regiao->no = new quadTree[1];
        regiao->no->xmin = regiao->xmin;
        regiao->no->xmax = (regiao->xmax + regiao->xmin)/2;
        regiao->no->ymin = ((regiao->ymax + regiao->ymin)/2);
        regiao->no->ymax = regiao->ymax;
        contruirArvoreLimiarVariavel(regiao->no, profundidade);

        //printf("ne\n");
        regiao->ne = new quadTree[1];
        regiao->ne->xmin = ((regiao->xmax + regiao->xmin)/2);
        regiao->ne->xmax = regiao->xmax;
        regiao->ne->ymin = ((regiao->ymax + regiao->ymin)/2);
        regiao->ne->ymax = regiao->ymax;
        contruirArvoreLimiarVariavel(regiao->ne, profundidade);

        //printf("so\n");
        regiao->so = new quadTree[1];
        regiao->so->xmin = regiao->xmin;
        regiao->so->xmax = (regiao->xmax + regiao->xmin)/2;
        regiao->so->ymin = regiao->ymin;
        regiao->so->ymax = (regiao->ymax + regiao->ymin)/2;
        contruirArvoreLimiarVariavel(regiao->so, profundidade);

        //printf("se\n");
        regiao->se = new quadTree[1];
        regiao->se->xmin = ((regiao->xmax + regiao->xmin)/2);
        regiao->se->xmax = regiao->xmax;
        regiao->se->ymin = regiao->ymin;
        regiao->se->ymax = (regiao->ymax + regiao->ymin)/2;
        contruirArvoreLimiarVariavel(regiao->se, profundidade);
    }
}


void preencherMatriz(int **matriz, quadTree *regiao) {
    for (int l=(regiao->ymin); l<=(regiao->ymax); l++) {
    	for (int c=(regiao->xmin); c<=(regiao->xmax); c++) {
    		if(l==regiao->ymin || l==regiao->ymax || c==regiao->xmin || c==regiao->xmax){
    			matriz[l][c] = -(regiao->cor);
    			if (regiao->cor == 0)
					matriz[l][c] = -256;
    		}else{
				matriz[l][c] = (regiao->cor);
			}
    	}
    }
    if (regiao->no != NULL && regiao->ne != NULL && regiao->so != NULL && regiao->se != NULL) {
		preencherMatriz(matriz, regiao->no);
		preencherMatriz(matriz, regiao->ne);
		preencherMatriz(matriz, regiao->so);
		preencherMatriz(matriz, regiao->se);
    }
}


void libera (quadTree *regiao){
	if (regiao->no != NULL && regiao->ne != NULL && regiao->so != NULL && regiao->se != NULL) {
		libera(regiao->no);
		libera(regiao->ne);
		libera(regiao->so);
		libera(regiao->se);
    }
    
	delete[] regiao;
}


int main(int argc, char *argv[]) {
    abrir_arquivos(argc,argv);
    ler_cabecalho();
    ler_imagem();
    fechar_arquivos();

    preProcessamento();

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    //----------- Primeira Tela - Original ----------------
    glutInitWindowPosition(10,35);
    glutInitWindowSize(ncol,nlin);
    glutCreateWindow("Original");
    glutDisplayFunc(desenhaOriginal);
    // Chama a função responsável por fazer as inicializações
    Inicializa();
    //FIM Primeira Tela


    //----------- Segunda Tela - Dividida Limiar Fixo ----------------
    glutInitWindowPosition(ncol+60,35);
    glutInitWindowSize(ncol,nlin);
    glutCreateWindow("Dividida: Limiar Fixo");
    glutDisplayFunc(desenhaProcessada1);
    // Chama a função responsável por fazer as inicializações
    Inicializa();
    //FIM Segunda Tela


    //----------- Terceira Tela - Dividida Limiar Variavel ----------------
    glutInitWindowPosition((ncol*2) + 60,35);
    glutInitWindowSize(ncol,nlin);
    glutCreateWindow("Dividida: Limiar Variavel");
    glutDisplayFunc(desenhaProcessada2);
    // Chama a função responsável por fazer as inicializações
    Inicializa();
    //FIM  Terceira Tela

    //----------- Quarta Tela - Reconstruida Limiar Fixo ----------------
    glutInitWindowPosition(ncol+60,nlin+60);
    glutInitWindowSize(ncol,nlin);
    glutCreateWindow("Reconstruida: Limiar Fixo");
    glutDisplayFunc(desenhaProcessada3);
    // Chama a função responsável por fazer as inicializações
    Inicializa();
    //FIM  Quarta Tela

    //----------- Quinta Tela - Reconstruida Limiar Variavel ----------------
    glutInitWindowPosition((ncol*2) + 60,nlin+60);
    glutInitWindowSize(ncol,nlin);
    glutCreateWindow("Reconstruida: Limiar Variavel");
    glutDisplayFunc(desenhaProcessada4);
    // Chama a função responsável por fazer as inicializações
    Inicializa();
    //FIM  Quinta Tela

    glutMainLoop();

    return 0;
}

