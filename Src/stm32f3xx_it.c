/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */
extern TIM_HandleTypeDef htim3;

struct lcdBlock{
	int up;
	int down;
};
struct lcdBlock;

//LCD Characters
typedef unsigned char byte;
byte UWDW[] = {
		  0x0E,
		  0x11,
		  0x0E,
		  0x00,
		  0x0E,
		  0x11,
		  0x0E,
		  0x00
};

byte UBDB[] = {
		  0x0E,
		  0x1F,
		  0x0E,
		  0x00,
		  0x0E,
		  0x1F,
		  0x0E,
		  0x00
};

byte UWDB[] = {
		  0x0E,
		  0x11,
		  0x0E,
		  0x00,
		  0x0E,
		  0x1F,
		  0x0E,
		  0x00
};

byte UBDW[] = {
		  0x0E,
		  0x1F,
		  0x0E,
		  0x00,
		  0x0E,
		  0x11,
		  0x0E,
		  0x00
};

byte UBDN[] = {
		  0x0E,
		  0x1F,
		  0x0E,
		  0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x00
};

byte UWDN[] = {
		  0x0E,
		  0x11,
		  0x0E,
		  0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x00
};

byte UNDB[] = {
		  0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x0E,
		  0x1F,
		  0x0E,
		  0x00
};

byte UNDW[] = {
		  0x00,
		  0x00,
		  0x00,
		  0x00,
		  0x0E,
		  0x11,
		  0x0E,
		  0x00
};


// Declaring Functions ... like ffs it's right there

void UpdatePos();
void UpdatePlayerName();
void UpdateCurrentBoard();
void CheckDifferences();
void ChangeLCDBlock(int x,int y);
void TryMakingMove();
void ActivateBuzzer();

int isLegalMove();
int Translate(struct lcdBlock block);


//Timer Variables
int totalTime = 60;
int currentTime = 60;
int sevenSegment = 0;
int buzzerOn = 0;
int buzzerCounter = 0;

//Board LED's Variables
int wrongMove = 0;
int ledOn = 0;
int toggleCounter = 0;

//Movement Variables
int currentX;
int currentY;
int movingX;
int movingY;
int firstClick = 0;

//Name Variables
int characterCounter = 0;
unsigned char firstName[7];
unsigned char secondName[7];

//End Game Variables
int whiteScore = 0;
int blackScore = 0;
int winner = 0;

//Callback Variables

GPIO_TypeDef *const Row_ports[] = {GPIOD, GPIOD, GPIOD, GPIOD};
const uint16_t Row_pins[] = {GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0};

GPIO_TypeDef *const Column_ports[] = {GPIOD, GPIOD, GPIOD, GPIOD};
const uint16_t Column_pins[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};
volatile uint32_t last_gpio_exti;

//Board Variables
int codeBoard[8][8];				//1 Means white , 2 means black and 0 means null
struct lcdBlock previousLCDBoard[4][8];
struct lcdBlock currentLCDBoard[4][8];

//Player Variables
int player = 2;
int white = 1;
int black = 2;

//Volume Vars
float volumeValue;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
//	for(int i = 0; i < 4 ; i++){
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_RESET);
//
//		  if(i == 1){	//Moving down (pressed 10)
//			  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET);
//			  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)){
////				  if(firstClick == 0)
////					  InitializeMap();
////				  currentX++;
////				  currentX = currentX%8;
////				  UpdatePos();
//			  }
//		  }
//
//	  }
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_SET);
//	  while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0));
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 and Touch Sense controller.
  */
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */
//	for(int i = 0; i < 4 ; i++){
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_RESET);
//
//		  if(i == 0){		//Moving left (pressed 5)
//			  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_SET);
//			  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//				  currentY--;
//				  if(currentY == -1)
//					  currentY = 7;
//				  UpdatePos();
//			  }
//		  }
//
//		  else if(i == 1){   //Pressed middle button (5)
//			  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET);
//			  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//				  if(IsLegalMove()){
//					  codeBoard[currentX][currentY] = player;
//					  FlipAllBrackets();
//					  if(!SwitchPlayer())
//						  EndGame();
////					  UpdateCurrentBoard();
//				  }
//			  }
//		  }
//		  else if(i == 2){	//Moving right (pressed 7)
//			  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_SET);
//			  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)){
//				  currentY++;
//				  currentY = currentY%8;
//				  UpdatePos();
//			  }
//		  }
//
//	  }
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_SET);
//	  while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0));
  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */

  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
