//*********************************************************************************************************************************************
//Revision History:
//---------------------------------------------------------------------------------------------------------------------------------------------
//Rev    Level     Originator     Date          Auditor      Date         Testtime/insertion
//---------------------------------------------------------------------------------------------------------------------------------------------
//00     Inital    Yan He         2025/02/27                 	                   TBD

//********************************************************************************************************************************************* 
//SC30xx TEST PROGRAM
//---------------------------------------------------------------------------------------------------------------------------------------------
//DEVICE ...........................SC3031X
//PACKAGE...........................SOT23-6
//DUT BOARD NUMBER..................SAFS3031X
//LOAD BOARD NUMBER.................8192P
//CABLE CONNECTION..................64PIN *1 , 16PIN *1

//********************************************************************************************************************************************* 
// PIN INOUT Definition
//---------------------------------------------------------------------------------------------------------------------------------------------  
//
//                           ________________
//                          |                |
//                   1  GND-|                |-DRV  6
//                          |    SC3031X     |
//                          |                |
//                   2   FB-|                |-VDD  5
//                          |                |
//                          |                |
//                   3  ZCD-|                |-CS   4                  
//                          |________________|
//
//********************************************************************************************************************************************* 
#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include "BoardCheck.h"
#include "Test_Method.h"
#include "treg.h"
#include "Temp_Char.h"
#include <string>
#include <iostream>
//#include "dio_plus.h"
//#include "EEPROM_Interface.h"
//#include "awg.h"

#pragma   warning ( disable: 4244 )
#pragma   warning ( disable: 4305 )
#pragma   warning ( disable: 4800 )

TREG    dut;
Test_Method test_method;

extern "C" int GetPgsFullPath(LPTSTR pgsPath, int chNum);
extern string int2str(DWORD n);



int BI_DET_ZCD[SITE_NUM] = { 0,0,0,0 };//trim bit 15
int HV_EN[SITE_NUM] = { 0,0,0,0 };//trim bit 16
int VBST_SEL[SITE_NUM] = { 1,1,1,1 };//trim bit 17
int HL_QR[SITE_NUM] = { 1,1,1,1 };//trim bit 18
int CS_JT[SITE_NUM] = { 0,0,0,0 };//trim bit 19
int VBST_Fx[SITE_NUM] = { 0,0,0,0 };//trim bit 20
int SCP_UVP[SITE_NUM] = { 1,1,1,1 };//trim bit 21
int OTP_L[SITE_NUM] = { 0,0,0,0 };//trim bit 22
int OVP[SITE_NUM] = { 0,0,0,0 };//trim bit 23
int PKL_EN[SITE_NUM] = { 0,0,0,0 };//trim bit 24
int PK_TIME1[SITE_NUM] = { 0,0,0,0 };//trim bit 25---------bit24=1
int PK_TIME2[SITE_NUM] = { 0,0,0,0 };//trim bit 26---------bit24=1
int PKL_Protect[SITE_NUM] = { 0,0,0,0 };//trim bit 27----------bit24=1
int OCP_HFsw[SITE_NUM] = { 1,1,1,1 };//trim bit 25----------bit24=0
int HFS[SITE_NUM] = { 0,0,0,0 };//trim bit 26----------bit24=0
int L_Cslim[SITE_NUM] = { 1,1,1,1 };//trim bit 27----------bit24=0

int Flag_HV[SITE_NUM] = { 0,0,0,0 };


int ee0_init_READ[SITE_NUM] = { 0,0,0,0 };
int ee1_init_READ[SITE_NUM] = { 0,0,0,0 };
int ee2_init_READ[SITE_NUM] = { 0,0,0,0 };
int ee3_init_READ[SITE_NUM] = { 0,0,0,0 };

double VBG[SITE_NUM] = { 0,0,0,0 };
double IBIAS[SITE_NUM] = { 0,0,0,0 };
double FSWCCM[SITE_NUM] = { 0,0,0,0 };
double SLOPE[SITE_NUM] = { 0,0,0,0 };
double KLINE[SITE_NUM] = { 0,0,0,0 };
double TBOUT_TIME[SITE_NUM] = { 0,0,0,0 };
double TCLK[SITE_NUM] = { 0,0,0,0 };
double UVLOH[SITE_NUM] = { 0,0,0,0 };

//========Pre_ABS_Arayy=================
double ABS_ZCD[SITE_NUM] = { 0,0 };
double ABS_FB[SITE_NUM] = { 0,0 };
double ABS_CS[SITE_NUM] = { 0,0 };
double ABS_DRV[SITE_NUM] = { 0,0 };
double ABS_VDD[SITE_NUM] = { 0,0 };
double ABS_HV[SITE_NUM] = { 0,0 };

int  site = 0;
int globalsite;
BYTE sitesta[SITE_NUM];


bool DO_TRIM = true;
bool QC = false;
bool DEBUG = true;	// Note: If DEBUG is true, Test function is independant, has power on and power off for each. If false, power on/off is disable for TTR
bool DO_FMEA = false;
bool Char_enable;
bool TRIM15 = false;
bool TRIM23 = false;
bool TRIM24 = false;
bool TRIM27 = false;

int PART_NO = 3031;

//board check coding
bool DO_BoardCheck = false;//bool DO_BoardCheck = true;
extern BOOL run_diags();     // global funtion for HW checker

bool burn_flag[SITE_NUM];


//==============不同版本flag==============================
bool SC3031_PKL1 = false;
bool SC3031_PKL0 = true;

TEMP temp;



#define ARRAY_SIZE(_array) (sizeof(_array) / sizeof(_array[0]))
//===============================================
//===============================================
//===============================================
//////////////FOVI resource//////////////////////
FOVI KELVIN_GND(0, "KELVIN_GND");
FOVI KELVIN_VDD(1, "KELVIN_VDD");
FOVI KELVIN_DRV(2, "KELVIN_DRV");
FOVI ZCD_VI(3, "ZCD_VI");//DATA
FOVI FB_VI(4, "FB_VI");
FOVI CS_VI(5, "CS_VI");//CLK
FOVI VDD_VI(6, "VDD_VI");
FOVI DRV_VI(7, "DRV_VI");

//////////////FPVI resource//////////////////////
FPVI10 DRV_FPVI0(0, "DRV_FPVI0");
FPVI10 VDD_FPVI1(1, "VDD_FPVI1");

//////////////QTMU//////////////////////////////
QTMU_PLUS QTMU_MEAS(0);

//////////////DIO///////////////////////////////
DIO dio(0);

//////////////CBIT///////////////////////////////
CBIT128 cbit;

//////////////GPVI////////////////////////////////
GPFOVI  ALL_FOVI("ALL_FOVI", ZCD_VI, FB_VI, CS_VI, VDD_VI, DRV_VI);
GPFOVI  KELVIN_VI("KELVIN_VI", KELVIN_GND, KELVIN_VDD, KELVIN_DRV);
GPFPVI10  ALL_FPVI("ALL_FPVI", DRV_FPVI0, VDD_FPVI1);


DUT_API void UserLoad()
{
    STSSetAlwaysRunFunction("TEST22_OS_FINAL");
    STSSetSiteStatus(0xFFFF);
    StsSetModuleToSite(MD_FOVI, SITE_1, 0, 1, 2, 3, 4, 5, 6, 7, -1); //SITE1
    StsSetModuleToSite(MD_FOVI, SITE_2, 8, 9, 10, 11, 12, 13, 14, 15, -1); //SITE2
    StsSetModuleToSite(MD_FOVI, SITE_3, 16, 17, 18, 19, 20, 21, 22, 23, -1); //SITE3
    StsSetModuleToSite(MD_FOVI, SITE_4, 24, 25, 26, 27, 28, 29, 30, 31, -1); //SITE4

    StsSetModuleToSite(MD_FPVI10, SITE_1, 0, 1, -1);//set channels 0-3 to SITE1
    StsSetModuleToSite(MD_FPVI10, SITE_2, 2, 3, -1);//set channels 4-7 to SITE2
    StsSetModuleToSite(MD_FPVI10, SITE_3, 4, 5, -1);//set channels 4-7 to SITE3
    StsSetModuleToSite(MD_FPVI10, SITE_4, 6, 7, -1);//set channels 4-7 to SITE4

    StsSetModuleToSite(MD_QTMUPLUS, SITE_1, 0, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_2, 1, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_3, 2, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_4, 3, -1);

    //StsSetModuleToSite(MD_HVI1K, SITE_1, 0, -1);//set channel 0 to SITE1

    AstBindingAllModule();

    if (DO_BoardCheck)
        run_diags();

    /*if (DO_FMEA)
        efmea.fmea_enable = 1;*/
}

DUT_API void UserInit()
{
    char pgsfullpath[300];
    GetPgsFullPath(pgsfullpath, 300);
    string fullpathpgs(pgsfullpath);
    string pathofpgs = "";
    int pos = fullpathpgs.find_last_of('\\');
    if (pos > -1)
    {
        pathofpgs = fullpathpgs.substr(0, pos + 1);
    }

    string pathoftreg;
    if (SC3031_PKL1) pathoftreg = pathofpgs + "3031_PKL1.treg";
    if (SC3031_PKL0) pathoftreg = pathofpgs + "3031_PKL0.treg";


    //if (!dut.init(pathoftreg.c_str(), SITE_NUM))   
    //{
    //	//return 0;
    //}

    if (!dut.init(pathoftreg.c_str(), SITE_NUM))
    {
        //return 0;
    }
}

