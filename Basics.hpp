#include "Config.hpp"

// Geral
void lerSensores(int* vetorDeLeitura)
{
	for (int i = 0; i < NUM_SENSORES; i++)
	{
		if (i > 0 && i < NUM_SENSORES - 2) {
			int leitura = analogRead(sensores[i]);
			if (leitura <= 100) {
				vetorDeLeitura[i] = 1;
			} else {
				vetorDeLeitura[i] = 0;
			}
		} 
		else {
			vetorDeLeitura[i] = !digitalRead(sensores[i]);
		}
    Serial.print(vetorDeLeitura[i]);
	}
  Serial.println("");
}

// Não utilizado
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
		if (pwmMotorDireito > 0)
		{
			analogWrite(IN1, abs(pwmMotorDireito));
			digitalWrite(IN2, LOW);
		} else {
			digitalWrite(IN1, LOW);
			analogWrite(IN2, abs(pwmMotorDireito));
		}
	}
  
	if (pwmMotorEsquerdo == 0) {
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, LOW);
	} else {
		if (pwmMotorEsquerdo > 0)
		{
			analogWrite(IN3, abs(pwmMotorEsquerdo));
			digitalWrite(IN4, LOW);
		} else {
			digitalWrite(IN3, LOW);
			analogWrite(IN4, abs(pwmMotorEsquerdo));
		}
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
	int pesos[NUM_SENSORES] = {0, -5, -2, -1, 0, 1, 2, 5, 0};
	for (int i = inicial; i < final; i++)
	{
		erro += leituraDosSensores[i] * pesos[i];
		if(pesos[i] != 0) somatorioSensores += leituraDosSensores[i];
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
	unsigned long currentTime = millis();

	static unsigned long prevTime = currentTime;
	static float prevErrorPID = 0;
	static float integral = 0;
	static float derivative = 0;
	float erroPID;

	unsigned long dt = currentTime - prevTime;

  // Reset PID
	if(!fullPID) {
		prevTime = 0;
		prevErrorPID = 0;
		integral = 0;
		derivative = 0;
	}

	if(dt > PIDRefreshRate && fullPID){
		derivative = (erro - prevErrorPID) / dt;
		integral += erro * dt;
    
    if (integral > MaxIntegral) integral = MaxIntegral;
		else if (integral < -MaxIntegral) integral = -MaxIntegral;
		
		prevErrorPID = erro;
		prevTime = currentTime;
	}


  erroPID = (erro * kp) + (ki * integral) + (kd * derivative);

  return erroPID;
}

void setup()
{	
	Serial.begin(9600);
  	ponteHSetup();
  	sensoresSetup();
	// verificarBateria();
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
	
	pwmMotorDireito = constrain(setSpeedA + ( erroPID ), 0, MaxSpeedA);
	pwmMotorEsquerdo = constrain(setSpeedB - ( erroPID ), 0, MaxSpeedB);
  // Serial.println(erroPID);
	controlarMotores(pwmMotorEsquerdo, pwmMotorDireito);

}