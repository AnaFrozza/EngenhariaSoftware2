/* -------------------------------
UTFPR-CM DACOM Engenharia de Software
Autor: Ana Carolina Frozza 
---------------------------------- */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct List{
    float num;
    struct List *prox;
} List;

List* criar_Lista(){
    return NULL;
}

List* inserir (List* raiz, float valor){
    List* novo;
    novo = (List*) malloc(sizeof(List));  
    novo->num = valor;
    novo->prox = raiz;
    return novo;
}

float media (List* list){
	float soma, media = 0;
	int tam = 0;

	for(list ; list != NULL; list = list->prox, tam++){
		soma += list->num;
	}

	// printf("soma da media: %.2f \n",soma);
	media =  (soma/tam);
	printf("Media: %.2f \n",media);

	// printf("---------------------\n");
	return media;
}

void desvio_padrao(List* list){
	float x_avg = media(list);
	float soma, x = 0;
	int tam = 0;
	float desvio;

	for(list ; list != NULL; list = list->prox , tam++){
		// printf("list->num: %.2f\n", list->num);

		// printf("media: %.2f ->", x_avg);
		x = pow(list->num - x_avg, 2);
		// printf("x: %f\n", x);
		soma += x;
		// printf("soma: %f\n", soma);
		
	}
	// printf("-----------\n");
	// printf("tam :%d\n", tam - 1);
	// printf("%.2f ->",sqrt(soma / (tam - 1)));

	desvio = sqrt(soma / (tam - 1));
	// printf("desvio: %.2f\n", desvio);

	printf("Desvio padrão: %.2f\n", desvio);

}

void remove_list (List* list){
	while (list != NULL) {
		List* p = list->prox;
		free(list);
		list = p;
	}
}


int main(){
	List* list;
	float num;
	int n = 10;
	list = criar_Lista();
	
	printf("Digite %d numeros para calular o desvio padrão:\n", n);
	for(int i = 0; i < n; i++){
		scanf("%f", &num);
		list = inserir(list, num);
	}
	
	desvio_padrao(list);
	
	remove_list(list);

	return 0;
}
