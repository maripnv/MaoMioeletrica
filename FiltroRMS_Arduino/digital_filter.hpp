#ifndef DIGITAL_FILTER_HPP
#define DIGITAL_FILTER_HPP

#include <Arduino.h>

#define N_COEF 9   // porque temos 9 coeficientes

// -------------------------
// Coeficientes do filtro IIR ( Inserir os filtros adquiridos no Colab)
// -------------------------

constexpr double b[N_COEF] = {
  0.55979398,
  0.0,
  -2.2391759,
  0.0,
  3.35876385,
  0.0,
  -2.2391759,
  0.0,
  0.55979398
};

constexpr double a[N_COEF] = {
  1.0,
  -0.49401762,
  -2.74104783,
  0.94030607,
  3.04723869,
  -0.64685113,
  -1.57145979,
  0.15479744,
  0.31337125
};

// Estados internos do filtro
extern double z[N_COEF - 1];

// Função que filtra uma única amostra
extern int16_t filter_iir_sample(int16_t sample);

#endif
