//*********************************************************************************************************************************************
//Revision History:
//---------------------------------------------------------------------------------------------------------------------------------------------
//Rev    Level     Originator     Date          Auditor      Date         Testtime/insertion
//---------------------------------------------------------------------------------------------------------------------------------------------
//00     Inital    Chunlei Li     2025/08/1                 	          TBD

//********************************************************************************************************************************************* 
//NX7750DQ TEST PROGRAM
//---------------------------------------------------------------------------------------------------------------------------------------------
//DEVICE ...........................NX7750DQ
//PACKAGE...........................
//DUT BOARD NUMBER..................SAFS7750DQ
//LOAD BOARD NUMBER.................8192P
//CABLE CONNECTION..................64PIN *1 , 16PIN *1

//********************************************************************************************************************************************* 
// PIN INOUT Definition
//---------------------------------------------------------------------------------------------------------------------------------------------  
//                               
//                                  VS 15
//                           ________|_______
//                          |                |
//                   1  GND-|                |-OUT0  14  
//                          |                |
//                   2  IN0-|                |-OUT0  13   
//                          |                |
//                   3  DEN-|                |-OUT0  12     
//                          |                |
//                   4   IS-|     NX7750DQ   |-NC1   11
//                          |                |
//                   5 DESL-|                |-OUT1  10    
//                          |                |
//                   6  IN1-|                |-OUT1  9 
//                          |                |  
//                   7  NC0-|                |-OUT1  8
//                          |                |
//                          |________________|
//
// 
//********************************************************************************************************************************************* 

#include "stdafx.h"
#include "sub.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

//#include "BoardCheck.h"

#include <map>
#include <string>
#include <iostream>
#include<cstdlib>


// 站点相关变量
int  site = 0;              // 当前站点索引
int globalsite;             // 全局站点索引

bool DO_BoardCheck = false;
extern BOOL run_diags();     // global funtion for HW checker


//bool DO_GNG_Flag = FALSE;
bool DO_P2P_Leak_Group_Flag = FALSE;

double Pre_P2P_LEAK[10][SITE_NUM] = { 0.0 };
double Pre_IQ[2][SITE_NUM] = { 0.0 };

int Trim_done_flag[SITE_NUM] = {0};

map<DWORD, map<DWORD, int>> EE_READ;
map<DWORD, map<DWORD, int>> EE_READ_Final;

int rfuse_data[SITE_NUM] = { 0 };
map<DWORD, map<DWORD, int>> Burn_data;
map<DWORD, map<DWORD, int>> EE_WRITE;


//===============================================
//===============================================
//===============================================
//////////////FOVI resource//////////////////////
FOVI KELVIN_VS(0, "KELVIN_VS"); // Logic channel 0
FOVI IN1_VI(1, "IN1_VI");
FOVI DEN_VI(2, "DEN_VI");
FOVI NC_VI(3, "NC_VI");
FOVI IN0_VI(4, "IN0_VI");
FOVI IS_DESL_VI(5, "IS_DESL_VI");
FOVI KELVIN_GND(6, "KELVIN_GND");
FOVI PullUp_VI(7, "PullUp_VI");

//////////////FPVI resource//////////////////////
FPVI10 KELVIN_OUT1(0, "KELVIN_OUT1");
FPVI10 KELVIN_OUT0(1, "KELVIN_OUT0");

//////////////QTMU//////////////////////////////
QTMU_PLUS QTMU_MEAS(0); // Logic channel 0

//////////////DIO///////////////////////////////
DIO_PLUS dio_plus; // 第一块DIO模块
DIO dio(0);

//////////////CBIT///////////////////////////////
CBIT128 cbit;

//////////////GPVI////////////////////////////////
GPFOVI ALL_FOVI("ALL_FOVI", IN1_VI, DEN_VI, NC_VI, IN0_VI, IS_DESL_VI, PullUp_VI, KELVIN_GND, KELVIN_VS);
GPFOVI  ALL_KELVIN_FOVI("KELVIN_VI", KELVIN_GND, KELVIN_VS);
GPFPVI10  ALL_KELVIN_FPVI("ALL_FPVI", KELVIN_OUT1, KELVIN_OUT0);

GPFOVI P2P_LEAK_FOVI_1("P2P_LEAK_FOVI_1", IN0_VI, IS_DESL_VI, IN1_VI, NC_VI);
GPFOVI P2P_LEAK_FOVI_2("P2P_LEAK_FOVI_2", DEN_VI, IS_DESL_VI, KELVIN_VS, NC_VI);


DUT_API void UserLoad()
{
    STSSetSiteStatus(0xFFFF);
    StsSetModuleToSite(MD_FOVI, SITE_1, 0, 1, 2, 3, 4, 5, 6, 7, -1); // SITE1: 0-7
    StsSetModuleToSite(MD_FOVI, SITE_2, 8, 9, 10, 11, 12, 13, 14, 15, -1); // SITE2: 8-15
    StsSetModuleToSite(MD_FOVI, SITE_3, 16, 17, 18, 19, 20, 21, 22, 23, -1); //SITE3: 16-23
    StsSetModuleToSite(MD_FOVI, SITE_4, 24, 25, 26, 27, 28, 29, 30, 31, -1); //SITE4: 24-31


    StsSetModuleToSite(MD_FPVI10, SITE_1, 0, 1, -1);//set channels 0-3 to SITE1
    StsSetModuleToSite(MD_FPVI10, SITE_2, 2, 3, -1);//set channels 4-7 to SITE2
    StsSetModuleToSite(MD_FPVI10, SITE_3, 4, 5, -1);//set channels 4-7 to SITE3
    StsSetModuleToSite(MD_FPVI10, SITE_4, 6, 7, -1);//set channels 4-7 to SITE4

    StsSetModuleToSite(MD_QTMUPLUS, SITE_1, 0, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_2, 1, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_3, 2, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_4, 3, -1);

    AstBindingAllModule();

	dio_plus.Init();
    dio_plus.Connect();
    delay_ms(1);

    dio_plus.LoadVectorFile("NX7750DQ.vecdio");

    dio_plus.SetVIH(5.0); //set driver high voltage
    dio_plus.SetVIL(0.0);//set driver low voltage
    dio_plus.SetVOH(0.8); //set compare high voltage
    dio_plus.SetVOL(0.5);//set compare low voltage

	delay_ms(2);

    if (DO_BoardCheck)
        run_diags();



}
/************************************************************************/
/*                                                                      */
/************************************************************************/


DUT_API void UserInitAfterLoad()
{

}
/************************************************************************/
/*                                                                      */
/************************************************************************/


