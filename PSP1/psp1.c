/* -------------------------------
UTFPR-CM DACOM Engenharia de Software
Autor: Ana Carolina Frozza 
---------------------------------- */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct List{
    float valor;
    struct List *prox;
} List;

List* criar_Lista(){
    return NULL;
}

List* inserir (List* raiz, float num){
    List* novo;
    novo = (List*) malloc(sizeof(List));  
    novo->valor = num;
    novo->prox = raiz;
    return novo;
}

float media (List* list){
	float soma, media = 0;
	int tam = 0;
	for(list ; list != NULL; list = list->prox, tam++){
		soma += list->valor;
	}
	media =  (soma/tam);	
    return media;
}

float soma (List* list){
	float soma = 0;
	for(list ; list != NULL; list = list->prox){
		soma += list->valor;
	}
	return soma;
}

float multiplica (List* x, List* y){
	float soma, multiplica = 0;
	for(x, y ; x != NULL && y != NULL; x = x->prox, y = y->prox){
		multiplica = x->valor * y->valor;
        soma += multiplica;
	}
	return soma;
}

void vareArquivo(FILE* arq, int n, List* x, List* y){
    char info[n];
    const char s[2] = " ";
    char* token;
    float list[n*2];
    int j = 0;
    double xk = 386;
    double x_avg, y_avg,x_soma, y_soma, x2_soma, y2_soma, xy_soma; 
    List* x2 = criar_Lista();
    List* y2 = criar_Lista();

    for(int i = 0; (fgets(info, sizeof(info), arq))!=NULL; i++){
        token = strtok(info, s);
        while( token != NULL ) {
            list[j]= atof(token);
            token = strtok(NULL, s);
            j++;
        }
    }

    for(int k=0; k<n*2; k++){
        if(k % 2 == 0){
            x = inserir(x, list[k]);
            x2 = inserir(x2, (list[k]*list[k]));
        }else{
            y = inserir(y, list[k]);
            y2 = inserir(y2, (list[k]*list[k]));
        }
    }

    printf("============ENTRADAS===============\n");
    
    printf("Numero de itens: %d\n", n);
    x_avg = media(x);
    printf("Media X: %.2f \n",x_avg);
    y_avg = media(y);
    printf("Media Y: %.2f \n",y_avg);
    x_soma = soma(x);
    printf("Soma X: %.0f \n",x_soma);
    y_soma = soma(y);
    printf("Soma Y: %.2f \n",y_soma);
    x2_soma = soma(x2);
    printf("Soma X2: %.0f \n",x2_soma);
    y2_soma = soma(y2);
    printf("Soma Y2: %.2f \n",y2_soma);
    xy_soma = multiplica(x, y);
    printf("Soma X*Y: %.0f \n",xy_soma);

    printf("============SAIDAS================\n");
    double b1 = (xy_soma - (n*x_avg*y_avg))/(x2_soma - (n*(x_avg*x_avg)));
    printf("B1: %f\n", b1);
    double b0 = y_avg - (b1*x_avg);
    printf("B0: %f\n", b0);
    double var = ((n*x2_soma) - (x_soma*x_soma)) * ((n*y2_soma)-(y_soma*y_soma));
    if(var < 0){
        var *= -1;
    }
    double rxy = ((n*xy_soma)-(x_soma*y_soma))/sqrt(var); 
    printf("rxy: %f\n", rxy);
    double r2 = rxy*rxy;
    printf("r2: %f\n", r2);
    double yk = b0 + (b1*xk);
    printf("Yk: %f\n", yk);

}

int numeroElementos(FILE* arq){
    int n = 0;
    char ch;
    while( (ch=fgetc(arq))!= EOF ){
        if(ch == '\n'){
            n++;
        }       
    }
    return n;
}

void remove_list (List* list){
	while (list != NULL) {
		List* p = list->prox;
		free(list);
		list = p;
	}
}

int main( int argc, char *argv[ ] ){
    FILE* entrada;
    List* x = criar_Lista();
    List* y = criar_Lista();
    
    entrada = fopen(argv[1],"r");
    if(entrada == NULL){
      printf("Arquivo de entrada invalido\n");
    }else{
        int n = numeroElementos(entrada);
        entrada = fopen(argv[1],"r");
        vareArquivo(entrada, n, x, y);
        fclose(entrada);
    } 
    remove_list(x);
    remove_list(y);
    return 0;
}