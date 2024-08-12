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

void seguirTest(int* leituraDosSensores){

	//	Seguidor identificando mais de uma linha
	if(identificarSensoresAlternados(leituraDosSensores)){
		float erro = calcularErro(leituraDosSensores, 3, 7, false);
		seguirLinha(erro * 1.5);
	}

	// Leitura duvidosa seguir reto
	else if(contarSensoresAtivos(leituraDosSensores) >= 5){
		float erro = calcularErro(leituraDosSensores, 3, 7, false);
		seguirLinha(erro * -1.5);
	}

	// Seguir linha normalmente
	else{
		float erro = calcularErro(leituraDosSensores);
		seguirLinha(erro);
	}

	if(identificarCruzamento(leituraDosSensores)){
		//	Curva de 90
		if((quadradosDireita + quadradosEsquerda) == 1){
			conversao(direcao);
		}
		//	Desafio de re
		else if(quadradosDireita == 1 && quadradosEsquerda == 1){
			controlarMotores(0,0);
			bip(2);
			while (1){}
		}
		//	Rotatoria
		else if(quadradosDireita >= 2 || quadradosEsquerda >= 2){
			controlarMotores(0,0);
			bip(3);
			while (1){}
		}
		//	Desafio nao identificado
		else{
			bip();
		}

		sensorEsquerdaEmLeitura = false;
		sensorDireitoEmLeitura = false;
		quadradosEsquerda = 0;
		quadradosDireita = 0;
		direcao = 0;
	}

	else if(identificarLinha(leituraDosSensores, 2,8) && !identificarSensoresAlternados(leituraDosSensores)){
		identificarQuadrados(leituraDosSensores);
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

	// printarErro(); Ver PID no monitor
	// printSens(leituraDosSensores);
	// Serial.println(calcularPID(calcularErro(leituraDosSensores)));

	// Serial.println((calcularErro(leituraDosSensores)));
	// printSens(leituraDosSensores);
	// return;

	if (identificarLinha(leituraDosSensores))
	{
		// seguirTest(leituraDosSensores);
		// Serial.println("identificarLinha");
		apenasSeguir(leituraDosSensores);
	}
	// Encontrar novamente a pista
	else if (abs(erroAnterior) > 1)
	{
		seguirLinha(erroAnterior, false);
	}
	// Entrar no menu
	else if (leituraDosSensores[0] && leituraDosSensores[NUM_SENSORES - 1])
	{
    // Serial.println("configMain");
		configMain();
	}
	// Parar
	else
	{
		controlarMotores(0,0);
	}
}