//multisite settings should be included here
DUT_API void HardWareCfg()
{

    StsSetModuleToSite(MD_FOVI, SITE_1, 0, 1, 2, 3, 4, 5, 6, 7, -1); // SITE1: 0-7
    StsSetModuleToSite(MD_FOVI, SITE_2, 8, 9, 10, 11, 12, 13, 14, 15, -1); // SITE2: 8-15
    StsSetModuleToSite(MD_FOVI, SITE_3, 16, 17, 18, 19, 20, 21, 22, 23, -1); //SITE3: 16-23
    StsSetModuleToSite(MD_FOVI, SITE_4, 24, 25, 26, 27, 28, 29, 30, 31, -1); //SITE4: 24-31


    StsSetModuleToSite(MD_FPVI10, SITE_1, 0, 1, -1);//set channels 0-3 to SITE1
    StsSetModuleToSite(MD_FPVI10, SITE_2, 2, 3, -1);//set channels 4-7 to SITE2
    StsSetModuleToSite(MD_FPVI10, SITE_3, 4, 5, -1);//set channels 4-7 to SITE3
    StsSetModuleToSite(MD_FPVI10, SITE_4, 6, 7, -1);//set channels 4-7 to SITE4

    StsSetModuleToSite(MD_QTMUPLUS, SITE_1, 0, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_2, 1, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_3, 2, -1);
    StsSetModuleToSite(MD_QTMUPLUS, SITE_4, 3, -1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initialize function will be called before all the test functions.
DUT_API void InitBeforeTestFlow()
{
    KELVIN_VS.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    IN0_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    cbit.SetOn(-1);


}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()
{
    KELVIN_VS.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    IN0_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
    PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    cbit.SetOn(-1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//Fail site hardware set function will be called after failed params, it can be called for serveral times. 
DUT_API void SetupFailSite(const unsigned char* byFailSite)
{
    ALL_FOVI_Power_Off(1);

    //cbit.SetOn(-1);// 不建议写在这个地方
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

DUT_API int TEST01_INFO(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *Part_No = StsGetParam(funcindex, "Part_No");
    CParam *SiteCheck_S1 = StsGetParam(funcindex, "SiteCheck_S1");
    CParam *SiteCheck_S2 = StsGetParam(funcindex, "SiteCheck_S2");
    CParam *SiteCheck_S3 = StsGetParam(funcindex, "SiteCheck_S3");
    CParam *SiteCheck_S4 = StsGetParam(funcindex, "SiteCheck_S4");
//}}AFX_STS_PARAM_PROTOTYPES

        // TODO: Add your function code here
    SERIAL Part_No->SetTestResult(SITE, 0, 7750);

    double adresults[SITE_NUM];

#pragma region site_check
    // ************************************************
    // Site_Check 
    // Measure each site Resistor to check production setup
    // Site1=1K, Site2=2K, Site3=3K, Site4=4K
    // ************************************************

    KELVIN_GND.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FI, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FI, 1e-4, FOVI_2V, FOVI_1MA, RELAY_ON); // 0.1mA
    delay_ms(3);

    KELVIN_GND.MeasureVI(50, 10); // sample time:50, interval time:10us,default to MEAS_NORMAL
    KELVIN_GND.Set(FI, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FV, 0, FOVI_1V, FOVI_1MA, RELAY_ON);

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

DUT_API int TEST02_KELVIN(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *Kelvin_VS = StsGetParam(funcindex, "Kelvin_VS");
    CParam *Kelvin_GND = StsGetParam(funcindex, "Kelvin_GND");
    CParam *Kelvin_OUT0 = StsGetParam(funcindex, "Kelvin_OUT0");
    CParam *Kelvin_OUT1 = StsGetParam(funcindex, "Kelvin_OUT1");
//}}AFX_STS_PARAM_PROTOTYPES
        // TODO: Add your function code here
    double MeasV_VS[SITE_NUM], MeasV_GND[SITE_NUM], MeasV_OUT0[SITE_NUM], MeasV_OUT1[SITE_NUM];
    double MeasR_VS[SITE_NUM], MeasR_GND[SITE_NUM], MeasR_OUT0[SITE_NUM], MeasR_OUT1[SITE_NUM];

    //*******************************************
    // KELVIN_FOVI 
    //*******************************************
	//===================	KELVIN_VS   ===================
	cbit.SetOn(K1_K2_VS_OUT1_KelvinCheck, -1);
	delay_ms(2);
	KELVIN_VS.Set(FI, 0.0, FOVI_10V, FOVI_1MA, RELAY_ON);
    KELVIN_VS.Set(FI, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_ms(3);
    KELVIN_VS.Set(FI, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);
    KELVIN_VS.Set(FI, 0.008, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    KELVIN_VS.MeasureVI(20, 10);
    SERIAL MeasV_VS[SITE] = KELVIN_VS.GetMeasResult(SITE, MVRET);
    SERIAL MeasR_VS[SITE] = MeasV_VS[SITE] / 0.008; //Ohm
    SERIAL Kelvin_VS->SetTestResult(SITE, 0, MeasR_VS[SITE]);

    //===================	KELVIN_GND	===================
	//-----------KELVIN_GND原理图设计有误，测试结果不准确
    cbit.SetOn(K3_K10_OUT0_GND_KelvinCheck, -1);
    delay_ms(2);
    KELVIN_GND.Set(FI, 0.0, FOVI_10V, FOVI_1MA, RELAY_ON);
    KELVIN_GND.Set(FI, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_ms(3);
    KELVIN_GND.Set(FI, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);
    KELVIN_GND.Set(FI, 0.008, FOVI_2V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    KELVIN_GND.MeasureVI(20, 10);

    SERIAL MeasV_GND[SITE] = KELVIN_GND.GetMeasResult(SITE, MVRET);
    SERIAL MeasR_GND[SITE] = MeasV_GND[SITE] / 0.008; //Ohm
    SERIAL Kelvin_GND->SetTestResult(SITE, 0, MeasR_GND[SITE]);


    //*******************************************
    // KELVIN_FPVI 
    //*******************************************
    //===================	KELVIN_OUT0	===================
    cbit.SetOn(K3_K10_OUT0_GND_KelvinCheck, -1);
    delay_ms(2);

    KELVIN_OUT0.Set(FI, 0.0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT0.Set(FI, 0.0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
    delay_ms(3);
    KELVIN_OUT0.Set(FI, 0.0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
    KELVIN_OUT0.Set(FI, 0.008, FPVI10_2V, FPVI10_10MA, RELAY_ON);
    delay_ms(2);

    KELVIN_OUT0.MeasureVI(20, 10);
    SERIAL MeasV_OUT0[SITE] = KELVIN_OUT0.GetMeasResult(SITE, MVRET);
    SERIAL MeasR_OUT0[SITE] = MeasV_OUT0[SITE] / 0.008;
    SERIAL Kelvin_OUT0->SetTestResult(SITE, 0, MeasR_OUT0[SITE]);

    //===================	KELVIN_OUT1	===================
    cbit.SetOn(K1_K2_VS_OUT1_KelvinCheck, -1);
    delay_ms(2);

    KELVIN_OUT1.Set(FI, 0.0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FI, 0.0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
    delay_ms(3);
    KELVIN_OUT1.Set(FI, 0.0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
    KELVIN_OUT1.Set(FI, 0.008, FPVI10_2V, FPVI10_10MA, RELAY_ON);
    delay_ms(2);

    KELVIN_OUT1.MeasureVI(20, 10);
    SERIAL MeasV_OUT1[SITE] = KELVIN_OUT1.GetMeasResult(SITE, MVRET);
    SERIAL MeasR_OUT1[SITE] = MeasV_OUT1[SITE] / 0.008;
    SERIAL Kelvin_OUT1->SetTestResult(SITE, 0, MeasR_OUT1[SITE]);

    // ALL_KELVIN_FPVI POWER DOWN
    ALL_KELVIN_FPVI.Set(FI, 0.0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
    ALL_KELVIN_FPVI.Set(FI, 0.0, FPVI10_10V, FPVI10_1MA, RELAY_ON);

    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}

DUT_API int TEST03_CONTACT(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *OS_SHORT = StsGetParam(funcindex, "OS_SHORT");
    CParam *OS_SOME_OPEN = StsGetParam(funcindex, "OS_SOME_OPEN");
    CParam *OS_ALL_OPEN = StsGetParam(funcindex, "OS_ALL_OPEN");
    CParam *Contact_IN_0 = StsGetParam(funcindex, "Contact_IN_0");
    CParam *Contact_IN_1 = StsGetParam(funcindex, "Contact_IN_1");
    CParam *Contact_DEN = StsGetParam(funcindex, "Contact_DEN");
    CParam *Contact_IS = StsGetParam(funcindex, "Contact_IS");
    CParam *Contact_DESL = StsGetParam(funcindex, "Contact_DESL");
    CParam *Contact_NC = StsGetParam(funcindex, "Contact_NC");
    CParam *Contact_NC_2 = StsGetParam(funcindex, "Contact_NC_2");
    CParam *Contact_OUT_0 = StsGetParam(funcindex, "Contact_OUT_0");
    CParam *Contact_OUT_1 = StsGetParam(funcindex, "Contact_OUT_1");
    CParam *Contact_VS = StsGetParam(funcindex, "Contact_VS");
//}}AFX_STS_PARAM_PROTOTYPES
        // TODO: Add your function code here

    CParam* OS_DLOG[10];  // 定义引脚参数数组

    // 初始化引脚参数
    OS_DLOG[0] = StsGetParam(funcindex, "Contact_IN_0");
    OS_DLOG[1] = StsGetParam(funcindex, "Contact_IN_1");
    OS_DLOG[2] = StsGetParam(funcindex, "Contact_DEN");
    OS_DLOG[3] = StsGetParam(funcindex, "Contact_IS");
    OS_DLOG[4] = StsGetParam(funcindex, "Contact_DESL");
    OS_DLOG[5] = StsGetParam(funcindex, "Contact_NC");
    OS_DLOG[6] = StsGetParam(funcindex, "Contact_NC_2");
    OS_DLOG[7] = StsGetParam(funcindex, "Contact_OUT_0");
    OS_DLOG[8] = StsGetParam(funcindex, "Contact_OUT_1");
    OS_DLOG[9] = StsGetParam(funcindex, "Contact_VS");

    // 定义状态标志数组
    int open_count[SITE_NUM] = { 0, 0, 0, 0 };              // 开路计数
    int SHORT_FLAG[SITE_NUM] = { 0, 0, 0, 0 };              // 短路标志
    int SOME_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };          // 部分开路标志
    int ALL_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };           // 全部开路标志
    int LEAK_FLAG[SITE_NUM] = { 0, 0, 0, 0 };               // 漏电标志
    int OS_DLOG_NUM = 10;                                   // 引脚数量
    double os_result[10][SITE_NUM] = { 0, 0, 0, 0 };         // 测试结果存储
    int os_count = 0;                                       // 循环计数器

    double results[SITE_NUM] = { 0, 0, 0, 0 };              // 临时结果存储

    IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //=====================Contact_IN_0===========================0
    IN0_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
    IN0_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON); // -0.1mA
	delay_ms(1);
    IN0_VI.MeasureVI(20, 10);
    groupgetresults(IN0_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[0][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_IN_0, 0, results, SITE_NUM);
    IN0_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_IN_1===========================1
    IN1_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	IN1_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
    IN1_VI.MeasureVI(20, 10);
    groupgetresults(IN1_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[1][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_IN_1, 0, results, SITE_NUM);
    IN1_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_DEN===========================2
    DEN_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	DEN_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
    DEN_VI.MeasureVI(20, 10);
    groupgetresults(DEN_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[2][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_DEN, 0, results, SITE_NUM);
    DEN_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_IS===========================3
    IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	IS_DESL_VI.Set(FI, 1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
    IS_DESL_VI.MeasureVI(20, 10);
    groupgetresults(IS_DESL_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[3][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_IS, 0, results, SITE_NUM);
    IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_DESL===========================4
    cbit.SetOn(K4_NC_IS_DESL_VI, -1); // 闭合K4A，切换 IS_DESL FOVI5资源
    delay_ms(4);

    IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	IS_DESL_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
    IS_DESL_VI.MeasureVI(20, 10);
    groupgetresults(IS_DESL_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[4][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_DESL, 0, results, SITE_NUM);
    IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_NC_2===========================6
    NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	NC_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
    NC_VI.MeasureVI(20, 10);
    groupgetresults(NC_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[6][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_NC_2, 0, results, SITE_NUM);
    NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_NC===========================5
    cbit.SetOn(-1); // 断开K4B，切换 NC_NC2 FOVI3资源
    delay_ms(4);

    NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	NC_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
    NC_VI.MeasureVI(20, 10);
    groupgetresults(NC_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[5][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_NC, 0, results, SITE_NUM);
    NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_OUT_0===========================7
    KELVIN_OUT0.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT0.Set(FI, 1e-4, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
    KELVIN_OUT0.MeasureVI(20, 10);
    groupgetresults(KELVIN_OUT0, MVRET, results, SITE_NUM);
    SERIAL os_result[7][SITE] = results[SITE];
    msLogResultAll(Contact_OUT_0, 0, results, SITE_NUM);
    KELVIN_OUT0.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_OUT_1===========================8
    KELVIN_OUT1.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT1.Set(FI, 1e-4, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
    KELVIN_OUT1.MeasureVI(20, 10);
    groupgetresults(KELVIN_OUT1, MVRET, results, SITE_NUM);
    SERIAL os_result[8][SITE] = results[SITE];
    msLogResultAll(Contact_OUT_1, 0, results, SITE_NUM);
    KELVIN_OUT1.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

    //=====================Contact_VS===========================9
    KELVIN_VS.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	KELVIN_VS.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
    KELVIN_VS.MeasureVI(20, 10);
    groupgetresults(KELVIN_VS, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
    SERIAL os_result[9][SITE] = results[SITE];          // 记录所有site的测试结果
    msLogResultAll(Contact_VS, 0, results, SITE_NUM);
    KELVIN_VS.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

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
        if (SHORT_FLAG[SITE] != 1 && open_count[SITE] == OS_DLOG_NUM) { ALL_OPEN_FLAG[SITE] = 1; }
        else if (SHORT_FLAG[SITE] != 1 && open_count[SITE] < OS_DLOG_NUM && open_count[SITE]>0) { SOME_OPEN_FLAG[SITE] = 1; }
    }

    SERIAL	OS_SHORT->SetTestResult(SITE, 0, SHORT_FLAG[SITE]);
    SERIAL	OS_SOME_OPEN->SetTestResult(SITE, 0, SOME_OPEN_FLAG[SITE]);
    SERIAL	OS_ALL_OPEN->SetTestResult(SITE, 0, ALL_OPEN_FLAG[SITE]);

    return 0;
}

DUT_API int TEST04_PRE_P2P_Leakage(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *P2P_LKG_IN_0 = StsGetParam(funcindex, "P2P_LKG_IN_0");
    CParam *P2P_LKG_IN_1 = StsGetParam(funcindex, "P2P_LKG_IN_1");
    CParam *P2P_LKG_DEN = StsGetParam(funcindex, "P2P_LKG_DEN");
    CParam *P2P_LKG_IS = StsGetParam(funcindex, "P2P_LKG_IS");
    CParam *P2P_LKG_DESL = StsGetParam(funcindex, "P2P_LKG_DESL");
    CParam *P2P_LKG_NC = StsGetParam(funcindex, "P2P_LKG_NC");
    CParam *P2P_LKG_NC_2 = StsGetParam(funcindex, "P2P_LKG_NC_2");
    CParam *P2P_LKG_OUT_0 = StsGetParam(funcindex, "P2P_LKG_OUT_0");
    CParam *P2P_LKG_OUT_1 = StsGetParam(funcindex, "P2P_LKG_OUT_1");
    CParam *P2P_LKG_VS = StsGetParam(funcindex, "P2P_LKG_VS");
//}}AFX_STS_PARAM_PROTOTYPES
    // 
        // TODO: Add your function code here
    double Results[SITE_NUM] = { -9999, -9999, -9999, -9999 };

    IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);


    if (DO_P2P_Leak_Group_Flag == TRUE)
    {
        //GPFPVI10  ALL_KELVIN_FPVI("ALL_FPVI", KELVIN_OUT1, KELVIN_OUT0);
        //GPFOVI P2P_LEAK_FOVI_1("P2P_LEAK_FOVI_1", IN0_VI, IS_DESL_VI, IN1_VI, NC_VI);
        //GPFOVI P2P_LEAK_FOVI_2("P2P_LEAK_FOVI_2", DEN_VI, IS_DESL_VI, KELVIN_VS, NC_VI);

        //--------------------奇数组
		IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

		KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
		KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
		delay_ms(2);


        P2P_LEAK_FOVI_1.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        P2P_LEAK_FOVI_1.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        P2P_LEAK_FOVI_1.MeasureVI(20, 10);
        SERIAL
        {
			P2P_LKG_IN_0->SetTestResult(SITE, 0, IN0_VI.GetMeasResult(SITE, MIRET) * 1e9);
			P2P_LKG_IS->SetTestResult(SITE, 0, IS_DESL_VI.GetMeasResult(SITE, MIRET) * 1e9);
			P2P_LKG_IN_1->SetTestResult(SITE, 0, IN1_VI.GetMeasResult(SITE, MIRET) * 1e9);
			P2P_LKG_NC->SetTestResult(SITE, 0, NC_VI.GetMeasResult(SITE, MIRET) * 1e9);
        }

        P2P_LEAK_FOVI_1.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(2);
        cbit.SetOn(K4_NC_IS_DESL_VI, -1); // 闭合K4
        delay_ms(4);

        //---------------------偶数组
		IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

		KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
		KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
		delay_ms(2);

        P2P_LEAK_FOVI_2.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        P2P_LEAK_FOVI_2.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        P2P_LEAK_FOVI_2.MeasureVI(20, 10);
        SERIAL
        {
			P2P_LKG_DEN->SetTestResult(SITE, 0, DEN_VI.GetMeasResult(SITE, MIRET) * 1e9);
			P2P_LKG_DESL->SetTestResult(SITE, 0, IS_DESL_VI.GetMeasResult(SITE, MIRET) * 1e9);
			P2P_LKG_VS->SetTestResult(SITE, 0, KELVIN_VS.GetMeasResult(SITE, MIRET) * 1e9);
			P2P_LKG_NC_2->SetTestResult(SITE, 0, NC_VI.GetMeasResult(SITE, MIRET) * 1e9);
        }

        //---------------------fpvi组
		IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
		KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

		KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
		KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
		delay_ms(2);

		ALL_KELVIN_FPVI.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
		ALL_KELVIN_FPVI.Set(FV, 0.05, FPVI10_2V, FPVI10_100UA, RELAY_ON);
        delay_ms(5);
        ALL_KELVIN_FPVI.MeasureVI(20, 10);
        SERIAL
        {
			P2P_LKG_OUT_0->SetTestResult(SITE, 0, KELVIN_OUT0.GetMeasResult(SITE, MIRET) * 1e9);
			P2P_LKG_OUT_1->SetTestResult(SITE, 0, KELVIN_OUT1.GetMeasResult(SITE, MIRET) * 1e9);
        }
    }
    else
    {
        Meas_P2P_LEAK(IN0_VI, P2P_LKG_IN_0, Pre_P2P_LEAK, 0); // 使用封装的Leakage测试函数
        Meas_P2P_LEAK(IN1_VI, P2P_LKG_IN_1, Pre_P2P_LEAK, 1);

        ////=========== P2P_LKG_IN_0 	================
        //IN0_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
        //IN0_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
        //IN0_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
        //delay_us(1000);

        //IN0_VI.MeasureVI(20, 10);
        //SERIAL Results[SITE] = IN0_VI.GetMeasResult(SITE, MIRET);
        //SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        //SERIAL Pre_P2P_LEAK[0][SITE] = Results[SITE];
        //SERIAL P2P_LKG_IN_0->SetTestResult(SITE, 0, Results[SITE]);
        //IN0_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
        //delay_us(100);

        ////=========== P2P_LKG_IN_1 	================
        //IN1_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
        //IN1_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
        //IN1_VI.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
        //delay_us(1000);

        //IN1_VI.MeasureVI(20, 10);
        //SERIAL Results[SITE] = IN1_VI.GetMeasResult(SITE, MIRET);
        //SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        //SERIAL Pre_P2P_LEAK[1][SITE] = Results[SITE];
        //SERIAL P2P_LKG_IN_1->SetTestResult(SITE, 0, Results[SITE]);
        //IN1_VI.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
        //delay_us(100);

        //=========== P2P_LKG_DEN 	================
        DEN_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        DEN_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        DEN_VI.MeasureVI(20, 10);
        SERIAL Results[SITE] = DEN_VI.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[2][SITE] = Results[SITE];
        SERIAL P2P_LKG_DEN->SetTestResult(SITE, 0, Results[SITE]);
        DEN_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_us(100);


        //=========== P2P_LKG_IS  	================
        IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        IS_DESL_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        IS_DESL_VI.MeasureVI(20, 10);
        SERIAL Results[SITE] = IS_DESL_VI.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[3][SITE] = Results[SITE];
        SERIAL P2P_LKG_IS->SetTestResult(SITE, 0, Results[SITE]);
        IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_us(100);

        //=========== P2P_LKG_DESL 	================
        cbit.SetOn(K4_NC_IS_DESL_VI, -1); // 闭合K4A，切换 IS_DESL FOVI5资源
        delay_ms(4);

        IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        IS_DESL_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        IS_DESL_VI.MeasureVI(20, 10);
        SERIAL Results[SITE] = IS_DESL_VI.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[4][SITE] = Results[SITE];
        SERIAL P2P_LKG_DESL->SetTestResult(SITE, 0, Results[SITE]);
        IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_us(100);

        //=========== P2P_LKG_NC_2  	================
        NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        NC_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        NC_VI.MeasureVI(20, 10);
        SERIAL Results[SITE] = NC_VI.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[5][SITE] = Results[SITE];
        SERIAL P2P_LKG_NC_2->SetTestResult(SITE, 0, Results[SITE]);
        NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_us(100);

        //=========== P2P_LKG_NC 	================
        cbit.SetOn(-1); // 断开K4B，切换 NC_NC2 FOVI3资源
        delay_ms(4);

        NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        NC_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        NC_VI.MeasureVI(20, 10);
        SERIAL Results[SITE] = NC_VI.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[6][SITE] = Results[SITE];
        SERIAL P2P_LKG_NC->SetTestResult(SITE, 0, Results[SITE]);
        NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_us(100);

        //=========== P2P_LKG_OUT_0	================
        KELVIN_OUT0.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
        KELVIN_OUT0.Set(FV, 0.05, FPVI10_2V, FPVI10_100UA, RELAY_ON);
        delay_ms(5);
        KELVIN_OUT0.MeasureVI(20, 10);
        SERIAL Results[SITE] = KELVIN_OUT0.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[7][SITE] = Results[SITE];
        SERIAL P2P_LKG_OUT_0->SetTestResult(SITE, 0, Results[SITE]);
        KELVIN_OUT0.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
        delay_us(100);

        //=========== P2P_LKG_OUT_1	================
        KELVIN_OUT1.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
        KELVIN_OUT1.Set(FV, 0.05, FPVI10_2V, FPVI10_100UA, RELAY_ON);
        delay_ms(5);
        KELVIN_OUT1.MeasureVI(20, 10);
        SERIAL Results[SITE] = KELVIN_OUT1.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[8][SITE] = Results[SITE];
        SERIAL P2P_LKG_OUT_1->SetTestResult(SITE, 0, Results[SITE]);
        KELVIN_OUT1.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
        delay_us(100);

        //=========== P2P_LKG_VS  	================
        KELVIN_VS.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        KELVIN_VS.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_ms(5);
        KELVIN_VS.MeasureVI(20, 10);
        SERIAL Results[SITE] = KELVIN_VS.GetMeasResult(SITE, MIRET);
        SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
        SERIAL Pre_P2P_LEAK[9][SITE] = Results[SITE];
        SERIAL P2P_LKG_VS->SetTestResult(SITE, 0, Results[SITE]);
        KELVIN_VS.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
        delay_us(100);
    }

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}

DUT_API int TEST05_PRE_IQ(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *IQ_VS_Isd = StsGetParam(funcindex, "IQ_VS_Isd");
    CParam *IQ_VS_Iq = StsGetParam(funcindex, "IQ_VS_Iq");
//}}AFX_STS_PARAM_PROTOTYPES

        // TODO: Add your function code here
    double Results_Isd[SITE_NUM] = { -9999, -9999, -9999, -9999 }; //临时存放测试结果 
    double Results_Iq[SITE_NUM] = { -9999, -9999, -9999, -9999 }; //临时存放测试结果 

    //------------------resource initial------------------------------//
    IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON); // IS floating
    KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //----------------IQ_VS_Isd
    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_10MA, RELAY_ON);
    delay_ms(2);
    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_1UA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    KELVIN_VS.MeasureVI(200, 10);
    SERIAL Results_Isd[SITE] = KELVIN_VS.GetMeasResult(SITE, MIRET);
    SERIAL Results_Isd[SITE] = Results_Isd[SITE] * 1e6; // uA
    SERIAL Pre_IQ[0][SITE] = Results_Isd[SITE];
    SERIAL IQ_VS_Isd->SetTestResult(SITE, 0, Results_Isd[SITE]);

    //----------------IQ_VS_Iq
	//cbit.SetOn(K7_DenPullUp, -1); // VS的电容是否要接入？
	//delay_ms(2);

	PullUp_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_10MA, RELAY_ON);
    DEN_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    KELVIN_VS.MeasureVI(200, 10);
    SERIAL Results_Iq[SITE] = KELVIN_VS.GetMeasResult(SITE, MIRET);
    SERIAL Results_Iq[SITE] = Results_Iq[SITE] * 1e3; // mA
    SERIAL Pre_IQ[1][SITE] = Results_Iq[SITE];
    SERIAL IQ_VS_Iq->SetTestResult(SITE, 0, Results_Iq[SITE]);

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}

DUT_API int TEST09_IN_VTH(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *IN0_VTH_Rising = StsGetParam(funcindex, "IN0_VTH_Rising");
    CParam *IN0_VTH_Falling = StsGetParam(funcindex, "IN0_VTH_Falling");
    CParam *IN1_VTH_Rising = StsGetParam(funcindex, "IN1_VTH_Rising");
    CParam *IN1_VTH_Falling = StsGetParam(funcindex, "IN1_VTH_Falling");
//}}AFX_STS_PARAM_PROTOTYPES
        // TODO: Add your function code here
	
    IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON); // IS floating
    KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    double Results[SITE_NUM] = { 0.0 }; //临时存放测试结果


    //------------------resource initial------------------------------//
	cbit.SetOn(K11_OutPullDown, K7_DenPullUp, -1);
    PullUp_VI.Set(FI, 0, FOVI_10V, FOVI_1UA, RELAY_ON);
	delay_ms(2);

    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_100MA, RELAY_ON);
    DEN_VI.Set(FV, 5, FOVI_10V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

	//----------------IN0_VTH_Rising
	Meas_IN_Vth(IN0_VI, KELVIN_OUT0, FOVI_10V, FOVI_10MA, 0.0, 3.0, 0.01, 100, 6.75, TRIG_RISING, Results); // 50% vs = 6.75V
	SERIAL IN0_VTH_Rising->SetTestResult(SITE, 0, Results[SITE]);
	//----------------IN0_VTH_Falling
	Meas_IN_Vth(IN0_VI, KELVIN_OUT0, FOVI_10V, FOVI_10MA, 3.0, 0.0, 0.01, 100, 2.7, TRIG_FALLING, Results); // 20% vs = 2.7V
	SERIAL IN0_VTH_Falling->SetTestResult(SITE, 0, Results[SITE]);

    //----------------IN1_VTH_Rising
    Meas_IN_Vth(IN1_VI, KELVIN_OUT1, FOVI_5V, FOVI_10MA, 0.0, 3.0, 0.01, 100, 6.75, TRIG_RISING, Results); // 50% vs = 6.75V
    SERIAL IN1_VTH_Rising->SetTestResult(SITE, 0, Results[SITE]);
    //----------------IN1_VTH_Falling
    Meas_IN_Vth(IN1_VI, KELVIN_OUT1, FOVI_5V, FOVI_10MA, 3.0, 0.0, 0.01, 100, 2.7, TRIG_FALLING, Results); // 20% vs = 2.7V
    SERIAL IN1_VTH_Falling->SetTestResult(SITE, 0, Results[SITE]);

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}

DUT_API int TEST10_Rdson(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *Rdson_VS_OUT0_Pre = StsGetParam(funcindex, "Rdson_VS_OUT0_Pre");
    CParam *Rdson_VS_OUT1_Pre = StsGetParam(funcindex, "Rdson_VS_OUT1_Pre");
//}}AFX_STS_PARAM_PROTOTYPES
        // TODO: Add your function code here

	IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
	cbit.SetOn(-1);
    delay_ms(2);

    double Results_0[SITE_NUM] = {999}; //临时存放测试结果 
    double Results_1[SITE_NUM] = {999}; //临时存放测试结果 
    double Results_i[SITE_NUM] = {999}; //临时存放测试结果 

	//cbitclose(K9_VS_FPVI1);
	//delay_ms(2);
	
    cbit.SetOn(K9_VS_FPVI1, K7_DenPullUp, -1);
    PullUp_VI.Set(FI, 0, FOVI_10V, FOVI_1UA, RELAY_ON);
    delay_ms(2);

	KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_100MA, RELAY_ON);
	DEN_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
    IN0_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(5);

	KELVIN_OUT0.Set(FI, -1, FPVI10_2V, FPVI10_10A, RELAY_ON); // 电压量程选择有待确定？
	delay_ms(2);

    KELVIN_OUT0.MeasureVI(50, 20);
	delay_ms(1);
	KELVIN_OUT0.Set(FI, 0, FPVI10_2V, FPVI10_10A, RELAY_ON);
    SERIAL Results_0[SITE] = KELVIN_OUT0.GetMeasResult(SITE, MVRET);
    SERIAL Results_i[SITE] = KELVIN_OUT0.GetMeasResult(SITE, MIRET);

    SERIAL Results_0[SITE] = (Results_0[SITE] / Results_i[SITE] ) * 1e3; // mohm
    SERIAL Rdson_VS_OUT0_Pre->SetTestResult(SITE, 0, Results_0[SITE]);
	
    KELVIN_OUT0.Set(FI, 0, FPVI10_2V, FPVI10_10A, RELAY_ON);
    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_10A, RELAY_ON);
	IN0_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(2);
	cbitopen(K9_VS_FPVI1);
	delay_ms(2);

	//---------------------------------------------------
	//cbitclose(K8_VS_FPVI0);
	//delay_ms(2);

    cbit.SetOn(K8_VS_FPVI0, K7_DenPullUp, -1);
    PullUp_VI.Set(FI, 0, FOVI_10V, FOVI_1UA, RELAY_ON);
    delay_ms(2);

	KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_100MA, RELAY_ON);
	DEN_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
    IN1_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
	KELVIN_OUT1.Set(FI, -1, FPVI10_20V, FPVI10_10A, RELAY_ON); // 电压量程选择有待确定？
	delay_ms(2);
    KELVIN_OUT1.MeasureVI(50, 10);
	delay_ms(1);
	KELVIN_OUT1.Set(FI, 0, FPVI10_20V, FPVI10_10A, RELAY_ON);

    SERIAL Results_1[SITE] = KELVIN_OUT1.GetMeasResult(SITE, MVRET);
	SERIAL Results_i[SITE] = KELVIN_OUT1.GetMeasResult(SITE, MIRET);

    SERIAL Results_1[SITE] = (Results_1[SITE] /Results_i[SITE]) * 1e3; // mohm
    SERIAL Rdson_VS_OUT1_Pre->SetTestResult(SITE, 0, Results_1[SITE]);

	KELVIN_OUT1.Set(FI, 0, FPVI10_20V, FPVI10_10A, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_20V, FPVI10_10A, RELAY_ON);
	IN1_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(2);
	cbitopen(K8_VS_FPVI0);
	delay_ms(2);

    //=====================POWER_DOWN===========================
    // 上下电顺序，先关大电流 再关vin
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
	cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}

DUT_API int TEST11_TIME(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *T_IN0_OUT0 = StsGetParam(funcindex, "T_IN0_OUT0");
    CParam *T_IN1_OUT1 = StsGetParam(funcindex, "T_IN1_OUT1");
    CParam *OUT0_T_delay_rise = StsGetParam(funcindex, "OUT0_T_delay_rise");
    CParam *OUT1_T_delay_rise = StsGetParam(funcindex, "OUT1_T_delay_rise");
    CParam *OUT0_T_delay_fall = StsGetParam(funcindex, "OUT0_T_delay_fall");
    CParam *OUT1_T_delay_fall = StsGetParam(funcindex, "OUT1_T_delay_fall");
//}}AFX_STS_PARAM_PROTOTYPES
        // TODO: Add your function code here
    double Results[SITE_NUM] = { 0.0 }; //临时存放测试结果
    // -----------------------闭合K6, QTMU默认连接 IN0 OUT0
    cbit.SetOn(K7_DenPullUp, K6_QTMU, K11_OutPullDown, K30_QTMU_AB, -1);
    PullUp_VI.Set(FI, 0, FOVI_10V, FOVI_1UA, RELAY_ON);
    delay_ms(2);

	KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_100MA, RELAY_ON);
    DEN_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);

	IN0_VI.Set(FI, 0, FOVI_10V, FOVI_1UA, RELAY_ON);
	IN1_VI.Set(FI, 0, FOVI_10V, FOVI_1UA, RELAY_ON);

	KELVIN_OUT0.Set(FI, 0, FPVI10_20V, FPVI10_10UA, RELAY_ON);
	KELVIN_OUT1.Set(FI, 0, FPVI10_20V, FPVI10_10UA, RELAY_ON);
    delay_ms(2);

    QTMU_MEAS.Init();
    //dio_plus.Init();
    dio_plus.Connect();
    dio_plus.SetVIH(5.0); //p-p hi
    dio_plus.SetVIL(0.0); //p-p low	
    delay_ms(4);

    //---------------------测试非周期信号 IN 和 OUT 之间的传输延时,将 IN 接入通道 0 的 CHB, OUT 接入通道 0 的 CHA
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(6.75, QTMU_PLUS_POS_SLOPE); //trigger =50% VS, Rising edge
    QTMU_MEAS.SetStopTrigger(2.5, QTMU_PLUS_POS_SLOPE);   //trigger =50% * 5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_DUAL_SOURCE);
    QTMU_MEAS.ChannelSetup(QTMU_PLUS_CHA_STOP); // set CHB as start signal
    QTMU_MEAS.Connect();
    delay_ms(1);

    QTMU_MEAS.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, 0); // 执行此函数来启动非周期信号的时间测量
    QTMU_MEAS.SetTimeOut(10); // 函数来设置最大测量等待时间
    dio_plus.Run("a1", "a2"); // IN0 signal input 
    QTMU_MEAS.SinglePlsMeas(0); // 执行此函数来完成非周期信号的时间测量。
    SERIAL Results[SITE] = QTMU_MEAS.GetMeasureResult(SITE); // us
    SERIAL T_IN0_OUT0->SetTestResult(SITE, 0, Results[SITE]);

    //-----------------------测试 OUT0 端非周期信号的上升沿时间 OUT0_T_delay_rise，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(2.7, QTMU_PLUS_POS_SLOPE); // trigger = 20% VS ， Rising edge
    QTMU_MEAS.SetStopTrigger(10.8, QTMU_PLUS_POS_SLOPE); // trigger = 80% VS ， Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);//SINGLE_SOURCE ，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.ChannelSetup(QTMU_PLUS_CHA_START); // set CHB as start signal   
	QTMU_MEAS.Connect();
    delay_ms(1);

    QTMU_MEAS.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, 0); // 执行此函数来启动非周期信号的时间测量
    QTMU_MEAS.SetTimeOut(10); // 函数来设置最大测量等待时间
    dio_plus.Run("a1", "a2"); // IN0 signal input 
    QTMU_MEAS.SinglePlsMeas(0); // 执行此函数来完成非周期信号的时间测量。
    SERIAL Results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);  // us
    SERIAL OUT0_T_delay_rise->SetTestResult(SITE, 0, Results[SITE]); // 完成  OUT1 端非周期信号的上升沿时间

    //-----------------------测试 OUT0 端非周期信号的下降沿时间 OUT0_T_delay_fall，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(10.8, QTMU_PLUS_NEG_SLOPE); // trigger = 80% VS ， falling edge
    QTMU_MEAS.SetStopTrigger(2.7, QTMU_PLUS_NEG_SLOPE); // trigger = 20% VS ， falling edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);//SINGLE_SOURCE ，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.Connect();
    delay_ms(1);

    QTMU_MEAS.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, 0); // 执行此函数来启动非周期信号的时间测量
    QTMU_MEAS.SetTimeOut(10); // 函数来设置最大测量等待时间
    dio_plus.Run("a1", "a2"); // IN0 signal input 
    QTMU_MEAS.SinglePlsMeas(0); // 执行此函数来完成非周期信号的时间测量。
    SERIAL Results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);  // us
    SERIAL OUT0_T_delay_fall->SetTestResult(SITE, 0, Results[SITE]);

    // -------------------------闭合K5，QTMU连接 IN1 OUT1
    cbit.SetOn(K5_QTMU_SWITCH, K6_QTMU, K11_OutPullDown, K30_QTMU_AB, -1);
    delay_ms(2);

	QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(6.75, QTMU_PLUS_POS_SLOPE); //trigger =50% VS, Rising edge
    QTMU_MEAS.SetStopTrigger(2.5, QTMU_PLUS_POS_SLOPE);   //trigger =50% * 5V, Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_DUAL_SOURCE);
    QTMU_MEAS.ChannelSetup(QTMU_PLUS_CHA_STOP); // set CHB as start signal
    QTMU_MEAS.Connect();
    delay_ms(1);

    QTMU_MEAS.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, 0); // 执行此函数来启动非周期信号的时间测量
    QTMU_MEAS.SetTimeOut(10); // 函数来设置最大测量等待时间
    dio_plus.Run("b1", "b2"); // IN1 signal input 
    QTMU_MEAS.SinglePlsMeas(0); // 执行此函数来完成非周期信号的时间测量。
    SERIAL Results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);  // us
    SERIAL T_IN1_OUT1->SetTestResult(SITE, 0, Results[SITE]); // 完成 测试非周期信号 IN1 和 OUT1 之间的传输延时

    //-----------------------测试 OUT1 端非周期信号的上升沿时间 OUT1_T_delay_rise，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(2.7, QTMU_PLUS_POS_SLOPE); // trigger = 20% VS ， Rising edge
    QTMU_MEAS.SetStopTrigger(10.8, QTMU_PLUS_POS_SLOPE); // trigger = 80% VS ， Rising edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);//SINGLE_SOURCE ，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.ChannelSetup(QTMU_PLUS_CHA_START); // set CHB as start signal  
    QTMU_MEAS.Connect();
    delay_ms(1);

    QTMU_MEAS.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, 0); // 执行此函数来启动非周期信号的时间测量
    QTMU_MEAS.SetTimeOut(10); // 函数来设置最大测量等待时间
    dio_plus.Run("b1", "b2"); // IN1 signal input 
    QTMU_MEAS.SinglePlsMeas(0); // 执行此函数来完成非周期信号的时间测量。
    SERIAL Results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);  // us
    SERIAL OUT1_T_delay_rise->SetTestResult(SITE, 0, Results[SITE]); // 完成  OUT1 端非周期信号的上升沿时间

    //-----------------------测试 OUT1 端非周期信号的下降沿时间 OUT1_T_delay_fall，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
    QTMU_MEAS.SetStartTrigger(10.8, QTMU_PLUS_NEG_SLOPE); // trigger = 80% VS ， falling edge
    QTMU_MEAS.SetStopTrigger(2.7, QTMU_PLUS_NEG_SLOPE); // trigger = 20% VS ， falling edge
    QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);//SINGLE_SOURCE ，将被测信号接入 QTMU 通道的 CHA.
    QTMU_MEAS.Connect();
    delay_ms(1);

    QTMU_MEAS.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, 0); // 执行此函数来启动非周期信号的时间测量
    QTMU_MEAS.SetTimeOut(10); // 函数来设置最大测量等待时间
    dio_plus.Run("b1", "b2"); // IN1 signal input 
    QTMU_MEAS.SinglePlsMeas(0); // 执行此函数来完成非周期信号的时间测量。
    SERIAL Results[SITE] = QTMU_MEAS.GetMeasureResult(SITE);  // us
    SERIAL OUT1_T_delay_fall->SetTestResult(SITE, 0, Results[SITE]);

    QTMU_MEAS.Disconnect();
    dio_plus.Disconnect();

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);


    return 0;
}

