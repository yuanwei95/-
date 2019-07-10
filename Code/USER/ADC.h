/*!
    \file  main.h
    \brief the header file of main 
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#ifndef __MAIN_ADC_H
#define __MAIN_ADC_H

#define VIDEO_P_PIN GPIO_PIN_2
#define VIDEO_P_GPIO GPIOA
#define VIDEO_P_CLK RCU_GPIOA
#define VIDEO_P_ADC_CHANNEL          ADC_CHANNEL_2

#define Trig_PIN GPIO_PIN_11
#define Trig_GPIO GPIOB
#define Trig_CLK RCU_GPIOB
#define Trig_EXTI_PortSourceGPIO EXTI_SOURCE_GPIOB
#define Trig_EXTI_PinSource EXTI_SOURCE_PIN11


#define DMA_ADC DMA_CH0
#define CHANNEL_ADC ADC_CHANNEL_2

#define EPC_PIX_Valid_Number  2288
#define EPC_PIX_Preamble_Number 49
#define EPC_PIX_Reserved_Number 61

#define EPC_PIX_Number ((EPC_PIX_Valid_Number + EPC_PIX_Preamble_Number + EPC_PIX_Reserved_Number))

#define EPC_Flag_0xAA  (EPC_PIX_Number-1)
#define EPC_Val_NumH  (EPC_Flag_0xAA-1)
#define EPC_Val_NumL   (EPC_Val_NumH-1)
#define EPC_Flag_0x55  (EPC_Val_NumL-1)
#define EPC_Flag_Deal   (EPC_Flag_0x55-1)
#define EPC_Pos_ThresholdH   (EPC_Flag_Deal-1)
#define EPC_Pos_ThresholdL   (EPC_Pos_ThresholdH-1)
#define EPC_Capture_Start (EPC_Pos_ThresholdL-1)
#define EPC_FrameNum (EPC_Capture_Start-1)
#define EPC_BiaoDingSystem_ValAngleH (EPC_FrameNum-1)
#define EPC_BiaoDingSystem_ValAngleL (EPC_BiaoDingSystem_ValAngleH-1)
#define EPC_BiaoDingSystem_ValDistanceH (EPC_BiaoDingSystem_ValAngleL-1)
#define EPC_BiaoDingSystem_ValDistanceL (EPC_BiaoDingSystem_ValDistanceH-1)
#define EPC_ValAngleH_Capture (EPC_BiaoDingSystem_ValDistanceL-1)
#define EPC_ValAngleL_Capture (EPC_ValAngleH_Capture-1)
#define EPC_AngleSampleIndexH_Capture (EPC_ValAngleL_Capture-1)
#define EPC_AngleSampleIndexL_Capture (EPC_AngleSampleIndexH_Capture-1)
#define EPC_AngleMaxIndexH_Capture (EPC_AngleSampleIndexL_Capture-1)
#define EPC_AngleMaxIndexL_Capture (EPC_AngleMaxIndexH_Capture-1)
#define EPC_Analy_Result_Capture (EPC_AngleMaxIndexL_Capture-1)
#define EPC_Analy_MidH_Capture (EPC_Analy_Result_Capture-1)
#define EPC_Analy_MidL_Capture (EPC_Analy_MidH_Capture-1)
#define EPC_Analy_NumH_Capture (EPC_Analy_MidL_Capture-1)
#define EPC_Analy_NumL_Capture (EPC_Analy_NumH_Capture-1)
#define EPC_Analy_AmpH_Capture (EPC_Analy_NumL_Capture-1)
#define EPC_Analy_AmpL_Capture (EPC_Analy_AmpH_Capture-1)
#define EPC_Analy_DistanceH_Capture (EPC_Analy_AmpL_Capture-1)
#define EPC_Analy_DistanceL_Capture (EPC_Analy_DistanceH_Capture-1)
#define EPC_Analy_HeatCenterHH_Capture (EPC_Analy_DistanceL_Capture-1)
#define EPC_Analy_HeatCenterHL_Capture (EPC_Analy_HeatCenterHH_Capture-1)
#define EPC_Analy_HeatCenterLH_Capture (EPC_Analy_HeatCenterHL_Capture-1)
#define EPC_Analy_HeatCenterLL_Capture (EPC_Analy_HeatCenterLH_Capture-1)

#define EPC_Analy_L_TempL (EPC_Analy_HeatCenterLL_Capture-1)
#define EPC_Analy_L_TempH (EPC_Analy_L_TempL-1)
#define EPC_Analy_R_TempL (EPC_Analy_L_TempH-1)
#define EPC_Analy_R_TempH (EPC_Analy_R_TempL-1)

#define EPC_Analy_ADC_FLAG_OVR (EPC_Analy_R_TempH-1)

#define EPC_Capture_End (EPC_Analy_ADC_FLAG_OVR-1)

#define EPC_Flag_Deal_OK    0x00
#define EPC_Flag_Deal_NOK    0xFF

extern bool Val_RayTouch_Cmd_Start_Matlab;
extern u32 ValNum_ADC_INT_WDE;
extern u8  EPC901_Pix_Value[EPC_Frame_Num][EPC_PIX_Number];
extern u8  Capture_EPC901_Pix_Value[EPC_PIX_Number];

void ADC_Initial(void);

#endif /* __MAIN_ADC_H */
