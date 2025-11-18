/**************************************************************
 * IDE Arduino: Atuadores controlados por potenciômetros
 * De: Mariana Pedroso Naves
 * Novembro de 2025
 * Parte do desenvolvimento da mão mioelétrica - UTFPR
*/

// ======================= DEFINIÇÃO DOS PINOS =======================
// Cada dedo utiliza um par de pinos IN1 e IN2 para controlar o sentido de rotação via DRV8833

// ---------------- POLEGAR (duas partes) ----------------
int polegarExterno_IN1 = 2;  
int polegarExterno_IN2 = 3;
int polegarInterno_IN1 = 4;  
int polegarInterno_IN2 = 5;

// ---------------- DEDOS PRINCIPAIS ----------------
int indicador_IN1 = 6;  
int indicador_IN2 = 7;
int medio_IN1 = 8;  
int medio_IN2 = 9;
int anelar_IN1 = 10;  
int anelar_IN2 = 11;
int minimo_IN1 = 12;  
int minimo_IN2 = 13;

// ========================= POTENCIÔMETROS =========================
// Cada potenciômetro controla um ou mais atuadores
int potPolegarExterno = A0;
int potPolegarInterno = A1;
int potIndicador = A2;
int potResto = A3;  // usado para médio, anelar e mínimo

// ======================== VARIÁVEIS GLOBAIS ========================
int pwm = 0;                     // valor atual de PWM (0–255)
int leitura = 0;                // leitura analógica do potenciômetro
const int ZONA_MORTA = 100;    // evita acionamentos por ruído

// ============================= SETUP ===============================
void setup() {
  Serial.begin(9600);

  // Configura todos os pinos como saída
  pinMode(polegarExterno_IN1, OUTPUT);
  pinMode(polegarExterno_IN2, OUTPUT);
  pinMode(polegarInterno_IN1, OUTPUT);
  pinMode(polegarInterno_IN2, OUTPUT);
  pinMode(indicador_IN1, OUTPUT);
  pinMode(indicador_IN2, OUTPUT);
  pinMode(medio_IN1, OUTPUT);
  pinMode(medio_IN2, OUTPUT);
  pinMode(anelar_IN1, OUTPUT);
  pinMode(anelar_IN2, OUTPUT);
  pinMode(minimo_IN1, OUTPUT);
  pinMode(minimo_IN2, OUTPUT);
}


// ===================== LOOP =====================
void loop() {

   // Polegar possui duas partes independentes
  controlarDedos("Polegar Externo", potPolegarExterno, polegarExterno_IN1, polegarExterno_IN2, 400, 800);
  controlarDedos("Polegar Interno", potPolegarInterno, polegarInterno_IN1, polegarInterno_IN2, 400, 700);

   // Os Demais dedos
  controlarDedos("Indicador", potIndicador, indicador_IN1, indicador_IN2, 400, 1024);
  controlarDedos("Médio", potResto, medio_IN1, medio_IN2, 400, 900);
  controlarDedos("Anelar", potResto, anelar_IN1, anelar_IN2, 400, 900);
  controlarDedos("Mínimo", potResto, minimo_IN1, minimo_IN2, 400, 900);
}

/* ====================================================================
// Função genérica para controle dos dedos a partir de um potenciômetro
// nome          -> nome do dedo para debug
// potPin        -> pino do potenciômetro
// IN1, IN2      -> pinos do driver do motor
// LIMITE_ABRIR  -> faixa que corresponde ao gesto de abrir
// LIMITE_FECHAR -> faixa que corresponde ao gesto de fechar
*/ 
void controlarDedos(String nome, int potPin, int IN1_A, int IN2_A, int LIMITE_ABRIR, int LIMITE_FECHAR) {
  
  leitura = analogRead(potPin);
  
  // Print de depuração no Serial
  Serial.print(nome);
  Serial.print(" | Pot ");
  Serial.print(potPin);
  Serial.print(" = ");
  Serial.println(leitura);

  // ----------------------- ZONA MORTA -----------------------
  // Se o valor está baixo demais, não ativa o motor
  if (leitura < ZONA_MORTA) {
    pararDedos();
    return;
  }


// ----------------------- ABRIR DEDO -----------------------
  if (leitura >= ZONA_MORTA && leitura <= LIMITE_ABRIR) {
    pwm = map(leitura, ZONA_MORTA, LIMITE_ABRIR, 0, 255);
    abrirDedos(IN1_A, IN2_A, pwm);
  }

// ----------------------- FECHAR DEDO -----------------------
  else if (leitura > LIMITE_ABRIR && leitura <= LIMITE_FECHAR) {
    pwm = map(leitura, LIMITE_ABRIR, LIMITE_FECHAR, 0, 255);
    fecharDedos(IN1_A, IN2_A, pwm);
  }

  // ----------------------- LIMITE EXCEDIDO -----------------------
  else {
    Serial.print("*** ");
    Serial.print(nome);
    Serial.println(" atingiu o limite!");
    pararDedos();
  }

  delay(10);
}

// ----------------------- DESATIVAR TODOS OS MOTORES -----------------------
void pararDedos() {
  analogWrite(polegarExterno_IN1, 0);
  analogWrite(polegarExterno_IN2, 0);
  analogWrite(polegarInterno_IN1, 0);
  analogWrite(polegarInterno_IN2, 0);
  analogWrite(indicador_IN1, 0);
  analogWrite(indicador_IN2, 0);
  analogWrite(medio_IN1, 0);
  analogWrite(medio_IN2, 0);
  analogWrite(anelar_IN1, 0);
  analogWrite(anelar_IN2, 0);
  analogWrite(minimo_IN1, 0);
  analogWrite(minimo_IN2, 0);
}

// ====================================================================
// Função para FECHAR um dedo com rampa suave de movimento
// ====================================================================
void fecharDedos(int IN1, int IN2, int velocidade) {
  Serial.println("Fechando dedo...");

  for (int i = 1; i <= 50; i += 1) {
    analogWrite(IN1, 0);
    analogWrite(IN2, velocidade);
    delay(7);
    pararDedos();
    delay(1);
  }
  pararDedos();
}

// ====================================================================
// Função para ABRIR um dedo com rampa suave de movimento
// Mesmo princípio da função fecharDedos()
// ====================================================================
void abrirDedos(int IN1, int IN2, int velocidade) {
  Serial.println("Abrindo dedo...");

  for (int i = 1; i <= 60; i += 1) {
    analogWrite(IN1, velocidade);
    analogWrite(IN2, 0);
    delay(4);
    pararDedos();
    delay(1);
  }
  pararDedos();
}

