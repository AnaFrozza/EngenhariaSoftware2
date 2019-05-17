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

float calculaP(float num_seg, float x, float dof, float p, float d){
    float pi = 3.14159;
    float w, diferenca = 0;
    float e = 0.0001; 
    float primeiroSoma, segundoSoma, f0, fx, parte1, parte2, p1, p2 = 0;
    printf("============Primeiro P====================\n");
    // Calculando o primeiro p
    printf("Num: %f\n", num_seg);
    w = x/num_seg;
    f0 = calculaFx(dof, pi, 0);
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
    f0 = calculaFx(dof, pi, 0);
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
        f0 = calculaFx(dof, pi, 0);
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
    printf("X: %f\n", x);

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
        printf("===============DENOVO==================\n");
        if(p2 > p){
            x -= d;
        }else{
            x += d;
        }

        return calculaP(num_seg, x, dof, p, d);
    }
}

int main(){
    float p , var_p = 0;
    float dof = 0;
    printf("Informe o valor de p: \n");
    scanf("%f",&p);
    printf("Informe o valor de graus de liberdade: \n");
    scanf("%f",&dof);
    float num_seg = 10;
    float x = 1.0;
    float d = 0.5;

    x = calculaP(num_seg, x, dof, p, d);
    printf("X: %f\n", x);
    printf("=========================================\n");

    return 0;
}
