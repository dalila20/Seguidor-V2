#include "Config.hpp"

// Debug
void printSens(const int* lei)
{
	for(int i = 0; i < NUM_SENSORES; i++)
	{
		Serial.print(lei[i]);
	}
	Serial.print("\n");
}

// Geral
void lerSensores(int* vetorDeLeitura)
{
	for(int i = 0; i < NUM_SENSORES; i++)
	{
		if (sensores[i] == A0 || sensores[i] == A1
		|| sensores[i] == A2 || sensores[i] == A3
		|| sensores[i] == A4)
		{
			int leitura = analogRead(sensores[i]);
			if (leitura <= 80) {
				vetorDeLeitura[i] = 1;
			} else {
				vetorDeLeitura[i] = 0;
			}
		} else {
			vetorDeLeitura[i] = !digitalRead(sensores[i]);
		}
    // Serial.print(vetorDeLeitura[i]);
	}
  // Serial.println("");
}

// Unused
int contarSensoresAtivos(const int* leituraDosSensores, int inicial = 1, int final = 9)
{
  	int total = 0;
	for (int i = inicial; i < final; i++)
	{
		if (leituraDosSensores[i])
		{
			 total++;
		}
	}
	return total;
}

// Unused
void bip(int count = 1, int base = 1000)
{
  	for(int i = 1; i <= count; i++)
	{
		tone(BUZZER, base, 150);
    	delay(300);
  	}
}

void verificarBateria(){
	
	pinMode(BUZZER, OUTPUT);
	pinMode(TENSAO, INPUT);
	// if(analogRead(TENSAO) < 350){ Para tocar quando ligado ao computador
	// 		tone(BUZZER, 2000,100); // Emite um som de 1000Hz (1kHz)
	// }
	if (analogRead(TENSAO) < 810 && analogRead(TENSAO) > 350)
	{
		while (1)
		{
			tone(BUZZER, 1000); // Emite um som de 1000Hz (1kHz)
			delay(500); // Espera 1 segundo
			noTone(BUZZER); // Para o som
			delay(500); // Espera 1 segundo
		}
	}
}

// Para os motores ou envia as velocidades desejadas
void controlarMotores(const int &pwmMotorEsquerdo,const int &pwmMotorDireito)
{
	if (pwmMotorDireito == 0) {
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, LOW);
	} else {
		analogWrite(ENA, abs(pwmMotorDireito));
		if (pwmMotorDireito > 0)
		{
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, HIGH);
		} else {
			digitalWrite(IN2, LOW);
			digitalWrite(IN1, HIGH);
		}
	}
  
	if (pwmMotorEsquerdo == 0) {
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, LOW);
	} else {
    	analogWrite(ENB, abs(pwmMotorEsquerdo));
		if (pwmMotorEsquerdo > 0)
		{
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
		} else {
			digitalWrite(IN4, LOW);
			digitalWrite(IN3, HIGH);
		}
  	}
}

// Unused
bool linhaCentralizada(const int* leituraDosSensores)
{
	for(int i = 0; i < 9; i++)
	{
		if (i > 2 && i < 6)
		{
			if (!leituraDosSensores[i])
			{
				return false;
			}
		}
		else if(leituraDosSensores[i])
		{
			return false;
		}
	}
	return true;
}

// 
void conversao(int direcao)
{
	int leituraDosSensores[NUM_SENSORES];
	lerSensores(leituraDosSensores);

	if(direcao > 0)
	{
		controlarMotores(-velocidadeDeCurva, velocidadeDeCurva);
	}
	else if (direcao < 0)
	{
		controlarMotores(velocidadeDeCurva, -velocidadeDeCurva);
	}
	else
	{
		return;
	}

	// Rodo até o sensor do meio ficar inativo
	while(leituraDosSensores[4])
	{
		lerSensores(leituraDosSensores);
	}

	// Depois rodo até o sensor do meio ficar ativo
	while(!leituraDosSensores[4])
	{
		lerSensores(leituraDosSensores);
	}
}

int identificarSensoresAlternados(const int* leituraDosSensores, int inicial = 1, int final = NUM_SENSORES - 1)
{
	int index = inicial;
	int sensoresAlternados = 0;
	bool sensorAtivo = false;

	//Encontrando o primeiro sensor ativo
	for (; index < final; index++)
	{
		if(leituraDosSensores[index])
		{
			sensorAtivo = true;
			break; 
		}
	}
	for (; index < final; index++)
	{
		if (leituraDosSensores[index] && !sensorAtivo)
		{
			sensoresAlternados++;
			sensorAtivo = true;
		}
		else if (!leituraDosSensores[index])
		{
			sensorAtivo = false;
		}

	}

	return sensoresAlternados;
}