//	for(int i = 0; i < 4 ; i++){
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_RESET);
//		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_RESET);
//
////		  if(i == 1){	// Move UP (Pressing 2)
////			  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET);
////			  if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)){
////				  currentX--;
////				  if(currentX == -1)
////					  currentX = 7;
////				  UpdatePos();
////			  }
////		  }
//	  }
//
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_SET);
//	  while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0));
  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  UpdateTotalTime();

  HAL_ADC_Start_IT(&hadc2);
  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  UpdateTime();

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	ChangeNumbers();
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
  extern unsigned char input[2];

  if(firstClick){
	  TranslateUSARTData(input);
	  UpdatePos();
	  TryMakingMove();
	  HAL_UART_Receive_IT(&huart2, input, sizeof(input));
  }

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles Timer 6 interrupt and DAC underrun interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	if(currentTime <= 10)
		ActivateBuzzer();
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

	if(wrongMove)
		ToggleLEDS();

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */


// --------------------------------------------------------------INITIALIZER--------------------------------------------------------------
void InitializeMap(){
	firstClick = 1;
	currentTime = totalTime;
	clear();
	CreateChars();
	InitializeCodeBoard();
	InitializeLCDBoard();
	DrawBoard();
	UpdatePlayerName();
}

void InitializeCodeBoard(){
	for(int i = 0; i < 8 ; i++){
		for(int j = 0; j < 8; j++){
			codeBoard[i][j] = 0;
		}
	}
	codeBoard[3][3] = 1;
	codeBoard[3][4] = 2;
	codeBoard[4][3] = 2;
	codeBoard[4][4] = 1;
}

void InitializeLCDBoard(){
	for(int i = 0; i < 4; i ++){
		for(int j = 0; j < 8; j++){
			previousLCDBoard[i][j].up = 0;
			previousLCDBoard[i][j].down = 0;
			currentLCDBoard[i][j].up = 0;
			currentLCDBoard[i][j].down = 0;
		}
	}

	previousLCDBoard[1][3].up = 0;
	previousLCDBoard[1][3].down = 1;

	previousLCDBoard[1][4].up = 0;
	previousLCDBoard[1][4].down = 2;

	previousLCDBoard[2][3].up = 2;
	previousLCDBoard[2][3].down = 0;

	previousLCDBoard[2][4].up = 1;
	previousLCDBoard[2][4].down = 0;


	currentLCDBoard[1][3].up = 0;
	currentLCDBoard[1][3].down = 1;

	currentLCDBoard[1][4].up = 0;
	currentLCDBoard[1][4].down = 2;

	currentLCDBoard[2][3].up = 2;
	currentLCDBoard[2][3].down = 0;

	currentLCDBoard[2][4].up = 1;
	currentLCDBoard[2][4].down = 0;

}

void CreateChars(){
	createChar(0,UNDW);
	createChar(1,UNDB);
	createChar(2,UWDN);
	createChar(3,UWDW);
	createChar(4,UWDB);
	createChar(5,UBDN);
	createChar(6,UBDW);
	createChar(7,UBDB);
}

void DrawBoard(){
	for(int i = 0; i < 4 ; i++){
		for (int j = 0; j < 8; j++){
			setCursor(j+7,i);
//			createChar(4,Translate(currentLCDBoard[i][j]));
			int blockType = Translate(currentLCDBoard[i][j]);
			if(blockType == -1)
				print("-");
			else
				write(blockType);
		}
	}
}

// --------------------------------------------------------------TRANSLATOR--------------------------------------------------------------
int Translate(struct lcdBlock block){
	if(block.up == 0 && block.down == 1)
		return 0;
	else if(block.up == 0 && block.down == 2)
		return 1;

	else if(block.up == 1 && block.down == 0)
		return 2;
	else if(block.up == 1 && block.down == 1)
		return 3;
	else if(block.up == 1 && block.down == 2)
		return 4;

	else if(block.up == 2 && block.down == 0)
		return 5;
	else if(block.up == 2 && block.down == 1)
		return 6;
	else if(block.up == 2 && block.down == 2)
		return 7;

	return -1;
}

