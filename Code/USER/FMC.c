/*!
    \file  FMC.c
    \brief main flash program, erase and reprogram
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "gd32f3x0.h"

P2D_Para_Type ValP2DPara;
u8 ValSNConfiure[SerialNumberNum];
Equipment_Configure_Type ValEquipmentConfiure;
Initial_Configure_Type ValIntialConfiure;

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void fmc_erase_pages(u32 FMC_WRITE_START_ADDR, u32 PageNum)
{
    uint32_t EraseCounter;

    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    
    /* erase the flash pages */
    for(EraseCounter = 0U; EraseCounter < PageNum; EraseCounter++){
        fmc_page_erase(FMC_WRITE_START_ADDR + (FMC_PAGE_SIZE * EraseCounter));
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    /* lock the main FMC after the erase operation */
    fmc_lock();
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void fmc_program(u32 FMC_WRITE_START_ADDR, u32 FMC_WRITE_END_ADDR, u32 *PtrData)
{
    u32 address;
    
    /* unlock the flash program/erase controller */
    fmc_unlock();

    address = FMC_WRITE_START_ADDR;

    /* program flash */
    while(address < FMC_WRITE_END_ADDR){
        fmc_word_program(address, *PtrData);
        address += 4U;
        PtrData++;
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    /* lock the main FMC after the program operation */
    fmc_lock();
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
bool fmc_erase_pages_check(u32 FMC_WRITE_START_ADDR, u32 PageNum)
{
    u32 i;
    u32 *ptrd;

    ptrd = (uint32_t *)FMC_WRITE_START_ADDR;

    /* check flash whether has been erased */
    for(i = 0U; i < ((FMC_PAGE_SIZE * PageNum)/sizeof(u32)); i++){
        if(0xFFFFFFFFU != (*ptrd)){
            return FALSE;
        }else{
            ptrd++;
        }
    }
    return TRUE;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
bool fmc_program_check(u32 FMC_WRITE_START_ADDR, u32 FMC_WRITE_END_ADDR, u32 *PtrData)
{
    u32 i;
    u32 *ptrd;
    
    ptrd = (u32 *)FMC_WRITE_START_ADDR;

    /* check flash whether has been programmed */
    for(i = 0U; i < ((FMC_WRITE_END_ADDR-FMC_WRITE_START_ADDR)/sizeof(u32)); i++){
        if((*ptrd) != *PtrData){
            return FALSE;
        }else{
            ptrd++;
            PtrData++;
        }
    }
    return TRUE;
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void ConfigureParaInitial(const Initial_Configure_Type *Ptr)
{    
    u8 * PtrSN;
    int i;

    TIM1_TimerPeriod = (*Ptr).TIM1_TimerPeriod;// = 200;
    TIM1_CH4PlusePercent = (*Ptr).TIM1_CH4PlusePercent;//60;  //设置LED激光管亮度
    TIM15_Period = (*Ptr).TIM15_Period;//0xFFFF;//36000;//TIM15_Period*TIM15_CCR4_Plus_Number;  
    
    Output2K4K5K = (*Ptr).Output2K4KConfigure;
    
    TIM2_Period = (*Ptr).TIM2_Period;//14;//14;
    TIM2_CCR2_Val = (*Ptr).TIM2_CCR2_Val;//TIM2_Period/2;
    
    TIM15_CCR1_Val = (*Ptr).TIM15_CCR1_Val;//17000; //17000   4.25KHZ
    TIM15_CCR1_Sample_Val = (*Ptr).TIM15_CCR1_Sample_Val;//12000;  //3000
    TIM15_CCR1_Exposure_Add_Val = (*Ptr).TIM15_CCR1_Exposure_Add_Val;//3000;  //12000设置前期曝光时间
    TIM15_CCR1_Exposure_Val = (*Ptr).TIM15_CCR1_Exposure_Val;//2000;           //设置后期曝光时间

    TIM15_CCR1_Exposure_Add_Val_Backup = TIM15_CCR1_Exposure_Add_Val;

    ValPixHThred = (*Ptr).ValPixHThred;//0x28;  //0x30   //AWD的上限阈值，下限阈值为0

    Offset_PositionToDisatance = (*Ptr).Offset_PositionToDisatance;//94;   //105 //设置距离偏移量
    
    AngleCorrectForRunPositive = (*Ptr).AngleCorrectForRunPositive;
    AngleCorrectForRunInversion = (*Ptr).AngleCorrectForRunInversion;

    AgeingTestingFlag = (*Ptr).AgeingTestingFlag;

    FlagSunNoiseDeal = (*Ptr).FlagSunNoiseDeal;
    
    ScanForceStopEnable = (*Ptr).ScanForceStopEnable;

    EPC901_AmpVolGain = (*Ptr).EPC901_AmpVolGain;

    CentroidCorrect = (*Ptr).CentroidCorrect;

    SanDataProcol = (*Ptr).SanDataProcol;

    SET_BaudRate = (*Ptr).SET_BaudRate;

    FlagGlossNoiseDeal = (*Ptr).FlagGlossNoiseDeal;

    RelevanceDealLevel = (*Ptr).RelevanceDealLevel;

    UpThresholdFor153600_Val = (*Ptr).UpThresholdFor153600_Val;
    UpThresholdFor115200_Val = (*Ptr).UpThresholdFor115200_Val;
    UpThresholdFor230400_Val = (*Ptr).UpThresholdFor230400_Val;
    DownThresholdFor115200_Val = (*Ptr).DownThresholdFor115200_Val;
    
      
    ValValidFlag = (*Ptr).ValValidFlag;

    TIM15_CCR1_Exposure_Step_Level = (*Ptr).TIM15_CCR1_Exposure_Step_Level;
          
    Ptr_P2D_Para = (P2D_Para_Type*)FA_P2D_Para_Start;
    if((*Ptr_P2D_Para).Check_Valid == P2D_Para_Check_Valid)
    {
        ValP2DPara.a = (*Ptr_P2D_Para).a;
        ValP2DPara.b = (*Ptr_P2D_Para).b;
        ValP2DPara.c = (*Ptr_P2D_Para).c;
        ValP2DPara.start_Position= (*Ptr_P2D_Para).start_Position; 
        //VsnprintfForDma("Notes: The P2D_Para using equipment configure!\r");
    }
    else
    {
        ValP2DPara.a = P2D_Para_Default.a;
        ValP2DPara.b = P2D_Para_Default.b;
        ValP2DPara.c = P2D_Para_Default.c;  
        ValP2DPara.start_Position = P2D_Para_Default.start_Position;
        //VsnprintfForDma("Warning: The P2D_Para is not valid, Use default P2D_Para!\r");
    }

    for(i = 0, PtrSN = (u8 *)FA_SN_Start; i < SerialNumberNum; i++)
    {
        ValSNConfiure[i] = *(PtrSN+i);
    }
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void ConfigureParaEquipment(const Equipment_Configure_Type *Ptr)
{
    u8 * PtrSN;
    int i;
    
    TIM1_TimerPeriod = (*Ptr).TIM1_TimerPeriod;// = 200;
    TIM1_CH4PlusePercent = (*Ptr).TIM1_CH4PlusePercent;//60;  //设置LED激光管亮度
    TIM15_Period = (*Ptr).TIM15_Period;//0xFFFF;//36000;//TIM15_Period*TIM15_CCR4_Plus_Number;  

    Output2K4K5K = (*Ptr).Output2K4KConfigure;
    
    TIM2_Period = (*Ptr).TIM2_Period;//14;//14;
    TIM2_CCR2_Val = (*Ptr).TIM2_CCR2_Val;//TIM2_Period/2;
    
    TIM15_CCR1_Val = (*Ptr).TIM15_CCR1_Val;//17000; //17000   4.25KHZ
    TIM15_CCR1_Sample_Val = (*Ptr).TIM15_CCR1_Sample_Val;//12000;  //3000
    TIM15_CCR1_Exposure_Add_Val = (*Ptr).TIM15_CCR1_Exposure_Add_Val;//3000;  //12000设置前期曝光时间
    TIM15_CCR1_Exposure_Val = (*Ptr).TIM15_CCR1_Exposure_Val;//2000;           //设置后期曝光时间

    TIM15_CCR1_Exposure_Add_Val_Backup = TIM15_CCR1_Exposure_Add_Val;

    ValPixHThred = (*Ptr).ValPixHThred;//0x28;  //0x30   //AWD的上限阈值，下限阈值为0

    Offset_PositionToDisatance = (*Ptr).Offset_PositionToDisatance;//94;   //105 //设置距离偏移量
    
    AngleCorrectForRunPositive = (*Ptr).AngleCorrectForRunPositive;
    AngleCorrectForRunInversion = (*Ptr).AngleCorrectForRunInversion;    

    AgeingTestingFlag = (*Ptr).AgeingTestingFlag;

    FlagSunNoiseDeal = (*Ptr).FlagSunNoiseDeal;
    
    ScanForceStopEnable = (*Ptr).ScanForceStopEnable;

    EPC901_AmpVolGain = (*Ptr).EPC901_AmpVolGain;

    CentroidCorrect = (*Ptr).CentroidCorrect;

    SanDataProcol = (*Ptr).SanDataProcol;

    SET_BaudRate = (*Ptr).SET_BaudRate;

    FlagGlossNoiseDeal = (*Ptr).FlagGlossNoiseDeal;

    RelevanceDealLevel = (*Ptr).RelevanceDealLevel;

    UpThresholdFor153600_Val = (*Ptr).UpThresholdFor153600_Val;
    UpThresholdFor115200_Val = (*Ptr).UpThresholdFor115200_Val;
    UpThresholdFor230400_Val = (*Ptr).UpThresholdFor230400_Val;
    DownThresholdFor115200_Val = (*Ptr).DownThresholdFor115200_Val;

      
    ValValidFlag = (*Ptr).ValValidFlag;

    TIM15_CCR1_Exposure_Step_Level = (*Ptr).TIM15_CCR1_Exposure_Step_Level;

    Ptr_P2D_Para = (P2D_Para_Type*)FA_P2D_Para_Start;
    if((*Ptr_P2D_Para).Check_Valid == P2D_Para_Check_Valid)
    {
        ValP2DPara.a = (*Ptr_P2D_Para).a;
        ValP2DPara.b = (*Ptr_P2D_Para).b;
        ValP2DPara.c = (*Ptr_P2D_Para).c;
        ValP2DPara.start_Position= (*Ptr_P2D_Para).start_Position; 
        VsnprintfForDma("Notes: The P2D_Para using equipment configure!\r");
    }
    else
    {
        ValP2DPara.a = P2D_Para_Default.a;
        ValP2DPara.b = P2D_Para_Default.b;
        ValP2DPara.c = P2D_Para_Default.c;  
        ValP2DPara.start_Position = P2D_Para_Default.start_Position;
        VsnprintfForDma("Warning: The P2D_Para is not valid, Use default P2D_Para!\r");
    }

    for(i = 0, PtrSN = (u8 *)FA_SN_Start; i < SerialNumberNum; i++)
    {
        ValSNConfiure[i] = *(PtrSN+i);
    }    
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void ReNewCopyEquipmentConfigure(FLASH_PROGRAM_SECTION_Enum Section)
{  
    switch(Section)
    {
        case FLASH_PROGRAM_SECTION_EquipmentConf:  
            ValEquipmentConfiure.ValValidFlag = ValidFlag;
            
            ValEquipmentConfiure.TIM1_TimerPeriod = TIM1_TimerPeriod;
            ValEquipmentConfiure.TIM1_CH4PlusePercent = TIM1_CH4PlusePercent;
            
            ValEquipmentConfiure.TIM2_Period = TIM2_Period;
            ValEquipmentConfiure.TIM2_CCR2_Val = TIM2_CCR2_Val;
            
            ValEquipmentConfiure.TIM15_Period = TIM15_Period;
            TIM15_CCR1_Val = TIM15_CCR1_Sample_Val+TIM15_CCR1_Exposure_Add_Val+TIM15_CCR1_Exposure_Val;
            ValEquipmentConfiure.TIM15_CCR1_Val = TIM15_CCR1_Val;
            ValEquipmentConfiure.TIM15_CCR1_Sample_Val = TIM15_CCR1_Sample_Val;
            ValEquipmentConfiure.TIM15_CCR1_Exposure_Add_Val = TIM15_CCR1_Exposure_Add_Val;
            ValEquipmentConfiure.TIM15_CCR1_Exposure_Val = TIM15_CCR1_Exposure_Val;
            
            ValEquipmentConfiure.ValPixHThred = ValPixHThred;
            
            ValEquipmentConfiure.Offset_PositionToDisatance = Offset_PositionToDisatance;

            ValEquipmentConfiure.AngleCorrectForRunPositive = AngleCorrectForRunPositive;            
            ValEquipmentConfiure.AngleCorrectForRunInversion = AngleCorrectForRunInversion;
            
            ValEquipmentConfiure.Output2K4KConfigure = Output2K4K5K;

            ValEquipmentConfiure.AgeingTestingFlag = AgeingTestingFlag;
            
            ValEquipmentConfiure.FlagSunNoiseDeal = FlagSunNoiseDeal;
            
            ValEquipmentConfiure.ScanForceStopEnable = ScanForceStopEnable;
            
            ValEquipmentConfiure.EPC901_AmpVolGain = EPC901_AmpVolGain;
            
            ValEquipmentConfiure.CentroidCorrect = CentroidCorrect;
            
            ValEquipmentConfiure.SanDataProcol = SanDataProcol;
            
            ValEquipmentConfiure.SET_BaudRate = SET_BaudRate;

            ValEquipmentConfiure.FlagGlossNoiseDeal = FlagGlossNoiseDeal;

            ValEquipmentConfiure.RelevanceDealLevel = RelevanceDealLevel;

            ValEquipmentConfiure.UpThresholdFor153600_Val = UpThresholdFor153600_Val;
            ValEquipmentConfiure.UpThresholdFor115200_Val = UpThresholdFor115200_Val;
            ValEquipmentConfiure.UpThresholdFor230400_Val = UpThresholdFor230400_Val;
            ValEquipmentConfiure.DownThresholdFor115200_Val = DownThresholdFor115200_Val;

            ValEquipmentConfiure.TIM15_CCR1_Exposure_Step_Level = TIM15_CCR1_Exposure_Step_Level;  
            //DisplayConfigure(ValEquipmentConfiure.ValValidFlag);
        break;
        case FLASH_PROGRAM_SECTION_InitialConf:  
            ValIntialConfiure.ValValidFlag = ValidFlag;
            ValIntialConfiure.TIM1_TimerPeriod = TIM1_TimerPeriod;
            ValIntialConfiure.TIM1_CH4PlusePercent = TIM1_CH4PlusePercent;
            
            ValIntialConfiure.TIM2_Period = TIM2_Period;
            ValIntialConfiure.TIM2_CCR2_Val = TIM2_CCR2_Val;
            
            ValIntialConfiure.TIM15_Period = TIM15_Period;
            TIM15_CCR1_Val = TIM15_CCR1_Sample_Val+TIM15_CCR1_Exposure_Add_Val+TIM15_CCR1_Exposure_Val;
            ValIntialConfiure.TIM15_CCR1_Val = TIM15_CCR1_Val;
            ValIntialConfiure.TIM15_CCR1_Sample_Val = TIM15_CCR1_Sample_Val;
            ValIntialConfiure.TIM15_CCR1_Exposure_Add_Val = TIM15_CCR1_Exposure_Add_Val;
            ValIntialConfiure.TIM15_CCR1_Exposure_Val = TIM15_CCR1_Exposure_Val;
            
            ValIntialConfiure.ValPixHThred = ValPixHThred;
            
            ValIntialConfiure.Offset_PositionToDisatance = Offset_PositionToDisatance;
            
            ValIntialConfiure.AngleCorrectForRunPositive = AngleCorrectForRunPositive;
            ValIntialConfiure.AngleCorrectForRunInversion = AngleCorrectForRunInversion;
            
            ValIntialConfiure.Output2K4KConfigure = Output2K4K5K;
            
            ValIntialConfiure.AgeingTestingFlag = AgeingTestingFlag;
            
            ValIntialConfiure.FlagSunNoiseDeal = FlagSunNoiseDeal;
            
            ValIntialConfiure.ScanForceStopEnable = ScanForceStopEnable;     

            ValIntialConfiure.EPC901_AmpVolGain = EPC901_AmpVolGain;

            ValIntialConfiure.CentroidCorrect = CentroidCorrect;

            ValIntialConfiure.SanDataProcol = SanDataProcol;
            
            ValIntialConfiure.SET_BaudRate = SET_BaudRate;

            ValIntialConfiure.FlagGlossNoiseDeal = FlagGlossNoiseDeal;
            
            ValIntialConfiure.RelevanceDealLevel = RelevanceDealLevel;

            ValIntialConfiure.UpThresholdFor153600_Val = UpThresholdFor153600_Val;           
            ValIntialConfiure.UpThresholdFor115200_Val = UpThresholdFor115200_Val;           
            ValIntialConfiure.UpThresholdFor230400_Val = UpThresholdFor230400_Val;           
            ValIntialConfiure.DownThresholdFor115200_Val = DownThresholdFor115200_Val;

            ValIntialConfiure.TIM15_CCR1_Exposure_Step_Level = TIM15_CCR1_Exposure_Step_Level;  
            //DisplayConfigure(ValIntialConfiure.ValValidFlag);
        break;
    }
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void DisplayConfigure(void)
{
    VsnprintfForDma("Now Configure :\r");  
    //VsnprintfForDma("ValValidFlag = 0X%X \r", ValValidFlagLocal);  
    //VsnprintfForDma("TimerPeriod = %d \r", TIM1_TimerPeriod);  
    //VsnprintfForDma("CH4PlusePercent = %d \r", TIM1_CH4PlusePercent);  
    VsnprintfForDma("Period = %d \r", TIM2_Period);  
    VsnprintfForDma("Pluse = %d \r", TIM2_CCR2_Val);  

    VsnprintfForDma("Period = %d \r", TIM15_Period); 
    VsnprintfForDma("ALL_Val = %d \r", TIM15_CCR1_Val); 
    VsnprintfForDma("Sam_Val = %d \r", TIM15_CCR1_Sample_Val);                 
    VsnprintfForDma("Exp_Add_Val = %d \r", TIM15_CCR1_Exposure_Add_Val);              
    VsnprintfForDma("Exp_Val = %d \r", TIM15_CCR1_Exposure_Val);  

    VsnprintfForDma("ValThred = %d \r", ValPixHThred);  
    //VsnprintfForDma("Offset_PositionToDisatance = %d \r", Offset_PositionToDisatance);                 
    VsnprintfForDma("AngleCorrectForRunPositive = %d \r", AngleCorrectForRunPositive); 
    //VsnprintfForDma("AngleCorrectForRunInversion = %d \r", AngleCorrectForRunInversion); 
    VsnprintfForDma("Output2K4K5K = %d \r", Output2K4K5K); 
    VsnprintfForDma("AgeingTestingFlag = %d \r", AgeingTestingFlag); 
    VsnprintfForDma("Flag_SN_Deal = %d \r", FlagSunNoiseDeal); 
    //VsnprintfForDma("ScanForceStopEnable = %d \r", ScanForceStopEnable);  
    //VsnprintfForDma("EPC901_AmpVolGain = %d \r", EPC901_AmpVolGain);  
    VsnprintfForDma("CentroidCorrect = %d.%2d\r", (int)(CentroidCorrect),(int)((CentroidCorrect-(int)(CentroidCorrect))*100));  
    VsnprintfForDma("SanDataProcol = %d\r", SanDataProcol);  
    VsnprintfForDma("SET_BaudRate = %d(%d)\r", SET_BaudRate, BaudRateList[SET_BaudRate]);  
    VsnprintfForDma("Flag_GN_Deal = %d \r", FlagGlossNoiseDeal); 
    //VsnprintfForDma("RelevanceDealLevel = 0X%2X \r", RelevanceDealLevel); 
    VsnprintfForDma("UpThFor153600_Val = %d \r", UpThresholdFor153600_Val); 
    VsnprintfForDma("UpThFor115200_Val = %d \r", UpThresholdFor115200_Val); 
    VsnprintfForDma("UpThFor230400_Val = %d \r", UpThresholdFor230400_Val); 
    VsnprintfForDma("DownThresholdFor115200_Val = %d \r", DownThresholdFor115200_Val);
    //VsnprintfForDma("SNOfPlatform_SeriesH = %d \r", ((SNOfPlatform_SeriesH/0x1000000)%0x100)*1000 + ((SNOfPlatform_SeriesH/0x10000)%0x100)*100 + ((SNOfPlatform_SeriesH/0x100)%0x100)*10 + ((SNOfPlatform_SeriesH/0x1)%0x100));
    VsnprintfForDma("Exposure_Step_Level = %d \r", TIM15_CCR1_Exposure_Step_Level);
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void CopyEquipmentConfigure(FLASH_PROGRAM_SECTION_Enum Section)
{  
    const Initial_Configure_Type *Ptr_Initial;
    const Equipment_Configure_Type *Ptr_Equipment;
    switch(Section)
    {
        case FLASH_PROGRAM_SECTION_InitialConf:
          Ptr_Initial = (Initial_Configure_Type *)FA_InitialConf_Start;
          TIM1_TimerPeriod = (*Ptr_Initial).TIM1_TimerPeriod;// = 200;
          TIM1_CH4PlusePercent = (*Ptr_Initial).TIM1_CH4PlusePercent;//60;  //设置LED激光管亮度

          TIM2_Period = (*Ptr_Initial).TIM2_Period;//14;//14;
          TIM2_CCR2_Val = (*Ptr_Initial).TIM2_CCR2_Val;//TIM2_Period/2;

          TIM15_Period = (*Ptr_Initial).TIM15_Period;//0xFFFF;//36000;//TIM15_Period*TIM15_CCR4_Plus_Number;  
          TIM15_CCR1_Val = (*Ptr_Initial).TIM15_CCR1_Val;//17000; //17000   4.25KHZ
          TIM15_CCR1_Sample_Val = (*Ptr_Initial).TIM15_CCR1_Sample_Val;//12000;  //3000
          TIM15_CCR1_Exposure_Add_Val = (*Ptr_Initial).TIM15_CCR1_Exposure_Add_Val;//3000;  //12000设置前期曝光时间
          TIM15_CCR1_Exposure_Val = (*Ptr_Initial).TIM15_CCR1_Exposure_Val;//2000;           //设置后期曝光时间

          ValPixHThred = (*Ptr_Initial).ValPixHThred;//0x28;  //0x30   //AWD的上限阈值，下限阈值为0

          Offset_PositionToDisatance = (*Ptr_Initial).Offset_PositionToDisatance;//94;   //105 //设置距离偏移量
          
          AngleCorrectForRunPositive = (*Ptr_Initial).AngleCorrectForRunPositive; 
          AngleCorrectForRunInversion = (*Ptr_Initial).AngleCorrectForRunInversion; 
          
          Output2K4K5K = (*Ptr_Initial).Output2K4KConfigure;
          
          AgeingTestingFlag = (*Ptr_Initial).AgeingTestingFlag;
          
          FlagSunNoiseDeal =  (*Ptr_Initial).FlagSunNoiseDeal;
          
          ScanForceStopEnable = (*Ptr_Initial).ScanForceStopEnable;
          
          EPC901_AmpVolGain = (*Ptr_Initial).EPC901_AmpVolGain;

          CentroidCorrect = (*Ptr_Initial).CentroidCorrect;

          SanDataProcol = (*Ptr_Initial) .SanDataProcol;

          SET_BaudRate = (*Ptr_Initial) .SET_BaudRate;

          FlagGlossNoiseDeal = (*Ptr_Initial) .FlagGlossNoiseDeal;

          RelevanceDealLevel =  (*Ptr_Initial) .RelevanceDealLevel; 
          
          UpThresholdFor153600_Val =  (*Ptr_Initial) .UpThresholdFor153600_Val; 
          UpThresholdFor115200_Val =  (*Ptr_Initial) .UpThresholdFor115200_Val; 
          UpThresholdFor230400_Val =  (*Ptr_Initial) .UpThresholdFor230400_Val; 
          DownThresholdFor115200_Val = (*Ptr_Initial).DownThresholdFor115200_Val;
          
          
          ValValidFlag = (*Ptr_Initial).ValValidFlag;

          TIM15_CCR1_Exposure_Step_Level = (*Ptr_Initial).TIM15_CCR1_Exposure_Step_Level;
          break;
        case FLASH_PROGRAM_SECTION_EquipmentConf:
          Ptr_Equipment = (Equipment_Configure_Type *)FA_EquipmentConf_Start;
          TIM1_TimerPeriod = (*Ptr_Equipment).TIM1_TimerPeriod;// = 200;
          TIM1_CH4PlusePercent = (*Ptr_Equipment).TIM1_CH4PlusePercent;//60;  //设置LED激光管亮度

          TIM2_Period = (*Ptr_Equipment).TIM2_Period;//14;//14;
          TIM2_CCR2_Val = (*Ptr_Equipment).TIM2_CCR2_Val;//TIM2_Period/2;

          TIM15_Period = (*Ptr_Equipment).TIM15_Period;//0xFFFF;//36000;//TIM15_Period*TIM15_CCR4_Plus_Number;  
          TIM15_CCR1_Val = (*Ptr_Equipment).TIM15_CCR1_Val;//17000; //17000   4.25KHZ
          TIM15_CCR1_Sample_Val = (*Ptr_Equipment).TIM15_CCR1_Sample_Val;//12000;  //3000
          TIM15_CCR1_Exposure_Add_Val = (*Ptr_Equipment).TIM15_CCR1_Exposure_Add_Val;//3000;  //12000设置前期曝光时间
          TIM15_CCR1_Exposure_Val = (*Ptr_Equipment).TIM15_CCR1_Exposure_Val;//2000;           //设置后期曝光时间

          ValPixHThred = (*Ptr_Equipment).ValPixHThred;//0x28;  //0x30   //AWD的上限阈值，下限阈值为0

          Offset_PositionToDisatance = (*Ptr_Equipment).Offset_PositionToDisatance;//94;   //105 //设置距离偏移量

          AngleCorrectForRunPositive = (*Ptr_Equipment).AngleCorrectForRunPositive;   
          AngleCorrectForRunInversion = (*Ptr_Equipment).AngleCorrectForRunInversion;   
          
          Output2K4K5K = (*Ptr_Equipment).Output2K4KConfigure;

          AgeingTestingFlag = (bool)(*Ptr_Equipment).AgeingTestingFlag;
          
          FlagSunNoiseDeal =  (bool)(*Ptr_Equipment).FlagSunNoiseDeal;
          
          ScanForceStopEnable = (bool)(*Ptr_Equipment).ScanForceStopEnable;

          EPC901_AmpVolGain = (*Ptr_Equipment).EPC901_AmpVolGain;

          CentroidCorrect = (*Ptr_Equipment).CentroidCorrect;

          SanDataProcol = (*Ptr_Equipment) .SanDataProcol;
          
          SET_BaudRate = (*Ptr_Equipment) .SET_BaudRate;

          FlagGlossNoiseDeal = (*Ptr_Equipment) .FlagGlossNoiseDeal;

          RelevanceDealLevel =  (*Ptr_Equipment) .RelevanceDealLevel; 
          
          UpThresholdFor153600_Val =  (*Ptr_Equipment) .UpThresholdFor153600_Val; 
          UpThresholdFor115200_Val =  (*Ptr_Equipment) .UpThresholdFor115200_Val; 
          UpThresholdFor230400_Val =  (*Ptr_Equipment) .UpThresholdFor230400_Val; 
          DownThresholdFor115200_Val = (*Ptr_Equipment).DownThresholdFor115200_Val;
            
          ValValidFlag = (*Ptr_Equipment).ValValidFlag;

          TIM15_CCR1_Exposure_Step_Level = (*Ptr_Equipment).TIM15_CCR1_Exposure_Step_Level;
          break;
        default:
          return;
          break;
     }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
int FlashProgram(FLASH_PROGRAM_SECTION_Enum Section, bool Erase)
{
  u32 StartAddress, StopAddress;
  u32 *PtrData;
  u32 ComputedCRC, ComputedCRCTemp;
  u32 CalMaker = ChipId;
  u32 NbrOfPage = 0x00;
  
  /* Define the number of page to be erased */
  switch(Section)
  {
      case FLASH_PROGRAM_SECTION_P2D:
        NbrOfPage = FA_P2D_NbrOfPage;
        StartAddress = FA_P2D_Para_Start;
        StopAddress = FA_P2D_Para_Stop;
        PtrData = (u32*)&ValP2DPara;
        break;
      case FLASH_PROGRAM_SECTION_SN:
        NbrOfPage = FA_SN_NbrOfPage;
        StartAddress = FA_SN_Start;
        StopAddress = FA_SN_Stop;
        PtrData = (u32*)&ValSNConfiure;
        break;
      case FLASH_PROGRAM_SECTION_InitialConf:
        ReNewCopyEquipmentConfigure(FLASH_PROGRAM_SECTION_InitialConf);
        NbrOfPage = FA_InitialConf_NbrOfPage;
        StartAddress = FA_InitialConf_Start;
        StopAddress = FA_InitialConf_Stop;    
        PtrData = (u32*)&ValIntialConfiure;
        break;
      case FLASH_PROGRAM_SECTION_EquipmentConf:
        ReNewCopyEquipmentConfigure(FLASH_PROGRAM_SECTION_EquipmentConf);
        NbrOfPage = FA_EquipmentConf_NbrOfPage;
        StartAddress = FA_EquipmentConf_Start;  
        StopAddress = FA_EquipmentConf_Stop;      
        PtrData = (u32*)&ValEquipmentConfiure;
        break;
      default:
        return FALSE;
        break;
  }

  /* Erase the FLASH pages */
  if(Erase == TRUE)
  {
      fmc_erase_pages(StartAddress, NbrOfPage);
      //VsnprintfForDma("fmc_erase_pages OK!\r"); 
	  delay_loop(3000);
      if(fmc_erase_pages_check(StartAddress, NbrOfPage) != TRUE)
      {
            return FALSE;
      }
      //VsnprintfForDma("fmc_erase_pages_check OK!\r"); 
  }
   
  /* Program the FLASH */
  fmc_program(StartAddress, StopAddress, PtrData);
  //VsnprintfForDma("fmc_program OK!\r");  
  delay_loop(3000);
  if(fmc_program_check(StartAddress, StopAddress, PtrData) != TRUE)
  {
        return FALSE;
  }
  //VsnprintfForDma("fmc_program_check OK!\r");  
  
  ComputedCRC = Cal_AllFlash_CRCValue(ChipId, FlashCRCWordStartAddr, FlashCRCWordSize); 
  /* Erase the FLASH pages */
  if(Erase == TRUE)
  {
      fmc_erase_pages(FA_CRCConf_Start, FA_CRCConf_NbrOfPage);
      //VsnprintfForDma("fmc_erase_pages OK!\r"); 
      delay_loop(3000);
      if(fmc_erase_pages_check(FA_CRCConf_Start, FA_CRCConf_NbrOfPage) != TRUE)
      {
            return FALSE;
      }
      //VsnprintfForDma("fmc_erase_pages_check OK!\r"); 
  }
  /* Program the FLASH */
  ComputedCRCTemp = ComputedCRC;
  fmc_program(FA_CRCConf_Start, FA_CRCConf_Start+sizeof(u32), &ComputedCRCTemp);
  delay_loop(3000);
  //VsnprintfForDma("fmc_program OK!\r");  
  if(fmc_program_check(FA_CRCConf_Start, FA_CRCConf_Start+sizeof(u32), &ComputedCRCTemp) != TRUE)
  {
        return FALSE;
  }
  ComputedCRCTemp = ComputedCRC + (CalMaker<< 0);
  fmc_program(FA_CRCConf_Start+sizeof(u32), FA_CRCConf_Start+2*sizeof(u32), &ComputedCRCTemp);
  delay_loop(3000);
  //VsnprintfForDma("fmc_program OK!\r");  
  if(fmc_program_check(FA_CRCConf_Start+sizeof(u32), FA_CRCConf_Start+2*sizeof(u32), &ComputedCRCTemp) != TRUE)
  {
        return FALSE;
  }
  ComputedCRCTemp = ComputedCRC + (CalMaker<< 8);
  fmc_program(FA_CRCConf_Start+2*sizeof(u32), FA_CRCConf_Start+3*sizeof(u32), &ComputedCRCTemp);
  delay_loop(3000);
  //VsnprintfForDma("fmc_program OK!\r");  
  if(fmc_program_check(FA_CRCConf_Start+2*sizeof(u32), FA_CRCConf_Start+3*sizeof(u32), &ComputedCRCTemp) != TRUE)
  {
        return FALSE;
  }
  ComputedCRCTemp = ComputedCRC + (CalMaker<< 16);
  fmc_program(FA_CRCConf_Start+3*sizeof(u32), FA_CRCConf_Start+4*sizeof(u32), &ComputedCRCTemp);
  delay_loop(3000);
  //VsnprintfForDma("fmc_program OK!\r");  
  if(fmc_program_check(FA_CRCConf_Start+3*sizeof(u32), FA_CRCConf_Start+4*sizeof(u32), &ComputedCRCTemp) != TRUE)
  {
        return FALSE;
  }  
  //VsnprintfForDma("fmc_program_check OK!\r");    
  return TRUE;  
}

