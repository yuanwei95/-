/*!
    \file  FMC.h
    \brief the header file of main
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#ifndef FMC_H
#define FMC_H

#define FMC_PAGE_SIZE           ((uint16_t)0x400U)

typedef enum
{
    FLASH_PROGRAM_SECTION_P2D,
    FLASH_PROGRAM_SECTION_SN,
    FLASH_PROGRAM_SECTION_InitialConf,
    FLASH_PROGRAM_SECTION_EquipmentConf,
    FLASH_PROGRAM_SECTION_Tail,
}FLASH_PROGRAM_SECTION_Enum;

#define SerialNumberNum 16

#define FA_P2D_Para_Start            0x0800F000
#define FA_P2D_Para_Stop             (FA_P2D_Para_Start+sizeof(P2D_Para_Type) -1)
#define FA_P2D_NbrOfPage            1

#define FA_SN_Start                       0x0800F400
#define FA_SN_Stop                        (FA_SN_Start + SerialNumberNum - 1)
#define FA_SN_NbrOfPage              FA_P2D_NbrOfPage

#define FA_EquipmentConf_Start     0x0800F800
#define FA_EquipmentConf_Stop     (FA_EquipmentConf_Start + sizeof(Equipment_Configure_Type) - 1)
#define FA_EquipmentConf_NbrOfPage            1

#define FA_InitialConf_Start             0x0800F800
#define FA_InitialConf_Stop             (FA_InitialConf_Start + sizeof(Initial_Configure_Type) - 1)
#define FA_InitialConf_NbrOfPage            1

#define FA_CRCConf_Start 0x0800FC00
#define FA_CRCConf_Stop 0x0800FC0F
#define FA_CRCConf_NbrOfPage            1

#define FA_DistanceToRayPointWidth_Start FA_P2D_Para_Start - 0x60  //0x60 是DistanceToRayPointWidth[][SNC_Tail]的长度

extern P2D_Para_Type ValP2DPara;
extern u8 ValSNConfiure[];

int FlashProgram(FLASH_PROGRAM_SECTION_Enum Section, bool Erase);

void ConfigureParaInitial(const Initial_Configure_Type *Ptr);
void ConfigureParaEquipment(const Equipment_Configure_Type *Ptr);
void DisplayConfigure(void);
void CopyEquipmentConfigure(FLASH_PROGRAM_SECTION_Enum Section);

#endif

