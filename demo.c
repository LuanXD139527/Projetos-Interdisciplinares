/* Projeto 3: demonstracao da mini biblioteca matematica */
#include "projetos.h"
#include "mathlib.h"
#include <stdio.h>
#include <math.h>

static double f_seno(double x, void *ctx) {
    (void)ctx;
    return sin(x);
}

void projeto_biblioteca(void) {
    printf("\n=== DEMO DA MATHLIB ===\n");

    /* Estatistica */
    double dados[] = {7.5, 8.0, 6.5, 9.0, 7.0, 8.5, 10.0, 6.0};
    size_t n = sizeof(dados) / sizeof(dados[0]);
    printf("\n[Estatistica] notas: media = %.3f, variancia = %.3f, desvio = %.3f\n",
           media(dados, n), variancia(dados, n), desvio_padrao(dados, n));

    /* Vetores */
    Vetor3 a = {1, 0, 0}, b = {1, 1, 0};
    Vetor3 s = vet_soma(a, b), v = vet_vetorial(a, b);
    printf("\n[Vetores] a = (1,0,0), b = (1,1,0)\n");
    printf("  a + b        = (%.1f, %.1f, %.1f)\n", s.x, s.y, s.z);
    printf("  a . b        = %.3f\n", vet_escalar(a, b));
    printf("  a x b        = (%.1f, %.1f, %.1f)\n", v.x, v.y, v.z);
    printf("  |b|          = %.4f\n", vet_modulo(b));
    printf("  angulo(a,b)  = %.2f graus\n", vet_angulo(a, b));

    /* Derivada: efeito do passo h */
    double x0 = PI / 3.0, exata = cos(x0);
    printf("\n[Derivada] f(x) = sen(x) em x = pi/3, valor exato cos(x) = %.10f\n", exata);
    printf("  %-10s %-16s %-12s\n", "h", "aproximacao", "erro");
    double hs[] = {1e-1, 1e-3, 1e-5, 1e-8, 1e-12};
    for (int i = 0; i < 5; i++) {
        double d = derivada(f_seno, x0, hs[i], NULL);
        printf("  %-10.0e %-16.10f %-12.2e\n", hs[i], d, fabs(d - exata));
    }
    printf("  (repare: h muito pequeno piora o resultado por erro de arredondamento!)\n");

    /* Integral */
    printf("\n[Integral] sen(x) de 0 a pi, valor exato = 2\n");
    printf("  %-6s %-14s %-14s\n", "n", "trapezio", "simpson");
    int ns[] = {4, 8, 16, 64};
    for (int i = 0; i < 4; i++)
        printf("  %-6d %-14.10f %-14.10f\n", ns[i],
               integral_trapezio(f_seno, 0.0, PI, ns[i], NULL),
               integral_simpson(f_seno, 0.0, PI, ns[i], NULL));
}
