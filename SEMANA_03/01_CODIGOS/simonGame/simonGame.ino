// Define os pinos onde os botões estão conectados
const uint8_t btnPins[] = {5, 17, 16, 18};
// Define os pinos onde os LEDs estão conectados
const uint8_t ledPins[] = {27, 26, 25, 33};

// Pino utilizado para resetar o jogo
const int resetBtn = 2;
// Define o comprimento máximo da sequência do jogo
#define MAX_SEQ_LENGTH 100
// Array para armazenar a sequência do jogo
uint8_t gameSeq[MAX_SEQ_LENGTH] = {0};
// Índice atual da sequência do jogo
uint8_t gameIdx = 0;
void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);
  // Configura os pinos dos LEDs como saída e os pinos dos botões como entrada com resistor de pull-up
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(btnPins[i], INPUT_PULLUP);
  }
  // Inicializa o gerador de números aleatórios
  randomSeed(analogRead(4));
}
void lightLedAndPlayTone(byte ledIndex) {
  // Acende o LED no índice especificado
  digitalWrite(ledPins[ledIndex], HIGH);
  // Mantém o LED aceso por um curto período
  delay(300);
  // Apaga o LED
  digitalWrite(ledPins[ledIndex], LOW);
}
void playSequence() {
  // Reproduz a sequência de LEDs armazenada até o momento
  for (int i = 0; i < gameIdx; i++) {
    byte currentLed = gameSeq[i];
    lightLedAndPlayTone(currentLed);
    // Intervalo entre cada LED acendendo
    delay(100);
  }
}
byte readButtons() {
  // Lê o estado dos botões continuamente
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = btnPins[i];
      // Verifica se algum botão foi pressionado
      if (digitalRead(buttonPin) == HIGH) {
        return i;
      }
    }
    // Verifica se o botão de reset foi pressionado
    if (digitalRead(resetBtn) == HIGH){
      Serial.println("Reset button pressed!");
      // Retorna um valor especial para indicar o reset
      return -1;
    }
    // Pequeno atraso para evitar leitura muito rápida
    delay(1);
  }
}
void gameOver() {
  // Imprime a pontuação e reseta o índice da sequência
  Serial.print("Game over! Your score: ");
  Serial.println(gameIdx - 1);
  gameIdx = 0;
  // Pequeno atraso antes de reiniciar
  delay(200);
}
bool checkUserSequence() {
  // Verifica se a sequência inserida pelo usuário está correta
  for (int i = 0; i < gameIdx; i++) {
    byte expectedButton = gameSeq[i];
    byte actualButton = readButtons();
    // Se um botão errado for pressionado
    if (expectedButton != actualButton) {
      return false;
    }
    // Acende o LED correspondente ao botão pressionado
    lightLedAndPlayTone(actualButton);
  }
  // Se a sequência estiver correta
  return true;
}
void loop() {
  // Adiciona uma cor aleatória ao final da sequência
  gameSeq[gameIdx] = random(0, 4);
  gameIdx++;
  // Impede que a sequência exceda o comprimento máximo
  if (gameIdx >= MAX_SEQ_LENGTH) {
    gameIdx = MAX_SEQ_LENGTH - 1;
  }
  // Reproduz a sequência de LEDs para o usuário
  playSequence();
  // Verifica a sequência inserida pelo usuário
  if (!checkUserSequence()) {
    // Se a sequência estiver errada, termina o jogo
    gameOver();
  }
  // Adiciona um pequeno atraso antes de iniciar a próxima rodada
  delay(300);
  // Adiciona um atraso adicional se uma nova cor foi adicionada à sequência
  if (gameIdx > 0) {
    delay(300);
  }
}