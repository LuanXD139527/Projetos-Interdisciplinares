#include "mathlib.h"
#include <math.h>
#include <stdlib.h>

/* ---------- Estatística ---------- */

double media(const double *v, size_t n) {
    double soma = 0.0;
    for (size_t i = 0; i < n; i++) soma += v[i];
    return soma / (double)n;
}

double variancia(const double *v, size_t n) {
    if (n < 2) return 0.0;
    double m = media(v, n), soma = 0.0;
    for (size_t i = 0; i < n; i++) soma += (v[i] - m) * (v[i] - m);
    return soma / (double)(n - 1);
}

double desvio_padrao(const double *v, size_t n) {
    return sqrt(variancia(v, n));
}

Regressao regressao_linear(const double *x, const double *y, size_t n) {
    double mx = media(x, n), my = media(y, n);
    double sxy = 0.0, sxx = 0.0, syy = 0.0;
    for (size_t i = 0; i < n; i++) {
        double dx = x[i] - mx, dy = y[i] - my;
        sxy += dx * dy;
        sxx += dx * dx;
        syy += dy * dy;
    }
    Regressao r;
    r.a  = sxy / sxx;
    r.b  = my - r.a * mx;
    r.r2 = (syy == 0.0) ? 1.0 : (sxy * sxy) / (sxx * syy);
    return r;
}

double aleatorio_uniforme(void) {
    return rand() / ((double)RAND_MAX + 1.0);
}

double aleatorio_normal(double mu, double sigma) {
    double u1 = 1.0 - aleatorio_uniforme();   /* evita log(0) */
    double u2 = aleatorio_uniforme();
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
    return mu + sigma * z;
}

/* ---------- Vetores ---------- */

Vetor3 vet_soma(Vetor3 a, Vetor3 b) {
    return (Vetor3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vetor3 vet_escala(Vetor3 a, double k) {
    return (Vetor3){a.x * k, a.y * k, a.z * k};
}

double vet_escalar(Vetor3 a, Vetor3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vetor3 vet_vetorial(Vetor3 a, Vetor3 b) {
    return (Vetor3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

double vet_modulo(Vetor3 a) {
    return sqrt(vet_escalar(a, a));
}

double vet_angulo(Vetor3 a, Vetor3 b) {
    double c = vet_escalar(a, b) / (vet_modulo(a) * vet_modulo(b));
    if (c > 1.0) c = 1.0;      /* protege acos de erros de arredondamento */
    if (c < -1.0) c = -1.0;
    return acos(c) * 180.0 / PI;
}

/* ---------- Cálculo ---------- */

double derivada(Funcao f, double x, double h, void *ctx) {
    return (f(x + h, ctx) - f(x - h, ctx)) / (2.0 * h);
}

double integral_trapezio(Funcao f, double a, double b, int n, void *ctx) {
    double h = (b - a) / n;
    double soma = (f(a, ctx) + f(b, ctx)) / 2.0;
    for (int i = 1; i < n; i++) soma += f(a + i * h, ctx);
    return soma * h;
}

double integral_simpson(Funcao f, double a, double b, int n, void *ctx) {
    if (n % 2 != 0) n++;                 /* Simpson exige n par */
    double h = (b - a) / n;
    double soma = f(a, ctx) + f(b, ctx);
    for (int i = 1; i < n; i++)
        soma += f(a + i * h, ctx) * (i % 2 == 0 ? 2.0 : 4.0);
    return soma * h / 3.0;
}
