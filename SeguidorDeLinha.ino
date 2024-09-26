#include "Config.hpp"
#include "Basics.hpp"

bool curvaDireitaIdentificada = false;
bool curvaEsquerdaIdentificada = false;
bool sensorCurvaAtivo = false;

void apenasSeguir(int* leituraDosSensores)
{
	float erro = calcularErro(leituraDosSensores);
  // Serial.println(erro);
	seguirLinha(erro);
}

void protocoloCurva90(int* s)
{

  int count = 0;
  for (int G = 1; G < 8; G++)
  {
    count += s[G];
  }

  // Indica quantos sensores estão ativos para início da curva - se 7 estiverem ativos, indica uma faixa branca perpendicular à direção do carro
  // Modifiquei para maior ou igual a 3 para identificar meia linha
  if (count >= 3)
  {

    // Para o seguidor quando detecta a linha
    Serial.println("Linha reta!");
    controlarMotores(0, 0);
    delay(100);

    lerSensores(s);

    // Acionará a curva à direita
    if (curvaDireitaIdentificada)
    {
      Serial.println("Curva direita!");
      while (!s[7] || !s[8])
      {
        lerSensores(s);
        controlarMotores(30, velocidadeCurva90);
        delay(40); // 40
      }
      curvaDireitaIdentificada = false; // Indica que a curva foi concluída 
    }

    else if (curvaEsquerdaIdentificada) // Acionará a curva à esquerda 
    {
      Serial.println("Curva esquerda!");
      while (!s[0] || !s[1])
      {
        lerSensores(s);
        controlarMotores(velocidadeCurva90, 30);
        delay(40);
      }
      curvaEsquerdaIdentificada = false; // Indica que a curva foi concluída
    }
  }

  else
  { 
    // Continua seguindo a linha reta
    lerSensores(s);
    apenasSeguir(s);
  }
}

// Percebe o quadro da curva de 90º à direita, quando os 2 sensores são ativos
void identificarCurva90(int* s)
{
  if (s[8] || s[0]) {
    sensorCurvaAtivo = true;
  }

  else {
  sensorCurvaAtivo = false;
  }

  if (sensorCurvaAtivo)
  {
    if (s[1] && !s[2] && !s[7] && !s[8])
    { // Identifica curva à esquerda
      curvaEsquerdaIdentificada = true;
    }

    else if (s[7] && !s[6] && !s[0] && !s[1])
    { // Identifica curva à direita
      curvaDireitaIdentificada = true;
    }
  }
}

int speed = 0;
void loop()
{
	int leituraDosSensores[NUM_SENSORES];
	lerSensores(leituraDosSensores);
	
	if (identificarLinha(leituraDosSensores))
	{
    identificarCurva90(leituraDosSensores);

    if (curvaDireitaIdentificada || curvaEsquerdaIdentificada) {
      Serial.println("Curva de 90 identificada");

      lerSensores(leituraDosSensores);
      protocoloCurva90(leituraDosSensores);
    }
    else {
      apenasSeguir(leituraDosSensores);
    }

		// Serial.println("identificarLinha");
		apenasSeguir(leituraDosSensores);
	}

	// Encontrar novamente a pista
	else if (abs(erroAnterior) > 1)
	{
		// Serial.print("Linha perdida: ");
		seguirLinha(erroAnterior);
	}

	// Parar
	else
	{
		Serial.println("Sem linha!");
		controlarMotores(0,0);
    
    // Resetando PID
    calcularPID(0, false);
	}
}