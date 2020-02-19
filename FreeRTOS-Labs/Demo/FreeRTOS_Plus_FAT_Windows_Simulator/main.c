/*
	FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
	All rights reserved

	VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

	This file is part of the FreeRTOS distribution.

	FreeRTOS is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License (version 2) as published by the
	Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

	***************************************************************************
	>>!   NOTE: The modification to the GPL is included to allow you to     !<<
	>>!   distribute a combined work that includes FreeRTOS without being   !<<
	>>!   obliged to provide the source code for proprietary components     !<<
	>>!   outside of the FreeRTOS kernel.                                   !<<
	***************************************************************************

	FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE.  Full license text is available on the following
	link: http://www.freertos.org/a00114.html

	***************************************************************************
	 *                                                                       *
	 *    FreeRTOS provides completely free yet professionally developed,    *
	 *    robust, strictly quality controlled, supported, and cross          *
	 *    platform software that is more than just the market leader, it     *
	 *    is the industry's de facto standard.                               *
	 *                                                                       *
	 *    Help yourself get started quickly while simultaneously helping     *
	 *    to support the FreeRTOS project by purchasing a FreeRTOS           *
	 *    tutorial book, reference manual, or both:                          *
	 *    http://www.FreeRTOS.org/Documentation                              *
	 *                                                                       *
	***************************************************************************

	http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
	the FAQ page "My application does not run, what could be wrong?".  Have you
	defined configASSERT()?

	http://www.FreeRTOS.org/support - In return for receiving this top quality
	embedded software for free we request you assist our global community by
	participating in the support forum.

	http://www.FreeRTOS.org/training - Investing in training allows your team to
	be as productive as possible as early as possible.  Now you can receive
	FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
	Ltd, and the world's leading authority on the world's leading RTOS.

	http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
	including FreeRTOS+Trace - an indispensable productivity tool, a DOS
	compatible FAT file system, and our tiny thread aware UDP/IP stack.

	http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
	Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

	http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
	Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
	licenses offer ticketed support, indemnification and commercial middleware.

	http://www.SafeRTOS.com - High Integrity Systems also provide a safety
	engineered and independently SIL3 certified version for use in safety and
	mission critical applications that require provable dependability.

	1 tab == 4 spaces!
*/

/*
 * Instructions for using this project are provided on:
 * http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/examples_FreeRTOS_simulator.html
 *
 * NOTE: Some versions of Visual Studio will generate erroneous compiler
 * warnings about variables being used before they are set.
 */

/* Standard includes. */
#include <stdio.h>
#include <conio.h>
#include <time.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+FAT includes. */
#include "ff_headers.h"
#include "ff_stdio.h"
#include "ff_ramdisk.h"

/* The number and size of sectors that will make up the RAM disk.  The RAM disk
is huge to allow some verbose FTP tests used in previous demos. */
#define mainRAM_DISK_SECTOR_SIZE	512UL /* Currently fixed! */
#define mainRAM_DISK_SECTORS		( ( 5UL * 1024UL * 1024UL ) / mainRAM_DISK_SECTOR_SIZE ) /* 5M bytes. */
#define mainIO_MANAGER_CACHE_SIZE	( 15UL * mainRAM_DISK_SECTOR_SIZE )

/* Where the RAM disk is mounted. */
#define mainRAM_DISK_NAME			"/ram"

/* Set to 0 to run the STDIO examples once only, or 1 to create multiple tasks
that run the tests continuously. */
#define mainRUN_STDIO_TESTS_IN_MULTIPLE_TASK 0

/*-----------------------------------------------------------*/

/*
 * Creates a RAM disk, then creates files on the RAM disk.
 */
static void prvCreateDiskAndExampleFiles( void * pvParameters );

/*
 * Functions used to create and then test files on a disk.
 */
extern void vCreateAndVerifyExampleFiles( const char *pcMountPath );
extern void vStdioWithCWDTest( const char *pcMountPath );
extern void vMultiTaskStdioWithCWDTest( const char *const pcMountPath, uint16_t usStackSizeWords );