void TranslateUSARTData(unsigned char input[]){
	switch(input[0]){
	case'A':
		currentY = 0;
		break;

	case'B':
		currentY = 1;
		break;

	case'C':
		currentY = 2;
		break;

	case'D':
		currentY = 3;
		break;

	case'E':
		currentY = 4;
		break;

	case'F':
		currentY = 5;
		break;

	case'G':
		currentY = 6;
		break;

	case'H':
		currentY = 7;
		break;

	default:
		currentY = 0;
		break;
	}

	switch(input[1]){
	case'1':
		currentX = 0;
		break;

	case'2':
		currentX = 1;
		break;

	case'3':
		currentX = 2;
		break;

	case'4':
		currentX = 3;
		break;

	case'5':
		currentX = 4;
		break;

	case'6':
		currentX = 5;
		break;

	case'7':
		currentX = 6;
		break;

	case'8':
		currentX = 7;
		break;

	default:
		currentX = 0;
		break;
	}

}
// --------------------------------------------------------------VALIDATION--------------------------------------------------------------

void TryMakingMove(){
	if(winner == 0){
		if(IsLegalMove()){
		  	  codeBoard[currentX][currentY] = player;
		  	  FlipAllBrackets();
		  	  currentTime = totalTime;
		  	  if(!SwitchPlayer())
			  	  EndGame();
		}
		else{
			toggleCounter = 0;
			wrongMove = 1;
		}
	}
}

int IsLegalMove(){
	if(codeBoard[currentX][currentY] == 0){
		for(int i = 1; i < 10;i++){
			if(i == 5)
				continue;
			if(CheckDirection(i)){
				return 1;
			}
		}
		return 0;
	}
	else
		return 0;
}

void FlipAllBrackets(){
	for(int i = 1; i < 10 ; i++){
		if(i == 5)
			continue;
		if(CheckDirection(i))
			FlipDirection(i);
	}
	UpdateCurrentBoard();
}

void FlipDirection(int direction){
	movingX = currentX;
	movingY = currentY;
	MoveInDirection(direction);
	while(codeBoard[movingX][movingY] == GetOpponent()){
		codeBoard[movingX][movingY] = player;
		MoveInDirection(direction);
	}
}

int CheckDirection(int direction){
	movingX = currentX;
	movingY = currentY;
	MoveInDirection(direction);
	if(AreXYValid()){
		if(codeBoard[movingX][movingY] == GetOpponent()){
			while(codeBoard[movingX][movingY] == GetOpponent()){
				MoveInDirection(direction);
				if(!AreXYValid())
					return 0;
			}
			if(codeBoard[movingX][movingY] == player)			//To avoid detecting empty spaces
				return 1;
		}
		return 0;
	}
	return 0;
}

int AreXYValid(){
	if(movingX < 0 || movingX > 7 || movingY < 0 || movingY > 7)
		return 0;
	else
		return 1;
}

void MoveInDirection(int direction){
	if(direction == 1){
		movingX++;
		movingY--;
	}
	else if(direction == 2){
		movingX++;
	}
	else if(direction == 3){
		movingX++;
		movingY++;
	}
	else if(direction == 4){
		movingY--;
	}
	else if(direction == 6){
		movingY++;
	}
	else if(direction == 7){
		movingX--;
		movingY--;
	}
	else if(direction == 8){
		movingX--;
	}
	else if(direction == 9){
		movingX--;
		movingY++;
	}
}

int GetOpponent(){
	if(player == 1)
		return 2;
	else
		return 1;
}
// --------------------------------------------------------------BOARD LEDS--------------------------------------------------------------

void ToggleLEDS(){
	if(toggleCounter < 3 && !ledOn){
		ledOn = 1;
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,1);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,1);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,1);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,1);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,1);
	}
	else if(toggleCounter < 3 && ledOn){
		toggleCounter++;
		ledOn = 0;
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,0);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,0);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,0);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,0);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,0);
	}
	if(toggleCounter >= 3){
		ledOn = 0;
		toggleCounter = 0;
		wrongMove = 0;
	}
}

// --------------------------------------------------------------UPDATING LCD BOARD--------------------------------------------------------------

