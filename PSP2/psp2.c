/* -------------------------------
UTFPR-CM DACOM Engenharia de Software
Autor: Ana Carolina Frozza 
---------------------------------- */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

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

float calculaP(float num_seg, float x, float dof, float p1, float p2){
    float pi = 3.14159;
    float w = 0;
    float e = 0.00001; 
    float primeiroSoma, segundoSoma, f0, fx, parte1, parte2 = 0;
    printf("============Primeiro P====================\n");
    // Calculando o primeiro p
    printf("Num: %f\n", num_seg);
    w = x/num_seg;
    f0 = calculaF0(dof, pi);
    printf("f0: %f\n", f0);
    primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
    printf("Soma1: %f\n", primeiroSoma);
    segundoSoma = calculaSoma2(dof, pi, num_seg, w);
    printf("Soma2: %f\n", segundoSoma);
    fx = calculaFx(dof, pi, x);
    printf("fx: %f\n", fx);
    p1 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
    printf("P1: %f \n", p1);

    printf("============Segundo P====================\n");
    // Calculando o segundo p    
    num_seg *= 2;
    printf("Num: %f\n", num_seg);
    w = x/num_seg;
    f0 = calculaF0(dof, pi);
    printf("f0: %f\n", f0);
    primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
    printf("Soma1: %f\n", primeiroSoma);
    segundoSoma = calculaSoma2(dof, pi, num_seg, w);
    printf("Soma2: %f\n", segundoSoma);
    fx = calculaFx(dof, pi, x);
    printf("fx: %f\n", fx);
    p2 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
    printf("P2: %f \n", p2);
    
    printf("============Verificando===================\n");
    // Verificando o erro
    while ((p1 - p2) > e){
        p1 = p2;
        num_seg *= 2;
        printf("Num: %f\n", num_seg);
        w = x/num_seg;
        f0 = calculaF0(dof, pi);
        printf("f0: %f\n", f0);
        primeiroSoma = calculaSoma1(dof, pi, num_seg, w);
        printf("Soma1: %f\n", primeiroSoma);    
        segundoSoma = calculaSoma2(dof, pi, num_seg, w);
        printf("Soma2: %f\n", segundoSoma);    
        fx = calculaFx(dof, pi, x);
        printf("fx: %f\n", fx);
        p2 = (w/3)* (f0 + primeiroSoma + segundoSoma + fx);
        printf("p2: %f\n", p2);
        printf("============Calculado P===================\n");

    } 
    return p2;
}

int main(){
    float num_seg = 0;
    float x = 0;
    float dof = 0;
    printf("Informe o valor de x: \n");
    scanf("%f",&x);
    printf("Informe o valor de graus de liberdade: \n");
    scanf("%f",&dof);
    printf("Informe o numero de segmento: \n");
    scanf("%f",&num_seg);
    float p1, p2 = 0;
    float p = calculaP(num_seg, x, dof, p1, p2);
    printf("p: %f\n", p);
    printf("==========================================\n");

    return 0;
}

// Executar: gcc psp2.c -o psp2 -lm
        // ./psp2