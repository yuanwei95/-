/*!
    \file  gd32f3x0_libopt.h
    \brief library optional for gd32f3x0
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-28, V1.0.0, demo for GD32F3x0
*/

#ifndef GD32F3X0_LIBOPT_H
#define GD32F3X0_LIBOPT_H

#include "gd32f3x0_adc.h"
#include "gd32f3x0_crc.h"
#include "gd32f3x0_ctc.h"
#include "gd32f3x0_dbg.h"
#include "gd32f3x0_dma.h"
#include "gd32f3x0_exti.h"
#include "gd32f3x0_fmc.h"
#include "gd32f3x0_gpio.h"
#include "gd32f3x0_syscfg.h"
#include "gd32f3x0_i2c.h"
#include "gd32f3x0_fwdgt.h"
#include "gd32f3x0_pmu.h"
#include "gd32f3x0_rcu.h"
#include "gd32f3x0_rtc.h"
#include "gd32f3x0_spi.h"
#include "gd32f3x0_timer.h"
#include "gd32f3x0_usart.h"
#include "gd32f3x0_wwdgt.h"
#include "gd32f3x0_misc.h"
#include "gd32f3x0_tsi.h"

#ifdef GD32F350
#include "gd32f3x0_cec.h"
#include "gd32f3x0_cmp.h"
#include "gd32f3x0_dac.h"
#endif /* GD32F350 */
///////////////////////////////////////////////////////

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */


#include "Mis.h"
#include "Uart.h"
#include "UI.h"
#include "FMC.h"
#include "IT.h"
#include "TIM.h"
#include "ADC.h"
#include "ENCODE.h"

#endif /* GD32F3X0_LIBOPT_H */