void UpdateCurrentBoard(){
	for(int i = 0; i < 8; i+=2){
		for(int j = 0; j < 8 ; j++){
			currentLCDBoard[i/2][j].up = codeBoard[i][j];
			currentLCDBoard[i/2][j].down = codeBoard[i+1][j];
		}
	}
	CheckDifferences();
}

void CheckDifferences(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 8; j++){
			if(currentLCDBoard[i][j].down != previousLCDBoard[i][j].down || currentLCDBoard[i][j].up != previousLCDBoard[i][j].up)
				ChangeLCDBlock(i,j);
		}
	}
}

void ChangeLCDBlock(int x, int y){
	previousLCDBoard[x][y] = currentLCDBoard[x][y];
	setCursor(y+7,x);
	int blockType = Translate(previousLCDBoard[x][y]);
	write(blockType);
}

// --------------------------------------------------------------LCD DETAIL--------------------------------------------------------------

void UpdatePlayerName(){
	char str[20];
	setCursor(0,1);
	JustCountScores();
	if(player == black){
		print("Black");
		sprintf(str,"Score:%d",blackScore);
		setCursor(0,0);
		print(str);
	}

	else{
		print("White");
		sprintf(str,"Score:%d",whiteScore);
		setCursor(0,0);
		print(str);
	}
}

void UpdatePos(){
	setCursor(0, 2);
	switch(currentY){
	case 0:
		print("A");
		break;
	case 1:
		print("B");
		break;
	case 2:
		print("C");
		break;
	case 3:
		print("D");
		break;
	case 4:
		print("E");
		break;
	case 5:
		print("F");
		break;
	case 6:
		print("G");
		break;
	case 7:
		print("H");
		break;
	}
	setCursor(1,2);
	switch(currentX){
	case 0:
		print("1");
		break;
	case 1:
		print("2");
		break;
	case 2:
		print("3");
		break;
	case 3:
		print("4");
		break;
	case 4:
		print("5");
		break;
	case 5:
		print("6");
		break;
	case 6:
		print("7");
		break;
	case 7:
		print("8");
		break;
	}
}

// --------------------------------------------------------------SWITCHING PLAYER--------------------------------------------------------------
int SwitchPlayer(){
	player++;
	if(player == 3)
		player = 1;
	if(HasLegalMove()){
		UpdatePlayerName();
		return 1;
	}

	player++;
	if(player == 3)
		player = 1;
	if(HasLegalMove()){
		UpdatePlayerName();
		return 1;
	}
	return 0;
}

int HasLegalMove(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			currentX = i;
			currentY = j;
			if(IsLegalMove()){
				currentX = 0;			//Comment for testing
				currentY = 0;
				UpdatePos();
				return 1;
			}

		}
	}
	return 0;
}

// --------------------------------------------------------------ENDING GAME--------------------------------------------------------------

void EndGame(){
	char str[20];
	clear();
	setCursor(1,6);
	print("Game Ended!");
	CountScoresAndSetWinner();

	setCursor(0,1);
	if(winner == 2)
		sprintf(str,"Black won score: %d",blackScore);

	else if(winner == 1)
		sprintf(str,"White won score: %d",whiteScore);

	HAL_UART_Transmit(&huart2,str,sizeof(unsigned char) * 20,1000);
	print(str);
}

void JustCountScores(){
	blackScore = 0;
	whiteScore = 0;
	for(int i = 0; i < 8 ; i++){
		for (int j = 0; j < 8; j++){
			if(codeBoard[i][j] == 1)
				whiteScore++;
			else if(codeBoard[i][j] == 2)
				blackScore++;
		}
	}
}

void CountScoresAndSetWinner(){
	blackScore = 0;
	whiteScore = 0;
	for(int i = 0; i < 8 ; i++){
		for (int j = 0; j < 8; j++){
			if(codeBoard[i][j] == 1)
				whiteScore++;
			else if(codeBoard[i][j] == 2)
				blackScore++;
		}
	}

	if(winner == 0){

		if(blackScore > whiteScore)
			winner = 2;
		else
			winner = 1;
	}
}
// --------------------------------------------------------------TIMER--------------------------------------------------------------
void UpdateTotalTime(){

	  volumeValue = HAL_ADC_GetValue(&hadc2);
	  volumeValue = ((float)volumeValue*60/4095);
	  if(volumeValue < 10)
		  volumeValue = 10;
	  else if(volumeValue > 60)
		  volumeValue = 60;

	  if(totalTime - volumeValue > 0.9 || volumeValue - totalTime > 0.9)
		  totalTime = (int)volumeValue;
}