// 2 a 6
int identificarLinha(const int* leituraDosSensores, int inicial = 2, int final = NUM_SENSORES - 2){
	for(int i = inicial; i < final; i++)
	{
		if (leituraDosSensores[i])
		{
			return 1;
		}
	}
	return 0;
}

float erroAnterior = 0;
float calcularErro(const int* leituraDosSensores,
					int inicial = 2,
					int final = NUM_SENSORES - 2,
					bool salvarErro = true)
{
	float erro = 0;
	int somatorioSensores = 0;
	int pesos[NUM_SENSORES] = {0, 0, -4, -1, 0, 1, 4, 0, 0};
	for (int i = inicial; i < final; i++)
	{
		erro += leituraDosSensores[i] * pesos[i];
		somatorioSensores += leituraDosSensores[i];
	}

	if (somatorioSensores != 0)
	{
		erro = erro / somatorioSensores;
	}

	if (erro != 0 && salvarErro) erroAnterior = erro;
	return erro;
}

float calcularPID(const float& erro, bool fullPID = true)
{
	static unsigned long prevTime = 0;
	static float prevErrorPID = 0;
	static float integral = 0;
	static float derivative = 0;
	float erroPID;

	unsigned long currentTime = millis();
	unsigned long dt = currentTime - prevTime;

	if(!fullPID) {
		prevTime = 0;
		prevErrorPID = 0;
		integral = 0;
		derivative = 0;
	}
	if(dt > PIDRefreshRate && fullPID){
		derivative = (erro - prevErrorPID) * dt;
		// if(derivative < 20) derivative = 0;
		integral += erro * dt;
		
		prevErrorPID = erro;
		prevTime = currentTime;
	}


	if(ki*integral > MaxIntegral) integral = MaxIntegral/ki;
  erroPID = (erro * kp) + (ki * integral) + (kd * derivative);

  return erroPID;
}

// Loop para selecionar configuracoes
int configMain()
{
  bip(1, 400);
  int selecionado = 0;
  int selecionadoAnterior = -1;
  int sair = 0;

  while (sair < 3)
  {
    selecionado = 0;
    delay(200);
    for(int i = 2; i < 7; i++)
	{ 
      if(!digitalRead(sensores[i])) selecionado = i-1;
    }

    if(selecionado == selecionadoAnterior) sair++;
    if(selecionado) selecionadoAnterior = selecionado;
  }

  bip(selecionado);

  switch (selecionado)
  {
  case 1: //Velocidade Normal
    setSpeedA = nominalSetSPDA;
    setSpeedB = nominalSetSPDB;
    break;

  case 2: //Velocidade alta
    setSpeedA = maxSetSPDA;
    setSpeedB = maxSetSPDB;
    break;
    
  default:
    break;
  }
  delay(2000);
}

void setup()
{
	Serial.begin(9600);
  	ponteHSetup();
  	sensoresSetup();
	verificarBateria();
}

void printarErro(float erro)
{
	static unsigned long previousMillis = 0;  // Variável para armazenar o tempo da última impressão
	const long interval = 250;
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= interval)
	{
		// Salva o tempo atual para a próxima verificação
		previousMillis = currentMillis;

		// Aqui você pode inserir a mensagem que deseja imprimir
		Serial.println(erro);
	}
}

void seguirLinha(const float &erro, bool fullPID = true){
	float erroPID = calcularPID(erro, fullPID);
	// printarErro(erroPID);
	int pwmMotorDireito = 0;
	int pwmMotorEsquerdo = 0;
	
	float erroPIDA = (maxSetSPDA * erroPID) / maxSetSPDB;
	float erroPIDB = (erroPID);

	pwmMotorDireito = constrain(setSpeedA - (erroPIDA), 0, MaxSpeedA);
	pwmMotorEsquerdo = constrain(setSpeedB + (erroPIDB), 0, MaxSpeedB);
	
  // Debug - pwm
  // Serial.print(pwmMotorDireito);
	// Serial.print(" ");
	// Serial.print(pwmMotorEsquerdo);
	// Serial.println("");

	controlarMotores(pwmMotorDireito, pwmMotorEsquerdo);
}