//multisite settings should be included here
DUT_API void HardWareCfg()
{
    /*For example: four channels dvi to config two sites*/
    StsSetModuleToSite(MD_FOVI, SITE_1, 0, 1, 2, 3, 4, 5, 6, 7, -1); //SITE1
    StsSetModuleToSite(MD_FOVI, SITE_2, 8, 9, 10, 11, 12, 13, 14, 15, -1); //SITE2
    StsSetModuleToSite(MD_FOVI, SITE_3, 16, 17, 18, 19, 20, 21, 22, 23, -1); //SITE3
    StsSetModuleToSite(MD_FOVI, SITE_4, 24, 25, 26, 27, 28, 29, 30, 31, -1); //SITE4

    StsSetModuleToSite(MD_FPVI10, SITE_1, 0, 1, -1);//set channels 0-3 to SITE1
    StsSetModuleToSite(MD_FPVI10, SITE_2, 2, 3, -1);//set channels 4-7 to SITE2
    StsSetModuleToSite(MD_FPVI10, SITE_3, 4, 5, -1);//set channels 4-7 to SITE3
    StsSetModuleToSite(MD_FPVI10, SITE_4, 6, 7, -1);//set channels 4-7 to SITE4

    StsSetModuleToSite(MD_QTMUPLUS, SITE_1, 0, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_2, 1, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_3, 2, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_4, 3, -1);

    //StsSetModuleToSite(MD_HVI1K, SITE_1, 0, -1);//set channel 0 to SITE1
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initialize function will be called before all the test functions.
DUT_API void InitBeforeTestFlow()
{
    KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    KELVIN_VDD.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    KELVIN_DRV.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    
    //ALL_FOVI_Power_Down();

    QTMU_MEAS.Init();
    cbit.SetOn(-1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()
{		
    KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    KELVIN_VDD.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    KELVIN_DRV.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    DRV_FPVI0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_OFF);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_OFF);

   
    //ALL_FOVI_Power_Off(1);

    cbit.SetOn(-1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//Fail site hardware set function will be called after failed params, it can be called for serveral times. 
DUT_API void SetupFailSite(const unsigned char*byFailSite)
{			
    ALL_FOVI_Power_Off(1);
    cbit.SetOn(-1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/


DUT_API int TEST00_INFO(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Part_No = StsGetParam(funcindex,"Part_No");
    CParam *QC_Flag = StsGetParam(funcindex,"QC_Flag");
    CParam *TRIM_Flag = StsGetParam(funcindex,"TRIM_Flag");
    CParam *SiteCheck_S1 = StsGetParam(funcindex,"SiteCheck_S1");
    CParam *SiteCheck_S2 = StsGetParam(funcindex,"SiteCheck_S2");
    CParam *SiteCheck_S3 = StsGetParam(funcindex,"SiteCheck_S3");
    CParam *SiteCheck_S4 = StsGetParam(funcindex,"SiteCheck_S4");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    SERIAL Part_No->SetTestResult(SITE, 0, 3031);
    SERIAL TRIM_Flag->SetTestResult(SITE, 0, DO_TRIM);
    SERIAL QC_Flag->SetTestResult(SITE, 0, QC);

    double adresults[SITE_NUM];

    //----------------------site check-------------------------------------
#pragma region site_check
// ************************************************
// Site_Check 
// Measure each site Resistor to check production setup
// Site1=10K, Site2=20K, Site3=30K, Site4=40K
// ************************************************
    cbitclose(K0_SiteCheck);
    KELVIN_GND.Set(FV, 0, FOVI_20V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FI, 0, FOVI_20V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FI, 0.1e-3, FOVI_20V, FOVI_1MA, RELAY_ON);
    delay_ms(3);

    KELVIN_GND.MeasureVI(50, 10);
    KELVIN_GND.Set(FI, 0, FOVI_20V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FV, 0, FOVI_20V, FOVI_1MA, RELAY_ON);


    for (int site = 0; site < SITE_NUM; site++)
    {
        adresults[site] = KELVIN_GND.GetMeasResult(site, MVRET) / 0.1;// KOhm

        switch (site)
        {
        case SITE_1:
            SiteCheck_S1->SetTestResult(site, 0, adresults[site]);
            break;

        case SITE_2:
            SiteCheck_S2->SetTestResult(site, 0, adresults[site]);
            break;

        case SITE_3:
            SiteCheck_S3->SetTestResult(site, 0, adresults[site]);
            break;

        case SITE_4:
            SiteCheck_S4->SetTestResult(site, 0, adresults[site]);
            break;
        }
    }
#pragma endregion site_check

    return 0;
}


DUT_API int TEST01_KELVIN_CHECK(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Kelvin_GND = StsGetParam(funcindex,"Kelvin_GND");
    CParam *Kelvin_VDD = StsGetParam(funcindex,"Kelvin_VDD");
    CParam *Kelvin_DRV = StsGetParam(funcindex,"Kelvin_DRV");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    double MeasV1[SITE_NUM], MeasV2[SITE_NUM], MeasV3[SITE_NUM];
    double MeasI1[SITE_NUM], MeasI2[SITE_NUM], MeasI3[SITE_NUM];

    cbitclose(K6_VDD_VI);
    cbitclose(K1_KelvinCheck);

    KELVIN_VI.Set(FI, 0.0, FOVI_10V, FOVI_1MA, RELAY_ON);
    KELVIN_VI.Set(FI, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_ms(3);
    KELVIN_VI.Set(FI, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);
    KELVIN_VI.Set(FI, 0.008, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    KELVIN_VI.MeasureVI(20, 10);

    //===================	KELVIN_GND	===================
    SERIAL MeasV1[SITE] = KELVIN_GND.GetMeasResult(SITE, MVRET);
    SERIAL MeasI1[SITE] = MeasV1[SITE] / 0.008; //Ohm
    SERIAL Kelvin_GND->SetTestResult(SITE, 0, MeasI1[SITE]);

    //===================	KELVIN_VDD  ===================
    SERIAL MeasV2[SITE] = KELVIN_VDD.GetMeasResult(SITE, MVRET);
    SERIAL MeasI2[SITE] = MeasV2[SITE] / 0.008; //Ohm
    SERIAL Kelvin_VDD->SetTestResult(SITE, 0, MeasI2[SITE]);

    //===================	KELVIN_DRV		===================
    SERIAL MeasV3[SITE] = KELVIN_DRV.GetMeasResult(SITE, MVRET);
    SERIAL MeasI3[SITE] = MeasV3[SITE] / 0.008; //Ohm
    SERIAL Kelvin_DRV->SetTestResult(SITE, 0, MeasI3[SITE]);

    //===================================================================================================
    //====================>POWER DOWN
    //===================================================================================================
    KELVIN_VI.Set(FI, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);
    KELVIN_VI.Set(FV, 0.0, FOVI_10V, FOVI_1MA, RELAY_ON);

    cbitopen(K1_KelvinCheck);
    cbitopen(K6_VDD_VI);
    delay_ms(3);


    return 0;
}


DUT_API int TEST02_OS(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *OS_ALL_OPEN = StsGetParam(funcindex,"OS_ALL_OPEN");
    CParam *OS_SOME_OPEN = StsGetParam(funcindex,"OS_SOME_OPEN");
    CParam *OS_SHORT = StsGetParam(funcindex,"OS_SHORT");
    CParam *OS_ZCD = StsGetParam(funcindex,"OS_ZCD");
    CParam *OS_FB = StsGetParam(funcindex,"OS_FB");
    CParam *OS_CS = StsGetParam(funcindex,"OS_CS");
    CParam *OS_DRV = StsGetParam(funcindex,"OS_DRV");
    CParam *OS_VDD = StsGetParam(funcindex,"OS_VDD");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    CParam* OS_DLOG[5];

    OS_DLOG[0] = StsGetParam(funcindex, "OS_ZCD");
    OS_DLOG[1] = StsGetParam(funcindex, "OS_FB");
    OS_DLOG[2] = StsGetParam(funcindex, "OS_CS");
    OS_DLOG[3] = StsGetParam(funcindex, "OS_DRV");
    OS_DLOG[4] = StsGetParam(funcindex, "OS_VDD");

    int open_count[SITE_NUM] = { 0, 0, 0, 0 };
    int SHORT_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int SOME_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int ALL_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int LEAK_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int OS_DLOG_NUM = 5;
    double os_result[5][SITE_NUM] = { 0, 0, 0, 0 };
    int os_count = 0;

    double results[SITE_NUM] = { 0, 0, 0, 0 };
    //===================================================================================================
    //====================>INIT
    //===================================================================================================
    //cbitclose(K3_ZCD_VI);
    //cbitclose(K4_FB_VI);
    //cbitclose(K5_CS_VI);
    //cbitclose(K6_VDD_VI);默认相连
    cbitclose(K7_DRV_VI, K10_DRV_PIN_Resource);
    //cbitclose(K11_FPVI_HVIN);
    delay_ms(3);

    ZCD_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //=====================OS_ZCD===========================
    ZCD_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    ZCD_VI.MeasureVI(20, 10);
    groupgetresults(ZCD_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[0][SITE] = results[SITE];
    msLogResultAll(OS_ZCD, 0, results, SITE_NUM);
    ZCD_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_FB===========================
    FB_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    FB_VI.MeasureVI(20, 10);
    groupgetresults(FB_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[1][SITE] = results[SITE];
    msLogResultAll(OS_FB, 0, results, SITE_NUM);
    FB_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_CS===========================
    CS_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    CS_VI.MeasureVI(20, 10);
    groupgetresults(CS_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[2][SITE] = results[SITE];
    msLogResultAll(OS_CS, 0, results, SITE_NUM);
    CS_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_DRV===========================
    DRV_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    DRV_VI.MeasureVI(20, 10);
    groupgetresults(DRV_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[3][SITE] = results[SITE];
    msLogResultAll(OS_DRV, 0, results, SITE_NUM);
    DRV_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_VDD===========================
    VDD_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    VDD_VI.MeasureVI(20, 10);
    groupgetresults(VDD_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[4][SITE] = results[SITE];
    msLogResultAll(OS_VDD, 0, results, SITE_NUM);
    VDD_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);


    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    //************************************ Add OS classify delog ***********************************************
    //test_method.OS_Classify(funcindex, pin_str_array, PIN_NUM, NULL, LEAK_NUM, os_result, NULL, vec_exclude);
    //************************************ Add OS classify delog end *******************************************

    for (os_count = 0; os_count < OS_DLOG_NUM; os_count++)  //judge short and get open count
    {


        SERIAL
        {
            if (OS_DLOG[os_count]->GetMinLimit() > 0) //if OS value>0
            {
                if (os_result[os_count][SITE] < OS_DLOG[os_count]->GetMinLimit()) { SHORT_FLAG[SITE] = 1; }
                if (SHORT_FLAG[SITE] != 1 && (os_result[os_count][SITE] > OS_DLOG[os_count]->GetMaxLimit())) { open_count[SITE]++; }


            }
            if (OS_DLOG[os_count]->GetMaxLimit() < 0) //if OS value<0
            {
                if (os_result[os_count][SITE] > OS_DLOG[os_count]->GetMaxLimit()) { SHORT_FLAG[SITE] = 1; }
                if (SHORT_FLAG[SITE] != 1 && (os_result[os_count][SITE] < OS_DLOG[os_count]->GetMinLimit())) { open_count[SITE]++; }

            }
        }



    }

    SERIAL    //judge some open or all open 
    {
        if (SHORT_FLAG[SITE] != 1 && open_count[SITE] == 5) { ALL_OPEN_FLAG[SITE] = 1; }
        else if (SHORT_FLAG[SITE] != 1 && open_count[SITE] < 5 && open_count[SITE]>0) { SOME_OPEN_FLAG[SITE] = 1; }
    }

    SERIAL	OS_SHORT->SetTestResult(SITE, 0, SHORT_FLAG[SITE]);
    SERIAL	OS_SOME_OPEN->SetTestResult(SITE, 0, SOME_OPEN_FLAG[SITE]);
    SERIAL	OS_ALL_OPEN->SetTestResult(SITE, 0, ALL_OPEN_FLAG[SITE]);

    return 0;
}


DUT_API int TEST03_P2P_LEAK(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *ZCD_P2P_LEAK = StsGetParam(funcindex,"ZCD_P2P_LEAK");
    CParam *FB_P2P_LEAK = StsGetParam(funcindex,"FB_P2P_LEAK");
    CParam *CS_P2P_LEAK = StsGetParam(funcindex,"CS_P2P_LEAK");
    CParam *DRV_P2P_LEAK = StsGetParam(funcindex,"DRV_P2P_LEAK");
    CParam *VDD_P2P_LEAK = StsGetParam(funcindex,"VDD_P2P_LEAK");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    double results[SITE_NUM] = { -9999, -9999, -9999, -9999 };
    //=================resource initial===================================
     //cbitclose(K3_ZCD_VI);
    //cbitclose(K4_FB_VI);
    //cbitclose(K5_CS_VI);
    //cbitclose(K6_VDD_VI);默认相连
    cbitclose(K7_DRV_VI, K10_DRV_PIN_Resource);
    //cbitclose(K11_FPVI_HVIN);
    delay_ms(3);

    ZCD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================ZCD_P2P_LEAK============================================
    ZCD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    ZCD_VI.MeasureVI(20, 10);
    groupgetresults(ZCD_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(ZCD_P2P_LEAK, 0, results, SITE_NUM);

    ZCD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================FB_P2P_LEAK============================================
    FB_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    FB_VI.MeasureVI(20, 10);
    groupgetresults(FB_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(FB_P2P_LEAK, 0, results, SITE_NUM);

    FB_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================CS_P2P_LEAK============================================
    CS_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    CS_VI.MeasureVI(20, 10);
    groupgetresults(CS_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(CS_P2P_LEAK, 0, results, SITE_NUM);

    CS_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================DRV_P2P_LEAK============================================
    DRV_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    DRV_VI.MeasureVI(20, 10);
    groupgetresults(DRV_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(DRV_P2P_LEAK, 0, results, SITE_NUM);

    DRV_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================VDD_P2P_LEAK============================================
    VDD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    VDD_VI.MeasureVI(20, 10);
    groupgetresults(VDD_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(VDD_P2P_LEAK, 0, results, SITE_NUM);

    VDD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);


    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST04_ABS_LEAK(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *ZCD_ABS_LEAK = StsGetParam(funcindex,"ZCD_ABS_LEAK");
    CParam *FB_ABS_LEAK = StsGetParam(funcindex,"FB_ABS_LEAK");
    CParam *CS_ABS_LEAK = StsGetParam(funcindex,"CS_ABS_LEAK");
    CParam *DRV_ABS_LEAK = StsGetParam(funcindex,"DRV_ABS_LEAK");
    CParam *VDD_ABS_LEAK = StsGetParam(funcindex,"VDD_ABS_LEAK");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    double results[SITE_NUM] = { 0,0,0,0 };
    double result_HV[SITE_NUM] = { 0,0,0,0 };
    //=================resource initial===================================
     //cbitclose(K3_ZCD_VI);
    //cbitclose(K4_FB_VI);
    //cbitclose(K5_CS_VI);默认相连
    cbitclose(K6_VDD_VI);
    cbitclose(K8_VDD_FPVI1);
    cbitclose(K7_DRV_VI, K10_DRV_PIN_Resource);
    //cbitclose(K11_FPVI_HVIN);
    delay_ms(3);

    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    delay_ms(2);

    //=========================================ZCD_ABS_LEAK============================================
    ZCD_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    ZCD_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    delay_ms(2);
    ZCD_VI.MeasureVI(10, 10);
    groupgetresults(ZCD_VI, MIRET, results, SITE_NUM);
    SERIAL ABS_ZCD[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(ZCD_ABS_LEAK, 0, ABS_ZCD, SITE_NUM);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================FB_ABS_LEAK============================================
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    FB_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    delay_ms(2);
    FB_VI.MeasureVI(10, 10);
    groupgetresults(FB_VI, MIRET, results, SITE_NUM);
    SERIAL ABS_FB[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(FB_ABS_LEAK, 0, ABS_FB, SITE_NUM);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================CS_ABS_LEAK============================================
    CS_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    CS_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    delay_ms(2);
    CS_VI.MeasureVI(10, 10);
    groupgetresults(CS_VI, MIRET, results, SITE_NUM);
    SERIAL ABS_CS[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(CS_ABS_LEAK, 0, ABS_CS, SITE_NUM);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================DRV_ABS_LEAK============================================
    //DRV_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    //delay_ms(1);
    //DRV_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    //DRV_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    //delay_ms(2);
    //DRV_VI.MeasureVI(10, 10);
    //groupgetresults(DRV_VI, MIRET, results, SITE_NUM);
    //SERIAL ABS_DRV[SITE] = 1e6 * results[SITE];//uA
    //msLogResultAll(DRV_ABS_LEAK, 0, ABS_DRV, SITE_NUM);
    //DRV_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    //delay_ms(2);

    //=========================================VDD_ABS_LEAK============================================
    VDD_FPVI1.Set(FV, 80, FPVI10_100V, FPVI10_100MA, RELAY_ON);//6.5
    delay_ms(1);
    VDD_FPVI1.Set(FV, 80, FPVI10_100V, FPVI10_1MA, RELAY_ON);
    delay_ms(1);
    VDD_FPVI1.MeasureVI(20, 10);
    groupgetresults(VDD_FPVI1, MIRET, results, SITE_NUM);
    SERIAL ABS_VDD[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(VDD_ABS_LEAK, 0, ABS_VDD, SITE_NUM);

    VDD_FPVI1.Set(FV, 10, FPVI10_10V, FPVI10_10MA, RELAY_ON);
    delay_ms(1);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
    delay_ms(2);
    cbitopen(K8_VDD_FPVI1);
    delay_ms(3);

    
    //====================POWER DOWN=======================
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    DRV_FPVI0.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    delay_ms(2);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST05_EE_INIT(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *EE0_READ = StsGetParam(funcindex,"EE0_READ");
    CParam *EE1_READ = StsGetParam(funcindex,"EE1_READ");
    CParam *EE2_READ = StsGetParam(funcindex,"EE2_READ");
    CParam *EE3_READ = StsGetParam(funcindex,"EE3_READ");
    //}}AFX_STS_PARAM_PROTOTYPES
    /*********Trim items define**********/
    TRIM_NODE& vbg = dut.trim("VBG");
    TRIM_NODE& ibias = dut.trim("IBIAS");
    TRIM_NODE& fsw = dut.trim("FSW");
    TRIM_NODE& slope = dut.trim("SLOPE");
    TRIM_NODE& kline = dut.trim("KLINE");
    /**********Optional trim items**********/
    SEL_NODE& bi_det_zcd = dut.sel("BI_DET_ZCD");//trim bit 15
    SEL_NODE& hv_en = dut.sel("HV_EN_FUN");//trim bit 16
    SEL_NODE& vbst_sel = dut.sel("VBST_SEL_FUN");//trim bit 17
    SEL_NODE& hl_qr = dut.sel("HL_QR_FUN");//trim bit 18
    SEL_NODE& cs_jt = dut.sel("CS_JT_FUN");//trim bit 19
    SEL_NODE& vbst_fx = dut.sel("VBST_Fx_FUN");//trim bit 20
    SEL_NODE& scp_uvp = dut.sel("SCP_UVP_FUN");//trim bit 21
    SEL_NODE& otp_l = dut.sel("OTP_L_FUN");//trim bit 22
    SEL_NODE& ovp = dut.sel("OVP_FUN");//trim bit 23
    SEL_NODE& pkl_en = dut.sel("PKL_EN_FUN");//trim bit 24
    // SEL_NODE& pk_time = dut.sel("PK_TIME_FUN");//trim bit 25/26---------bit24=1
     //SEL_NODE& pkl_protect = dut.sel("PKL_Protect_FUN");//trim bit 27----------bit24=1
    SEL_NODE& ocp_hfsw = dut.sel("OCP_HFsw_FUN");//trim bit 25----------bit24=0
    SEL_NODE& hfs = dut.sel("HFS_FUN");//trim bit 26----------bit24=0
    SEL_NODE& l_cslim = dut.sel("L_Cslim_FUN");//trim bit 27----------bit24=0

    // TODO: Add your function code here
    int ee0_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee1_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee2_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee3_READ[SITE_NUM] = { -1,-1,-1,-1 };

    int dio_ch[4] = { 1, 3, 5, 7 };//DATA CHANNEL
    // TODO: Add your function code here
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(2);
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    delay_ms(3);
    Enter_TM_Read(1);//进入标准读模式

    Enter_Read_Mode();//期望DATA为高，data有数据会被拉低-->输出为低

    SERIAL ee0_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 2, 9, LSB));
    SERIAL ee0_init_READ[SITE] = 255 - ee0_READ[SITE];//data拉低=寄存器里为1，所以输出值需要取反
    SERIAL EE0_READ->SetTestResult(SITE, 0, 255 - ee0_READ[SITE]);

    SERIAL ee1_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 10, 17, LSB));
    SERIAL ee1_init_READ[SITE] = 255 - ee1_READ[SITE];
    SERIAL EE1_READ->SetTestResult(SITE, 0, 255 - ee1_READ[SITE]);

    SERIAL ee2_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 18, 25, LSB));
    SERIAL ee2_init_READ[SITE] = 255 - ee2_READ[SITE];
    SERIAL EE2_READ->SetTestResult(SITE, 0, 255 - ee2_READ[SITE]);

    SERIAL ee3_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 26, 33, LSB));
    SERIAL ee3_init_READ[SITE] = 255 - ee3_READ[SITE];
    SERIAL EE3_READ->SetTestResult(SITE, 0, 255 - ee3_READ[SITE]);

    dio.StopPattern();
    dio.Disconnect();
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);


    SERIAL
    {
            if ((ee0_init_READ[SITE]) || (ee1_init_READ[SITE]) || (ee2_init_READ[SITE]) || (ee3_init_READ[SITE]) || QC) //ALL bank 
            {
                burn_flag[SITE] = false;
            }
    }

        ////////////save read back value for comparasion/////////////////////////
    SERIAL dut.assy("EEPROM0").set_read_back((ee0_init_READ[SITE]), SITE);
    SERIAL dut.assy("EEPROM1").set_read_back((ee1_init_READ[SITE]), SITE);
    SERIAL dut.assy("EEPROM2").set_read_back((ee2_init_READ[SITE]), SITE);
    SERIAL dut.assy("EEPROM3").set_read_back((ee3_init_READ[SITE]), SITE);

    dut.assy("EEPROM0").copy_read_to_prog();
    dut.assy("EEPROM1").copy_read_to_prog();
    dut.assy("EEPROM2").copy_read_to_prog();
    dut.assy("EEPROM3").copy_read_to_prog();

    ///////////////////////////////////////////////////


    ///////////////////////////////////////////////////
    //#EEPROM0
    SERIAL if (burn_flag[SITE] == 0)
    {

        vbg.set_trim_allowed(false, SITE);
        ibias.set_trim_allowed(false, SITE);
        fsw.set_trim_allowed(false, SITE);
        slope.set_trim_allowed(false, SITE);
        kline.set_trim_allowed(false, SITE);


        //TRIM_ODC.set_trim_allowed(false,SITE);
        dut.assy("EEPROM0").copy_read_to_work(SITE);
        dut.assy("EEPROM1").copy_read_to_work(SITE);
        dut.assy("EEPROM2").copy_read_to_work(SITE);
        dut.assy("EEPROM3").copy_read_to_work(SITE);


    }
    else
    {

        vbg.set_trim_allowed(true, SITE);
        ibias.set_trim_allowed(true, SITE);
        fsw.set_trim_allowed(true, SITE);
        slope.set_trim_allowed(true, SITE);
        kline.set_trim_allowed(true, SITE);

    }


    return 0;
}


DUT_API int TEST06_VBG_IBIAS_TRIM(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VBG_Step0 = StsGetParam(funcindex,"VBG_Step0");
    CParam *VBG_Step1 = StsGetParam(funcindex,"VBG_Step1");
    CParam *VBG_Step2 = StsGetParam(funcindex,"VBG_Step2");
    CParam *VBG_Step3 = StsGetParam(funcindex,"VBG_Step3");
    CParam *VBG_Step4 = StsGetParam(funcindex,"VBG_Step4");
    CParam *VBG_Step5 = StsGetParam(funcindex,"VBG_Step5");
    CParam *VBG_Step6 = StsGetParam(funcindex,"VBG_Step6");
    CParam *VBG_Step7 = StsGetParam(funcindex,"VBG_Step7");
    CParam *VBG_Pre = StsGetParam(funcindex,"VBG_Pre");
    CParam *VBG_Pre_bit = StsGetParam(funcindex,"VBG_Pre_bit");
    CParam *VBG_Post = StsGetParam(funcindex,"VBG_Post");
    CParam *VBG_Post_bit = StsGetParam(funcindex,"VBG_Post_bit");
    CParam *VBG_Target = StsGetParam(funcindex,"VBG_Target");
    CParam *VBG_Guessed = StsGetParam(funcindex,"VBG_Guessed");
    CParam *VBG_Updated = StsGetParam(funcindex,"VBG_Updated");
    CParam *IBIAS_Step0 = StsGetParam(funcindex,"IBIAS_Step0");
    CParam *IBIAS_Step1 = StsGetParam(funcindex,"IBIAS_Step1");
    CParam *IBIAS_Step2 = StsGetParam(funcindex,"IBIAS_Step2");
    CParam *IBIAS_Step3 = StsGetParam(funcindex,"IBIAS_Step3");
    CParam *IBIAS_Step4 = StsGetParam(funcindex,"IBIAS_Step4");
    CParam *IBIAS_Step5 = StsGetParam(funcindex,"IBIAS_Step5");
    CParam *IBIAS_Step6 = StsGetParam(funcindex,"IBIAS_Step6");
    CParam *IBIAS_Step7 = StsGetParam(funcindex,"IBIAS_Step7");
    CParam *IBIAS_Pre = StsGetParam(funcindex,"IBIAS_Pre");
    CParam *IBIAS_Pre_bit = StsGetParam(funcindex,"IBIAS_Pre_bit");
    CParam *IBIAS_Post = StsGetParam(funcindex,"IBIAS_Post");
    CParam *IBIAS_Post_bit = StsGetParam(funcindex,"IBIAS_Post_bit");
    CParam *IBIAS_Target = StsGetParam(funcindex,"IBIAS_Target");
    CParam *IBIAS_Guessed = StsGetParam(funcindex,"IBIAS_Guessed");
    CParam *IBIAS_Updated = StsGetParam(funcindex,"IBIAS_Updated");
    //}}AFX_STS_PARAM_PROTOTYPES
    /*********Trim items define**********/
    TRIM_NODE& vbg = dut.trim("VBG");
    TRIM_NODE& ibias = dut.trim("IBIAS");
    TRIM_NODE& fsw = dut.trim("FSW");
    TRIM_NODE& slope = dut.trim("SLOPE");
    TRIM_NODE& kline = dut.trim("KLINE");
    /**********Optional trim items**********/
    SEL_NODE& bi_det_zcd = dut.sel("BI_DET_ZCD");//trim bit 15
    SEL_NODE& hv_en = dut.sel("HV_EN_FUN");//trim bit 16
    SEL_NODE& vbst_sel = dut.sel("VBST_SEL_FUN");//trim bit 17
    SEL_NODE& hl_qr = dut.sel("HL_QR_FUN");//trim bit 18
    SEL_NODE& cs_jt = dut.sel("CS_JT_FUN");//trim bit 19
    SEL_NODE& vbst_fx = dut.sel("VBST_Fx_FUN");//trim bit 20
    SEL_NODE& scp_uvp = dut.sel("SCP_UVP_FUN");//trim bit 21
    SEL_NODE& otp_l = dut.sel("OTP_L_FUN");//trim bit 22
    SEL_NODE& ovp = dut.sel("OVP_FUN");//trim bit 23
    SEL_NODE& pkl_en = dut.sel("PKL_EN_FUN");//trim bit 24
    // SEL_NODE& pk_time = dut.sel("PK_TIME_FUN");//trim bit 25/26---------bit24=1
     //SEL_NODE& pkl_protect = dut.sel("PKL_Protect_FUN");//trim bit 27----------bit24=1
    SEL_NODE& ocp_hfsw = dut.sel("OCP_HFsw_FUN");//trim bit 25----------bit24=0
    SEL_NODE& hfs = dut.sel("HFS_FUN");//trim bit 26----------bit24=0
    SEL_NODE& l_cslim = dut.sel("L_Cslim_FUN");//trim bit 27----------bit24=0

    // TODO: Add your function code here
    double  vbg_result[SITE_NUM] = { 0,0,0,0 };
    double  ibias_result[SITE_NUM] = { 0,0,0,0 };


    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FI, 0, FOVI_10V, FOVI_100UA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    /**************************************VBG trim**************************************/
    //=================>Preview BGTC all step's data
    if (vbg.table_char_active())//
    {
        vbg.save_working();//保存默认状态
        for (int step = 0; step < 8; step++)//BG  2^3=8
        {
            SERIAL vbg.set_working(step, SITE);//设置pre bit
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure vbg_pre------------------------------//
            Enter_TM2(1);
            cbitclose(K20_FB_1NF);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(3);

            CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(5);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);//测试条件建立

            FB_VI.MeasureVI(20, 10);
            groupgetresults(FB_VI, MVRET, vbg_result, SITE_NUM);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            cbitopen(K20_FB_1NF);
            delay_ms(3);

            vbg.table_char(vbg_result, step);
            switch (step)
            {
            case 0:SERIAL	VBG_Step0->SetTestResult(SITE, 0, vbg_result[SITE]);
            case 1:SERIAL	VBG_Step1->SetTestResult(SITE, 0, vbg_result[SITE]);
            case 2:SERIAL	VBG_Step2->SetTestResult(SITE, 0, vbg_result[SITE]);
            case 3:SERIAL	VBG_Step3->SetTestResult(SITE, 0, vbg_result[SITE]);
            case 4:SERIAL	VBG_Step4->SetTestResult(SITE, 0, vbg_result[SITE]);
            case 5:SERIAL	VBG_Step5->SetTestResult(SITE, 0, vbg_result[SITE]);
            case 6:SERIAL	VBG_Step6->SetTestResult(SITE, 0, vbg_result[SITE]);
            case 7:SERIAL	VBG_Step7->SetTestResult(SITE, 0, vbg_result[SITE]);
            }

        }
        vbg.restore_working();//恢复默认状态
    }

    //=========================================================
    //====================>Pre_Test
    //=========================================================
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    Enter_TM_Pre(1);//进入Pre模式
    RunPreviewCodes();//run pre bit pattern
    TM_RESET(1);//RESET TM
    //---------Measure vbg_pre----------------//
    Enter_TM2(1);
    cbitclose(K20_FB_1NF);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(3);

    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(5);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);//测试条件建立

    FB_VI.MeasureVI(20, 10);
    groupgetresults(FB_VI, MVRET, vbg_result, SITE_NUM);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_us(20);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitopen(K20_FB_1NF);
    delay_ms(3);

    //////////////////////////////TEST DATALOG/////////////////////////////////////////////////
    SERIAL	VBG_Pre->SetTestResult(SITE, 0, vbg_result[SITE]);
    SERIAL	VBG_Pre_bit->SetTestResult(SITE, 0, vbg.get_working(SITE));

    //=========================================================
    //====================>post value
    //=========================================================
    if (!QC && DO_TRIM)
    {
        vbg.pre(vbg_result);

        if (vbg.updated_by_trim())
        {
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure vbg_post------------------------------//
            Enter_TM2(1);
            cbitclose(K20_FB_1NF);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(3);

            CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(5);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);//测试条件建立

            FB_VI.MeasureVI(20, 10);
            groupgetresults(FB_VI, MVRET, vbg_result, SITE_NUM);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            cbitopen(K20_FB_1NF);
            delay_ms(3);

        }

        vbg.post(vbg_result);
    }

    SERIAL	VBG_Post->SetTestResult(SITE, 0, vbg_result[SITE]);
    SERIAL	VBG_Post_bit->SetTestResult(SITE, 0, vbg.get_working(SITE));
    SERIAL	VBG_Target->SetTestResult(SITE, 0, vbg.get_target(SITE));
    SERIAL	VBG_Guessed->SetTestResult(SITE, 0, vbg.get_guessed_final(SITE));
    SERIAL	VBG_Updated->SetTestResult(SITE, 0, vbg.updated_by_trim(SITE));

    //----------------------POWER DOWN--------------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);

    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    /**************************************IBIAS trim**************************************/
    //=================>Preview ibias all step's data
    if (ibias.table_char_active())//
    {
        ibias.save_working();//保存默认状态
        for (int step = 0; step < 8; step++)//BG  2^3=8
        {
            SERIAL ibias.set_working(step, SITE);//设置pre bit
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure ibias_pre------------------------------//
            Enter_TM2(1);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(3);

            CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(5);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            delay_ms(2);//测试条件建立

            FB_VI.MeasureVI(20, 10);
            groupgetresults(FB_VI, MIRET, ibias_result, SITE_NUM);
            msScaleDataAll(1e6, ibias_result, SITE_NUM);//换算为uA

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(3);

            ibias.table_char(ibias_result, step);
            switch (step)
            {
            case 0:SERIAL	IBIAS_Step0->SetTestResult(SITE, 0, ibias_result[SITE]);
            case 1:SERIAL	IBIAS_Step1->SetTestResult(SITE, 0, ibias_result[SITE]);
            case 2:SERIAL	IBIAS_Step2->SetTestResult(SITE, 0, ibias_result[SITE]);
            case 3:SERIAL	IBIAS_Step3->SetTestResult(SITE, 0, ibias_result[SITE]);
            case 4:SERIAL	IBIAS_Step4->SetTestResult(SITE, 0, ibias_result[SITE]);
            case 5:SERIAL	IBIAS_Step5->SetTestResult(SITE, 0, ibias_result[SITE]);
            case 6:SERIAL	IBIAS_Step6->SetTestResult(SITE, 0, ibias_result[SITE]);
            case 7:SERIAL	IBIAS_Step7->SetTestResult(SITE, 0, ibias_result[SITE]);
            }

        }
        ibias.restore_working();//恢复默认状态
    }

    //=========================================================
    //====================>Pre_Test
    //=========================================================
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    Enter_TM_Pre(1);//进入Pre模式
    RunPreviewCodes();//run pre bit pattern
    TM_RESET(1);//RESET TM
    //---------Measure vbg_pre----------------//
    Enter_TM2(1);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(3);

    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(5);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    delay_ms(2);//测试条件建立

    FB_VI.MeasureVI(20, 10);
    groupgetresults(FB_VI, MIRET, ibias_result, SITE_NUM);
    msScaleDataAll(1e6, ibias_result, SITE_NUM);//换算为uA

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_us(20);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitopen(K20_FB_1NF);
    delay_ms(3);

    //////////////////////////////TEST DATALOG/////////////////////////////////////////////////
    SERIAL	IBIAS_Pre->SetTestResult(SITE, 0, ibias_result[SITE]);
    SERIAL	IBIAS_Pre_bit->SetTestResult(SITE, 0, ibias.get_working(SITE));

    //=========================================================
    //====================>post value
    //=========================================================
    if (!QC && DO_TRIM)
    {
        ibias.pre(ibias_result);

        if (ibias.updated_by_trim())
        {
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure ibias_post------------------------------//
            Enter_TM2(1);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(3);

            CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(5);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            delay_ms(2);//测试条件建立

            FB_VI.MeasureVI(20, 10);
            groupgetresults(FB_VI, MIRET, ibias_result, SITE_NUM);
            msScaleDataAll(1e6, ibias_result, SITE_NUM);//换算为uA

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            cbitopen(K20_FB_1NF);
            delay_ms(3);

        }

        ibias.post(ibias_result);
    }

    SERIAL	IBIAS_Post->SetTestResult(SITE, 0, ibias_result[SITE]);
    SERIAL	IBIAS_Post_bit->SetTestResult(SITE, 0, ibias.get_working(SITE));
    SERIAL	IBIAS_Target->SetTestResult(SITE, 0, ibias.get_target(SITE));
    SERIAL	IBIAS_Guessed->SetTestResult(SITE, 0, ibias.get_guessed_final(SITE));
    SERIAL	IBIAS_Updated->SetTestResult(SITE, 0, ibias.updated_by_trim(SITE));

    //----------------------POWER DOWN--------------------------------------//
    dio.StopPattern();
    dio.Disconnect();
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST07_FSW_TRIM(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *FSW_Step0 = StsGetParam(funcindex,"FSW_Step0");
    CParam *FSW_Step1 = StsGetParam(funcindex,"FSW_Step1");
    CParam *FSW_Step2 = StsGetParam(funcindex,"FSW_Step2");
    CParam *FSW_Step3 = StsGetParam(funcindex,"FSW_Step3");
    CParam *FSW_Step4 = StsGetParam(funcindex,"FSW_Step4");
    CParam *FSW_Step5 = StsGetParam(funcindex,"FSW_Step5");
    CParam *FSW_Step6 = StsGetParam(funcindex,"FSW_Step6");
    CParam *FSW_Step7 = StsGetParam(funcindex,"FSW_Step7");
    CParam *FSW_Step8 = StsGetParam(funcindex,"FSW_Step8");
    CParam *FSW_Step9 = StsGetParam(funcindex,"FSW_Step9");
    CParam *FSW_Step10 = StsGetParam(funcindex,"FSW_Step10");
    CParam *FSW_Step11 = StsGetParam(funcindex,"FSW_Step11");
    CParam *FSW_Step12 = StsGetParam(funcindex,"FSW_Step12");
    CParam *FSW_Step13 = StsGetParam(funcindex,"FSW_Step13");
    CParam *FSW_Step14 = StsGetParam(funcindex,"FSW_Step14");
    CParam *FSW_Step15 = StsGetParam(funcindex,"FSW_Step15");
    CParam *FSW_Pre = StsGetParam(funcindex,"FSW_Pre");
    CParam *FSW_Pre_bit = StsGetParam(funcindex,"FSW_Pre_bit");
    CParam *FSW_Post = StsGetParam(funcindex,"FSW_Post");
    CParam *FSW_Post_bit = StsGetParam(funcindex,"FSW_Post_bit");
    CParam *FSW_Target = StsGetParam(funcindex,"FSW_Target");
    CParam *FSW_Guessed = StsGetParam(funcindex,"FSW_Guessed");
    CParam *FSW_Updated = StsGetParam(funcindex,"FSW_Updated");
    //}}AFX_STS_PARAM_PROTOTYPES

    /*********Trim items define**********/
    TRIM_NODE& vbg = dut.trim("VBG");
    TRIM_NODE& ibias = dut.trim("IBIAS");
    TRIM_NODE& fsw = dut.trim("FSW");
    TRIM_NODE& slope = dut.trim("SLOPE");
    TRIM_NODE& kline = dut.trim("KLINE");
    /**********Optional trim items**********/
    SEL_NODE& bi_det_zcd = dut.sel("BI_DET_ZCD");//trim bit 15
    SEL_NODE& hv_en = dut.sel("HV_EN_FUN");//trim bit 16
    SEL_NODE& vbst_sel = dut.sel("VBST_SEL_FUN");//trim bit 17
    SEL_NODE& hl_qr = dut.sel("HL_QR_FUN");//trim bit 18
    SEL_NODE& cs_jt = dut.sel("CS_JT_FUN");//trim bit 19
    SEL_NODE& vbst_fx = dut.sel("VBST_Fx_FUN");//trim bit 20
    SEL_NODE& scp_uvp = dut.sel("SCP_UVP_FUN");//trim bit 21
    SEL_NODE& otp_l = dut.sel("OTP_L_FUN");//trim bit 22
    SEL_NODE& ovp = dut.sel("OVP_FUN");//trim bit 23
    SEL_NODE& pkl_en = dut.sel("PKL_EN_FUN");//trim bit 24
    //SEL_NODE& pk_time = dut.sel("PK_TIME_FUN");//trim bit 25/26---------bit24=1
   // SEL_NODE& pkl_protect = dut.sel("PKL_Protect_FUN");//trim bit 27----------bit24=1
    SEL_NODE& ocp_hfsw = dut.sel("OCP_HFsw_FUN");//trim bit 25----------bit24=0
    SEL_NODE& hfs = dut.sel("HFS_FUN");//trim bit 26----------bit24=0
    SEL_NODE& l_cslim = dut.sel("L_Cslim_FUN");//trim bit 27----------bit24=0
    // TODO: Add your function code here
    double  fswccm_result[SITE_NUM] = { 0,0,0,0 };
    double  results[SITE_NUM] = { 0,0,0,0 };

    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    delay_ms(3);

    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(4.0, QTMU_PLUS_NEG_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    /**************************************FSWCCM trim**************************************/
    //=================>Preview fswccm all step's data
    if (fsw.table_char_active())

    {
        fsw.save_working();



        for (int step = 0;step < 16;step++)
        {

            SERIAL fsw.set_working(step, SITE);
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure FSWCCM_pre------------------------------//
            Enter_TM8(1);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);

            CS_VI.Set(FV, 0.6, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);

            QTMU_MEAS.MeasFreq(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 20, 10);//cycle number=20，timeout=10ms
            SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
            SERIAL fswccm_result[SITE] = results[SITE];

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(1);


            fsw.table_char(fswccm_result, step);
            switch (step)
            {

            case 0:SERIAL	FSW_Step0->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 1:SERIAL	FSW_Step1->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 2:SERIAL	FSW_Step2->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 3:SERIAL	FSW_Step3->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 4:SERIAL	FSW_Step4->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 5:SERIAL	FSW_Step5->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 6:SERIAL	FSW_Step6->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 7:SERIAL	FSW_Step7->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 8:SERIAL	FSW_Step8->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 9:SERIAL	FSW_Step9->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 10:SERIAL	FSW_Step10->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 11:SERIAL	FSW_Step11->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 12:SERIAL	FSW_Step12->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 13:SERIAL	FSW_Step13->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 14:SERIAL	FSW_Step14->SetTestResult(SITE, 0, fswccm_result[SITE]);
            case 15:SERIAL	FSW_Step15->SetTestResult(SITE, 0, fswccm_result[SITE]);

            }

        }

        fsw.restore_working();
    }

    //=========================================================
    //====================>Pre_Test
    //=========================================================
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    //delay_ms(3);
    Enter_TM_Pre(1);//进入Pre模式
    RunPreviewCodes();//run pre bit pattern
    TM_RESET(1);//RESET TM
    //------------------Measure FSWCCM_pre------------------------------//
    Enter_TM8(1);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.6, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);//测试条件建立

    QTMU_MEAS.MeasFreq(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 20, 10);//cycle number=20，timeout=10ms
    SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL fswccm_result[SITE] = results[SITE];

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_us(20);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(1);
    //////////////////////////////TEST DATALOG/////////////////////////////////////////////////
    SERIAL	FSW_Pre->SetTestResult(SITE, 0, fswccm_result[SITE]);
    SERIAL	FSW_Pre_bit->SetTestResult(SITE, 0, fsw.get_working(SITE));

    //=========================================================
   //====================>post value
   //=========================================================
    if (!QC && DO_TRIM)
    {
        fsw.pre(fswccm_result);

        if (fsw.updated_by_trim())
        {
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure fswccm_post------------------------------//
            Enter_TM8(1);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);

            CS_VI.Set(FV, 0.6, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);//测试条件建立

            QTMU_MEAS.MeasFreq(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 20, 10);//cycle number=20，timeout=10ms
            SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
            SERIAL fswccm_result[SITE] = results[SITE];

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(1);

        }

        fsw.post(fswccm_result);
    }

    SERIAL  FSWCCM[SITE] = fswccm_result[SITE];
    SERIAL	FSW_Post->SetTestResult(SITE, 0, fswccm_result[SITE]);
    SERIAL	FSW_Post_bit->SetTestResult(SITE, 0, fsw.get_working(SITE));
    SERIAL	FSW_Target->SetTestResult(SITE, 0, fsw.get_target(SITE));
    SERIAL	FSW_Guessed->SetTestResult(SITE, 0, fsw.get_guessed_final(SITE));
    SERIAL	FSW_Updated->SetTestResult(SITE, 0, fsw.updated_by_trim(SITE));

    //----------------------POWER DOWN--------------------------------------//
    dio.StopPattern();
    dio.Disconnect();
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST08_SLOPE_TRIM(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *SLOPE_Step0 = StsGetParam(funcindex,"SLOPE_Step0");
    CParam *SLOPE_Step1 = StsGetParam(funcindex,"SLOPE_Step1");
    CParam *SLOPE_Step2 = StsGetParam(funcindex,"SLOPE_Step2");
    CParam *SLOPE_Step3 = StsGetParam(funcindex,"SLOPE_Step3");
    CParam *SLOPE_Pre = StsGetParam(funcindex,"SLOPE_Pre");
    CParam *SLOPE_Pre_bit = StsGetParam(funcindex,"SLOPE_Pre_bit");
    CParam *SLOPE_Post = StsGetParam(funcindex,"SLOPE_Post");
    CParam *SLOPE_Post_bit = StsGetParam(funcindex,"SLOPE_Post_bit");
    CParam *SLOPE_Target = StsGetParam(funcindex,"SLOPE_Target");
    CParam *SLOPE_Guessed = StsGetParam(funcindex,"SLOPE_Guessed");
    CParam *SLOPE_Updated = StsGetParam(funcindex,"SLOPE_Updated");
    //}}AFX_STS_PARAM_PROTOTYPES

   /*********Trim items define**********/
    TRIM_NODE& vbg = dut.trim("VBG");
    TRIM_NODE& ibias = dut.trim("IBIAS");
    TRIM_NODE& fsw = dut.trim("FSW");
    TRIM_NODE& slope = dut.trim("SLOPE");
    TRIM_NODE& kline = dut.trim("KLINE");
    /**********Optional trim items**********/
    SEL_NODE& bi_det_zcd = dut.sel("BI_DET_ZCD");//trim bit 15
    SEL_NODE& hv_en = dut.sel("HV_EN_FUN");//trim bit 16
    SEL_NODE& vbst_sel = dut.sel("VBST_SEL_FUN");//trim bit 17
    SEL_NODE& hl_qr = dut.sel("HL_QR_FUN");//trim bit 18
    SEL_NODE& cs_jt = dut.sel("CS_JT_FUN");//trim bit 19
    SEL_NODE& vbst_fx = dut.sel("VBST_Fx_FUN");//trim bit 20
    SEL_NODE& scp_uvp = dut.sel("SCP_UVP_FUN");//trim bit 21
    SEL_NODE& otp_l = dut.sel("OTP_L_FUN");//trim bit 22
    SEL_NODE& ovp = dut.sel("OVP_FUN");//trim bit 23
    SEL_NODE& pkl_en = dut.sel("PKL_EN_FUN");//trim bit 24
    // SEL_NODE& pk_time = dut.sel("PK_TIME_FUN");//trim bit 25/26---------bit24=1
    // SEL_NODE& pkl_protect = dut.sel("PKL_Protect_FUN");//trim bit 27----------bit24=1
    SEL_NODE& ocp_hfsw = dut.sel("OCP_HFsw_FUN");//trim bit 25----------bit24=0
    SEL_NODE& hfs = dut.sel("HFS_FUN");//trim bit 26----------bit24=0
    SEL_NODE& l_cslim = dut.sel("L_Cslim_FUN");//trim bit 27----------bit24=0
    // TODO: Add your function code here
     // TODO: Add your function code here
    double  slope_result[SITE_NUM] = { 0,0,0,0 };
    double  vfb_result[SITE_NUM] = { 0,0,0,0 };
    double  results[SITE_NUM] = { 0,0,0,0 };

    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    delay_ms(3);

    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(1.5, QTMU_PLUS_POS_SLOPE);//trigger=1.5V, Rising edge
    QTMU_MEAS.SetStopTrigger(1.5, QTMU_PLUS_NEG_SLOPE);
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    /**************************************SLOPE trim**************************************/
    //=================>Preview slope all step's data
    if (slope.table_char_active())

    {
        slope.save_working();

        for (int step = 0;step < 4;step++)
        {

            SERIAL slope.set_working(step, SITE);
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure SLOPE_pre------------------------------//
            Enter_TM4(1);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 3, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);

            QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_US, 10);
            FB_VI.MeasureVI(10, 10);
            groupgetresults(FB_VI, MVRET, vfb_result, SITE_NUM);
            SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);//返回us
            SERIAL slope_result[SITE] = 1e3 * (vfb_result[SITE] - 0.15) / 4.2;//单位mv
            SERIAL slope_result[SITE] = slope_result[SITE] / (results[SITE] + 1e-9);//单位mv/us

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(1);


            slope.table_char(slope_result, step);
            switch (step)
            {

            case 0:SERIAL	SLOPE_Step0->SetTestResult(SITE, 0, slope_result[SITE]);
            case 1:SERIAL	SLOPE_Step1->SetTestResult(SITE, 0, slope_result[SITE]);
            case 2:SERIAL	SLOPE_Step2->SetTestResult(SITE, 0, slope_result[SITE]);
            case 3:SERIAL	SLOPE_Step3->SetTestResult(SITE, 0, slope_result[SITE]);

            }

        }

        slope.restore_working();
    }

    //=========================================================
    //====================>Pre_Test
    //=========================================================
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    //delay_ms(3);
    Enter_TM_Pre(1);//进入Pre模式
    RunPreviewCodes();//run pre bit pattern
    TM_RESET(1);//RESET TM
    //------------------Measure SLOPE_pre------------------------------//
    Enter_TM4(1);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 3, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_US, 10);
    FB_VI.MeasureVI(10, 10);
    groupgetresults(FB_VI, MVRET, vfb_result, SITE_NUM);
    SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);//返回us
    SERIAL slope_result[SITE] = 1e3 * (vfb_result[SITE] - 0.15) / 4.2;//单位mv
    SERIAL slope_result[SITE] = slope_result[SITE] / (results[SITE] + 1e-9);//单位mv/us

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_us(20);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(1);
    //////////////////////////////TEST DATALOG/////////////////////////////////////////////////
    SERIAL	SLOPE_Pre->SetTestResult(SITE, 0, slope_result[SITE]);
    SERIAL	SLOPE_Pre_bit->SetTestResult(SITE, 0, slope.get_working(SITE));

    //=========================================================
   //====================>post value
   //=========================================================
    if (!QC && DO_TRIM)
    {
        slope.pre(slope_result);

        if (slope.updated_by_trim())
        {
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure slope_post------------------------------//
            Enter_TM4(1);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 3, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(2);

            QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_US, 10);
            FB_VI.MeasureVI(10, 10);
            groupgetresults(FB_VI, MVRET, vfb_result, SITE_NUM);
            SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);//返回us
            SERIAL slope_result[SITE] = 1e3 * (vfb_result[SITE] - 0.15) / 4.2;//单位mv
            SERIAL slope_result[SITE] = slope_result[SITE] / (results[SITE] + 1e-9);//单位mv/us

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(1);

        }

        slope.post(slope_result);
    }

    SERIAL	SLOPE_Post->SetTestResult(SITE, 0, slope_result[SITE]);
    SERIAL	SLOPE_Post_bit->SetTestResult(SITE, 0, slope.get_working(SITE));
    SERIAL	SLOPE_Target->SetTestResult(SITE, 0, slope.get_target(SITE));
    SERIAL	SLOPE_Guessed->SetTestResult(SITE, 0, slope.get_guessed_final(SITE));
    SERIAL	SLOPE_Updated->SetTestResult(SITE, 0, slope.updated_by_trim(SITE));

    //----------------------POWER DOWN--------------------------------------//
    dio.StopPattern();
    dio.Disconnect();
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST09_KLINE_TRIM(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *KLINE_Step0 = StsGetParam(funcindex,"KLINE_Step0");
    CParam *KLINE_Step1 = StsGetParam(funcindex,"KLINE_Step1");
    CParam *KLINE_Step2 = StsGetParam(funcindex,"KLINE_Step2");
    CParam *KLINE_Step3 = StsGetParam(funcindex,"KLINE_Step3");
    CParam *KLINE_Pre = StsGetParam(funcindex,"KLINE_Pre");
    CParam *KLINE_Pre_bit = StsGetParam(funcindex,"KLINE_Pre_bit");
    CParam *KLINE_Post = StsGetParam(funcindex,"KLINE_Post");
    CParam *KLINE_Post_bit = StsGetParam(funcindex,"KLINE_Post_bit");
    CParam *KLINE_Target = StsGetParam(funcindex,"KLINE_Target");
    CParam *KLINE_Guessed = StsGetParam(funcindex,"KLINE_Guessed");
    CParam *KLINE_Updated = StsGetParam(funcindex,"KLINE_Updated");
    //}}AFX_STS_PARAM_PROTOTYPES

   /*********Trim items define**********/
    TRIM_NODE& vbg = dut.trim("VBG");
    TRIM_NODE& ibias = dut.trim("IBIAS");
    TRIM_NODE& fsw = dut.trim("FSW");
    TRIM_NODE& slope = dut.trim("SLOPE");
    TRIM_NODE& kline = dut.trim("KLINE");
    /**********Optional trim items**********/
    SEL_NODE& bi_det_zcd = dut.sel("BI_DET_ZCD");//trim bit 15
    SEL_NODE& hv_en = dut.sel("HV_EN_FUN");//trim bit 16
    SEL_NODE& vbst_sel = dut.sel("VBST_SEL_FUN");//trim bit 17
    SEL_NODE& hl_qr = dut.sel("HL_QR_FUN");//trim bit 18
    SEL_NODE& cs_jt = dut.sel("CS_JT_FUN");//trim bit 19
    SEL_NODE& vbst_fx = dut.sel("VBST_Fx_FUN");//trim bit 20
    SEL_NODE& scp_uvp = dut.sel("SCP_UVP_FUN");//trim bit 21
    SEL_NODE& otp_l = dut.sel("OTP_L_FUN");//trim bit 22
    SEL_NODE& ovp = dut.sel("OVP_FUN");//trim bit 23
    SEL_NODE& pkl_en = dut.sel("PKL_EN_FUN");//trim bit 24
    //SEL_NODE& pk_time = dut.sel("PK_TIME_FUN");//trim bit 25/26---------bit24=1
    //SEL_NODE& pkl_protect = dut.sel("PKL_Protect_FUN");//trim bit 27----------bit24=1
    SEL_NODE& ocp_hfsw = dut.sel("OCP_HFsw_FUN");//trim bit 25----------bit24=0
    SEL_NODE& hfs = dut.sel("HFS_FUN");//trim bit 26----------bit24=0
    SEL_NODE& l_cslim = dut.sel("L_Cslim_FUN");//trim bit 27----------bit24=0
    // TODO: Add your function code here
    double  kline_result[SITE_NUM] = { 0,0,0,0 };
    double  i_cs_300[SITE_NUM] = { 0,0,0,0 };
    double  i_cs_400[SITE_NUM] = { 0,0,0,0 };
    double  results[SITE_NUM] = { 0,0,0,0 };


    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K10_DRV_PIN_Resource);
    delay_ms(3);

    /**************************************KLINE trim**************************************/
    //=================>Preview KLINE all step's data
    if (kline.table_char_active())//
    {
        kline.save_working();//保存默认状态
        for (int step = 0; step < 8; step++)//BG  2^3=8
        {
            SERIAL kline.set_working(step, SITE);//设置pre bit
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
            //delay_ms(3);
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure KLINE_pre------------------------------//
            Enter_TM1(1);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            ZCD_VI.Set(FI, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(3);

            FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
            CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_ms(2);//测试条件建立
            ZCD_VI.Set(FI, -300 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_ms(3);
            CS_VI.MeasureVI(40, 20);
            SERIAL i_cs_300[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
            ZCD_VI.Set(FI, -400 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_ms(3);
            CS_VI.MeasureVI(40, 20);
            SERIAL i_cs_400[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
            SERIAL kline_result[SITE] = 100 * 1e-6 / ((i_cs_400[SITE] - i_cs_300[SITE]) + 1e-12);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(1);

            kline.table_char(kline_result, step);
            switch (step)
            {
            case 0:SERIAL	KLINE_Step0->SetTestResult(SITE, 0, kline_result[SITE]);
            case 1:SERIAL	KLINE_Step1->SetTestResult(SITE, 0, kline_result[SITE]);
            case 2:SERIAL	KLINE_Step2->SetTestResult(SITE, 0, kline_result[SITE]);
            case 3:SERIAL	KLINE_Step3->SetTestResult(SITE, 0, kline_result[SITE]);
            }

        }
        kline.restore_working();//恢复默认状态
    }

    ////=========================================================
    ////====================>Pre_Test
    ////=========================================================
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    Enter_TM_Pre(1);//进入Pre模式
    RunPreviewCodes();//run pre bit pattern
    TM_RESET(1);//RESET TM
    ////---------Measure KLINE_pre----------------//
    Enter_TM1(1);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    ZCD_VI.Set(FI, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(3);

    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);//测试条件建立
    ZCD_VI.Set(FI, -300 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    CS_VI.MeasureVI(40, 20);
    SERIAL i_cs_300[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
    ZCD_VI.Set(FI, -400 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    CS_VI.MeasureVI(40, 20);
    SERIAL i_cs_400[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
    SERIAL kline_result[SITE] = 100 * 1e-6 / ((i_cs_400[SITE] - i_cs_300[SITE]) + 1e-12);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_us(20);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(3);

    //////////////////////////////TEST DATALOG/////////////////////////////////////////////////
    SERIAL	KLINE_Pre->SetTestResult(SITE, 0, kline_result[SITE]);
    SERIAL	KLINE_Pre_bit->SetTestResult(SITE, 0, kline.get_working(SITE));

    ////=========================================================
    ////====================>post value
    ////=========================================================
    if (!QC && DO_TRIM)
    {
        kline.pre(kline_result);

        if (kline.updated_by_trim())
        {
            HV_FPVI_Power(1, TRIM15);
            VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
            delay_ms(1);
            TM_LATCH(1);//进入测试模式
            Enter_TM_Pre(1);//进入Pre模式
            RunPreviewCodes();//run pre bit pattern
            TM_RESET(1);//RESET TM
            //------------------Measure KLINE_post------------------------------//
            Enter_TM1(1);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            ZCD_VI.Set(FI, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(3);

            FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
            CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_ms(2);//测试条件建立
            ZCD_VI.Set(FI, -300 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_ms(3);
            CS_VI.MeasureVI(40, 20);
            SERIAL i_cs_300[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
            ZCD_VI.Set(FI, -400 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_ms(3);
            CS_VI.MeasureVI(40, 20);
            SERIAL i_cs_400[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
            SERIAL kline_result[SITE] = 100 * 1e-6 / ((i_cs_400[SITE] - i_cs_300[SITE]) + 1e-12);

            CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            delay_us(20);
            CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
            VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
            HV_FPVI_Power_Down(1, TRIM15);
            ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
            FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
            delay_ms(1);

        }

        kline.post(kline_result);
    }

    SERIAL	KLINE_Post->SetTestResult(SITE, 0, kline_result[SITE]);
    SERIAL	KLINE_Post_bit->SetTestResult(SITE, 0, kline.get_working(SITE));
    SERIAL	KLINE_Target->SetTestResult(SITE, 0, kline.get_target(SITE));
    SERIAL	KLINE_Guessed->SetTestResult(SITE, 0, kline.get_guessed_final(SITE));
    SERIAL	KLINE_Updated->SetTestResult(SITE, 0, kline.updated_by_trim(SITE));

    //----------------------POWER DOWN--------------------------------------//
    dio.StopPattern();
    dio.Disconnect();
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST10_EE_CHECK(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *EE0_COMP = StsGetParam(funcindex,"EE0_COMP");
    CParam *EE1_COMP = StsGetParam(funcindex,"EE1_COMP");
    CParam *EE2_COMP = StsGetParam(funcindex,"EE2_COMP");
    CParam *EE3_COMP = StsGetParam(funcindex,"EE3_COMP");
    CParam *EE0_BURN = StsGetParam(funcindex,"EE0_BURN");
    CParam *EE1_BURN = StsGetParam(funcindex,"EE1_BURN");
    CParam *EE2_BURN = StsGetParam(funcindex,"EE2_BURN");
    CParam *EE3_BURN = StsGetParam(funcindex,"EE3_BURN");
    CParam *VBG_BURN = StsGetParam(funcindex,"VBG_BURN");
    CParam *IBIAS_BURN = StsGetParam(funcindex,"IBIAS_BURN");
    CParam *FSW_BURN = StsGetParam(funcindex,"FSW_BURN");
    CParam *SLOPE_BURN = StsGetParam(funcindex,"SLOPE_BURN");
    CParam *KLINE_BURN = StsGetParam(funcindex,"KLINE_BURN");
    CParam *BI_DET_BURN = StsGetParam(funcindex,"BI_DET_BURN");
    CParam *HV_EN_BURN = StsGetParam(funcindex,"HV_EN_BURN");
    CParam *VBST_SEL_BURN = StsGetParam(funcindex,"VBST_SEL_BURN");
    CParam *HL_QR_BURN = StsGetParam(funcindex,"HL_QR_BURN");
    CParam *CS_JT_BURN = StsGetParam(funcindex,"CS_JT_BURN");
    CParam *VBST_Fx_BURN = StsGetParam(funcindex,"VBST_Fx_BURN");
    CParam *SCP_UVP_BURN = StsGetParam(funcindex,"SCP_UVP_BURN");
    CParam *OTP_L_BURN = StsGetParam(funcindex,"OTP_L_BURN");
    CParam *OVP_L_BURN = StsGetParam(funcindex,"OVP_L_BURN");
    CParam *PKL_EN_BURN = StsGetParam(funcindex,"PKL_EN_BURN");
    CParam *HFSW_BURN = StsGetParam(funcindex,"HFSW_BURN");
    CParam *HFS_BURN = StsGetParam(funcindex,"HFS_BURN");
    CParam *CS_Limit_BURN = StsGetParam(funcindex,"CS_Limit_BURN");
    //}}AFX_STS_PARAM_PROTOTYPES

    /*********Trim items define**********/
    TRIM_NODE& vbg = dut.trim("VBG");
    TRIM_NODE& ibias = dut.trim("IBIAS");
    TRIM_NODE& fsw = dut.trim("FSW");
    TRIM_NODE& slope = dut.trim("SLOPE");
    TRIM_NODE& kline = dut.trim("KLINE");
    /**********Optional trim items**********/
    SEL_NODE& bi_det_zcd = dut.sel("BI_DET_ZCD");//trim bit 15
    SEL_NODE& hv_en = dut.sel("HV_EN_FUN");//trim bit 16
    SEL_NODE& vbst_sel = dut.sel("VBST_SEL_FUN");//trim bit 17
    SEL_NODE& hl_qr = dut.sel("HL_QR_FUN");//trim bit 18
    SEL_NODE& cs_jt = dut.sel("CS_JT_FUN");//trim bit 19
    SEL_NODE& vbst_fx = dut.sel("VBST_Fx_FUN");//trim bit 20
    SEL_NODE& scp_uvp = dut.sel("SCP_UVP_FUN");//trim bit 21
    SEL_NODE& otp_l = dut.sel("OTP_L_FUN");//trim bit 22
    SEL_NODE& ovp = dut.sel("OVP_FUN");//trim bit 23
    SEL_NODE& pkl_en = dut.sel("PKL_EN_FUN");//trim bit 24
    //SEL_NODE& pk_time = dut.sel("PK_TIME_FUN");//trim bit 25/26---------bit24=1
    //SEL_NODE& pkl_protect = dut.sel("PKL_Protect_FUN");//trim bit 27----------bit24=1
    SEL_NODE& ocp_hfsw = dut.sel("OCP_HFsw_FUN");//trim bit 25----------bit24=0
    SEL_NODE& hfs = dut.sel("HFS_FUN");//trim bit 26----------bit24=0
    SEL_NODE& l_cslim = dut.sel("L_Cslim_FUN");//trim bit 27----------bit24=0
    // TODO: Add your function code here
    int ee0_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee1_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee2_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee3_READ[SITE_NUM] = { -1,-1,-1,-1 };

    int dio_ch[4] = { 1, 3, 5, 7 };//DATA CHANNEL
    int EE_READ[4][SITE_NUM] = { 0 };

    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(2);

    //================ALL_BURN==========================

    if (QC || !DO_TRIM)      //we only read back the EEPROM data for QC and un-trimming units
    {
        return 0;
    }

    else
    {
        CS_VI.Set(FV, -0.5, FOVI_10V, FOVI_10MA, RELAY_ON);//Pull down CS firstly. Unlock the test mode.
        delay_ms(1);
        SERIAL
        {
            BEGIN_SINGLE_SITE(SITE)
                if (burn_flag[SITE])
                {

                    FB_VI.Set(FV, 5, FOVI_10V, FOVI_100MA, RELAY_ON);
                }
                else
                {
                    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
                }
                END_SINGLE_SITE()
        }
        delay_us(1000);
        //FB_VI.Set(FV, 5, FOVI_10V, FOVI_100MA, RELAY_ON);
        HV_FPVI_Power(1, TRIM15);
        VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
        //FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
        CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON, 1);//Capload to avoid CLK error
        delay_ms(1);

        TM_LATCH(1);//进入测试模式
        Enter_TM_TRIM(1);//进入trim模式
        LoadBurnCodes();

        FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
        VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON, 1.5);
        HV_FPVI_Power_Down(1, TRIM15);
        delay_ms(1);
        dio.StopPattern();
        dio.Disconnect();
        //===Power Down

        SERIAL
        {
            if (burn_flag[SITE])
            {
                dut.assy("EEPROM0").programmed(SITE);
                dut.assy("EEPROM1").programmed(SITE);
                dut.assy("EEPROM2").programmed(SITE);
                dut.assy("EEPROM3").programmed(SITE);

            }
        }

            //================ALL_READ==========================
        cbit.SetOn(-1);
        VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
        CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
        cbitclose(K16_DIO0_CS_CLK);//CLK
        cbitclose(K17_DIO1_ZCD_DATA);//DATA
        delay_ms(2);
        HV_FPVI_Power(1, TRIM15);
        VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
        delay_ms(1);
        TM_LATCH(1);//进入测试模式
        cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
        delay_ms(3);
        Enter_TM_Read(1);//进入标准读模式

        Enter_Read_Mode();//期望DATA为高，data有数据会被拉低-->输出为低

        SERIAL ee0_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 2, 9, LSB));
        SERIAL ee0_init_READ[SITE] = 255 - ee0_READ[SITE];//data拉低=寄存器里为1，所以输出值需要取反

        SERIAL ee1_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 10, 17, LSB));
        SERIAL ee1_init_READ[SITE] = 255 - ee1_READ[SITE];

        SERIAL ee2_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 18, 25, LSB));
        SERIAL ee2_init_READ[SITE] = 255 - ee2_READ[SITE];

        SERIAL ee3_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 26, 33, LSB));
        SERIAL ee3_init_READ[SITE] = 255 - ee3_READ[SITE];

        SERIAL dut.assy("EEPROM0").set_read_back((ee0_init_READ[SITE]), SITE);
        SERIAL dut.assy("EEPROM1").set_read_back((ee1_init_READ[SITE]), SITE);
        SERIAL dut.assy("EEPROM2").set_read_back((ee2_init_READ[SITE]), SITE);
        SERIAL dut.assy("EEPROM3").set_read_back((ee3_init_READ[SITE]), SITE);

        SERIAL EE0_COMP->SetTestResult(SITE, 0, dut.assy("EEPROM0").comp_prog_to_read(SITE));
        SERIAL EE1_COMP->SetTestResult(SITE, 0, dut.assy("EEPROM1").comp_prog_to_read(SITE));
        SERIAL EE2_COMP->SetTestResult(SITE, 0, dut.assy("EEPROM2").comp_prog_to_read(SITE));
        SERIAL EE3_COMP->SetTestResult(SITE, 0, dut.assy("EEPROM3").comp_prog_to_read(SITE));

        SERIAL EE0_BURN->SetTestResult(SITE, 0, ee0_init_READ[SITE]);
        SERIAL EE1_BURN->SetTestResult(SITE, 0, ee1_init_READ[SITE]);
        SERIAL EE2_BURN->SetTestResult(SITE, 0, ee2_init_READ[SITE]);
        SERIAL EE3_BURN->SetTestResult(SITE, 0, ee3_init_READ[SITE]);

        SERIAL VBG_BURN->SetTestResult(SITE, 0, vbg.get_read_back(SITE));
        SERIAL IBIAS_BURN->SetTestResult(SITE, 0, ibias.get_read_back(SITE));
        SERIAL FSW_BURN->SetTestResult(SITE, 0, fsw.get_read_back(SITE));
        SERIAL SLOPE_BURN->SetTestResult(SITE, 0, slope.get_read_back(SITE));
        SERIAL KLINE_BURN->SetTestResult(SITE, 0, kline.get_read_back(SITE));
        SERIAL BI_DET_BURN->SetTestResult(SITE, 0, bi_det_zcd.get_read_back(SITE));
        SERIAL HV_EN_BURN->SetTestResult(SITE, 0, hv_en.get_read_back(SITE));
        SERIAL VBST_SEL_BURN->SetTestResult(SITE, 0, vbst_sel.get_read_back(SITE));
        SERIAL HL_QR_BURN->SetTestResult(SITE, 0, hl_qr.get_read_back(SITE));
        SERIAL CS_JT_BURN->SetTestResult(SITE, 0, cs_jt.get_read_back(SITE));
        SERIAL VBST_Fx_BURN->SetTestResult(SITE, 0, vbst_fx.get_read_back(SITE));
        SERIAL SCP_UVP_BURN->SetTestResult(SITE, 0, scp_uvp.get_read_back(SITE));
        SERIAL OTP_L_BURN->SetTestResult(SITE, 0, otp_l.get_read_back(SITE));
        SERIAL OVP_L_BURN->SetTestResult(SITE, 0, ovp.get_read_back(SITE));
        SERIAL PKL_EN_BURN->SetTestResult(SITE, 0, pkl_en.get_read_back(SITE));
        SERIAL HFSW_BURN->SetTestResult(SITE, 0, ocp_hfsw.get_read_back(SITE));
        SERIAL HFS_BURN->SetTestResult(SITE, 0, hfs.get_read_back(SITE));
        SERIAL CS_Limit_BURN->SetTestResult(SITE, 0, l_cslim.get_read_back(SITE));


        //----------------------POWER DOWN--------------------------------------//
        dio.StopPattern();
        dio.Disconnect();
        HV_FPVI_Power_Down(1, TRIM15);
        ALL_FOVI_Power_Down();
        delay_ms(3);
        cbit.SetOn(-1);
        delay_ms(3);

    }

   
        

    return 0;
}


DUT_API int TEST11_OSC_FUNC(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *FSW_MAX = StsGetParam(funcindex,"FSW_MAX");
    CParam *FSW_MIN = StsGetParam(funcindex,"FSW_MIN");
    CParam *FSW_CCM = StsGetParam(funcindex,"FSW_CCM");
    CParam *FSWVDD_FSWCCM = StsGetParam(funcindex,"FSWVDD_FSWCCM");
    CParam *TOFF_MAX = StsGetParam(funcindex,"TOFF_MAX");
    CParam *TBOUT = StsGetParam(funcindex,"TBOUT");
    CParam *T_CLK = StsGetParam(funcindex,"T_CLK");
    //}}AFX_STS_PARAM_PROTOTYPES
    double  fswmax_result[SITE_NUM] = { 0,0,0,0 };
    double  fswmin_result[SITE_NUM] = { 0,0,0,0 };
    double  results[SITE_NUM] = { 0,0,0,0 };
    double  fswccm_9v[SITE_NUM] = { 0,0,0,0 };
    double  fswccm_24v[SITE_NUM] = { 0,0,0,0 };
    double  toffmax_result[SITE_NUM] = { 0,0,0,0 };
    double  tbout[SITE_NUM] = { 0,0,0,0 };
    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    delay_ms(3);

    //------------------FSW_MAX ------------------------------//
    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(4.0, QTMU_PLUS_NEG_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    //delay_ms(3);
    Enter_TM7(1);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.6, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.MeasFreq(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 20, 10);//cycle number=20，timeout=10ms
    SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL fswmax_result[SITE] = results[SITE];

    SERIAL  FSW_MAX->SetTestResult(SITE, 0, fswmax_result[SITE]);

    //------------------FSW_MIN ------------------------------//
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.MeasFreq(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 20, 10);//cycle number=20，timeout=10ms
    SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL fswmin_result[SITE] = results[SITE];

    SERIAL  FSW_MIN->SetTestResult(SITE, 0, fswmin_result[SITE]);

    //------------------FSW_CCM ------------------------------//
    SERIAL  FSW_CCM->SetTestResult(SITE, 0, FSWCCM[SITE]);

    //------------------FSWVDD_FSWCCM------------------------------//
    TM_RESET(1);//RESET TM
    Enter_TM8(1);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.6, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    VDD_VI.Set(FV, 9, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    QTMU_MEAS.MeasFreq(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 20, 10);//cycle number=20，timeout=10ms
    SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL fswccm_9v[SITE] = results[SITE];

    VDD_VI.Set(FV, 24, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    QTMU_MEAS.MeasFreq(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 20, 10);//cycle number=20，timeout=10ms

    SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL fswccm_24v[SITE] = results[SITE];
    SERIAL results[SITE] = fswccm_24v[SITE] - fswccm_9v[SITE];
    SERIAL results[SITE] = results[SITE] / (FSWCCM[SITE] + 1e-12);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(1);

    //----------------------TOFFMAX--------------------------------------//
    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(4.0, QTMU_PLUS_NEG_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetStopTrigger(7.0, QTMU_PLUS_POS_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0.5, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);//返回值是us
    SERIAL results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL toffmax_result[SITE] = results[SITE];

    SERIAL  TOFF_MAX->SetTestResult(SITE, 0, toffmax_result[SITE]);

    //----------------------TBOUT--------------------------------------//
    cbit.SetOn(-1);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    if (!TRIM15)
    {
        CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
        ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_1MA, RELAY_ON);
        FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
        VDD_VI.Set(FV, 20, FOVI_50V, FOVI_10MA, RELAY_ON);
        delay_ms(8);
    }
    else
    {
        CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
        ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
        FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
        VDD_VI.Set(FV, 20, FOVI_50V, FOVI_10MA, RELAY_ON);
        delay_ms(8);
    }

    Meas_TBOUT(TBOUT_TIME, TRIM15);
    SERIAL  TBOUT->SetTestResult(SITE, 0, TBOUT_TIME[SITE]);

    //----------------------T_CLK--------------------------------------//
    SERIAL  TCLK[SITE] = 1e3 * tbout[SITE] / 1920;//us
    SERIAL  T_CLK->SetTestResult(SITE, 0, TCLK[SITE]);

    //----------------------POWER DOWN--------------------------------------//
    dio.StopPattern();
    dio.Disconnect();
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);


    return 0;
}


DUT_API int TEST12_VDD_FUNC(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VDD_ON = StsGetParam(funcindex,"VDD_ON");
    CParam *VDD_OFF = StsGetParam(funcindex,"VDD_OFF");
    CParam *VDD_HOLDL = StsGetParam(funcindex,"VDD_HOLDL");
    CParam *VDD_HOLDH = StsGetParam(funcindex,"VDD_HOLDH");
    CParam *I_ST = StsGetParam(funcindex,"I_ST");
    CParam *IVDD = StsGetParam(funcindex,"IVDD");
    CParam *IVDD_BT = StsGetParam(funcindex,"IVDD_BT");
    CParam *VDD_OVP = StsGetParam(funcindex,"VDD_OVP");
    CParam *IVDD_OVP = StsGetParam(funcindex,"IVDD_OVP");
    CParam *TVDD_OVP = StsGetParam(funcindex,"TVDD_OVP");
    CParam *IVDD_FAULT = StsGetParam(funcindex,"IVDD_FAULT");
    CParam *TD_FAULT = StsGetParam(funcindex,"TD_FAULT");
    CParam *IVDD_FAULT1 = StsGetParam(funcindex,"IVDD_FAULT1");
    CParam *V_VDDLO = StsGetParam(funcindex,"V_VDDLO");
    CParam *IVDD_LATCH = StsGetParam(funcindex,"IVDD_LATCH");
    //}}AFX_STS_PARAM_PROTOTYPES
    double VDDON_result[SITE_NUM] = { 0.0 };
    double VDDOFF_result[SITE_NUM] = { 0.0 };
    double VDDHOLDL_result[SITE_NUM] = { 0.0 };
    double VDDHOLDH_result[SITE_NUM] = { 0.0 };
    double VDDOVP_result[SITE_NUM] = { 0.0 };
    double IVDDOVP_result[SITE_NUM] = { 0.0 };
    double ForceV[SITE_NUM] = { 0.0 };
    double OffV[SITE_NUM] = { 0.0 };
    double IST_result[SITE_NUM] = { 0.0 };
    double IVDD_result[SITE_NUM] = { 0.0 };
    double IVDDBT_result[SITE_NUM] = { 0.0 };
    double IVDDFT_result[SITE_NUM] = { 0.0 };
    double VDDLO_result[SITE_NUM] = { 0.0 };
    double IVDDLO_result[SITE_NUM] = { 0.0 };
    double results[SITE_NUM] = { 0.0 };
    // TODO: Add your function code here
    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);

    //====================VDD_ON=================================//
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 14, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 1.2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    Meas_VDDTH(VDD_VI, FOVI_50V, FOVI_10MA, 14, 18, 0.02, 20, 100 * 1E-6, TRIG_RISING, VDDON_result);
    SERIAL VDD_ON->SetTestResult(SITE, 0, VDDON_result[SITE]);
    SERIAL
    {
        if (VDDON_result[SITE] != -999)
            ForceV[SITE] = VDDON_result[SITE] - 1;
            UVLOH[SITE] = VDDON_result[SITE];
    }

        //====================VDD_OFF=================================//
    Meas_VDDTH(VDD_VI, FOVI_50V, FOVI_10MA, 10, 7, 0.02, 20, 150 * 1E-6, TRIG_FALLING, VDDOFF_result);
    SERIAL VDD_OFF->SetTestResult(SITE, 0, VDDOFF_result[SITE]);
    SERIAL OffV[SITE] = 7.6 + 0.15;
    SERIAL
    {
        if (VDDOFF_result[SITE] != -999)
            OffV[SITE] = VDDOFF_result[SITE] + 0.15;
    }
    HV_FPVI_Power_Down(1, TRIM15);
    //====================I_SartUp=================================//
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    VDD_VI.SetSyn(FV, ForceV, SITE_NUM, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    VDD_VI.SetSyn(FV, ForceV, SITE_NUM, FOVI_50V, FOVI_10UA, RELAY_ON);
    delay_ms(1);
    VDD_VI.MeasureVI(30, 10);
    SERIAL IST_result[SITE] = 1e6 * VDD_VI.GetMeasResult(SITE, MIRET);//UA
    SERIAL I_ST->SetTestResult(SITE, 0, IST_result[SITE]);
    VDD_VI.SetSyn(FV, ForceV, SITE_NUM, FOVI_50V, FOVI_100MA, RELAY_ON);
    //HV_FPVI_Power_Down(1, TRIM15);
    //====================I_VDD=================================//
    //HV_FPVI_Power(1, TRIM15);
    CS_VI.Set(FV, 0.4, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    VDD_VI.SetSyn(FV, ForceV, SITE_NUM, FOVI_50V, FOVI_10MA, RELAY_ON);
    delay_ms(1);
    VDD_VI.MeasureVI(30, 10);
    SERIAL IVDD_result[SITE] = 1e3 * VDD_VI.GetMeasResult(SITE, MIRET);//MA
    SERIAL IVDD->SetTestResult(SITE, 0, IVDD_result[SITE]);
    VDD_VI.SetSyn(FV, ForceV, SITE_NUM, FOVI_50V, FOVI_100MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    //=================I_BURST================================//
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(2);
    HV_FPVI_Power(1, TRIM15);
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0.4, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    VDD_VI.Set(FV, 25, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(2);
    VDD_VI.Set(FV, 15, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    VDD_VI.Set(FV, 15, FOVI_50V, FOVI_1MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(5);
    VDD_VI.MeasureVI(30, 10);
    SERIAL IVDDBT_result[SITE] = 1e6 * VDD_VI.GetMeasResult(SITE, MIRET);//UA
    SERIAL IVDD_BT->SetTestResult(SITE, 0, IVDDBT_result[SITE]);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_1MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    delay_ms(2);
    //=================VIN_HOLDONL================================//
    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    delay_ms(3);
    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(4.0, QTMU_PLUS_NEG_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    //QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(2);
    HV_FPVI_Power(1, TRIM15);
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0.4, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_100MA, RELAY_ON);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(5);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);

    Meas_VDDHOLD(VDD_VI, 9.5, 7.8, OffV, 0.03, VDDHOLDL_result, 50, 1, 0);
    SERIAL VDD_HOLDL->SetTestResult(SITE, 0, VDDHOLDL_result[SITE]);
    //=================VIN_HOLDONH================================//
    Meas_VDDHOLD(VDD_VI, 7.85, 9.5, OffV, 0.03, VDDHOLDH_result, 50, 1, 0);
    SERIAL VDD_HOLDH->SetTestResult(SITE, 0, VDDHOLDH_result[SITE]);

    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_10MA, RELAY_ON);
    HV_FPVI_Power_Down(1, TRIM15);
    delay_ms(2);
    cbitopen(K24_DRV_BUF_IN);
    cbitopen(K25_QTMUA_BUF_OUT);
    cbitopen(K10_DRV_PIN_Resource);
    delay_ms(3);
    //=================VDD_OVP================================//
    //=================IVDD_OVP================================//
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(2);

    HV_FPVI_Power_Down(1, TRIM15);
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(2);
    VDD_VI.Set(FV, 25, FOVI_50V, FOVI_10MA, RELAY_ON);
    delay_ms(5);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(2);

    Meas_VDDOVP(VDD_VI, FOVI_50V, FOVI_10MA, 25, 30, 0.05, 10, 1E-3, TRIG_RISING, VDDOVP_result, IVDDOVP_result);
    SERIAL VDD_OVP->SetTestResult(SITE, 0, VDDOVP_result[SITE]);
    SERIAL IVDD_OVP->SetTestResult(SITE, 0, IVDDOVP_result[SITE]);
    //=================IVDD_FAULT================================//
    VDD_VI.Set(FV, 22, FOVI_50V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 22, FOVI_50V, FOVI_1MA, RELAY_ON);
    delay_ms(1);//For I_Fault test
    VDD_VI.MeasureVI(30, 10);
    groupgetresults(VDD_VI, MIRET, IVDDFT_result, SITE_NUM);
    SERIAL IVDDFT_result[SITE] = 1e6 * IVDDFT_result[SITE];//Covert to uA
    SERIAL IVDD_FAULT->SetTestResult(SITE, 0, IVDDFT_result[SITE]);
    //=================VDD_LO================================//BIT23=1，则进行测试
    //=================IVDD_LATCH================================//
    if (TRIM23)
    {
        VDD_VI.Set(FV, 22, FOVI_50V, FOVI_1MA, RELAY_ON);
        VDD_VI.Set(FV, 22, FOVI_50V, FOVI_100UA, RELAY_ON);
        delay_ms(2);

        Meas_VDDLO(VDD_VI, FOVI_50V, FOVI_100UA, 6, 3, 0.02, 20, 10 * 1E-6, TRIG_FALLING, VDDLO_result, IVDDLO_result);
        SERIAL V_VDDLO->SetTestResult(SITE, 0, VDDLO_result[SITE]);
        SERIAL IVDD_LATCH->SetTestResult(SITE, 0, IVDDLO_result[SITE]);//要等IVDD稳定后再测，需要调试
        VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100UA, RELAY_ON);
    }
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_1MA, RELAY_ON);
    //=================POWER DOWN================================//
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);
    //=================TD_FAULT================================//
    SERIAL results[SITE] = 24576 * TCLK[SITE];
    SERIAL TD_FAULT->SetTestResult(SITE, 0, IVDDLO_result[SITE]);
    //=================IVDD_FAULT1================================//
    SERIAL IVDD_FAULT1->SetTestResult(SITE, 0, IVDDOVP_result[SITE]);
    //=================TVDD_OVP================================//
    SERIAL results[SITE] = 4 * TCLK[SITE];
    SERIAL TVDD_OVP->SetTestResult(SITE, 0, results[SITE]);

    return 0;
}


DUT_API int TEST13_ZCD_FUNC(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VZCD_OVP = StsGetParam(funcindex,"VZCD_OVP");
    CParam *VZCD_UVP = StsGetParam(funcindex,"VZCD_UVP");
    CParam *IZCD_MAX = StsGetParam(funcindex,"IZCD_MAX");
    CParam *VZCD_CLAMP = StsGetParam(funcindex,"VZCD_CLAMP");
    CParam *TLEB1_OVP = StsGetParam(funcindex,"TLEB1_OVP");
    CParam *TLEB2_OVP = StsGetParam(funcindex,"TLEB2_OVP");
    CParam *VZCD_H = StsGetParam(funcindex,"VZCD_H");
    CParam *VZCD_L = StsGetParam(funcindex,"VZCD_L");
    CParam *VZCD_Caul = StsGetParam(funcindex,"VZCD_Caul");
    CParam *IZCD_BI = StsGetParam(funcindex,"IZCD_BI");
    CParam *IZCD_BO = StsGetParam(funcindex,"IZCD_BO");
    CParam *IZCD_HIGHLINE = StsGetParam(funcindex,"IZCD_HIGHLINE");
    CParam *IZCD_LINEOVP = StsGetParam(funcindex,"IZCD_LINEOVP");
    CParam *TZCD_LINEOVP = StsGetParam(funcindex,"TZCD_LINEOVP");
    CParam *ILINECOMPST = StsGetParam(funcindex,"ILINECOMPST");
    CParam *KLINECOMP = StsGetParam(funcindex,"KLINECOMP");
    CParam *CS_OTP_EX = StsGetParam(funcindex,"CS_OTP_EX");
    CParam *TCS_OTP = StsGetParam(funcindex,"TCS_OTP");
    //}}AFX_STS_PARAM_PROTOTYPES
    double VZCDOVP_result[SITE_NUM] = { 0.0 };
    double VZCDUVP_result[SITE_NUM] = { 0.0 };
    double IZCDMAX_result[SITE_NUM] = { 0.0 };
    double VZCDCLAMP_result[SITE_NUM] = { 0.0 };
    double VZCDH_result[SITE_NUM] = { 0.0 };
    double VZCDL_result[SITE_NUM] = { 0.0 };
    double TLEB1OVP_result[SITE_NUM] = { 0.0 };
    double TLEB2OVP_result[SITE_NUM] = { 0.0 };
    double VZCDCAUL_result[SITE_NUM] = { 0.0 };
    double IZCDBI_result[SITE_NUM] = { 0.0 };
    double IZCDBO_result[SITE_NUM] = { 0.0 };
    double IZCDHL_result[SITE_NUM] = { 0.0 };
    double IZCDLOVP_result[SITE_NUM] = { 0.0 };
    double TZCDLOVP_result[SITE_NUM] = { 0.0 };
    double ILCMOPST_result[SITE_NUM] = { 0.0 };
    double KLCOMP_result[SITE_NUM] = { 0.0 };
    double CSOTPEX_result[SITE_NUM] = { 0.0 };
    double TCSOTP_result[SITE_NUM] = { 0.0 };
    double i_cs_300[SITE_NUM] = { 0,0,0,0 };
    double i_cs_400[SITE_NUM] = { 0,0,0,0 };
    // TODO: Add your function code here
    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    //=================VZCD_OVP================================//
    HV_FPVI_Power_Down(1, TRIM15);
    cbitclose(K19_VDD_2P2UF);//常连，直到测完VZCDCLAMP
    delay_ms(3);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_1MA, RELAY_ON);
    FB_VI.SetClamp(50, 50);
    delay_ms(3);
    ZCD_VI.Set(FV, 3, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 3, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    Meas_VZCD(ZCD_VI, FB_VI, FOVI_10V, FOVI_10MA, 3, 5, 0.01, 10, 2.5, TRIG_RISING, VZCDOVP_result);
    ZCD_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    SERIAL  VZCD_OVP->SetTestResult(SITE, 0, VZCDOVP_result[SITE]);
    //=================VZCD_UVP================================//
    ZCD_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    Meas_VZCD(ZCD_VI, FB_VI, FOVI_10V, FOVI_10MA, 2, 0.5, 0.01, 10, 0.5, TRIG_FALLING, VZCDUVP_result);
    ZCD_VI.Set(FV, 0.5, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    SERIAL  VZCD_UVP->SetTestResult(SITE, 0, VZCDUVP_result[SITE]);
    //=================IZCD_MAX================================//
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    //HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    //delay_ms(3);
    Enter_TM7(1);//随便进入一个测试模式
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);//可以加到-0.3V
    FB_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    ZCD_VI.MeasureVI(20, 10);
    groupgetresults(ZCD_VI, MIRET, IZCDMAX_result, SITE_NUM);
    SERIAL  IZCDMAX_result[SITE] = 1e3 * IZCDMAX_result[SITE];//ma
    SERIAL  IZCD_MAX->SetTestResult(SITE, 0, IZCDMAX_result[SITE]);
    //=================VZCD_CLAMP================================//
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_1MA, RELAY_ON);//可以加到-0.3V
    delay_ms(2);
    ZCD_VI.MeasureVI(20, 10);
    groupgetresults(ZCD_VI, MVRET, VZCDCLAMP_result, SITE_NUM);
    SERIAL  VZCDCLAMP_result[SITE] = 1e3 * VZCDCLAMP_result[SITE];//mV
    SERIAL  VZCD_CLAMP->SetTestResult(SITE, 0, VZCDCLAMP_result[SITE]);
    cbitopen(K19_VDD_2P2UF);
    delay_ms(3);
    //=================TLEB1_OVP================================//
    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(4.0, QTMU_PLUS_NEG_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetStopTrigger(7.0, QTMU_PLUS_POS_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_RESET(1);//RESET TM
    Enter_TM4(1);
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FI, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FI, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);//返回值是us
    SERIAL TLEB1OVP_result[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL  TLEB1_OVP->SetTestResult(SITE, 0, TLEB1OVP_result[SITE]);
    //=================TLEB2_OVP================================//
    FB_VI.Set(FV, 2.5, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);//返回值是us
    SERIAL TLEB2OVP_result[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL  TLEB2_OVP->SetTestResult(SITE, 0, TLEB2OVP_result[SITE]);
    cbitopen(K24_DRV_BUF_IN);
    cbitopen(K25_QTMUA_BUF_OUT);
    cbitopen(K10_DRV_PIN_Resource);
    delay_ms(3);
    //=================VZCD_H================================//
    cbitclose(K23_DRV_1NF);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_RESET(1);//RESET TM
    Enter_TM6(1);
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    Meas_VZCD(ZCD_VI, FB_VI, FOVI_10V, FOVI_10MA, 0, 0.5, 0.001, 10, 0.5, TRIG_RISING, VZCDH_result);
    ZCD_VI.Set(FV, 0.5, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitopen(K23_DRV_1NF);
    delay_ms(3);
    SERIAL  VZCD_H->SetTestResult(SITE, 0, VZCDH_result[SITE]);
    //=================VZCD_L================================//
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_RESET(1);//RESET TM
    Enter_TM5(1);
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    Meas_VZCD(ZCD_VI, FB_VI, FOVI_10V, FOVI_10MA, 0.1, 0.3, 0.001, 10, 0.5, TRIG_FALLING, VZCDL_result);
    ZCD_VI.Set(FV, 0.3, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    SERIAL  VZCD_L->SetTestResult(SITE, 0, VZCDL_result[SITE]);
    //=================VZCD_Caul================================//
    Meas_VZCD(ZCD_VI, FB_VI, FOVI_10V, FOVI_10MA, 0.3, -0.1, 0.001, 10, 0.5, TRIG_FALLING, VZCDCAUL_result);
    ZCD_VI.Set(FV, -0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    SERIAL  VZCD_Caul->SetTestResult(SITE, 0, VZCDCAUL_result[SITE]);
    //=================IZCD_BI================================//
    cbitclose(K23_DRV_1NF);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_RESET(1);//RESET TM
    Enter_TM1(1);
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FI, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FI, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FI, -60 * 1e-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    Meas_IZCD(ZCD_VI, FOVI_10V, FOVI_1MA, FB_VI, -60 * 1e-6, -120 * 1e-6, 1e-6, 200, 0.1, TRIG_RISING, IZCDBI_result);
    SERIAL IZCD_BI->SetTestResult(SITE, 0, IZCDBI_result[SITE]);
    //=================IZCD_BO================================//
    ZCD_VI.Set(FI, -120 * 1e-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    Meas_IZCD(ZCD_VI, FOVI_10V, FOVI_1MA, FB_VI, -120 * 1e-6, -60 * 1e-6, 1e-6, 150, 2, TRIG_FALLING, IZCDBO_result);
    SERIAL IZCD_BO->SetTestResult(SITE, 0, IZCDBO_result[SITE]);
    //=================IZCD_HIGHLINE================================//
    ZCD_VI.Set(FI, -200 * 1e-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    Meas_IZCD(ZCD_VI, FOVI_10V, FOVI_1MA, FB_VI, -200 * 1e-6, -300 * 1e-6, 1e-6, 150, 2, TRIG_FALLING, IZCDHL_result);
    SERIAL IZCD_HIGHLINE->SetTestResult(SITE, 0, IZCDHL_result[SITE]);
    //=================IZCD_LINEOVP================================//
    ZCD_VI.Set(FI, -300 * 1e-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    Meas_IZCD(ZCD_VI, FOVI_10V, FOVI_1MA, FB_VI, -300 * 1e-6, -400 * 1e-6, 1e-6, 150, 2, TRIG_FALLING, IZCDLOVP_result);
    SERIAL IZCD_LINEOVP->SetTestResult(SITE, 0, IZCDLOVP_result[SITE]);
    //=================TZCD_LINEOVP================================//
    SERIAL TZCDLOVP_result[SITE] = 9216 * TCLK[SITE];
    SERIAL TZCD_LINEOVP->SetTestResult(SITE, 0, TZCDLOVP_result[SITE]);
    //=================ILINECOMPST================================//
    ZCD_VI.Set(FI, -100 * 1e-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    Meas_Ilinecst(ZCD_VI, FOVI_10V, FOVI_1MA, CS_VI, -100 * 1e-6, -200 * 1e-6, 1e-6, 150, 2, TRIG_FALLING, ILCMOPST_result);//需调试，trig level指CS两点之间的电流间隔
    SERIAL ILINECOMPST->SetTestResult(SITE, 0, ILCMOPST_result[SITE]);
    //=================KLINECOMP================================//
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);//测试条件建立
    ZCD_VI.Set(FI, -300 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    CS_VI.MeasureVI(40, 20);
    SERIAL i_cs_300[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
    ZCD_VI.Set(FI, -400 * 1E-6, FOVI_10V, FOVI_1MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(3);
    CS_VI.MeasureVI(40, 20);
    SERIAL i_cs_400[SITE] = CS_VI.GetMeasResult(SITE, MIRET);
    SERIAL KLCOMP_result[SITE] = 100 * 1e-6 / ((i_cs_400[SITE] - i_cs_300[SITE]) + 1e-12);
    SERIAL KLINECOMP->SetTestResult(SITE, 0, KLCOMP_result[SITE]);
    cbitopen(K23_DRV_1NF);
    //=================CS_OTP_EX================================//
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_RESET(1);//RESET TM
    Enter_TM7(1);
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FI, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    ZCD_VI.Set(FI, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.6, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    Meas_Icsotp(ZCD_VI, FOVI_10V, FOVI_1MA, CS_VI, 0, 1, 0.001, 10, -2, TRIG_FALLING, CSOTPEX_result);//需调试，trig level指CS两点之间的电流间隔
    SERIAL CS_OTP_EX->SetTestResult(SITE, 0, CSOTPEX_result[SITE]);
    //=================TCS_OTP================================//
    SERIAL TCSOTP_result[SITE] = 6656 * TCLK[SITE];
    SERIAL TCS_OTP->SetTestResult(SITE, 0, TCSOTP_result[SITE]);

    //=================POWER DOWN================================//
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST14_FB_FUNC(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VFB_OPEN = StsGetParam(funcindex,"VFB_OPEN");
    CParam *RFB = StsGetParam(funcindex,"RFB");
    CParam *VFB_BSTH = StsGetParam(funcindex,"VFB_BSTH");
    CParam *VFB_BSTHYS = StsGetParam(funcindex,"VFB_BSTHYS");
    CParam *VFB_SWMAX = StsGetParam(funcindex,"VFB_SWMAX");
    CParam *PKL_TRIG_Vth = StsGetParam(funcindex,"PKL_TRIG_Vth");
    //}}AFX_STS_PARAM_PROTOTYPES
    double VFBOPEN_result[SITE_NUM] = { 0.0 };
    double RFB_result[SITE_NUM] = { 0.0 };
    double VFBBSTH_result[SITE_NUM] = { 0.0 };
    double VFBBSTHYS_result[SITE_NUM] = { 0.0 };
    double VFBSWMAX_result[SITE_NUM] = { 0.0 };
    double PKLTRIGVTH_result[SITE_NUM] = { 0.0 };
    // TODO: Add your function code here
    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    //=================VFB_OPEN================================//
    HV_FPVI_Power_Down(1, TRIM15);
    cbitclose(K19_VDD_2P2UF);//常连，直到测完RFB
    cbitclose(K20_FB_1NF);
    delay_ms(3);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_100MA, RELAY_ON);
    if (TRIM24)
    {
        FB_VI.Set(FI, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
        FB_VI.Set(FI, 0, FOVI_50V, FOVI_10UA, RELAY_ON);
    }
    else
    {
        FB_VI.Set(FI, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
        FB_VI.Set(FI, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    }
    delay_ms(3);
    FB_VI.MeasureVI(10, 20);
    SERIAL VFBOPEN_result[SITE] = FB_VI.GetMeasResult(SITE, MVRET);
    SERIAL VFB_OPEN->SetTestResult(SITE, 0, VFBOPEN_result[SITE]);
    //=================RFB================================//
    FB_VI.Set(FI, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);
    FB_VI.MeasureVI(10, 20);
    SERIAL RFB_result[SITE] = FB_VI.GetMeasResult(SITE, MIRET);
    SERIAL RFB_result[SITE] = 1E-3 * VFBOPEN_result[SITE] / (RFB_result[SITE] + 1E-12);//KOhm
    SERIAL RFB->SetTestResult(SITE, 0, RFB_result[SITE]);
    cbitopen(K20_FB_1NF);
    cbitopen(K19_VDD_2P2UF);
    //=================VFB_BSTH================================//
    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    delay_ms(3);
    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(4.0, QTMU_PLUS_NEG_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    //QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 1, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(2);

    Meas_VFBBST(FB_VI, 1, 0, 0.03, VFBBSTH_result, 50, 1, 0);
    SERIAL VFB_BSTH->SetTestResult(SITE, 0, VFBBSTH_result[SITE]);
    //=================VFB_BSTHYS================================//
    Meas_VFBBST(FB_VI, 0, 1, 0.03, VFBBSTHYS_result, 50, 1, 0);
    SERIAL VFB_BSTHYS->SetTestResult(SITE, 0, VFBBSTHYS_result[SITE]);
    //=================VFB_SWMAX================================//
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    //HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    //delay_ms(3);
    Enter_TM3(1);//随便进入一个测试模式
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.3, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    Meas_VFBSWMAX(FB_VI, 0, 1.5, 0.03, VFBSWMAX_result, 50, 1, 0);
    FB_VI.Set(FV, 1.5, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    cbitopen(K24_DRV_BUF_IN);
    cbitopen(K25_QTMUA_BUF_OUT);
    cbitopen(K10_DRV_PIN_Resource);
    delay_ms(3);
    //=================PKL_TRIG_Vth================================//
    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    if (TRIM24)
    {
        FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
        FB_VI.Set(FV, 2, FOVI_10V, FOVI_1MA, RELAY_ON);
        delay_ms(2);
        Meas_pkltrig(FB_VI, FOVI_10V, FOVI_1MA, 2, 4, 0.01, 10, 50E-6, TRIG_FALLING, PKLTRIGVTH_result);
    }
    else
    {
        FB_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
        FB_VI.Set(FV, 1, FOVI_10V, FOVI_1MA, RELAY_ON);
        delay_ms(2);
        Meas_pkltrig(FB_VI, FOVI_10V, FOVI_1MA, 1, 2, 0.01, 10, 50E-6, TRIG_FALLING, PKLTRIGVTH_result);
    }
    SERIAL PKL_TRIG_Vth->SetTestResult(SITE, 0, PKLTRIGVTH_result[SITE]);

    //=================POWER DOWN================================//
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST15_CS_FUNC(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VCS_OLP = StsGetParam(funcindex,"VCS_OLP");
    CParam *VCS_MAX = StsGetParam(funcindex,"VCS_MAX");
    CParam *VCS_SSCP = StsGetParam(funcindex,"VCS_SSCP");
    CParam *TLEB_CBC = StsGetParam(funcindex,"TLEB_CBC");
    CParam *TLEB_SSCP = StsGetParam(funcindex,"TLEB_SSCP");
    CParam *VCS_SLOPE2 = StsGetParam(funcindex,"VCS_SLOPE2");
    CParam *VCS_SLOPE1 = StsGetParam(funcindex,"VCS_SLOPE1");
    CParam *TSSCS = StsGetParam(funcindex,"TSSCS");
    CParam *TCS_OLP2 = StsGetParam(funcindex,"TCS_OLP2");
    //}}AFX_STS_PARAM_PROTOTYPES
    double VCSOLP_result[SITE_NUM] = { 0.0 };
    double VCSMAX_result[SITE_NUM] = { 0.0 };
    double VCSSSCP_result[SITE_NUM] = { 0.0 };
    double TLEBCBC_result[SITE_NUM] = { 0.0 };
    double TLEBSSCP_result[SITE_NUM] = { 0.0 };
    double SLOPE1_result[SITE_NUM] = { 0.0 };
    double SLOPE2_result[SITE_NUM] = { 0.0 };
    double T_result[SITE_NUM] = { 0.0 };
    double V_result[SITE_NUM] = { 0.0 };
    double TSSCS_result[SITE_NUM] = { 0.0 };
    double TCSOLP2_result[SITE_NUM] = { 0.0 };
    // TODO: Add your function code here
    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    //=================TLEB_CBC================================//
    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(4.0, QTMU_PLUS_POS_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetStopTrigger(7.0, QTMU_PLUS_NEG_SLOPE);//trigger=2.5V, falling edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0.6, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);//返回值是us
    SERIAL TLEBCBC_result[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL TLEBCBC_result[SITE] = 1e3 * TLEBCBC_result[SITE];//转换为ns
    SERIAL TLEB_CBC->SetTestResult(SITE, 0, TLEBCBC_result[SITE]);
    //=================TLEB_SSCP================================//
    CS_VI.Set(FV, 1.3, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);//返回值是us
    SERIAL TLEBSSCP_result[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL TLEBSSCP_result[SITE] = 1e3 * TLEBSSCP_result[SITE];//转换为ns
    SERIAL TLEB_SSCP->SetTestResult(SITE, 0, TLEBSSCP_result[SITE]);
    /*cbitopen(K24_DRV_BUF_IN);
    cbitopen(K25_QTMUA_BUF_OUT);
    cbitopen(K10_DRV_PIN_Resource);
    delay_ms(3);*/
    //=================VCS_SLOPE2================================//
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    //HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    //delay_ms(3);
    Enter_TM4(1);
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 3, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);//返回值是us
    FB_VI.MeasureVI(10, 20);
    SERIAL T_result[SITE] = QTMU_MEAS.GetMeasureResult(SITE);
    SERIAL V_result[SITE] = FB_VI.GetMeasResult(SITE, MVRET);
    SERIAL V_result[SITE] = 1e3 * (V_result[SITE] - 0.15) / 4.2;//MV
    SERIAL SLOPE2_result[SITE] = V_result[SITE] / (T_result[SITE] + 1e-12);//转换为mv/us
    SERIAL VCS_SLOPE2->SetTestResult(SITE, 0, SLOPE2_result[SITE]);
    //=================VCS_SLOPE1================================//
    SERIAL SLOPE1_result[SITE] = SLOPE2_result[SITE] - 2 * (6 / 21);//转换为mv/us
    SERIAL VCS_SLOPE1->SetTestResult(SITE, 0, SLOPE1_result[SITE]);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    //=================VCS_OLP================================//
    if (!TRIM24 & TRIM27)
    {
        cbitclose(K16_DIO0_CS_CLK);//CLK
        cbitclose(K17_DIO1_ZCD_DATA);//DATA
        delay_ms(3);
        VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
        delay_ms(1);
        TM_RESET(1);//RESET TM
        Enter_TM3(1);
        cbitopen(K16_DIO0_CS_CLK);//CLK
        cbitopen(K17_DIO1_ZCD_DATA);//DATA
        delay_ms(3);

        CS_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
        ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
        FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
        delay_ms(2);

        CS_VI.Set(FV, 0.1, FOVI_2V, FOVI_10MA, RELAY_ON);
        ZCD_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
        FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
        FB_VI.Set(FV, 2, FOVI_10V, FOVI_1MA, RELAY_ON);
        FB_VI.SetClamp(50, 50);
        delay_ms(3);
        Meas_VCS(CS_VI, FB_VI, FOVI_2V, FOVI_10MA, 0.1, 0.6, 0.01, 10, 1, TRIG_RISING, VCSOLP_result);
        SERIAL VCS_OLP->SetTestResult(SITE, 0, VCSOLP_result[SITE]);
        //=================VCS_MAX================================//
        CS_VI.Set(FV, 0.1, FOVI_2V, FOVI_10MA, RELAY_ON);
        delay_ms(2);
        Meas_VCS(CS_VI, FB_VI, FOVI_2V, FOVI_10MA, 0.1, 0.6, 0.01, 10, 2, TRIG_RISING, VCSMAX_result);
        SERIAL VCS_MAX->SetTestResult(SITE, 0, VCSMAX_result[SITE]);
        //=================VCS_SSCP================================//
        CS_VI.Set(FV, 0.6, FOVI_2V, FOVI_10MA, RELAY_ON);
        delay_ms(2);
        Meas_VCS(CS_VI, FB_VI, FOVI_2V, FOVI_10MA, 0.6, 1.3, 0.01, 10, 2, TRIG_FALLING, VCSSSCP_result);
        SERIAL VCS_SSCP->SetTestResult(SITE, 0, VCSSSCP_result[SITE]);

        CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
        ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
        FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
        delay_ms(2);
    }
    //=================TSSCS================================//
    SERIAL TSSCS_result[SITE] = 64 * 3 * TCLK[SITE];
    SERIAL TSSCS->SetTestResult(SITE, 0, TSSCS_result[SITE]);
    //=================TCS_OLP2================================//
    SERIAL TCSOLP2_result[SITE] = 2048 * TCLK[SITE];
    SERIAL TCS_OLP2->SetTestResult(SITE, 0, TCSOLP2_result[SITE]);

    //=================POWER DOWN================================//
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);


    return 0;
}


DUT_API int TEST16_DRV_FUNC(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VOL = StsGetParam(funcindex,"VOL");
    CParam *IDRV_SOURCE = StsGetParam(funcindex,"IDRV_SOURCE");
    CParam *VOH = StsGetParam(funcindex,"VOH");
    CParam *IDRV_SINK = StsGetParam(funcindex,"IDRV_SINK");
    CParam *V_CLAMP = StsGetParam(funcindex,"V_CLAMP");
    CParam *TR = StsGetParam(funcindex,"TR");
    CParam *TF = StsGetParam(funcindex,"TF");
    //}}AFX_STS_PARAM_PROTOTYPES
    double VOL_result[SITE_NUM] = { 0.0 };
    double IDRVSOURCE_result[SITE_NUM] = { 0.0 };
    double VOH_result[SITE_NUM] = { 0.0 };
    double IDRVSINK_result[SITE_NUM] = { 0.0 };
    double VCLAMP_result[SITE_NUM] = { 0.0 };
    double TR_result[SITE_NUM] = { 0.0 };
    double TF_result[SITE_NUM] = { 0.0 };
    // TODO: Add your function code here
    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    //=================VOL================================//
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FI, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    cbitclose(K10_DRV_PIN_Resource);
    cbitclose(K7_DRV_VI);
    delay_ms(3);

    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    DRV_VI.Set(FI, 0, FOVI_1V, FOVI_100MA, RELAY_ON);
    delay_ms(30);
    DRV_VI.MeasureVI(20, 10);
    SERIAL VOL_result[SITE] = DRV_VI.GetMeasResult(SITE, MVRET);
    SERIAL VOL->SetTestResult(SITE, 0, VOL_result[SITE]);
    //=================IDRV_SINK================================//
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_1A, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_1V, FOVI_100MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_20V, FOVI_1A, RELAY_ON);
    delay_ms(3);
    DRV_VI.Set(FV, 10, FOVI_20V, FOVI_1A, RELAY_ON);

    DRV_VI.MeasureVI(30, 10);
    double res_cap[SITE_NUM][100];
    SERIAL IDRVSINK_result[SITE] = 0;
    SERIAL DRV_VI.BlockRead(SITE, 0, 30, res_cap[SITE], MIRET);
    SERIAL{

        for (int i = 6;i < 14;i++)
        {
            IDRVSINK_result[SITE] += res_cap[SITE][i];
        }
    }//芯片发热导致电流会一直降低，直接取电流前几个点

    SERIAL IDRVSINK_result[SITE] = 1E3 * IDRVSINK_result[SITE] / 8;//求平均值并转换为MV
    SERIAL IDRV_SINK->SetTestResult(SITE, 0, IDRVSINK_result[SITE]);
    DRV_VI.Set(FV, 0, FOVI_20V, FOVI_1A, RELAY_ON);
    delay_ms(3);
    //=================VOH================================//
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_20V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FI, 0, FOVI_20V, FOVI_1UA, RELAY_ON);

    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);
    //HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    //cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    //delay_ms(3);
    Enter_TM9(1);
    cbitopen(K16_DIO0_CS_CLK);//CLK
    cbitopen(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(3);

    CS_VI.Set(FV, 0.1, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, -0.2, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(2);
    VDD_VI.Set(FV, 15, FOVI_50V, FOVI_1A, RELAY_ON);
    delay_ms(2);

    DRV_VI.MeasureVI(10, 10);
    SERIAL VOH_result[SITE] = DRV_VI.GetMeasResult(SITE, MVRET);
    SERIAL VOH->SetTestResult(SITE, 0, VOH_result[SITE]);
    //=================V_CLAMP================================//
    DRV_VI.Set(FI, 0, FOVI_20V, FOVI_1UA, RELAY_ON);
    VDD_VI.Set(FV, 25, FOVI_50V, FOVI_1A, RELAY_ON);
    delay_ms(3);

    DRV_VI.MeasureVI(10, 10);
    SERIAL VCLAMP_result[SITE] = DRV_VI.GetMeasResult(SITE, MVRET);
    SERIAL V_CLAMP->SetTestResult(SITE, 0, VCLAMP_result[SITE]);
    //=================IDRV_SOURCE================================//
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_1A, RELAY_ON);
    DRV_VI.Set(FI, 0, FOVI_20V, FOVI_1UA, RELAY_ON);
    DRV_VI.Set(FI, 0, FOVI_20V, FOVI_1A, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_20V, FOVI_1A, RELAY_ON);
    delay_ms(3);

    DRV_VI.MeasureVI(20, 10);
    SERIAL IDRVSOURCE_result[SITE] = DRV_VI.GetMeasResult(SITE, MIRET);
    SERIAL IDRVSOURCE_result[SITE] = -1E3 * IDRVSOURCE_result[SITE];//ma
    SERIAL IDRV_SOURCE->SetTestResult(SITE, 0, IDRVSOURCE_result[SITE]);
    DRV_VI.Set(FI, 0, FOVI_20V, FOVI_1A, RELAY_ON);
    DRV_VI.Set(FI, 0, FOVI_20V, FOVI_1UA, RELAY_ON);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(3);
    cbitopen(K7_DRV_VI);
    delay_ms(3);
    //=================TR================================//
    QTMU_MEAS.Init();
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(2.0, QTMU_PLUS_POS_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetStopTrigger(8.0, QTMU_PLUS_POS_SLOPE);//trigger=2.5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    QTMU_MEAS.Connect();
    QTMU_MEAS.SetTimeOut(10); //timeout=10ms

    cbitclose(K24_DRV_BUF_IN);
    cbitclose(K25_QTMUA_BUF_OUT);
    cbitclose(K10_DRV_PIN_Resource);
    cbitclose(K19_VDD_2P2UF);
    cbitclose(K23_DRV_1NF);
    delay_ms(3);

    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    CS_VI.Set(FV, 0.4, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 1, FOVI_10V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    delay_ms(1);
    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);
    SERIAL TR_result[SITE] = QTMU_MEAS.GetMeasureResult(SITE);//返回us
    SERIAL TR_result[SITE] = 1e3 * TR_result[SITE];//ns
    SERIAL TR->SetTestResult(SITE, 0, TR_result[SITE]);
    //=================TF================================//
    delay_ms(3);
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(8, QTMU_PLUS_NEG_SLOPE);//trigger=8V, Falling edge
    QTMU_MEAS.SetStopTrigger(1.5, QTMU_PLUS_NEG_SLOPE);//trigger=1.5V,Falling edge 
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
    delay_ms(1);

    QTMU_MEAS.Meas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS, 10);
    SERIAL TF_result[SITE] = QTMU_MEAS.GetMeasureResult(SITE);//返回us
    SERIAL TF_result[SITE] = 1e3 * TF_result[SITE];//ns
    SERIAL TF->SetTestResult(SITE, 0, TF_result[SITE]);

    //=================POWER DOWN================================//
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);



    return 0;
}


DUT_API int TEST17_EE_FINAL(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *EE0_COMP_FINAL = StsGetParam(funcindex,"EE0_COMP_FINAL");
    CParam *EE1_COMP_FINAL = StsGetParam(funcindex,"EE1_COMP_FINAL");
    CParam *EE2_COMP_FINAL = StsGetParam(funcindex,"EE2_COMP_FINAL");
    CParam *EE3_COMP_FINAL = StsGetParam(funcindex,"EE3_COMP_FINAL");
    CParam *EE0_FINAL = StsGetParam(funcindex,"EE0_FINAL");
    CParam *EE1_FINAL = StsGetParam(funcindex,"EE1_FINAL");
    CParam *EE2_FINAL = StsGetParam(funcindex,"EE2_FINAL");
    CParam *EE3_FINAL = StsGetParam(funcindex,"EE3_FINAL");
    CParam *VBG_FINAL = StsGetParam(funcindex,"VBG_FINAL");
    CParam *IBIAS_FINAL = StsGetParam(funcindex,"IBIAS_FINAL");
    CParam *FSW_FINAL = StsGetParam(funcindex,"FSW_FINAL");
    CParam *SLOPE_FINAL = StsGetParam(funcindex,"SLOPE_FINAL");
    CParam *KLINE_FINAL = StsGetParam(funcindex,"KLINE_FINAL");
    CParam *BI_DET_FINAL = StsGetParam(funcindex,"BI_DET_FINAL");
    CParam *HV_EN_FINAL = StsGetParam(funcindex,"HV_EN_FINAL");
    CParam *VBST_SEL_FINAL = StsGetParam(funcindex,"VBST_SEL_FINAL");
    CParam *HL_QR_FINAL = StsGetParam(funcindex,"HL_QR_FINAL");
    CParam *CS_JT_FINAL = StsGetParam(funcindex,"CS_JT_FINAL");
    CParam *VBST_Fx_FINAL = StsGetParam(funcindex,"VBST_Fx_FINAL");
    CParam *SCP_UVP_FINAL = StsGetParam(funcindex,"SCP_UVP_FINAL");
    CParam *OTP_L_FINAL = StsGetParam(funcindex,"OTP_L_FINAL");
    CParam *OVP_L_FINAL = StsGetParam(funcindex,"OVP_L_FINAL");
    CParam *PKL_EN_FINAL = StsGetParam(funcindex,"PKL_EN_FINAL");
    CParam *HFSW_FINAL = StsGetParam(funcindex,"HFSW_FINAL");
    CParam *HFS_FINAL = StsGetParam(funcindex,"HFS_FINAL");
    CParam *CS_Limit_FINAL = StsGetParam(funcindex,"CS_Limit_FINAL");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here
     /*********Trim items define**********/
    TRIM_NODE& vbg = dut.trim("VBG");
    TRIM_NODE& ibias = dut.trim("IBIAS");
    TRIM_NODE& fsw = dut.trim("FSW");
    TRIM_NODE& slope = dut.trim("SLOPE");
    TRIM_NODE& kline = dut.trim("KLINE");
    /**********Optional trim items**********/
    SEL_NODE& bi_det_zcd = dut.sel("BI_DET_ZCD");//trim bit 15
    SEL_NODE& hv_en = dut.sel("HV_EN_FUN");//trim bit 16
    SEL_NODE& vbst_sel = dut.sel("VBST_SEL_FUN");//trim bit 17
    SEL_NODE& hl_qr = dut.sel("HL_QR_FUN");//trim bit 18
    SEL_NODE& cs_jt = dut.sel("CS_JT_FUN");//trim bit 19
    SEL_NODE& vbst_fx = dut.sel("VBST_Fx_FUN");//trim bit 20
    SEL_NODE& scp_uvp = dut.sel("SCP_UVP_FUN");//trim bit 21
    SEL_NODE& otp_l = dut.sel("OTP_L_FUN");//trim bit 22
    SEL_NODE& ovp = dut.sel("OVP_FUN");//trim bit 23
    SEL_NODE& pkl_en = dut.sel("PKL_EN_FUN");//trim bit 24
    //SEL_NODE& pk_time = dut.sel("PK_TIME_FUN");//trim bit 25/26---------bit24=1
    //SEL_NODE& pkl_protect = dut.sel("PKL_Protect_FUN");//trim bit 27----------bit24=1
    SEL_NODE& ocp_hfsw = dut.sel("OCP_HFsw_FUN");//trim bit 25----------bit24=0
    SEL_NODE& hfs = dut.sel("HFS_FUN");//trim bit 26----------bit24=0
    SEL_NODE& l_cslim = dut.sel("L_Cslim_FUN");//trim bit 27----------bit24=0
    // TODO: Add your function code here
    int ee0_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee1_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee2_READ[SITE_NUM] = { -1,-1,-1,-1 };
    int ee3_READ[SITE_NUM] = { -1,-1,-1,-1 };

    int dio_ch[4] = { 1, 3, 5, 7 };//DATA CHANNEL
    int EE_READ[4][SITE_NUM] = { 0 };

    //------------------resource initial------------------------------//
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(2);

    

   //================ALL_READ==========================
    cbit.SetOn(-1);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    cbitclose(K16_DIO0_CS_CLK);//CLK
    cbitclose(K17_DIO1_ZCD_DATA);//DATA
    delay_ms(2);
    HV_FPVI_Power(1, TRIM15);
    VDD_VI.Set(FV, 20, FOVI_50V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    TM_LATCH(1);//进入测试模式
    cbitclose(K3_ZCD_VI);//ZCD与FOVI断开
    delay_ms(3);
    Enter_TM_Read(1);//进入标准读模式

    Enter_Read_Mode();//期望DATA为高，data有数据会被拉低-->输出为低

    SERIAL ee0_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 2, 9, LSB));
    SERIAL ee0_init_READ[SITE] = 255 - ee0_READ[SITE];//data拉低=寄存器里为1，所以输出值需要取反

    SERIAL ee1_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 10, 17, LSB));
    SERIAL ee1_init_READ[SITE] = 255 - ee1_READ[SITE];

    SERIAL ee2_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 18, 25, LSB));
    SERIAL ee2_init_READ[SITE] = 255 - ee2_READ[SITE];

    SERIAL ee3_READ[SITE] = int(dio.GetSerialPatternResult(dio_ch[SITE], 26, 33, LSB));
    SERIAL ee3_init_READ[SITE] = 255 - ee3_READ[SITE];

    SERIAL dut.assy("EEPROM0").set_read_back((ee0_init_READ[SITE]), SITE);
    SERIAL dut.assy("EEPROM1").set_read_back((ee1_init_READ[SITE]), SITE);
    SERIAL dut.assy("EEPROM2").set_read_back((ee2_init_READ[SITE]), SITE);
    SERIAL dut.assy("EEPROM3").set_read_back((ee3_init_READ[SITE]), SITE);

    SERIAL EE0_COMP_FINAL->SetTestResult(SITE, 0, dut.assy("EEPROM0").comp_prog_to_read(SITE));
    SERIAL EE1_COMP_FINAL->SetTestResult(SITE, 0, dut.assy("EEPROM1").comp_prog_to_read(SITE));
    SERIAL EE2_COMP_FINAL->SetTestResult(SITE, 0, dut.assy("EEPROM2").comp_prog_to_read(SITE));
    SERIAL EE3_COMP_FINAL->SetTestResult(SITE, 0, dut.assy("EEPROM3").comp_prog_to_read(SITE));

    SERIAL EE0_FINAL->SetTestResult(SITE, 0, ee0_init_READ[SITE]);
    SERIAL EE1_FINAL->SetTestResult(SITE, 0, ee1_init_READ[SITE]);
    SERIAL EE2_FINAL->SetTestResult(SITE, 0, ee2_init_READ[SITE]);
    SERIAL EE3_FINAL->SetTestResult(SITE, 0, ee3_init_READ[SITE]);

    SERIAL VBG_FINAL->SetTestResult(SITE, 0, vbg.get_read_back(SITE));
    SERIAL IBIAS_FINAL->SetTestResult(SITE, 0, ibias.get_read_back(SITE));
    SERIAL FSW_FINAL->SetTestResult(SITE, 0, fsw.get_read_back(SITE));
    SERIAL SLOPE_FINAL->SetTestResult(SITE, 0, slope.get_read_back(SITE));
    SERIAL KLINE_FINAL->SetTestResult(SITE, 0, kline.get_read_back(SITE));
    SERIAL BI_DET_FINAL->SetTestResult(SITE, 0, bi_det_zcd.get_read_back(SITE));
    SERIAL HV_EN_FINAL->SetTestResult(SITE, 0, hv_en.get_read_back(SITE));
    SERIAL VBST_SEL_FINAL->SetTestResult(SITE, 0, vbst_sel.get_read_back(SITE));
    SERIAL HL_QR_FINAL->SetTestResult(SITE, 0, hl_qr.get_read_back(SITE));
    SERIAL CS_JT_FINAL->SetTestResult(SITE, 0, cs_jt.get_read_back(SITE));
    SERIAL VBST_Fx_FINAL->SetTestResult(SITE, 0, vbst_fx.get_read_back(SITE));
    SERIAL SCP_UVP_FINAL->SetTestResult(SITE, 0, scp_uvp.get_read_back(SITE));
    SERIAL OTP_L_FINAL->SetTestResult(SITE, 0, otp_l.get_read_back(SITE));
    SERIAL OVP_L_FINAL->SetTestResult(SITE, 0, ovp.get_read_back(SITE));
    SERIAL PKL_EN_FINAL->SetTestResult(SITE, 0, pkl_en.get_read_back(SITE));
    SERIAL HFSW_FINAL->SetTestResult(SITE, 0, ocp_hfsw.get_read_back(SITE));
    SERIAL HFS_FINAL->SetTestResult(SITE, 0, hfs.get_read_back(SITE));
    SERIAL CS_Limit_FINAL->SetTestResult(SITE, 0, l_cslim.get_read_back(SITE));


    //----------------------POWER DOWN--------------------------------------//
    dio.StopPattern();
    dio.Disconnect();
    HV_FPVI_Power_Down(1, TRIM15);
    ALL_FOVI_Power_Down();
    delay_ms(3);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST18_ABS_LEAK_FINAL(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *ZCD_ABS_LEAK_FINAL = StsGetParam(funcindex,"ZCD_ABS_LEAK_FINAL");
    CParam *FB_ABS_LEAK_FINAL = StsGetParam(funcindex,"FB_ABS_LEAK_FINAL");
    CParam *CS_ABS_LEAK_FINAL = StsGetParam(funcindex,"CS_ABS_LEAK_FINAL");
    CParam *DRV_ABS_LEAK_FINAL = StsGetParam(funcindex,"DRV_ABS_LEAK_FINAL");
    CParam *VDD_ABS_LEAK_FINAL = StsGetParam(funcindex,"VDD_ABS_LEAK_FINAL");
    // TODO: Add your function code here
    double results[SITE_NUM] = { 0,0,0,0 };
    double result_HV[SITE_NUM] = { 0,0,0,0 };
    //=================resource initial===================================
     //cbitclose(K3_ZCD_VI);
    //cbitclose(K4_FB_VI);
    //cbitclose(K5_CS_VI);默认相连
    cbitclose(K6_VDD_VI);
    cbitclose(K8_VDD_FPVI1);
    cbitclose(K7_DRV_VI, K10_DRV_PIN_Resource);
    //cbitclose(K11_FPVI_HVIN);
    delay_ms(3);

    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    delay_ms(2);

    //=========================================ZCD_ABS_LEAK============================================
    ZCD_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    ZCD_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    ZCD_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    delay_ms(2);
    ZCD_VI.MeasureVI(10, 10);
    groupgetresults(ZCD_VI, MIRET, results, SITE_NUM);
    SERIAL ABS_ZCD[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(ZCD_ABS_LEAK_FINAL, 0, ABS_ZCD, SITE_NUM);
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================FB_ABS_LEAK============================================
    FB_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    FB_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    delay_ms(2);
    FB_VI.MeasureVI(10, 10);
    groupgetresults(FB_VI, MIRET, results, SITE_NUM);
    SERIAL ABS_FB[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(FB_ABS_LEAK_FINAL, 0, ABS_FB, SITE_NUM);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================CS_ABS_LEAK============================================
    CS_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    CS_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    delay_ms(2);
    CS_VI.MeasureVI(10, 10);
    groupgetresults(CS_VI, MIRET, results, SITE_NUM);
    SERIAL ABS_CS[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(CS_ABS_LEAK_FINAL, 0, ABS_CS, SITE_NUM);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================DRV_ABS_LEAK============================================
    //DRV_VI.Set(FV, 2, FOVI_10V, FOVI_100MA, RELAY_ON);
    //delay_ms(1);
    //DRV_VI.Set(FV, 6.5, FOVI_10V, FOVI_100MA, RELAY_ON);
    //DRV_VI.Set(FV, 6.5, FOVI_10V, FOVI_1MA, RELAY_ON);//6.5
    //delay_ms(2);
    //DRV_VI.MeasureVI(10, 10);
    //groupgetresults(DRV_VI, MIRET, results, SITE_NUM);
    //SERIAL ABS_DRV[SITE] = 1e6 * results[SITE];//uA
    //msLogResultAll(DRV_ABS_LEAK_FINAL, 0, ABS_DRV, SITE_NUM);
    //DRV_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    //delay_ms(2);

    //=========================================VDD_ABS_LEAK============================================
    VDD_FPVI1.Set(FV, 80, FPVI10_100V, FPVI10_100MA, RELAY_ON);//6.5
    delay_ms(1);
    VDD_FPVI1.Set(FV, 80, FPVI10_100V, FPVI10_1MA, RELAY_ON);
    delay_ms(1);
    VDD_FPVI1.MeasureVI(20, 10);
    groupgetresults(VDD_FPVI1, MIRET, results, SITE_NUM);
    SERIAL ABS_VDD[SITE] = 1e6 * results[SITE];//uA
    msLogResultAll(VDD_ABS_LEAK_FINAL, 0, ABS_VDD, SITE_NUM);

    VDD_FPVI1.Set(FV, 10, FPVI10_10V, FPVI10_10MA, RELAY_ON);
    delay_ms(1);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
    delay_ms(2);
    cbitopen(K8_VDD_FPVI1);
    delay_ms(3);

 
    //------------------POWER DOWN-----------------------//
    ZCD_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_50V, FOVI_100MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
    DRV_FPVI0.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_100V, FPVI10_100MA, RELAY_ON);
    delay_ms(2);
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST19_P2P_LEAK_FINAL(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *ZCD_P2P_LEAK_FINAL = StsGetParam(funcindex,"ZCD_P2P_LEAK_FINAL");
    CParam *FB_P2P_LEAK_FINAL = StsGetParam(funcindex,"FB_P2P_LEAK_FINAL");
    CParam *CS_P2P_LEAK_FINAL = StsGetParam(funcindex,"CS_P2P_LEAK_FINAL");
    CParam *DRV_P2P_LEAK_FINAL = StsGetParam(funcindex,"DRV_P2P_LEAK_FINAL");
    CParam *VDD_P2P_LEAK_FINAL = StsGetParam(funcindex,"VDD_P2P_LEAK_FINAL");
    //}}AFX_STS_PARAM_PROTOTYPES

   // TODO: Add your function code here
    double results[SITE_NUM] = { -9999, -9999, -9999, -9999 };
    //=================resource initial===================================
     //cbitclose(K3_ZCD_VI);
    //cbitclose(K4_FB_VI);
    //cbitclose(K5_CS_VI);
    //cbitclose(K6_VDD_VI);默认相连
    cbitclose(K7_DRV_VI, K10_DRV_PIN_Resource);
    //cbitclose(K11_FPVI_HVIN);
    delay_ms(3);

    ZCD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //=========================================ZCD_P2P_LEAK============================================
    ZCD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    ZCD_VI.MeasureVI(20, 10);
    groupgetresults(ZCD_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(ZCD_P2P_LEAK_FINAL, 0, results, SITE_NUM);

    ZCD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================FB_P2P_LEAK============================================
    FB_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    FB_VI.MeasureVI(20, 10);
    groupgetresults(FB_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(FB_P2P_LEAK_FINAL, 0, results, SITE_NUM);

    FB_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================CS_P2P_LEAK============================================
    CS_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    CS_VI.MeasureVI(20, 10);
    groupgetresults(CS_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(CS_P2P_LEAK_FINAL, 0, results, SITE_NUM);

    CS_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================DRV_P2P_LEAK============================================
    DRV_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    DRV_VI.MeasureVI(20, 10);
    groupgetresults(DRV_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(DRV_P2P_LEAK_FINAL, 0, results, SITE_NUM);

    DRV_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=========================================VDD_P2P_LEAK============================================
    VDD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
    delay_us(1000);

    VDD_VI.MeasureVI(20, 10);
    groupgetresults(VDD_VI, MIRET, results, SITE_NUM);
    SERIAL results[SITE] = 1e9 * results[SITE];
    msLogResultAll(VDD_P2P_LEAK_FINAL, 0, results, SITE_NUM);

    VDD_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //------------------POWER DOWN-----------------------//
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}


DUT_API int TEST20_OS_FINAL(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *OS_ALL_OPEN_FINAL = StsGetParam(funcindex,"OS_ALL_OPEN_FINAL");
    CParam *OS_SOME_OPEN_FINAL = StsGetParam(funcindex,"OS_SOME_OPEN_FINAL");
    CParam *OS_SHORT_FINAL = StsGetParam(funcindex,"OS_SHORT_FINAL");
    CParam *OS_ZCD_FINAL = StsGetParam(funcindex,"OS_ZCD_FINAL");
    CParam *OS_FB_FINAL = StsGetParam(funcindex,"OS_FB_FINAL");
    CParam *OS_CS_FINAL = StsGetParam(funcindex,"OS_CS_FINAL");
    CParam *OS_DRV_FINAL = StsGetParam(funcindex,"OS_DRV_FINAL");
    CParam *OS_VDD_FINAL = StsGetParam(funcindex,"OS_VDD_FINAL");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    CParam* OS_DLOG[6];

    OS_DLOG[0] = StsGetParam(funcindex, "OS_ZCD");
    OS_DLOG[1] = StsGetParam(funcindex, "OS_FB");
    OS_DLOG[2] = StsGetParam(funcindex, "OS_CS");
    OS_DLOG[3] = StsGetParam(funcindex, "OS_DRV");
    OS_DLOG[4] = StsGetParam(funcindex, "OS_VDD");
    OS_DLOG[5] = StsGetParam(funcindex, "OS_HV");

    int open_count[SITE_NUM] = { 0, 0, 0, 0 };
    int SHORT_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int SOME_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int ALL_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int LEAK_FLAG[SITE_NUM] = { 0, 0, 0, 0 };
    int OS_DLOG_NUM = 5;
    double os_result[5][SITE_NUM] = { 0, 0, 0, 0 };
    int os_count = 0;

    double results[SITE_NUM] = { 0, 0, 0, 0 };
    //===================================================================================================
    //====================>INIT
    //===================================================================================================
    //cbitclose(K3_ZCD_VI);
    //cbitclose(K4_FB_VI);
    //cbitclose(K5_CS_VI);
    //cbitclose(K6_VDD_VI);默认相连
    cbitclose(K7_DRV_VI, K10_DRV_PIN_Resource);
    //cbitclose(K11_FPVI_HVIN);
    delay_ms(3);

    ZCD_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    FB_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    CS_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    VDD_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DRV_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    DRV_FPVI0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    VDD_FPVI1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //=====================OS_ZCD===========================
    ZCD_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    ZCD_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    ZCD_VI.MeasureVI(20, 10);
    groupgetresults(ZCD_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[0][SITE] = results[SITE];
    msLogResultAll(OS_ZCD_FINAL, 0, results, SITE_NUM);
    ZCD_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_FB===========================
    FB_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    FB_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    FB_VI.MeasureVI(20, 10);
    groupgetresults(FB_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[1][SITE] = results[SITE];
    msLogResultAll(OS_FB_FINAL, 0, results, SITE_NUM);
    FB_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_CS===========================
    CS_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    CS_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    CS_VI.MeasureVI(20, 10);
    groupgetresults(CS_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[2][SITE] = results[SITE];
    msLogResultAll(OS_CS_FINAL, 0, results, SITE_NUM);
    CS_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_DRV===========================
    DRV_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    DRV_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    DRV_VI.MeasureVI(20, 10);
    groupgetresults(DRV_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[3][SITE] = results[SITE];
    msLogResultAll(OS_DRV_FINAL, 0, results, SITE_NUM);
    DRV_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

    //=====================OS_VDD===========================
    VDD_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    VDD_VI.Set(FI, -1e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(400);
    VDD_VI.MeasureVI(20, 10);
    groupgetresults(VDD_VI, MVRET, results, SITE_NUM);
    SERIAL os_result[4][SITE] = results[SITE];
    msLogResultAll(OS_VDD_FINAL, 0, results, SITE_NUM);
    VDD_VI.Set(FV, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_us(100);

   //---------------POWER DOWN-------------------//

    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    //************************************ Add OS classify delog ***********************************************
    //test_method.OS_Classify(funcindex, pin_str_array, PIN_NUM, NULL, LEAK_NUM, os_result, NULL, vec_exclude);
    //************************************ Add OS classify delog end *******************************************

    for (os_count = 0; os_count < OS_DLOG_NUM; os_count++)  //judge short and get open count
    {


        SERIAL
        {
            if (OS_DLOG[os_count]->GetMinLimit() > 0) //if OS value>0
            {
                if (os_result[os_count][SITE] < OS_DLOG[os_count]->GetMinLimit()) { SHORT_FLAG[SITE] = 1; }
                if (SHORT_FLAG[SITE] != 1 && (os_result[os_count][SITE] > OS_DLOG[os_count]->GetMaxLimit())) { open_count[SITE]++; }


            }
            if (OS_DLOG[os_count]->GetMaxLimit() < 0) //if OS value<0
            {
                if (os_result[os_count][SITE] > OS_DLOG[os_count]->GetMaxLimit()) { SHORT_FLAG[SITE] = 1; }
                if (SHORT_FLAG[SITE] != 1 && (os_result[os_count][SITE] < OS_DLOG[os_count]->GetMinLimit())) { open_count[SITE]++; }

            }
        }



    }

    SERIAL    //judge some open or all open 
    {
        if (SHORT_FLAG[SITE] != 1 && open_count[SITE] == 5) { ALL_OPEN_FLAG[SITE] = 1; }
        else if (SHORT_FLAG[SITE] != 1 && open_count[SITE] < 5 && open_count[SITE]>0) { SOME_OPEN_FLAG[SITE] = 1; }
    }

    SERIAL	OS_SHORT_FINAL->SetTestResult(SITE, 0, SHORT_FLAG[SITE]);
    SERIAL	OS_SOME_OPEN_FINAL->SetTestResult(SITE, 0, SOME_OPEN_FLAG[SITE]);
    SERIAL	OS_ALL_OPEN_FINAL->SetTestResult(SITE, 0, ALL_OPEN_FLAG[SITE]);

    return 0;
}


