#pragma once
// Sensor
#define SENSOR1 8
#define SENSOR2 A7
#define SENSOR3 A6
#define SENSOR4 A5
#define SENSOR5 A4
#define SENSOR6 A3
#define SENSOR7 A2
#define SENSOR8 A1
#define SENSOR9 7

#define NUM_SENSORES 9

#define BUZZER 11
#define TENSAO A5

const int sensores[NUM_SENSORES] = {SENSOR1, SENSOR2, SENSOR3, SENSOR4,
                                    SENSOR5, SENSOR6, SENSOR7, SENSOR8, SENSOR9};

void sensoresSetup()
{
  for (int i = 0; i < NUM_SENSORES; i++)
  {
    pinMode(sensores[i], INPUT);
  }
}

// Ponte H
#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 6

void ponteHSetup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// Cerca de 65 no PWM é capaz de fazer o seguidor começar a se mover
// Isso equivale a cerca de 4v
// #define nominalSetSPDA 80
// #define nominalSetSPDB 80

// #define MaxSpeedA 75
// #define MaxSpeedB 75

// Os novos motores são menos potentes
#define nominalSetSPDA 150 // 150
#define nominalSetSPDB 150

#define MaxSpeedA 150
#define MaxSpeedB 150

#define kp 45 // 35
#define kd 350 // 350
#define ki 0.01 // 0.08
int setSpeedA = nominalSetSPDA;
int setSpeedB = nominalSetSPDB;
#define MaxIntegral 650
#define PIDRefreshRate 30 // tempo (ms)

#define velocidadeCurva90 150
