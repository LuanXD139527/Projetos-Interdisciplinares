/* Projeto 1: Cinética química com regressão linear
 *
 * Uma reação A -> produtos pode ser de ordem 0, 1 ou 2. Cada ordem vira uma
 * RETA se plotarmos a grandeza certa contra o tempo:
 *   ordem 0:  [A]    x t   (inclinação = -k)
 *   ordem 1:  ln[A]  x t   (inclinação = -k)
 *   ordem 2:  1/[A]  x t   (inclinação = +k)
 * O programa ajusta as três retas e escolhe a ordem com maior R².
 */
#include "projetos.h"
#include "mathlib.h"
#include <stdio.h>
#include <math.h>

#define MAX_PONTOS 50

static void gerar_dados_simulados(double *t, double *conc, int *n) {
    const double A0 = 1.0;        /* mol/L */
    const double k_real = 0.05;   /* 1/s, reação de 1ª ordem */
    const double ruido = 0.02;    /* 2% de erro experimental */

    *n = 11;
    printf("Gerando dados de uma reacao de 1a ordem (k real = %.3f 1/s) com ruido de %.0f%%...\n",
           k_real, ruido * 100);
    for (int i = 0; i < *n; i++) {
        t[i] = i * 10.0;
        conc[i] = A0 * exp(-k_real * t[i]) * aleatorio_normal(1.0, ruido);
    }
}

static int ler_dados_manual(double *t, double *conc, int *n) {
    *n = (int)ler_numero("Quantos pontos (3 a 50)? ");
    if (*n < 3 || *n > MAX_PONTOS) {
        printf("Quantidade invalida.\n");
        return 0;
    }
    for (int i = 0; i < *n; i++) {
        printf("Ponto %d\n", i + 1);
        t[i] = ler_numero("  tempo (s): ");
        conc[i] = ler_numero("  [A] (mol/L, > 0): ");
        if (conc[i] <= 0.0) {
            printf("Concentracao deve ser positiva.\n");
            return 0;
        }
    }
    return 1;
}

void projeto_cinetica(void) {
    double t[MAX_PONTOS], conc[MAX_PONTOS];
    double y0[MAX_PONTOS], y1[MAX_PONTOS], y2[MAX_PONTOS];
    int n;

    printf("\n=== CINETICA QUIMICA: descobrindo a ordem da reacao ===\n");
    printf("1) Usar dados simulados\n2) Digitar meus dados\n");
    int op = (int)ler_numero("Escolha: ");

    if (op == 2) {
        if (!ler_dados_manual(t, conc, &n)) return;
    } else {
        gerar_dados_simulados(t, conc, &n);
    }

    printf("\n%8s %12s %12s %12s\n", "t (s)", "[A]", "ln[A]", "1/[A]");
    for (int i = 0; i < n; i++) {
        y0[i] = conc[i];
        y1[i] = log(conc[i]);
        y2[i] = 1.0 / conc[i];
        printf("%8.1f %12.5f %12.5f %12.5f\n", t[i], y0[i], y1[i], y2[i]);
    }

    Regressao r[3];
    r[0] = regressao_linear(t, y0, n);
    r[1] = regressao_linear(t, y1, n);
    r[2] = regressao_linear(t, y2, n);

    printf("\nAjuste linear de cada hipotese:\n");
    for (int i = 0; i < 3; i++)
        printf("  Ordem %d: R^2 = %.6f\n", i, r[i].r2);

    int melhor = 0;
    for (int i = 1; i < 3; i++)
        if (r[i].r2 > r[melhor].r2) melhor = i;

    double A0 = conc[0];
    double k, t_meia;
    if (melhor == 0)      { k = -r[0].a; t_meia = A0 / (2.0 * k); }
    else if (melhor == 1) { k = -r[1].a; t_meia = log(2.0) / k; }
    else                  { k =  r[2].a; t_meia = 1.0 / (k * A0); }

    printf("\n>> Melhor ajuste: ordem %d\n", melhor);
    printf(">> Constante de velocidade k = %.5f\n", k);
    printf(">> Tempo de meia-vida t1/2   = %.2f s\n", t_meia);
    if (k <= 0.0)
        printf("(k nao positivo: confira se os dados representam consumo de A)\n");
}