DUT_API int TEST13_POST_IQ(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *POST_IQ_VS_Isd = StsGetParam(funcindex, "POST_IQ_VS_Isd");
    CParam *POST_IQ_VS_Iq = StsGetParam(funcindex, "POST_IQ_VS_Iq");
    CParam *Delta_IQ_VS_Isd = StsGetParam(funcindex, "Delta_IQ_VS_Isd");
    CParam *Delta_IQ_VS_Iq = StsGetParam(funcindex, "Delta_IQ_VS_Iq");
//}}AFX_STS_PARAM_PROTOTYPES
        // TODO: Add your function code here

    double Results_Isd[SITE_NUM] = { -9999, -9999, -9999, -9999 }; //临时存放测试结果 
    double Results_Iq[SITE_NUM] = { -9999, -9999, -9999, -9999 }; //临时存放测试结果 

    //------------------resource initial------------------------------//

    IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON); // IS floating
    KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //----------------IQ_VS_Isd
    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_10MA, RELAY_ON);
    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_1UA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    delay_ms(2);

    KELVIN_VS.MeasureVI(200, 10);
    SERIAL Results_Isd[SITE] = KELVIN_VS.GetMeasResult(SITE, MIRET);
    SERIAL Results_Isd[SITE] = Results_Isd[SITE] * 1e6; // uA
    SERIAL POST_IQ_VS_Isd->SetTestResult(SITE, 0, Results_Isd[SITE]);
	SERIAL Delta_IQ_VS_Isd->SetTestResult(SITE, 0, Results_Isd[SITE] - Pre_IQ[0][SITE]);

    //----------------IQ_VS_Iq
	PullUp_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_10MA, RELAY_ON);
    DEN_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

    KELVIN_VS.MeasureVI(200, 10);
    SERIAL Results_Iq[SITE] = KELVIN_VS.GetMeasResult(SITE, MIRET);
    SERIAL Results_Iq[SITE] = Results_Iq[SITE] * 1e3; // mA
    SERIAL POST_IQ_VS_Iq->SetTestResult(SITE, 0, Results_Iq[SITE]);
    SERIAL Delta_IQ_VS_Iq->SetTestResult(SITE, 0, Results_Iq[SITE] - Pre_IQ[1][SITE]);

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);


    return 0;
}

