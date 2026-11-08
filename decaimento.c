/* Projeto 2: Decaimento radioativo
 *
 *   N(t) = N0 * exp(-lambda*t),   lambda = ln2 / T_meia
 *
 * Mostra tres olhares sobre o mesmo fenomeno:
 *   - CALCULO: derivada numerica e integrais (trapezio x Simpson)
 *   - PROBABILIDADE: simulacao de Monte Carlo atomo a atomo
 *   - QUIMICA: meia-vida e vida media
 */
#include "projetos.h"
#include "mathlib.h"
#include <stdio.h>
#include <math.h>

typedef struct {
    double lambda;
    double n0;
} Decaimento;

static double n_analitico(double t, void *ctx) {
    const Decaimento *d = ctx;
    return d->n0 * exp(-d->lambda * t);
}

/* taxa de decaimento: quantos nucleos decaem por unidade de tempo */
static double taxa(double t, void *ctx) {
    const Decaimento *d = ctx;
    return d->lambda * d->n0 * exp(-d->lambda * t);
}

/* integrando da vida media: t * lambda * exp(-lambda t) */
static double integrando_vida_media(double t, void *ctx) {
    const Decaimento *d = ctx;
    return t * d->lambda * exp(-d->lambda * t);
}

static void parte_calculo(const Decaimento *d, double T) {
    printf("\n--- CALCULO ---\n");

    double dn_num = derivada(n_analitico, T, 1e-4 * T, (void *)d);
    double dn_ref = -d->lambda * n_analitico(T, (void *)d);
    printf("dN/dt em t = T (derivada numerica): %.6f\n", dn_num);
    printf("dN/dt em t = T (-lambda*N)        : %.6f\n", dn_ref);

    double t_fim = 3.0 * T;
    double exato = d->n0 * (1.0 - exp(-d->lambda * t_fim));
    double trap  = integral_trapezio(taxa, 0.0, t_fim, 10, (void *)d);
    double simp  = integral_simpson(taxa, 0.0, t_fim, 10, (void *)d);
    printf("\nNucleos que decaem de 0 a 3T (integral da taxa), n = 10 subintervalos:\n");
    printf("  Exato   : %.4f\n", exato);
    printf("  Trapezio: %.4f (erro %.4f)\n", trap, fabs(trap - exato));
    printf("  Simpson : %.4f (erro %.4f)\n", simp, fabs(simp - exato));

    double vida_media = integral_simpson(integrando_vida_media, 0.0, 20.0 * T, 2000, (void *)d);
    printf("\nVida media = integral de t*lambda*e^(-lambda t): %.5f\n", vida_media);
    printf("Teoria 1/lambda = T/ln2                       : %.5f\n", 1.0 / d->lambda);
}

static void parte_monte_carlo(const Decaimento *d, double T) {
    printf("\n--- MONTE CARLO (cada atomo decide aleatoriamente) ---\n");

    int n0 = (int)d->n0;
    int passos_por_T = 10;
    double dt = T / passos_por_T;
    double p = 1.0 - exp(-d->lambda * dt);   /* prob. de decair em um passo */
    int vivos = n0;
    double t_meia_sim = -1.0;

    printf("%10s %12s %12s %10s\n", "t", "simulado", "teorico", "erro %");
    for (int passo = 0; passo <= 5 * passos_por_T; passo++) {
        double t = passo * dt;
        if (passo % 5 == 0) {
            double teo = n_analitico(t, (void *)d);
            printf("%10.3f %12d %12.1f %9.2f%%\n", t, vivos, teo, 100.0 * (vivos - teo) / teo);
        }
        int antes = vivos;
        int decaem = 0;
        for (int i = 0; i < antes; i++)
            if (aleatorio_uniforme() < p) decaem++;
        vivos -= decaem;

        if (t_meia_sim < 0.0 && vivos <= n0 / 2) {
            /* interpolacao linear dentro do passo */
            t_meia_sim = t + dt * (antes - n0 / 2.0) / (double)(antes - vivos);
        }
    }
    printf("\nMeia-vida estimada pela simulacao: %.4f (real: %.4f)\n", t_meia_sim, T);
}

void projeto_decaimento(void) {
    printf("\n=== DECAIMENTO RADIOATIVO ===\n");
    double T  = ler_numero("Meia-vida (em qualquer unidade de tempo, ex. 8.02 para o iodo-131 em dias): ");
    double n0 = ler_numero("Numero inicial de atomos (ex. 10000): ");
    if (T <= 0.0 || n0 < 10.0) {
        printf("Valores invalidos.\n");
        return;
    }
    Decaimento d = { log(2.0) / T, n0 };
    printf("lambda = ln2 / T = %.6f\n", d.lambda);

    parte_calculo(&d, T);
    parte_monte_carlo(&d, T);
}
