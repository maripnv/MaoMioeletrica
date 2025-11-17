# 🦾 Mão Mioelétrica 
O presente documento descreve de forma detalhada os procedimentos necessários para executar os códigos referentes ao desenvolvimento do protótipo de prótese de mão mioelétrica. A estrutura deste repositório contempla tanto os algoritmos embarcados utilizados no microcontrolador quanto os scripts auxiliares responsáveis por aquisição, tratamento e análise de sinais.

Em termos gerais, o projeto engloba três frentes principais:

1. Controle dos Atuadores, composto pelos drivers ponte H, rotinas PWM e lógica de abertura e fechamento dos dedos;
2. Processamento de Sinais EMG, responsável pela leitura, filtragem, cálculo de RMS e interpretação do esforço muscular;
3. Ferramentas de Aquisição e Análise, que incluem os scripts Python para coleta via porta serial, geração de CSV e plotagem dos sinais.

Todos os códigos foram desenvolvidos, testados e validados em ambiente embarcado utilizando o microcontrolador ATmega2560 (Arduino Mega), bem como em ambiente computacional para análises, utilizando Python 3.13.

## 🖥️ Ambiente de Desenvolvimento

Os testes e execuções foram conduzidos em máquina local configurada com:
* Sistema operacional: Windows 11
* IDE principal: Arduino IDE 2.3.3 / Google Colab e VS Code
* Placa utilizada: Arduino Mega 2560 e Arduino Nano

Sensores e módulos:
* Módulo EMG (AD8232 adaptado para EMG)
* Drivers DRV8833
* Motores CC n20

Além disso, os scripts Python utilizados para aquisição de dados foram testados no Google Colab com Python 3, sem necessidade da GPU: T4.

## Estrutura da Aplicação

O repositório está organizado em módulos que representam as principais funcionalidades do sistema:

**1. IDE Arduino: Atuadores controlados por potenciômetros**
* Controle PWM dos dedos;
* Rampas de aceleração;
* Leitura de potenciômetros;
* Acionamento dos motores via DRV8833;
* Máquina de estados para alternar entre abrir/fechar.
* Cada dedo é controlado por um par IN1/IN2 (sentido horário e anti horário)

**2. Google Colab: Filtro Butterworth**
* Geração dos coeficientes para a implementações na IDE do Arduino
* Diagrama bode

**3. IDE Arduino: Atuadores controlados por sEMG com filtro IIR + RMS**
* Leitura contínua do sinal bruto do A0;
* Armazenamento em janela fixa;
* Implementação do filtro IIR
* Cálculo do valor RMS;

**4. VS Code: Leitura de dados pela porta serial e comparação entre o sinal bruto com o RMS**
Inclui scripts auxiliares responsáveis por:
* Leitura de dados pela porta serial;
* Salvamento automático em arquivos .csv;
* Plotagens comparativas entre sinal bruto e RMS;
* Geração de gráficos de análise para documentação e TCC.

## 🚀 Execução dos Códigos

Para executar os códigos embarcados:

* Conecte o módulo EMG ao Arduino Mega conforme o diagrama disponível no repositório.
* Faça upload do arquivo .ino correspondente.
* Ajuste a porta serial, a placa e a taxa de baud quando necessário (está configurada para 9600).
* Caso deseje realizar aquisição dos dados, execute o script Python informando a porta COM utilizada (não se esqueça de fechar a IDE do Arduino, se não executa).

## 🔧 Melhorias
* Utilizar a biblioteca TimerInterrupt para melhorar as rotinas;
* Fazer uso do millis();