DUT_API int TEST14_POST_P2P_Leakage(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *POST_P2P_LKG_IN_0 = StsGetParam(funcindex, "POST_P2P_LKG_IN_0");
    CParam *POST_P2P_LKG_IN_1 = StsGetParam(funcindex, "POST_P2P_LKG_IN_1");
    CParam *POST_P2P_LKG_DEN = StsGetParam(funcindex, "POST_P2P_LKG_DEN");
    CParam *POST_P2P_LKG_IS = StsGetParam(funcindex, "POST_P2P_LKG_IS");
    CParam *POST_P2P_LKG_DESL = StsGetParam(funcindex, "POST_P2P_LKG_DESL");
    CParam *POST_P2P_LKG_NC = StsGetParam(funcindex, "POST_P2P_LKG_NC");
    CParam *POST_P2P_LKG_NC_2 = StsGetParam(funcindex, "POST_P2P_LKG_NC_2");
    CParam *POST_P2P_LKG_OUT_0 = StsGetParam(funcindex, "POST_P2P_LKG_OUT_0");
    CParam *POST_P2P_LKG_OUT_1 = StsGetParam(funcindex, "POST_P2P_LKG_OUT_1");
    CParam *POST_P2P_LKG_VS = StsGetParam(funcindex, "POST_P2P_LKG_VS");
    CParam *Delta_P2P_LKG_IN_0 = StsGetParam(funcindex, "Delta_P2P_LKG_IN_0");
    CParam *Delta_P2P_LKG_IN_1 = StsGetParam(funcindex, "Delta_P2P_LKG_IN_1");
    CParam *Delta_P2P_LKG_DEN = StsGetParam(funcindex, "Delta_P2P_LKG_DEN");
    CParam *Delta_P2P_LKG_IS = StsGetParam(funcindex, "Delta_P2P_LKG_IS");
    CParam *Delta_P2P_LKG_DESL = StsGetParam(funcindex, "Delta_P2P_LKG_DESL");
    CParam *Delta_P2P_LKG_NC = StsGetParam(funcindex, "Delta_P2P_LKG_NC");
    CParam *Delta_P2P_LKG_NC_2 = StsGetParam(funcindex, "Delta_P2P_LKG_NC_2");
    CParam *Delta_P2P_LKG_OUT_0 = StsGetParam(funcindex, "Delta_P2P_LKG_OUT_0");
    CParam *Delta_P2P_LKG_OUT_1 = StsGetParam(funcindex, "Delta_P2P_LKG_OUT_1");
    CParam *Delta_P2P_LKG_VS = StsGetParam(funcindex, "Delta_P2P_LKG_VS");
//}}AFX_STS_PARAM_PROTOTYPES
        // TODO: Add your function code here
    double Results[SITE_NUM] = { -9999, -9999, -9999, -9999 };

    IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
    KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

    KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
    delay_ms(2);

    //=========== P2P_LKG_IN_0 	================
    IN0_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    IN0_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    IN0_VI.MeasureVI(20, 10);
    SERIAL Results[SITE] = IN0_VI.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    SERIAL POST_P2P_LKG_IN_0->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_IN_0->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[0][SITE]);
    IN0_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_IN_1 	================
    IN1_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    IN1_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    IN1_VI.MeasureVI(20, 10);
    SERIAL Results[SITE] = IN1_VI.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    SERIAL POST_P2P_LKG_IN_1->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_IN_1->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[1][SITE]);
    IN1_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_DEN 	================
    DEN_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    DEN_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    DEN_VI.MeasureVI(20, 10);
    SERIAL Results[SITE] = DEN_VI.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[2][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_DEN->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_DEN->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[2][SITE]);
    DEN_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_IS  	================
    IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    IS_DESL_VI.MeasureVI(20, 10);
    SERIAL Results[SITE] = IS_DESL_VI.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[3][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_IS->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_IS->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[3][SITE]);
    IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_DESL 	================
    cbit.SetOn(K4_NC_IS_DESL_VI, -1); // 闭合K4A，切换 IS_DESL FOVI5资源
    delay_ms(4);

    IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    IS_DESL_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    IS_DESL_VI.MeasureVI(20, 10);
    SERIAL Results[SITE] = IS_DESL_VI.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[4][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_DESL->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_DESL->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[4][SITE]);
    IS_DESL_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_NC_2  	================
    NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    NC_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    NC_VI.MeasureVI(20, 10);
    SERIAL Results[SITE] = NC_VI.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[5][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_NC_2->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_NC_2->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[5][SITE]);
    NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_NC 	================
    cbit.SetOn(-1); // 断开K4B，切换 NC_NC2 FOVI3资源
    delay_ms(4);

    NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    NC_VI.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    NC_VI.MeasureVI(20, 10);
    SERIAL Results[SITE] = NC_VI.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[6][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_NC->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_NC->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[6][SITE]);
    NC_VI.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_OUT_0	================
    KELVIN_OUT0.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
    KELVIN_OUT0.Set(FV, 0.05, FPVI10_2V, FPVI10_100UA, RELAY_ON);
    delay_ms(5);
    KELVIN_OUT0.MeasureVI(20, 10);
    SERIAL Results[SITE] = KELVIN_OUT0.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[7][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_OUT_0->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_OUT_0->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[7][SITE]);
    KELVIN_OUT0.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_OUT_1	================
    KELVIN_OUT1.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
    KELVIN_OUT1.Set(FV, 0.05, FPVI10_2V, FPVI10_100UA, RELAY_ON);
    delay_ms(5);
    KELVIN_OUT1.MeasureVI(20, 10);
    SERIAL Results[SITE] = KELVIN_OUT1.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[8][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_OUT_1->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_OUT_1->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[8][SITE]);
    KELVIN_OUT1.Set(FV, 0.0, FPVI10_2V, FPVI10_100UA, RELAY_ON);
    delay_us(100);

    //=========== P2P_LKG_VS  	================
    KELVIN_VS.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    KELVIN_VS.Set(FV, 0.05, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_ms(5);
    KELVIN_VS.MeasureVI(20, 10);
    SERIAL Results[SITE] = KELVIN_VS.GetMeasResult(SITE, MIRET);
    SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
    //SERIAL Pre_P2P_LEAK[9][SITE] = Results[SITE];
    SERIAL POST_P2P_LKG_VS->SetTestResult(SITE, 0, Results[SITE]);
    SERIAL Delta_P2P_LKG_VS->SetTestResult(SITE, 0, Results[SITE] - Pre_P2P_LEAK[9][SITE]);
    KELVIN_VS.Set(FV, 0.0, FOVI_2V, FOVI_100UA, RELAY_ON);
    delay_us(100);

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    delay_ms(3);

    return 0;
}
 
 
DUT_API int TEST15_POST_CONTACT(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *POST_OS_SHORT = StsGetParam(funcindex, "POST_OS_SHORT");
    CParam *POST_OS_SOME_OPEN = StsGetParam(funcindex, "POST_OS_SOME_OPEN");
    CParam *POST_OS_ALL_OPEN = StsGetParam(funcindex, "POST_OS_ALL_OPEN");
    CParam *POST_Contact_IN_0 = StsGetParam(funcindex, "POST_Contact_IN_0");
    CParam *POST_Contact_IN_1 = StsGetParam(funcindex, "POST_Contact_IN_1");
    CParam *POST_Contact_DEN = StsGetParam(funcindex, "POST_Contact_DEN");
    CParam *POST_Contact_IS = StsGetParam(funcindex, "POST_Contact_IS");
    CParam *POST_Contact_DESL = StsGetParam(funcindex, "POST_Contact_DESL");
    CParam *POST_Contact_NC = StsGetParam(funcindex, "POST_Contact_NC");
    CParam *POST_Contact_NC_2 = StsGetParam(funcindex, "POST_Contact_NC_2");
    CParam *POST_Contact_OUT_0 = StsGetParam(funcindex, "POST_Contact_OUT_0");
    CParam *POST_Contact_OUT_1 = StsGetParam(funcindex, "POST_Contact_OUT_1");
    CParam *POST_Contact_VS = StsGetParam(funcindex, "POST_Contact_VS");
//}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here


	CParam* OS_DLOG[10];  // 定义引脚参数数组

	// 初始化引脚参数
	OS_DLOG[0] = StsGetParam(funcindex, "POST_Contact_IN_0");
	OS_DLOG[1] = StsGetParam(funcindex, "POST_Contact_IN_1");
	OS_DLOG[2] = StsGetParam(funcindex, "POST_Contact_DEN");
	OS_DLOG[3] = StsGetParam(funcindex, "POST_Contact_IS");
	OS_DLOG[4] = StsGetParam(funcindex, "POST_Contact_DESL");
	OS_DLOG[5] = StsGetParam(funcindex, "POST_Contact_NC");
	OS_DLOG[6] = StsGetParam(funcindex, "POST_Contact_NC_2");
	OS_DLOG[7] = StsGetParam(funcindex, "POST_Contact_OUT_0");
	OS_DLOG[8] = StsGetParam(funcindex, "POST_Contact_OUT_1");
	OS_DLOG[9] = StsGetParam(funcindex, "POST_Contact_VS");

	// 定义状态标志数组
	int open_count[SITE_NUM] = { 0, 0, 0, 0 };              // 开路计数
	int SHORT_FLAG[SITE_NUM] = { 0, 0, 0, 0 };              // 短路标志
	int SOME_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };          // 部分开路标志
	int ALL_OPEN_FLAG[SITE_NUM] = { 0, 0, 0, 0 };           // 全部开路标志
	int LEAK_FLAG[SITE_NUM] = { 0, 0, 0, 0 };               // 漏电标志
	int OS_DLOG_NUM = 10;                                   // 引脚数量
	double os_result[10][SITE_NUM] = { 0, 0, 0, 0 };         // 测试结果存储
	int os_count = 0;                                       // 循环计数器

	double results[SITE_NUM] = { 0, 0, 0, 0 };              // 临时结果存储

	IN0_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
	IN1_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
	DEN_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
	NC_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
	IS_DESL_VI.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);
	KELVIN_VS.Set(FV, 0, FOVI_2V, FOVI_1MA, RELAY_ON);

	KELVIN_OUT0.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
	KELVIN_OUT1.Set(FV, 0, FPVI10_2V, FPVI10_1MA, RELAY_ON);
	delay_ms(2);

	//=====================Contact_IN_0===========================0
	IN0_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	IN0_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON); // -0.1mA
	delay_ms(1);
	IN0_VI.MeasureVI(20, 10);
	groupgetresults(IN0_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[0][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_IN_0, 0, results, SITE_NUM);
	IN0_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_IN_1===========================1
	IN1_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	IN1_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	IN1_VI.MeasureVI(20, 10);
	groupgetresults(IN1_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[1][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_IN_1, 0, results, SITE_NUM);
	IN1_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_DEN===========================2
	DEN_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	DEN_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	DEN_VI.MeasureVI(20, 10);
	groupgetresults(DEN_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[2][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_DEN, 0, results, SITE_NUM);
	DEN_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_IS===========================3
	IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	IS_DESL_VI.Set(FI, 1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	IS_DESL_VI.MeasureVI(20, 10);
	groupgetresults(IS_DESL_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[3][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_IS, 0, results, SITE_NUM);
	IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_DESL===========================4
	cbit.SetOn(K4_NC_IS_DESL_VI, -1); // 闭合K4A，切换 IS_DESL FOVI5资源
	delay_ms(4);

	IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	IS_DESL_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	IS_DESL_VI.MeasureVI(20, 10);
	groupgetresults(IS_DESL_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[4][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_DESL, 0, results, SITE_NUM);
	IS_DESL_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_NC_2===========================6
	NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	NC_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	NC_VI.MeasureVI(20, 10);
	groupgetresults(NC_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[6][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_NC_2, 0, results, SITE_NUM);
	NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_NC===========================5
	cbit.SetOn(-1); // 断开K4B，切换 NC_NC2 FOVI3资源
	delay_ms(4);

	NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	NC_VI.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	NC_VI.MeasureVI(20, 10);
	groupgetresults(NC_VI, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[5][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_NC, 0, results, SITE_NUM);
	NC_VI.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_OUT_0===========================7
	KELVIN_OUT0.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT0.Set(FI, 1e-4, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	KELVIN_OUT0.MeasureVI(20, 10);
	groupgetresults(KELVIN_OUT0, MVRET, results, SITE_NUM);
	SERIAL os_result[7][SITE] = results[SITE];
	msLogResultAll(POST_Contact_OUT_0, 0, results, SITE_NUM);
	KELVIN_OUT0.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_OUT_1===========================8
	KELVIN_OUT1.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT1.Set(FI, 1e-4, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	KELVIN_OUT1.MeasureVI(20, 10);
	groupgetresults(KELVIN_OUT1, MVRET, results, SITE_NUM);
	SERIAL os_result[8][SITE] = results[SITE];
	msLogResultAll(POST_Contact_OUT_1, 0, results, SITE_NUM);
	KELVIN_OUT1.Set(FI, 0, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	//=====================Contact_VS===========================9
	KELVIN_VS.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	KELVIN_VS.Set(FI, -1e-4, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	KELVIN_VS.MeasureVI(20, 10);
	groupgetresults(KELVIN_VS, MVRET, results, SITE_NUM);      // 将测试结果存入串行输出数组
	SERIAL os_result[9][SITE] = results[SITE];          // 记录所有site的测试结果
	msLogResultAll(POST_Contact_VS, 0, results, SITE_NUM);
	KELVIN_VS.Set(FI, 0, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

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
		if (SHORT_FLAG[SITE] != 1 && open_count[SITE] == OS_DLOG_NUM) { ALL_OPEN_FLAG[SITE] = 1; }
		else if (SHORT_FLAG[SITE] != 1 && open_count[SITE] < OS_DLOG_NUM && open_count[SITE]>0) { SOME_OPEN_FLAG[SITE] = 1; }
	}

	SERIAL	POST_OS_SHORT->SetTestResult(SITE, 0, SHORT_FLAG[SITE]);
	SERIAL	POST_OS_SOME_OPEN->SetTestResult(SITE, 0, SOME_OPEN_FLAG[SITE]);
	SERIAL	POST_OS_ALL_OPEN->SetTestResult(SITE, 0, ALL_OPEN_FLAG[SITE]);

    return 0;
}
 
DUT_API int TEST06_EE_INIT(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *EE0_READ = StsGetParam(funcindex, "EE0_READ");
    CParam *EE1_READ = StsGetParam(funcindex, "EE1_READ");
    CParam *EE2_READ = StsGetParam(funcindex, "EE2_READ");
    CParam *EE3_READ = StsGetParam(funcindex, "EE3_READ");
    CParam *EE4_READ = StsGetParam(funcindex, "EE4_READ");
    CParam *EE5_READ = StsGetParam(funcindex, "EE5_READ");
    CParam *EE6_READ = StsGetParam(funcindex, "EE6_READ");
    CParam *EE7_READ = StsGetParam(funcindex, "EE7_READ");
    CParam *VBG_READ = StsGetParam(funcindex, "VBG_READ");
    CParam *IB10U_READ = StsGetParam(funcindex, "IB10U_READ");
    CParam *OSCLF_READ = StsGetParam(funcindex, "OSCLF_READ");
    CParam *ILIMCH1_READ = StsGetParam(funcindex, "ILIMCH1_READ");
    CParam *ILIMCH2_READ = StsGetParam(funcindex, "ILIMCH2_READ");
    CParam *SR_READ = StsGetParam(funcindex, "SR_READ");
    CParam *CILIM_READ = StsGetParam(funcindex, "CILIM_READ");
    CParam *TSD_READ = StsGetParam(funcindex, "TSD_READ");
    CParam *TSW_READ = StsGetParam(funcindex, "TSW_READ");
    CParam *IOL_MORE_READ = StsGetParam(funcindex, "IOL_MORE_READ");
    CParam *CSCH1_READ = StsGetParam(funcindex, "CSCH1_READ");
    CParam *CSCH2_READ = StsGetParam(funcindex, "CSCH2_READ");
    CParam *Version_ID_READ = StsGetParam(funcindex, "Version_ID_READ");
    CParam *Lot_ID_READ = StsGetParam(funcindex, "Lot_ID_READ");
    CParam *CCL_FASTPD_wiCL_READ = StsGetParam(funcindex, "CCL_FASTPD_wiCL_READ");
    CParam *CLOFF_DELAY_READ = StsGetParam(funcindex, "CLOFF_DELAY_READ");
    CParam *ILIM_PD_READ = StsGetParam(funcindex, "ILIM_PD_READ");
    CParam *CLOFFCH_EN_READ = StsGetParam(funcindex, "CLOFFCH_EN_READ");
    CParam *CCL_PD_FAST_TIME_READ = StsGetParam(funcindex, "CCL_PD_FAST_TIME_READ");
    CParam *CL_VS_HALF_READ = StsGetParam(funcindex, "CL_VS_HALF_READ");
    CParam *CL_VS_READ = StsGetParam(funcindex, "CL_VS_READ");
    CParam *CL_VS_DOWN_READ = StsGetParam(funcindex, "CL_VS_DOWN_READ");
    CParam *INVERSEON_DIS_READ = StsGetParam(funcindex, "INVERSEON_DIS_READ");
    CParam *COUNTER20_READ = StsGetParam(funcindex, "COUNTER20_READ");
    CParam *VERA_READ = StsGetParam(funcindex, "VERA_READ");
    CParam *BURN_LOCK_READ = StsGetParam(funcindex, "BURN_LOCK_READ");
    CParam *Trim_Flag = StsGetParam(funcindex, "Trim_Flag");
//}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here

    cbit.SetOn(K4_NC_IS_DESL_VI, K12_NC_IN1, -1);// 切换VI源到 NC，DESL
    delay_ms(2);

    //int rfuse_data[SITE_NUM] = {0};
    bool bOK = false;
    int bank = 0;

    Enter_TM();

    for (int bank = 0; bank < BANK_NUM; ++bank)
    {
        EE_Read(bank, EE_READ, bOK); // read bank
    }
    
    SERIAL
    {
        EE0_READ->SetTestResult(SITE, 0, EE_READ[SITE][0]);
        EE1_READ->SetTestResult(SITE, 0, EE_READ[SITE][1]);
        EE2_READ->SetTestResult(SITE, 0, EE_READ[SITE][2]);
        EE3_READ->SetTestResult(SITE, 0, EE_READ[SITE][3]);
        EE4_READ->SetTestResult(SITE, 0, EE_READ[SITE][4]);
        EE5_READ->SetTestResult(SITE, 0, EE_READ[SITE][5]);
        EE6_READ->SetTestResult(SITE, 0, EE_READ[SITE][6]);
        EE7_READ->SetTestResult(SITE, 0, EE_READ[SITE][7]);
      
        rfuse_data[SITE] = EE_READ[SITE][0] & 0X1F;  // VBG_READ: DTRM_BG_AVDDBG<4:0> → 低5位（0x1F）
        VBG_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][0] & 0XE0) >> 5; // IB10U_READ: DTRIM_IB10U_AVDD<2:0> → 高3位（0xE0，需右移5位还原值）
        IB10U_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);
       
        rfuse_data[SITE] = EE_READ[SITE][1] & 0X07; // OSCLF_READ: DTRIM_OSCLF_DVDD<2:0> → 低3位（0x07）
        OSCLF_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);
        
        rfuse_data[SITE] = (EE_READ[SITE][1] & 0x08) >> 3; // ILIMCH1_READ: DTRIM_ILIMCH1_AVDD<0> → Bit 3（0x08掩码，再右移3位）
        rfuse_data[SITE] = rfuse_data[SITE] | ((EE_READ[SITE][2] & 0xE0) >> 4);// ILIMCH1_AVDD<3:1> → Bits 7-5（0xE0掩码，右移4位）
        ILIMCH1_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][1] & 0xF0) >> 4; // ILIMCH2_READ: DTRIM_ILIMCH2_AVDD<3:1><0> → Bits 7-4（0xF0掩码，右移4位）
        ILIMCH2_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][2] & 0x07); // SR_READ: DTRIM_SR_AVDD → Bit 0-2
        SR_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][2] & 0x18) >> 3;// CILIM_READ: DTRIM_CILIM_AVDD<1:0> → Bits 3-4（0x18掩码，右移3位）
        CILIM_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);
 
        rfuse_data[SITE] = EE_READ[SITE][3] & 0x07; // TSD_READ: DTRIM_TSD_AVDD<2:0> →  → 低3位（0x07）
        TSD_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);
        
        rfuse_data[SITE] = (EE_READ[SITE][3] & 0x38) >> 3;// TSW_READ: DTRIM_TSW_AVDD<2:0> → Bits 3-5（0x38掩码，右移3位）
        TSW_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][3] & 0xC0) >> 6; // IOL_MORE_READ: DTRIM_IOL_MORE_AVD → Bits 6-7（0xE0掩码，右移6位）
        IOL_MORE_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ[SITE][4] & 0x0F; // CSCH1_READ: DTRIM_CSCH1_AVDD<3:0> → Bits 0-3
        CSCH1_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][4] & 0xF0) >> 4;// CSCH2_READ: DTRIM_CSCH2_AVDD<3:0> → Bits 4-7
        CSCH2_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        // ----------------------------Version ID固定 burn为 001--------------------------------------
        rfuse_data[SITE] = EE_READ[SITE][5] & 0x07;// Version_ID: Version ID<2:0> → Bits 2-0（掩码 0x07）
        if (rfuse_data[SITE] == 1)
        {
            Trim_done_flag[SITE] = 1; 
        }
        Trim_Flag->SetTestResult(SITE, 0, Trim_done_flag[SITE]);
        Version_ID_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);
       
        rfuse_data[SITE] = (EE_READ[SITE][5] & 0xF8) >> 3; // Lot_ID: Lot ID<4:0> → Bits 7-3（掩码 0xF8，右移 3 位）
        Lot_ID_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ[SITE][6] & 0x03;// CCL_FASTPD_wiCL: DTRIM_CCL_FASTPD_wiCL<1:0> → Bits 1-0（掩码 0x03）
        CCL_FASTPD_wiCL_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][6] & 0x0C) >> 2;// CLOFF_DELAY: DTRIM_CLOFF_DELAY<1:0> → Bits 3-2（掩码 0x0C，右移 2 位）
        CLOFF_DELAY_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][6] & 0x30) >> 4;// ILIM_PD: DTRIM_ILIM_PD<1:0> → Bits 5-4（掩码 0x30，右移 4 位）
        rfuse_data[SITE] = rfuse_data[SITE] | ((EE_READ[SITE][7] & 0x08) >> 2);// 高位
        ILIM_PD_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);
        
        rfuse_data[SITE] = (EE_READ[SITE][6] & 0x40) >> 6;// CLOFFCH_EN: DTRIM_CLOFFC_H_EN → Bit 6（掩码 0x40，右移 6 位）
        CLOFFCH_EN_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][6] & 0x80) >> 7;// CCL_PD_FAST_TIME: DTRIM_CCL_PD_FAST_TIME → Bit 7（掩码 0x80，右移 7 位）
        CCL_PD_FAST_TIME_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ[SITE][7] & 0x01; // CL_VS_HALF: DTRIM_CL_VS_HALF_AVDD → Bit 0（掩码 0x01）
        CL_VS_HALF_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][7] & 0x02) >> 1;// CL_VS: DTRIM_CL_VS_AVDD → Bit 1（掩码 0x02，右移 1 位）
        CL_VS_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][7] & 0x04) >> 2;// CL_VS_DOWN: DTRIM_CL_VS_DOWN_AVDD → Bit 2（掩码 0x04，右移 2 位）
        CL_VS_DOWN_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][7] & 0x10) >> 4; // INVERSEON_DIS: DTRIM_INVERSEO N_DIS_DVDD → Bit 4（掩码 0x10，右移 4 位）
        INVERSEON_DIS_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][7] & 0x20) >> 5; // COUNTER20: DTRIM_COUNT ER20_D VDD → Bit 5（掩码 0x20，右移 5 位）
        COUNTER20_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][7] & 0x40) >> 6; // VERA: DTRIM_VERA → Bit 6（掩码 0x40，右移 6 位）
        VERA_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ[SITE][7] & 0x80) >> 7; // BURN_LOCK: BURN_LOCK → Bit 7（掩码 0x80，右移 7 位）
        BURN_LOCK_READ->SetTestResult(SITE, 0, rfuse_data[SITE]);
    }

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}
 
