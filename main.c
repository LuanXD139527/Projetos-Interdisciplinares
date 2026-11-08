#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "projetos.h"

#ifdef _WIN32
#include <windows.h>
#endif

double ler_numero(const char *prompt) {
    double valor;
    for (;;) {
        printf("%s", prompt);
        int r = scanf("%lf", &valor);
        if (r == 1) return valor;
        if (r == EOF) exit(0);
        while (getchar() != '\n') { }   /* descarta a entrada invalida */
        printf("Entrada invalida, tente de novo.\n");
    }
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);   /* console do CLion mostra o prompt na hora */
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    srand((unsigned)time(NULL));

    int opcao;
    do {
        printf("\n===== LAB INTERDISCIPLINAR EM C =====\n");
        printf("1) Cinetica quimica (Quimica + Estatistica)\n");
        printf("2) Decaimento radioativo (Quimica + Calculo + Probabilidade)\n");
        printf("3) Demo da biblioteca matematica\n");
        printf("0) Sair\n");
        opcao = (int)ler_numero("Escolha: ");

        switch (opcao) {
            case 1: projeto_cinetica();   break;
            case 2: projeto_decaimento(); break;
            case 3: projeto_biblioteca(); break;
            case 0: printf("Ate mais!\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
