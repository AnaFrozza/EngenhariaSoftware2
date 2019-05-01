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



float media (List* list, int n){
	float soma, media = 0;
    float tam = n;
	for(list ; list != NULL; list = list->prox){
        soma += list->valor;
    }
	media =  (soma/tam);	
    return media;
}

float variancia(List* list, float avg, int n){
    float soma, var = 0;
    float media = avg;
    float tam = n - 1;
    for(list ; list != NULL; list = list->prox){
        soma += pow(list->valor - media, 2);
    }
	var =  (soma/tam);	
    return var;
}

void vareArquivo(FILE* arq, int n, List* x, List* y){
    char info[n*2];
    const char s[2] = " ";
    char* token;
    double list[n*2];
    int j = 0;
    double avg, var, desvio, PP, P, M, G, GG;
    float e = 2.72; 
    List* xi = criar_Lista();
    List* ln = criar_Lista();
    
    for(int i = 0; (fgets(info, sizeof(info), arq))!=NULL; i++){
        token = strtok(info, s);
        while( token != NULL ) {
            list[j]= strtod(token, NULL);
            // printf("%d - %f\n", j, list[j]);
            token = strtok(NULL, s);
            j++;
        }
    }

    for(int k=0; k<n*2; k++){
        if(k % 2 == 0){
            // printf("X: %.0f\n", list[k]);
            x = inserir(x, list[k]);
            
        }else{
            // printf("Y: %f\n", list[k]);
            y = inserir(y, list[k]);
        }
    }

    for(x, y ; x != NULL && y != NULL; x = x->prox, y = y->prox){
        xi = inserir(xi, (x->valor)/(y->valor));
    }

    for(xi ; xi != NULL; xi = xi->prox){
        ln = inserir(ln, log(xi->valor));
    }

    printf("============ENTRADAS===============\n");
    
    printf("Numero de itens: %d\n", n);
    avg = media(ln, n);
    printf("Media: %.2f \n", avg);
    var = variancia(ln, avg, n);
    printf("Variancia: %.2f \n", var);
    desvio = sqrt(var);
    printf("Desvio Padrao: %.2f \n", desvio);

    printf("============SAIDAS================\n");
    PP = pow(e, avg-(2*desvio));
    printf("Muito pequeno: %.2f \n", PP);    
    P = pow(e, avg-desvio);
    printf("Pequeno: %.2f \n", P);    
    M = pow(e, avg);
    printf("Medio: %.2f \n", M);    
    G = pow(e, avg+desvio);
    printf("Grande: %.2f \n", G);    
    GG = pow(e, avg+(2*desvio));
    printf("Muito grande: %.2f \n", GG);    


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

int main(int argc, char *argv[ ]){
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


// Executar: gcc psp1.1.c -o psp1.1 -lm
        // ./psp1 caso1.1.txt