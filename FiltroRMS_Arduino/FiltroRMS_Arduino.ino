/* IDE Arduino: Atuadores controlados por sEMG com filtro IIR + RMS
 * De: Mariana Pedroso Naves
 * Novembro 2025
 * Parte do desenvolvimento da mão mioelétrica - UTFPR
*/

// Não se esqueça de colocar as bibliotecas como "new file" no mesmo sketch
#include "digital_filter.hpp"


// Tamanho da janela usada para o cálculo do RMS.
// Uma janela menor gera resposta mais rápida, porém menos suave.
#define JANELA 100           
#define EMG_PIN A0 // Pino do sinal módulo EMG

// Saídas PWM responsáveis por controlar o motor (ponte H).
// Teste primeiro um dedo, depois declare os demais.
const int IN1 = 5;
const int IN2 = 6;

// ==========================================================================
// Parâmetros de detecção e controle
// ==========================================================================

// Limiar que define quando o RMS indica uma contração muscular.
int LIMIAR_CONTRACAO = 20;         // ideal para EMG filtrado 20–450 Hz
const int HISTERESIS = 5;         // evita que o motor fique tremendo 
const int TEMPO_DEBOUNCE = 800;  // Tempo mínimo entre duas ativações consecutivas
const int PWM_MAX = 255;

// ==========================================================================
// Variáveis 
// ==========================================================================
bool direcao = false;          // Direção do movimento: false = abrir, true = fechar.
bool emgAtivo = false;         // Indica se o sistema reconheceu que há contração ativa.
unsigned long ultimoTempoAcionamento = 0; // Usado para controlar o intervalo entre ativações (debounce).
int amostras[JANELA];   // armazena as últimas N amostras filtradas
int index = 0;     // posição atual no buffer circular
bool bufferCheio = false;

// ==========================================================================
// Cálculo do RMS
// ==========================================================================
double calcular_rms() {
  int64_t soma = 0;

  // Soma dos quadrados das amostras filtradas
  for (int i = 0; i < JANELA; i++) {
    soma += (int64_t)amostras[i] * (int64_t)amostras[i];
  }
  // RMS = sqrt(média dos quadrados)
  return sqrt((double)soma / JANELA);
}

// ==========================================================================
// Configuração inicial
// ==========================================================================
void setup() {
  Serial.begin(115200);

  pinMode(EMG_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {

  double rms = 0;

  // Leitura bruta do sinal emg
  int16_t bruto = analogRead(EMG_PIN);

  // Aplicação do filtro digital IIR (Butterworth 20–450 Hz)
  int16_t filtrado = filter_iir_sample(bruto);

  // Armazena no buffer circular
  amostras[index] = filtrado;
  index++;

  if (index >= JANELA) {
    index = 0;
    bufferCheio = true;
  }

  // Calcula RMS somente quando o buffer estiver cheio
  if (bufferCheio) {
    rms = calcular_rms();
  }

  // Debug serial ( visualização no serial plotter)
  Serial.print(filtrado);
  Serial.print("\t");
  Serial.println(rms);

  // ======================================================================
  // Detecção de contração
  // ======================================================================

  // Detecta início da contração
  if (rms > LIMIAR_CONTRACAO && !emgAtivo) {

    unsigned long agora = millis();

    // Debounce — evita múltiplas ativações
    if (agora - ultimoTempoAcionamento > TEMPO_DEBOUNCE) {

      emgAtivo = true;
      ultimoTempoAcionamento = agora;

      // Alterna direção (abre/fecha)
      direcao = !direcao;

      // Executa o movimento
      moverMotor(direcao);
    }
  }

  // Detecta RELAXAMENTO (histerese)
  if (rms < LIMIAR_CONTRACAO - HISTERESIS) {
    emgAtivo = false;
  }

  delay(1); // mantém ~1000 Hz de amostragem
}

// ==========================================================================
// Movimento do dedo com rampa
// ==========================================================================
void mexerDedo(int IN1, int IN2, int velocidade) {
  //Movimento com rampa curta (liga e desliga o motor --> simula twitch)
  for (int i = 0; i <= 60; i++) {
    analogWrite(IN1, velocidade);
    analogWrite(IN2, 0);
    delay(4);
    analogWrite(IN1, 0);
    delay(1);
  }
}

// ==========================================================================
// Alterna motor abrir/fechar
// ==========================================================================
void moverMotor(bool fechar) {
  // Serial.print("Contração detectada → ");
  // Serial.println(fechar ? "FECHAR" : "ABRIR");

  if (fechar) {
    mexerDedo(IN1, IN2, PWM_MAX);
  } else {
    mexerDedo(IN2, IN1, PWM_MAX); // abre (inverte polaridade do motor)
  }
}

