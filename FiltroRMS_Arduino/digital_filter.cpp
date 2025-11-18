#include "digital_filter.hpp"

// vetor de estado (8 elementos, já que temos 9 coeficientes)
double z[N_COEF - 1] = {0};

// -----------------------------------------------------------
//      IMPLEMENTAÇÃO DO FILTRO (Transposed Direct Form II)
// -----------------------------------------------------------

int16_t filter_iir_sample(int16_t sample) {

  double x = static_cast<double>(sample);

  // saída do filtro
  double y = b[0] * x + z[0];

  // atualiza estados
  for (int i = 1; i < N_COEF - 1; i++) {
    z[i - 1] = b[i] * x + z[i] - a[i] * y;
  }

  // atualiza último estado
  z[N_COEF - 2] = b[N_COEF - 1] * x - a[N_COEF - 1] * y;

  // retorna como inteiro
  return static_cast<int16_t>(y);
}