void UpdateTime(){
	  if(firstClick){
		  if(currentTime > 0)
		  	  currentTime--;
		  else if(winner == 0){
		  	  winner = GetOpponent();
		  	  EndGame();
		    }
	  }
}

void ActivateBuzzer(){

	if(!buzzerOn && buzzerCounter == 0){
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 50);
		buzzerOn = 1;
		buzzerCounter = currentTime;
	}

	else{
		buzzerOn = 0;
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		buzzerCounter--;
	}

}
// --------------------------------------------------------------7-SEG FUNCTIONS--------------------------------------------------------------

void ChangeNumbers(){

	int totalTemp = totalTime;
	int currentTemp = currentTime;
	sevenSegment++;

		//XXXY
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, 1);
		if (sevenSegment % 4 == 1){
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, 1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, 0);
			BCDConverter(totalTime % 10);
		}

		if (sevenSegment % 4 == 2){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1,0);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, 0);
			totalTemp = totalTime / 10;
			BCDConverter(totalTemp % 10);

		}

		if (sevenSegment % 4 == 3){
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, 0);
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_2, 0);
		    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, 0);
		    BCDConverter(currentTime % 10);
		}

		if (sevenSegment % 4 == 0){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
		    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_2 , 0);
		    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, 0);
		    currentTemp = currentTime / 10;
		    BCDConverter(currentTemp % 10);

		}

}

void BCDConverter(int i){
  int x1 = i & 1;
  int x2 = i & 2;
  int x3 = i & 4;
  int x4 = i & 8;
  if(x1 > 0) x1 = 1;
  if(x2 > 0) x2 = 1;
  if(x3 > 0) x3 = 1;
  if(x4 > 0) x4 = 1;
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, x1);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, x4);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, x3);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, x2);
}

// --------------------------------------------------------------CALL BACK--------------------------------------------------------------

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (last_gpio_exti + 200 > HAL_GetTick()) // Simple button debouncing
  {
    return;
  }
  last_gpio_exti = HAL_GetTick();

  int8_t row_number = -1;
  int8_t column_number = -1;

  if (GPIO_Pin == GPIO_PIN_0)
  {
    // blue_button_pressed = 1;
    // return;
  }

  for (uint8_t row = 0; row < 4; row++) // Loop through Rows
  {
    if (GPIO_Pin == Row_pins[row])
    {
      row_number = row;
    }
  }

  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 0);
  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 0);
  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 0);
  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 0);

  for (uint8_t col = 0; col < 4; col++) // Loop through Columns
  {
    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
    if (HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number]))
    {
      column_number = col;
    }
    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
  }

  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);

  if (row_number == -1 || column_number == -1)
  {
    return; // Reject invalid scan
  }
  //   C0   C1   C2   C3
  // +----+----+----+----+
  // | 1  | 2  | 3  | 4  |  R0
  // +----+----+----+----+
  // | 5  | 6  | 7  | 8  |  R1
  // +----+----+----+----+
  // | 9  | 10 | 11 | 12 |  R2
  // +----+----+----+----+
  // | 13 | 14 | 15 | 16 |  R3
  // +----+----+----+----+
  const uint8_t button_number = row_number * 4 + column_number + 1;

  switch (button_number)
  {

  case 1:
	  ActivateBuzzer();
	  break;

  case 2:
	  //Moving Up
	currentX--;
	if(currentX == -1)
		currentX = 7;
	UpdatePos();
  break;

  case 5:
	  //Moving Left
	  currentY--;
	  if(currentY == -1)
		  currentY = 7;
	  UpdatePos();
    break;

  case 6:
	  //Select Button
	  TryMakingMove();
    break;

  case 7:
	  //Moving Right
	currentY++;
	currentY = currentY%8;
	UpdatePos();
    break;

  case 10:
	  //Moving Down
	if(firstClick == 0)
		InitializeMap();
	  	currentX++;
	  	currentX = currentX%8;
	  	UpdatePos();
    break;

  default:
    break;
  }

}

void ConfirmCharacter(char character){

}

char PressedOne(){

	if(characterCounter%3 == 1){

	}
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
