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

double calc_sigma (List* x, List* y, int n, double b0, double b1){
	double var_sigma, soma = 0;
    float  var = 0;
	for(x, y ; x != NULL && y != NULL; x = x->prox, y = y->prox){
		soma = pow(y->valor - b0 - (b1 * x->valor), 2);
        var_sigma += soma;
	}
    // printf("var sigma : %f\n", var_sigma);
    var_sigma = 0.125 * var_sigma;
	return var_sigma;
}

double calc_var_range (List* x, int n, float xavg){
	double var_range, soma = 0;
    double  var = 0;
	for(x ; x != NULL; x = x->prox){
		soma = pow(x->valor - xavg, 2);
        var_range += soma;
	}
    // printf("varRange : %f\n ", var_range);
    var = pow(386 - xavg, 2);
    // printf("var : %f\n", var);
    var_range = 1 + 0.1 * ( var / var_range);
	return var_range;
}

// =================== Calcula X e P ===================================

float gama(float n) {
    int aux = n;
    if(aux == n){
        if(n == 1  || n == 0){
            return 1;
        }else{
            return gama(n-1)*n;
        }
    }else {
        if(n == 0.5){
            return 0.5*sqrt(3.14159);
        }else{
            return gama(n-1)*n;
        }
    }
}

float calculaF0(float dof, float pi){
    float parte1, parte2, f = 0;
    parte1 = gama(((dof+1)/2)-1)/((sqrt(dof * pi))*gama((dof/2)-1));
    parte2 = (pow(1+((pow(0,2))/dof), ((dof+1)/2)*(-1)));
    f = parte1 * parte2;
    return f;
}

float calculaFx(float dof, float pi, float x){
    float parte1, parte2, f = 0;
    parte1 = gama(((dof+1)/2)-1)/((sqrt(dof * pi))*gama((dof/2)-1));
    parte2 = (pow(1+((pow(x,2))/dof), ((dof+1)/2)*(-1)));
    f = parte1 * parte2;
    return f;
}

float calculaSoma1(float dof, float pi, float num_seg, float w){
    int i;
    float valor = 0;
    for(i=1; i<(num_seg-1); i+=2){
        valor += 4*calculaFx(dof, pi, i*w);
    }
    return valor;
}

float calculaSoma2(float dof, float pi, float num_seg, float w){
    int i;
    float valor = 0;
    for(i=2; i<(num_seg-2); i+=2){
        valor += 2*calculaFx(dof, pi, i*w);
    }
    return valor;
}

float calculaX(float num_seg, float x, float dof, float p, float d){
    float pi = 3.14159;
    float w, diferenca = 0;
    float e = 0.0001; 
    float primeiroSoma, segundoSoma, f0, fx, parte1, parte2, p1, p2 = 0;
    // printf("============Primeiro P====================\n");
    // Calculando o primeiro p
    // printf("Num: %f\n", num_seg);
    w = x/num_seg;
    f0 = calculaFx(dof, pi, 0);
    // printf("f0: %f\n", f0);
    primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
    // printf("Soma1: %f\n", primeiroSoma);
    segundoSoma = calculaSoma2(dof, pi, num_seg, w);
    // printf("Soma2: %f\n", segundoSoma);
    fx = calculaFx(dof, pi, x);
    // printf("fx: %f\n", fx);
    p1 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
    // printf("P1: %f \n", p1);

    // printf("============Segundo P====================\n");
    // Calculando o segundo p    
    num_seg *= 2;
    // printf("Num: %f\n", num_seg);
    w = x/num_seg;
    f0 = calculaFx(dof, pi, 0);
    // printf("f0: %f\n", f0);
    primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
    // printf("Soma1: %f\n", primeiroSoma);
    segundoSoma = calculaSoma2(dof, pi, num_seg, w);
    // printf("Soma2: %f\n", segundoSoma);
    fx = calculaFx(dof, pi, x);
    // printf("fx: %f\n", fx);
    p2 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
    // printf("P2: %f \n", p2);
    
    // printf("============Verificando===================\n");
    // Verificando o erro
    while ((p1 - p2) > e){
        p1 = p2;
        num_seg *= 2;
        // printf("Num: %f\n", num_seg);
        w = x/num_seg;
        f0 = calculaFx(dof, pi, 0);
        // printf("f0: %f\n", f0);
        primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
        // printf("Soma1: %f\n", primeiroSoma);    
        segundoSoma = calculaSoma2(dof, pi, num_seg, w);
        // printf("Soma2: %f\n", segundoSoma);    
        fx = calculaFx(dof, pi, x);
        // printf("fx: %f\n", fx);
        p2 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
        // printf("p2: %f\n", p2);
        // printf("============Calculado P===================\n");

    } 
    // printf("X: %f\n", x);

    if((p - p2) < 0){
        diferenca = (p - p2) * -1;
    } else{
        diferenca = (p - p2);
    }    

    if(diferenca < e){
        return x;
    } else{
        if(p - p2 < 0){
            d = d/2;
        }
        // printf("===============DENOVO==================\n");
        if(p2 > p){
            x -= d;
        }else{
            x += d;
        }

        return calculaX(num_seg, x, dof, p, d);
    }
}