DUT_API int TEST07_VBG_Trim(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VBG_Step0 = StsGetParam(funcindex, "VBG_Step0");
    CParam *VBG_Step1 = StsGetParam(funcindex, "VBG_Step1");
    CParam *VBG_Step2 = StsGetParam(funcindex, "VBG_Step2");
    CParam *VBG_Step3 = StsGetParam(funcindex, "VBG_Step3");
    CParam *VBG_Step4 = StsGetParam(funcindex, "VBG_Step4");
    CParam *VBG_Step5 = StsGetParam(funcindex, "VBG_Step5");
    CParam *VBG_Step6 = StsGetParam(funcindex, "VBG_Step6");
    CParam *VBG_Step7 = StsGetParam(funcindex, "VBG_Step7");
    CParam *VBG_Step8 = StsGetParam(funcindex, "VBG_Step8");
    CParam *VBG_Step9 = StsGetParam(funcindex, "VBG_Step9");
    CParam *VBG_Step10 = StsGetParam(funcindex, "VBG_Step10");
    CParam *VBG_Step11 = StsGetParam(funcindex, "VBG_Step11");
    CParam *VBG_Step12 = StsGetParam(funcindex, "VBG_Step12");
    CParam *VBG_Step13 = StsGetParam(funcindex, "VBG_Step13");
    CParam *VBG_Step14 = StsGetParam(funcindex, "VBG_Step14");
    CParam *VBG_Step15 = StsGetParam(funcindex, "VBG_Step15");
    CParam *VBG_Step16 = StsGetParam(funcindex, "VBG_Step16");
    CParam *VBG_Step17 = StsGetParam(funcindex, "VBG_Step17");
    CParam *VBG_Step18 = StsGetParam(funcindex, "VBG_Step18");
    CParam *VBG_Step19 = StsGetParam(funcindex, "VBG_Step19");
    CParam *VBG_Step20 = StsGetParam(funcindex, "VBG_Step20");
    CParam *VBG_Step21 = StsGetParam(funcindex, "VBG_Step21");
    CParam *VBG_Step22 = StsGetParam(funcindex, "VBG_Step22");
    CParam *VBG_Step23 = StsGetParam(funcindex, "VBG_Step23");
    CParam *VBG_Step24 = StsGetParam(funcindex, "VBG_Step24");
    CParam *VBG_Step25 = StsGetParam(funcindex, "VBG_Step25");
    CParam *VBG_Step26 = StsGetParam(funcindex, "VBG_Step26");
    CParam *VBG_Step27 = StsGetParam(funcindex, "VBG_Step27");
    CParam *VBG_Step28 = StsGetParam(funcindex, "VBG_Step28");
    CParam *VBG_Step29 = StsGetParam(funcindex, "VBG_Step29");
    CParam *VBG_Step30 = StsGetParam(funcindex, "VBG_Step30");
    CParam *VBG_Step31 = StsGetParam(funcindex, "VBG_Step31");
    CParam *VBG_pre = StsGetParam(funcindex, "VBG_pre");
    CParam *VBG_pre_bit = StsGetParam(funcindex, "VBG_pre_bit");
    CParam *VBG_post = StsGetParam(funcindex, "VBG_post");
    CParam *VBG_post_bit = StsGetParam(funcindex, "VBG_post_bit");
    CParam *VBG_target = StsGetParam(funcindex, "VBG_target");
//}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here

    CParam* VBG_Steps[32] = 
    {   VBG_Step0  ,VBG_Step1  ,VBG_Step2  ,VBG_Step3  ,VBG_Step4  ,VBG_Step5  ,VBG_Step6  ,VBG_Step7  ,
        VBG_Step8  ,VBG_Step9  ,VBG_Step10 ,VBG_Step11 ,VBG_Step12 ,VBG_Step13 ,VBG_Step14 ,VBG_Step15 ,
        VBG_Step16 ,VBG_Step17 ,VBG_Step18 ,VBG_Step19 ,VBG_Step20 ,VBG_Step21 ,VBG_Step22 ,VBG_Step23 ,
        VBG_Step24 ,VBG_Step25 ,VBG_Step26 ,VBG_Step27 ,VBG_Step28 ,VBG_Step29 ,VBG_Step30 ,VBG_Step31 ,
    };

    int remain_data[SITE_NUM] = { 0 };
    int preview_data[SITE_NUM] = { 0 };
    int nErr = 0;
    double result[SITE_NUM] = {0.0};
    double target = 1.233;
    //map<DWORD, map<DWORD, double>> preview_value;
    double preview_value[SITE_NUM][32] = { 0.0 };
    double preview_offset[SITE_NUM][32] = { 0.0 };

    int min_step[SITE_NUM] = { 0 };

    cbit.SetOn(K4_NC_IS_DESL_VI, K12_NC_IN1, K7_DenPullUp, -1);// 切换VI源到 IN1--->NC(SEL1)，IS--->DESL(SEL0)
    delay_ms(2);

    Enter_TM();

    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_100MA, RELAY_ON);
    IN1_VI.Set(FI, 0, FOVI_2V, FOVI_10UA, RELAY_SENSE_ON);
    delay_ms(2);

    SERIAL VBG_target->SetTestResult(SITE, 0, target);

    rfuse_data[SITE] = EE_READ[SITE][0] & 0X1F;  // VBG_READ: DTRM_BG_AVDDBG<4:0> → 低5位（0x1F）
    SERIAL VBG_pre_bit->SetTestResult(SITE, 0, int(rfuse_data[SITE]));

    nErr = dio_plus.Run("TM2_VBG_str", "TM2_VBG_stp");
    delay_ms(2);
    IN1_VI.MeasureVI(50, 10);
    nErr = dio_plus.Run("dummy_str", "dummy_stp");
    SERIAL result[SITE] = IN1_VI.GetMeasResult(SITE, MVRET);
    SERIAL VBG_pre->SetTestResult(SITE, 0, result[SITE]);


    SERIAL remain_data[SITE] = EE_READ[SITE][0] & 0XE0; // 保留高三位
    for (int step = 0; step < 32; ++step)
    {
        SERIAL preview_data[SITE] = remain_data[SITE] | step; // 拼接 高三位 + 低五位 
        SERIAL EE_Preview(SITE, 0, preview_data[SITE], nErr); 
        delay_us(200);
        nErr = dio_plus.Run("Pre_Bank_str", "Pre_Bank_stp");
        delay_us(200);
        nErr = dio_plus.Run("TM2_VBG_str", "TM2_VBG_stp");
        delay_ms(2);
        IN1_VI.MeasureVI(50, 10);
        nErr = dio_plus.Run("dummy_str", "dummy_stp");
        SERIAL result[SITE] = IN1_VI.GetMeasResult(SITE, MVRET);
        SERIAL VBG_Steps[step]->SetTestResult(SITE, 0, result[SITE]);
        SERIAL preview_value[SITE][step] = result[SITE]; // 保存preview的VBG数据
        SERIAL preview_offset[SITE][step] = abs(result[SITE] - target);
    }

    FindRowMins(preview_offset, min_step); // 计算最接近target的step
    SERIAL EE_WRITE[SITE][0] = min_step[SITE];
    SERIAL VBG_post_bit->SetTestResult(SITE, 0, min_step[SITE]);
    SERIAL VBG_post->SetTestResult(SITE, 0, preview_value[SITE][min_step[SITE]]);
   
    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}
 