/*-----------------------------------------------------------*/

/*
 * NOTE: Some versions of Visual Studio will generate erroneous compiler
 * warnings about variables being used before they are set.
 */
int main( void )
{
	/* Create the task that demonstrates the file system using a RAM disk. */
	xTaskCreate( prvCreateDiskAndExampleFiles, /* Function that implements the task. */
				 "FAT",						   /* Text name for the task, only used for debugging. */
				 configMINIMAL_STACK_SIZE,	   /* Stack size, can be anything for the Win32 port, but not any other ports. */
				 NULL,						   /* Not passing in any parameters. */
				 tskIDLE_PRIORITY,			   /* Priority at which the task will execute. */
				 NULL );					   /* Not storing a handle to the task. */

	/* Start the RTOS scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details (this is standard text that is not
	really applicable to the Win32 simulator port). */
	for( ;; )
	{
	}
}
/*-----------------------------------------------------------*/

static void prvCreateDiskAndExampleFiles( void * pvParameters )
{
static uint8_t ucRAMDisk[ mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE ];
FF_Disk_t *pxDisk;

	/* Remove compiler warnings about unused parameters. */
	( void ) pvParameters;

	/* Create the RAM disk. */
	pxDisk = FF_RAMDiskInit( mainRAM_DISK_NAME, ucRAMDisk, mainRAM_DISK_SECTORS, mainIO_MANAGER_CACHE_SIZE );
	configASSERT( pxDisk );

	/* Print out information on the disk. */
	FF_RAMDiskShowPartition( pxDisk );

	/* Create a few example files on the disk.  These are not deleted again. */
	vCreateAndVerifyExampleFiles( mainRAM_DISK_NAME );

	/* A few sanity checks only - can only be called after
	vCreateAndVerifyExampleFiles(). */
	#if( mainRUN_STDIO_TESTS_IN_MULTIPLE_TASK == 1 )
	{
		/* Note the stack size is not actually used in the Windows port. */
		vMultiTaskStdioWithCWDTest( mainRAM_DISK_NAME, configMINIMAL_STACK_SIZE * 2U );
	}
	#else
	{
		vStdioWithCWDTest( mainRAM_DISK_NAME );
	}
	#endif

	FF_PRINTF( "Done.  Press a key to exit." );
	while( !_kbhit() )
	{
		vTaskDelay( pdMS_TO_TICKS( 1000 ) );
	}

	/* FreeRTOS tasks must not be allowed to run off their implementing function. */
	vPortEndScheduler();
}
/*-----------------------------------------------------------*/

void vAssertCalled( const char *pcFile, uint32_t ulLine )
{
const uint32_t ulLongSleep = 1000UL;
volatile uint32_t ulBlockVariable = 0UL;
volatile char *pcFileName = ( volatile char *  ) pcFile;
volatile uint32_t ulLineNumber = ulLine;

	( void ) pcFileName;
	( void ) ulLineNumber;

	/* Setting ulBlockVariable to a non-zero value in the debugger will allow
	this function to be exited. */
	taskDISABLE_INTERRUPTS();
	{
		while( ulBlockVariable == 0UL )
		{
			Sleep( ulLongSleep );
		}
	}
	taskENABLE_INTERRUPTS();
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	vAssertCalled( __FILE__, __LINE__ );
}
/*-----------------------------------------------------------*/

struct tm *gmtime_r( const time_t *pxTime, struct tm *tmStruct )
{
	/* Dummy time functions to keep the file system happy in the absence of
	target support. */
	memcpy( tmStruct, gmtime( pxTime ), sizeof( * tmStruct ) );
	return tmStruct;
}
/*-----------------------------------------------------------*/

time_t FreeRTOS_time( time_t *pxTime )
{
time_t xReturn;

	xReturn = time( &xReturn );

	if( pxTime != NULL )
	{
		*pxTime = xReturn;
	}

	return xReturn;
}
/*-----------------------------------------------------------*/


