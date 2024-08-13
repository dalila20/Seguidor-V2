#include "Config.hpp"
#include "Basics.hpp"

bool identificarCruzamentoPadrao(int* leituraDosSensores)
{
	return (!identificarSensoresAlternados(leituraDosSensores)
			&& leituraDosSensores[0]
			&& leituraDosSensores[NUM_SENSORES - 1]
			&& identificarLinha(leituraDosSensores, 2, NUM_SENSORES - 1));
}

bool identificarCruzamentoIncompleto(int* leituraDosSensores)
{
	if (leituraDosSensores[0] || leituraDosSensores[NUM_SENSORES - 1])
	{
		for(int i = 1; i < NUM_SENSORES - 1; i++)
		{
			if(leituraDosSensores[i]) return false;
		}
		
		return true;
	}

	else {
		return false;
	}
}

bool identificarCruzamento(int* leituraDosSensores)
{
	if (identificarCruzamentoPadrao(leituraDosSensores))
	{
		return true;
	}
	else if (identificarCruzamentoIncompleto(leituraDosSensores))
	{
		return true;
	}
	else {
		return false;
	}
}

int quadradosEsquerda = 0;
int quadradosDireita = 0;
int direcao = 0;
bool sensorDireitoEmLeitura = false;
bool sensorEsquerdaEmLeitura = false;

void identificarQuadrados(int* leituraDosSensores)
{
	if (sensorDireitoEmLeitura && !leituraDosSensores[NUM_SENSORES - 1])
	{
		sensorDireitoEmLeitura = false;
		quadradosDireita++;
		direcao = 1;
	}
	if (sensorEsquerdaEmLeitura && !leituraDosSensores[0])
	{
		sensorEsquerdaEmLeitura = false;
		quadradosEsquerda++;
		direcao = -1;
	}
	if (leituraDosSensores[0] && !sensorEsquerdaEmLeitura)
	{
		sensorEsquerdaEmLeitura = true;

		// Serial.print(quadradosEsquerda);
		// Serial.print(" - ");
		// Serial.println(quadradosDireita);
		// printSens(leituraDosSensores);
	}
	if (leituraDosSensores[NUM_SENSORES - 1] && !sensorDireitoEmLeitura)
	{
		sensorDireitoEmLeitura = true;

		// Serial.print(quadradosEsquerda);
		// Serial.print(" - ");
		// Serial.println(quadradosDireita);
		// printSens(leituraDosSensores);
	}
}

void apenasSeguir(int* leituraDosSensores)
{
	float erro = calcularErro(leituraDosSensores);
  	// Serial.println(erro);
	seguirLinha(erro);
}

void loop()
{
	int leituraDosSensores[NUM_SENSORES];
	lerSensores(leituraDosSensores);

  // Debug - pinos analogicos
	// Serial.print(analogRead(SENSOR3));
	// Serial.print(" ");
  // Serial.print(digitalRead(SENSOR3));
  // Serial.print(" ");
	// Serial.print(analogRead(SENSOR4));
	// Serial.print(" ");
  // Serial.print(digitalRead(SENSOR4));
  // Serial.print(" ");
	// Serial.print(analogRead(SENSOR5));
	// Serial.print(" ");
  // Serial.print(digitalRead(SENSOR5));
  // Serial.print(" ");
	// Serial.print(analogRead(SENSOR6));
	// Serial.print(" ");
  // Serial.print(digitalRead(SENSOR6));
  // Serial.print(" ");
	// Serial.print(analogRead(SENSOR7));
  // Serial.print(" ");
  // Serial.print(digitalRead(SENSOR7));
	// Serial.println("");

	// printarErro(); Ver PID no monitor
	// printSens(leituraDosSensores);
	// Serial.println(calcularPID(calcularErro(leituraDosSensores)));

	// Serial.println((calcularErro(leituraDosSensores)));
	// printSens(leituraDosSensores);
	// return;

	if (identificarLinha(leituraDosSensores))
	{
		// Serial.println("identificarLinha");
		apenasSeguir(leituraDosSensores);
	}
	// Encontrar novamente a pista
	else if (abs(erroAnterior) > 1)
	{
		seguirLinha(erroAnterior, false);
	}
	// // Entrar no menu
	// else if (leituraDosSensores[0] && leituraDosSensores[NUM_SENSORES - 1])
	// {
    // // Serial.println("configMain");
	// 	configMain();
	// }
	// Parar
	else
	{
		controlarMotores(0,0);
	}
}