DUT_API int TEST08_EEPROM_Burn(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *eFUSE_BANK0_PRE = StsGetParam(funcindex, "eFUSE_BANK0_PRE");
    CParam *eFUSE_BANK0_READ = StsGetParam(funcindex, "eFUSE_BANK0_READ");
    CParam *VBG_READ1 = StsGetParam(funcindex, "VBG_READ1");
    CParam *EE0_Check = StsGetParam(funcindex, "EE0_Check");
//}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here

    cbit.SetOn(K4_NC_IS_DESL_VI, K12_NC_IN1, K7_DenPullUp, -1);// 切换VI源到 IN1--->NC(SEL1)，IS--->DESL(SEL0)
    delay_ms(2);

    Enter_TM();

    KELVIN_VS.Set(FV, 13.5, FOVI_20V, FOVI_100MA, RELAY_ON);
    IN1_VI.Set(FI, 0, FOVI_2V, FOVI_10UA, RELAY_SENSE_ON);
    delay_ms(2);

    int nErr = 0;
    int burn_data[SITE_NUM] = { 0 };
    int remain_data[SITE_NUM] = { 0 };
    int burn_version_id = 1;

    bool bOK = false;
    EE_Read(0, EE_READ, bOK); // read bank0
    SERIAL eFUSE_BANK0_PRE->SetTestResult(SITE, 0, EE_READ[SITE][0]);

    SERIAL
    {
        if (Trim_done_flag[SITE] == 1)
        {
            EE_no_Write(SITE, 0, nErr);
        }
        else
        {
            burn_data[SITE] = EE_WRITE[SITE][0]; // 按字节burn，默认该字节已经trim完毕，一起burn
            EE_Write(SITE, 0, burn_data[SITE], nErr); // 向SITE的bank0 burn
            EE_Write(SITE, 0, burn_version_id, nErr); // 向SITE的bank5 burn 001
            delay_us(200);
        }
        nErr = dio_plus.Run("Burn_str", "Burn_stp");
    }

    EE_Read(0, EE_READ, bOK); // read bank0
    SERIAL
    { 
        eFUSE_BANK0_READ->SetTestResult(SITE, 0, EE_READ[SITE][0]);
        rfuse_data[SITE] = EE_READ[SITE][0] & 0X1F;
        VBG_READ1->SetTestResult(SITE, 0, rfuse_data[SITE]);

        if (EE_WRITE[SITE][0] == EE_READ[SITE][0])
        {
            EE0_Check->SetTestResult(SITE, 0, 1);
        }
        else
        {
            EE0_Check->SetTestResult(SITE, 0, 0);
        }
    }

    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}

 
