/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include  "stm32f1xx_hal.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/


TIM_HandleTypeDef timer6;
void TIM6_IRQHandler(void)
{
	if(__HAL_TIM_GET_FLAG(&timer6,TIM_FLAG_UPDATE))
	{
		__HAL_TIM_CLEAR_FLAG(&timer6,TIM_FLAG_UPDATE);
		__HAL_TIM_SET_COUNTER(&timer6,0);
		prvvTIMERExpiredISR();
	}
}

BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	
	__HAL_RCC_TIM6_CLK_ENABLE();
	
	timer6.Instance = TIM6;
	/* ??????? */
	timer6.Init.Period = usTim1Timerout50us - 1;
	/* ?????? */
	timer6.Init.Prescaler = 3600-1;
	/* ?????? */
	timer6.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	HAL_TIM_Base_Init(&timer6);
	
	HAL_NVIC_SetPriority(TIM6_IRQn,2,3);
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
	
	HAL_TIM_Base_Start_IT(&timer6);
	__HAL_TIM_ENABLE_IT(&timer6,TIM_IT_UPDATE);
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
		__HAL_TIM_SET_COUNTER(&timer6,0);
		HAL_TIM_Base_Start_IT(&timer6);

    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
}

inline void
vMBPortTimersDisable(  )
{
	__HAL_TIM_DISABLE_IT(&timer6,TIM_IT_UPDATE);
	__HAL_TIM_SET_COUNTER(&timer6,0);
	HAL_TIM_Base_Stop(&timer6);
    /* Disable any pending timers. */
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

