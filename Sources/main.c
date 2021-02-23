/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K1xx
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"
#include "FreeRTOS.h"
#include "task.h"

  volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */

  static uint8_t DefaultRamHeap[512];
  static uint8_t __attribute__ ((used,section(".myHeapSection"))) HeapInOwnSection[6244] __attribute__ ((aligned (4))); /* placed in in no_init section inside SRAM_LOWER */
  static HeapRegion_t xHeapRegions[] =
  {
      {&HeapInOwnSection[0], sizeof(HeapInOwnSection)},
      {&DefaultRamHeap[0], sizeof(DefaultRamHeap)},
      {NULL, 0}
  };

  void TestTask(void *parameter)
  {
	  static bool status = false;

	  for(;;)
	  {
		  vTaskDelay(pdMS_TO_TICKS(250));

		  if(status == false)
		  {
			  PINS_DRV_WritePin(RGB_BLUE_PORT, RGB_BLUE_PIN, 0);
			  status = true;
		  }
		  else
		  {
			  PINS_DRV_WritePin(RGB_BLUE_PORT, RGB_BLUE_PIN, 1);
			  status = false;
		  }
	  }
  }

 void PreSleepProcessing(uint32_t xModifiableIdleTime)
 {
	 /* sleep mode */
	 //POWER_SYS_SetMode(1, POWER_MANAGER_POLICY_AGREEMENT);
 }

 void PostSleepProcessing(uint32_t xModifiableIdleTime)
 {
	 /* Run mode */
	 //POWER_SYS_SetMode(0, POWER_MANAGER_POLICY_AGREEMENT);
 }


/*! 
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your local variable definition here */

	vPortDefineHeapRegions(xHeapRegions);

	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

	/* initialize LPIT */
	LPIT_DRV_Init(INST_LPIT1, &lpit1_InitConfig);

	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

	POWER_SYS_Init(powerConfigsArr, POWER_MANAGER_CONFIG_CNT, powerStaticCallbacksConfigsArr, POWER_MANAGER_CALLBACK_CNT);

	/* Run mode */
	//POWER_SYS_SetMode(0, POWER_MANAGER_POLICY_AGREEMENT);

	/* sleep mode */
	//POWER_SYS_SetMode(1, POWER_MANAGER_POLICY_AGREEMENT);

	/* create test task */
	xTaskCreate(TestTask, "TestTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

	/* start scheduler */
	vTaskStartScheduler();

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP S32K series of microcontrollers.
**
** ###################################################################
*/
