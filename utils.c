#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

// Função auxiliar para gerar números aleatórios com distribuição exponencial
double exponential_random(double lambda) {
    double u = rand() / (RAND_MAX + 1.0);
    return -log(1 - u) / lambda;
}

// Função auxiliar para gerar números aleatórios com distribuição normal (Box-Muller)
double normal_random(double mean, double stddev) {
    static bool hasSpare = false;
    static double spare;

    if (hasSpare) {
        hasSpare = false;
        return mean + stddev * spare;
    }

    hasSpare = true;
    double u, v, s;
    do {
        u = (rand() / (RAND_MAX + 1.0)) * 2.0 - 1.0;
        v = (rand() / (RAND_MAX + 1.0)) * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    s = sqrt(-2.0 * log(s) / s);
    spare = v * s;
    return mean + stddev * u * s;
}

// Função auxiliar para gerar números inteiros aleatórios em um intervalo
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Função auxiliar para gerar números com distribuição de Poisson
int poisson_random(double lambda) {
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;

    do {
        k++;
        p *= rand() / (RAND_MAX + 1.0);
    } while (p > L);

    return k - 1;
}
