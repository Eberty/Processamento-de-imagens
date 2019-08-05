/* 		ATIVIDADE 8
 * Implementar um falso DPCM:
 * 		– Usar como preditor a preditor da imagem: valor que mais aparece
 * 		– Calcular a quantidade de bits necessária para armazenar a imagem comprimida:
 * 			– Imagem original: 8 (bits) * resolução
 * 			– Imagem comprimida: varrer a imagem calculando e somando a quantidade de bits necessária para cad pixel (não esquecer de considerar um bit para o sinal)
 * Implementar o descompressor
 * 		– O preditor deve vir no cabeçalho
 * Usar imagens monocromáticas
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

FILE *fpin;
int **imagemR, **imagemG, **imagemB, ncol, nlin, grade;
char controle[4];
int aux[256] = {0}, preditor, totalBits=0;
int **imagem;


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
void criarArquivoComprimido(void);
void abrirArquivoComprimido(void);
int qtdBits(int);


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
        for (lin=0; lin<nlin; lin++) {
            for (col=0; col<ncol; col++) {
                fscanf(fpin,"%d ",&imagemR[lin][col]);
                fscanf(fpin,"%d ",&imagemG[lin][col]);
                fscanf(fpin,"%d ",&imagemB[lin][col]);
            }
        }
    } else {
        for (lin=0; lin<nlin; lin++) {
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

    for (lin=nlin-1, l=0; lin>=0; lin--,l++) {
        for (col=0, c=0; col<ncol; col++, c++) {
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
    int col, lin, l, c, cor;
    glClearColor(0.0,1.0,1.0,0); //Especifica um cor para o fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (lin=nlin-1, l=0; lin>=0; lin--,l++) {
        for (col=0, c=0; col<ncol; col++, c++) {
            cor = imagem[l][c] + preditor;
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


void criarArquivoComprimido(void) {
    int valor;
    for (int l=0; l<nlin; l++) {
        for (int c=0; c<ncol; c++) {
            valor = (imagemR[l][c]+imagemG[l][c]+imagemB[l][c])/3;
            aux[valor]++;
        }
    }

    preditor = 0;
    for(int i=0; i<256; i++) {
        //printf("Valor[%3d]: %d\n", i, aux[i]);
        if (aux[i] > aux[preditor])
            preditor = i;
    }

    //printf("preditor: aux[%d] = %d\n", preditor, aux[preditor]);
    printf("Imagem original:   %d bits\n", 8*ncol*nlin);

    if ((fpin = fopen("arquivoComprimido.txt", "w"))==NULL) {
        printf("Nao foi possivel abrir arquivo de imagem arquivoComprimido.txt\n");
        exit(1);
    }

    fprintf(fpin, "%s\n%d\n%d\n%d\n%d\n", controle, preditor, ncol, nlin, 255);
    for (int l=0; l<nlin; l++) {
        for (int c=0; c<ncol; c++) {
            valor = (imagemR[l][c]+imagemG[l][c]+imagemB[l][c])/3;
            fprintf(fpin, "%d\n", valor-preditor);
        }
    }

    fclose(fpin);
}


void abrirArquivoComprimido(void) {
    int dois55;
    imagem = (int **) malloc((nlin+1)*sizeof(int *));

    for (int cont=0; cont<nlin; cont++) {
        imagem[cont]=(int *)malloc((ncol+1)*sizeof(int));
        if (imagem[cont]==NULL) { /* Alocar memoria para a matriz de pixels */
            printf("Falha na alocacao de memoria - 1\n");
            exit(1);
        }
    }

    if ((fpin = fopen("arquivoComprimido.txt", "r"))==NULL) {
        printf("Nao foi possivel abrir arquivo de imagem arquivoComprimido.txt\n");
        exit(1);
    }

    fscanf(fpin, "%s\n%d\n%d\n%d\n%d\n", controle, &preditor, &ncol, &nlin, &dois55);
    for (int l=0; l<nlin; l++) {
        for (int c=0; c<ncol; c++) {
            fscanf(fpin, "%d\n", &imagem[l][c]);
            totalBits+=qtdBits(imagem[l][c]);
        }
    }
    fclose(fpin);

    printf("Imagem Comprimida: %d bits\n", totalBits);
}



int qtdBits(int x) {
    int qtd = 1, num = 1;
    while(num < abs(x)) {
        num<<=1;
        qtd++;
    }

    if(num == abs(x)) {
        if(x>0 || x == -1) {
            return qtd+1;
        } else {
            return qtd;
        }
    } else {
        return qtd;
    }
}


int main(int argc, char *argv[]) {
    abrir_arquivos(argc,argv);
    ler_cabecalho();
    ler_imagem();
    fechar_arquivos();

    criarArquivoComprimido();
    abrirArquivoComprimido();

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
    glutCreateWindow("Imagem Comprimida");
    glutDisplayFunc(desenhaProcessada);
    // Chama a função responsável por fazer as inicializações
    Inicializa();
    //FIM  Segunda Tela

    glutMainLoop();

    return 0;
}