DUT_API int TEST12_EEPROM_Final_Read(short funcindex, LPCTSTR funclabel)
{
//{{AFX_STS_PARAM_PROTOTYPES
    CParam *EE0_READ_Final = StsGetParam(funcindex, "EE0_READ_Final");
    CParam *EE1_READ_Final = StsGetParam(funcindex, "EE1_READ_Final");
    CParam *EE2_READ_Final = StsGetParam(funcindex, "EE2_READ_Final");
    CParam *EE3_READ_Final = StsGetParam(funcindex, "EE3_READ_Final");
    CParam *EE4_READ_Final = StsGetParam(funcindex, "EE4_READ_Final");
    CParam *EE5_READ_Final = StsGetParam(funcindex, "EE5_READ_Final");
    CParam *EE6_READ_Final = StsGetParam(funcindex, "EE6_READ_Final");
    CParam *EE7_READ_Final = StsGetParam(funcindex, "EE7_READ_Final");
    CParam *VBG_READ_Final = StsGetParam(funcindex, "VBG_READ_Final");
    CParam *IB10U_READ_Final = StsGetParam(funcindex, "IB10U_READ_Final");
    CParam *OSCLF_READ_Final = StsGetParam(funcindex, "OSCLF_READ_Final");
    CParam *ILIMCH1_READ_Final = StsGetParam(funcindex, "ILIMCH1_READ_Final");
    CParam *ILIMCH2_READ_Final = StsGetParam(funcindex, "ILIMCH2_READ_Final");
    CParam *SR_READ_Final = StsGetParam(funcindex, "SR_READ_Final");
    CParam *CILIM_READ_Final = StsGetParam(funcindex, "CILIM_READ_Final");
    CParam *TSD_READ_Final = StsGetParam(funcindex, "TSD_READ_Final");
    CParam *TSW_READ_Final = StsGetParam(funcindex, "TSW_READ_Final");
    CParam *IOL_MORE_READ_Final = StsGetParam(funcindex, "IOL_MORE_READ_Final");
    CParam *CSCH1_READ_Final = StsGetParam(funcindex, "CSCH1_READ_Final");
    CParam *CSCH2_READ_Final = StsGetParam(funcindex, "CSCH2_READ_Final");
    CParam *Version_ID_READ_Final = StsGetParam(funcindex, "Version_ID_READ_Final");
    CParam *Lot_ID_READ_Final = StsGetParam(funcindex, "Lot_ID_READ_Final");
    CParam *CCL_FASTPD_wiCL_READ_Final = StsGetParam(funcindex, "CCL_FASTPD_wiCL_READ_Final");
    CParam *CLOFF_DELAY_READ_Final = StsGetParam(funcindex, "CLOFF_DELAY_READ_Final");
    CParam *ILIM_PD_READ_Final = StsGetParam(funcindex, "ILIM_PD_READ_Final");
    CParam *CLOFFCH_EN_READ_Final = StsGetParam(funcindex, "CLOFFCH_EN_READ_Final");
    CParam *CCL_PD_FAST_TIME_READ_Final = StsGetParam(funcindex, "CCL_PD_FAST_TIME_READ_Final");
    CParam *CL_VS_HALF_READ_Final = StsGetParam(funcindex, "CL_VS_HALF_READ_Final");
    CParam *CL_VS_READ_Final = StsGetParam(funcindex, "CL_VS_READ_Final");
    CParam *CL_VS_DOWN_READ_Final = StsGetParam(funcindex, "CL_VS_DOWN_READ_Final");
    CParam *INVERSEON_DIS_READ_Final = StsGetParam(funcindex, "INVERSEON_DIS_READ_Final");
    CParam *COUNTER20_READ_Final = StsGetParam(funcindex, "COUNTER20_READ_Final");
    CParam *VERA_READ_Final = StsGetParam(funcindex, "VERA_READ_Final");
    CParam *BURN_LOCK_READ_Final = StsGetParam(funcindex, "BURN_LOCK_READ_Final");

    CParam *BG_code_check = StsGetParam(funcindex, "BG_code_check");
    CParam *Version_ID_check = StsGetParam(funcindex, "Version_ID_check");

//}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here

    int rfuse_data[SITE_NUM] = { 0 };
    bool bOK = false;
    map<DWORD, map<DWORD, int>> EE_READ;
    int bank = 0;

    delay_ms(4);
    Enter_TM();

    for (int bank = 0; bank < BANK_NUM; ++bank)
    {
        EE_Read(bank, EE_READ_Final, bOK); // read bank
    }

    SERIAL
    {
        EE0_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][0]);
        EE1_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][1]);
        EE2_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][2]);
        EE3_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][3]);
        EE4_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][4]);
        EE5_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][5]);
        EE6_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][6]);
        EE7_READ_Final->SetTestResult(SITE, 0, EE_READ_Final[SITE][7]);
    

        rfuse_data[SITE] = EE_READ_Final[SITE][0] & 0X1F;  // VBG_READ: DTRM_BG_AVDDBG<4:0> → 低5位（0x1F）
        VBG_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        if (rfuse_data[SITE] == (EE_WRITE[SITE][0] & 0X1F)) //(Read指令用11)再次把所有efuse bit的结果读出来并log，其中BG code和Version ID要判断一下是否burn正确
        {
            BG_code_check->SetTestResult(SITE, 0, 1);
        }
        else
        {
            BG_code_check->SetTestResult(SITE, 0, 0);
        }

        rfuse_data[SITE] = (EE_READ_Final[SITE][0] & 0XE0) >> 5; // IB10U_READ: DTRIM_IB10U_AVDD<2:0> → 高3位（0xE0，需右移5位还原值）
        IB10U_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ_Final[SITE][1] & 0X07; // OSCLF_READ: DTRIM_OSCLF_DVDD<2:0> → 低3位（0x07）
        OSCLF_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][1] & 0x08) >> 3; // ILIMCH1_READ: DTRIM_ILIMCH1_AVDD<0> → Bit 3（0x08掩码，再右移3位）
        rfuse_data[SITE] = rfuse_data[SITE] | ((EE_READ_Final[SITE][2] & 0xE0) >> 4);// ILIMCH1_AVDD<3:1> → Bits 7-5（0xE0掩码，右移4位）
        ILIMCH1_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][1] & 0xF0) >> 4; // ILIMCH2_READ: DTRIM_ILIMCH2_AVDD<3:1><0> → Bits 7-4（0xF0掩码，右移4位）
        ILIMCH2_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][2] & 0x07); // SR_READ: DTRIM_SR_AVDD → Bit 0-2
        SR_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][2] & 0x18) >> 3;// CILIM_READ: DTRIM_CILIM_AVDD<1:0> → Bits 3-4（0x18掩码，右移3位）
        CILIM_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ_Final[SITE][3] & 0x07; // TSD_READ: DTRIM_TSD_AVDD<2:0> →  → 低3位（0x07）
        TSD_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][3] & 0x38) >> 3;// TSW_READ: DTRIM_TSW_AVDD<2:0> → Bits 3-5（0x38掩码，右移3位）
        TSW_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][3] & 0xC0) >> 6; // IOL_MORE_READ: DTRIM_IOL_MORE_AVD → Bits 6-7（0xE0掩码，右移6位）
        IOL_MORE_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ_Final[SITE][4] & 0x0F; // CSCH1_READ: DTRIM_CSCH1_AVDD<3:0> → Bits 0-3
        CSCH1_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][4] & 0xF0) >> 4;// CSCH2_READ: DTRIM_CSCH2_AVDD<3:0> → Bits 4-7
        CSCH2_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ_Final[SITE][5] & 0x07;// Version_ID: Version ID<2:0> → Bits 2-0（掩码 0x07）
        Version_ID_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        if (rfuse_data[SITE] == 1)//(Read指令用11)再次把所有efuse bit的结果读出来并log，其中BG code和Version ID要判断一下是否burn正确
        {
            Version_ID_check->SetTestResult(SITE, 0, 1);
        }
        else
        {
            Version_ID_check->SetTestResult(SITE, 0, 0);
        }

        rfuse_data[SITE] = (EE_READ_Final[SITE][5] & 0xF8) >> 3; // Lot_ID: Lot ID<4:0> → Bits 7-3（掩码 0xF8，右移 3 位）
        Lot_ID_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ_Final[SITE][6] & 0x03;// CCL_FASTPD_wiCL: DTRIM_CCL_FASTPD_wiCL<1:0> → Bits 1-0（掩码 0x03）
        CCL_FASTPD_wiCL_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][6] & 0x0C) >> 2;// CLOFF_DELAY: DTRIM_CLOFF_DELAY<1:0> → Bits 3-2（掩码 0x0C，右移 2 位）
        CLOFF_DELAY_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][6] & 0x30) >> 4;// ILIM_PD: DTRIM_ILIM_PD<1:0> → Bits 5-4（掩码 0x30，右移 4 位）
        rfuse_data[SITE] = rfuse_data[SITE] | ((EE_READ_Final[SITE][7] & 0x08) >> 2);// 高位
        ILIM_PD_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][6] & 0x40) >> 6;// CLOFFCH_EN: DTRIM_CLOFFC_H_EN → Bit 6（掩码 0x40，右移 6 位）
        CLOFFCH_EN_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][6] & 0x80) >> 7;// CCL_PD_FAST_TIME: DTRIM_CCL_PD_FAST_TIME → Bit 7（掩码 0x80，右移 7 位）
        CCL_PD_FAST_TIME_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = EE_READ_Final[SITE][7] & 0x01; // CL_VS_HALF: DTRIM_CL_VS_HALF_AVDD → Bit 0（掩码 0x01）
        CL_VS_HALF_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][7] & 0x02) >> 1;// CL_VS: DTRIM_CL_VS_AVDD → Bit 1（掩码 0x02，右移 1 位）
        CL_VS_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][7] & 0x04) >> 2;// CL_VS_DOWN: DTRIM_CL_VS_DOWN_AVDD → Bit 2（掩码 0x04，右移 2 位）
        CL_VS_DOWN_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][7] & 0x10) >> 4; // INVERSEON_DIS: DTRIM_INVERSEO N_DIS_DVDD → Bit 4（掩码 0x10，右移 4 位）
        INVERSEON_DIS_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][7] & 0x20) >> 5; // COUNTER20: DTRIM_COUNT ER20_D VDD → Bit 5（掩码 0x20，右移 5 位）
        COUNTER20_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][7] & 0x40) >> 6; // VERA: DTRIM_VERA → Bit 6（掩码 0x40，右移 6 位）
        VERA_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);

        rfuse_data[SITE] = (EE_READ_Final[SITE][7] & 0x80) >> 7; // BURN_LOCK: BURN_LOCK → Bit 7（掩码 0x80，右移 7 位）
        BURN_LOCK_READ_Final->SetTestResult(SITE, 0, rfuse_data[SITE]);
    }

    dio_plus.Disconnect();
    
    //=====================POWER_DOWN===========================
    ALL_FPVI_Power_Down();
    ALL_FOVI_Power_Down();
    cbit.SetOn(-1);
    delay_ms(3);

    return 0;
}