float calculaP(float num_seg, float x, float dof, float p1, float p2){
    float pi = 3.14159;
    float w = 0;
    float e = 0.00001; 
    float primeiroSoma, segundoSoma, f0, fx, parte1, parte2 = 0;
    // printf("============Primeiro P====================\n");
    // Calculando o primeiro p
    // printf("Num: %f\n", num_seg);
    w = x/num_seg;
    f0 = calculaF0(dof, pi);
    // printf("f0: %f\n", f0);
    primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
    // printf("Soma1: %f\n", primeiroSoma);
    segundoSoma = calculaSoma2(dof, pi, num_seg, w);
    // printf("Soma2: %f\n", segundoSoma);
    fx = calculaFx(dof, pi, x);
    // printf("fx: %f\n", fx);
    p1 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
    // printf("P1: %f \n", p1);

    // printf("============Segundo P====================\n");
    // Calculando o segundo p    
    num_seg *= 2;
    // printf("Num: %f\n", num_seg);
    w = x/num_seg;
    f0 = calculaF0(dof, pi);
    // printf("f0: %f\n", f0);
    primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
    // printf("Soma1: %f\n", primeiroSoma);
    segundoSoma = calculaSoma2(dof, pi, num_seg, w);
    // printf("Soma2: %f\n", segundoSoma);
    fx = calculaFx(dof, pi, x);
    // printf("fx: %f\n", fx);
    p2 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
    // printf("P2: %f \n", p2);
    
    // printf("============Verificando===================\n");
    // Verificando o erro
    while ((p1 - p2) > e){
        p1 = p2;
        num_seg *= 2;
        // printf("Num: %f\n", num_seg);
        w = x/num_seg;
        f0 = calculaF0(dof, pi);
        // printf("f0: %f\n", f0);
        primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
        // printf("Soma1: %f\n", primeiroSoma);    
        segundoSoma = calculaSoma2(dof, pi, num_seg, w);
        // printf("Soma2: %f\n", segundoSoma);    
        fx = calculaFx(dof, pi, x);
        // printf("fx: %f\n", fx);
        p2 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
        // printf("p2: %f\n", p2);
        // printf("============Calculado P===================\n");

    } 
    return p2;
}


// ====================================================

float t_calc(float p, int dof){
    float num_seg = 10;
    float x = 1.0;
    float d = 0.5;

    x = calculaX(num_seg, x, dof, p, d);
    return x;
}

float p_calc(float x, int dof){
    float num_seg = 10;
    float p1, p2 = 0;
    float p = calculaP(num_seg, x, dof, p1, p2);
    return p;
}

void vareArquivo(FILE* arq, int n, List* x, List* y){
    char info[n*2];
    const char s[2] = " ";
    char* token;
    double list[n*2];
    int j = 0;
    double xk = 386;
    double x_avg, y_avg, x_soma, y_soma, x2_soma, y2_soma, xy_soma; 
    List* x2 = criar_Lista();
    List* y2 = criar_Lista();

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
            x2 = inserir(x2, (list[k]*list[k]));
        }else{
            // printf("Y: %f\n", list[k]);
            y = inserir(y, list[k]);
            y2 = inserir(y2, (list[k]*list[k]));
        }
    }

    printf("============ENTRADAS===============\n");
    
    printf("Numero de itens: %d\n", n);
    x_avg = media(x, n);
    printf("Media X: %.2f \n",x_avg);
    y_avg = media(y, n);
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
    float t = t_calc(0.35, n-2);
    // printf("t: %f\n", t);


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

    double modulo_rxy = rxy;
    if(modulo_rxy < 0){
        modulo_rxy *= -1;
    }

    float x_var = (modulo_rxy * sqrt(n-2))/sqrt(1-pow(rxy,2));
    // printf("Variavel X: %f\n", x_var);

    double p = p_calc(x_var, n-2);
    printf("P: %f\n", p);
    double tail_area = 1-2*p;
    printf("Tail Area: %f\n", tail_area);
    double sigma = sqrt(calc_sigma(x, y, n, b0, b1));
    printf("Sigma: %f\n", sigma);
    double var_range = sqrt(calc_var_range (x, n, x_avg));
    // printf("RangeVar: %f\n", var_range);
    double range = t * sigma * var_range;
    printf("Range: %f\n", range);
    double UPI = yk + range;
    printf("UPI: %f\n", UPI);
    double LPI = yk - range;
    printf("LPI: %f\n", LPI);

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

// Executar: gcc main.c -o main -lm
        // ./main caso1.txt