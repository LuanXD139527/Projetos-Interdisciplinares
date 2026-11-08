#ifndef MATHLIB_H
#define MATHLIB_H

#include <stddef.h>

#define PI 3.14159265358979323846

/* Função matemática f(x). O ponteiro 'ctx' carrega parâmetros extras
   (ex.: a constante de decaimento). Se não precisar, passe NULL. */
typedef double (*Funcao)(double x, void *ctx);

/* ---------- Probabilidade e Estatística ---------- */
double media(const double *v, size_t n);
double variancia(const double *v, size_t n);      /* amostral (n-1) */
double desvio_padrao(const double *v, size_t n);

/* Regressão linear simples: y = a*x + b */
typedef struct {
    double a;   /* coeficiente angular */
    double b;   /* coeficiente linear  */
    double r2;  /* coeficiente de determinação R² */
} Regressao;
Regressao regressao_linear(const double *x, const double *y, size_t n);

double aleatorio_uniforme(void);                  /* em [0, 1) */
double aleatorio_normal(double mu, double sigma); /* Box-Muller */

/* ---------- Geometria Analítica ---------- */
typedef struct { double x, y, z; } Vetor3;
Vetor3 vet_soma(Vetor3 a, Vetor3 b);
Vetor3 vet_escala(Vetor3 a, double k);
double vet_escalar(Vetor3 a, Vetor3 b);           /* produto escalar */
Vetor3 vet_vetorial(Vetor3 a, Vetor3 b);          /* produto vetorial */
double vet_modulo(Vetor3 a);
double vet_angulo(Vetor3 a, Vetor3 b);            /* em graus */

/* ---------- Cálculo ---------- */
double derivada(Funcao f, double x, double h, void *ctx);   /* diferença central */
double integral_trapezio(Funcao f, double a, double b, int n, void *ctx);
double integral_simpson(Funcao f, double a, double b, int n, void *ctx);

#endif
