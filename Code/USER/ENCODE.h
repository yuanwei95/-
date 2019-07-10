/*!
    \file  ENCODE.h
    \brief 
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#ifndef __ENCODE_H
#define __ENCODE_H

#define AngleSampleIndexMode

#define ENCODE_PIN GPIO_PIN_11
#define ENCODE_GPIO GPIOA
#define ENCODE_CLK RCU_GPIOA

#define Encode_Captrue_DMAChannel DMA_CH3


#define Time0EncodePerio (360*12*10)

#define AgnleNTimes 64

typedef enum
{
  RunDirection_Positive = 0, 
  RunDirection_Inversion = 1,
  RunDirection_Tail,
}RunDirection_Enum;

#ifndef Angle_UseSingleEdge
#define NumberPlusOfRing 30
#define NormalAngleForOnePlus 12
#define Start1AngleForOnePlus 18
#define Start2AngleForOnePlus 6
#else
#define NumberPlusOfRing 15
#define NormalAngleForOnePlus 24
#define Start1AngleForOnePlus 18//30
#define Start2AngleForOnePlus 30//18
#define Start13AngleForOnePlus 20
#define Start14AngleForOnePlus 28
#define Start1AngleForOnePlusForS4Platform 30
#define Start2AngleForOnePlusForS4Platform 18
#endif

#define NumberForEncodeCap NumberPlusOfRing
#define MaxTimeCountForEncodeCap 0xFFFF

#define Angle_Error_Flag 0xFFFF

extern u16 EncodeValue[NumberForEncodeCap];
extern bool ChangeCalibrationPlatform;
extern u32 Capture_Encode_DifValue_All_Backup;
extern u16 RayTouch_RunFreq;
extern u16 PointsForOneRingBackup;
extern u8 AutoAdj0CStage;
extern bool AutoAdj0C;
extern u16 AutoAdj0CAddAngle;
extern bool SetPointsForOneRingFlag;

//void RunAngle_Initial(void);
void ENCODE_Config(bool ChangeCalibrationPlatform);
void FindRay_0CAngle(Frame_Type *PtrFrame);
void FindRay_Angle(Frame_Type *PtrFrame);
void Cal_ENCODE_Dif(Frame_Type *PtrFrame);
void PrintCaptureEncode(void);

#endif /* __ENCODE_H */
