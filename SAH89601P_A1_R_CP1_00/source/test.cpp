#include "stdafx.h"

//multisite settings should be included here
#include "Pin_Channel_define.h"
#include "BoardCheck.h"






int globalsite;
DCM dcm;
CBITe cbit;
Test_Method test_method;


//board check coding
bool DO_BoardCheck = true;
extern BOOL run_diags();

//////normal use
int ScanLoad_flag = -1;
bool burn_flag[SITE_NUM];
int flag[SITE_NUM];
double adresult[SITE_NUM];
double adresult_R[SITE_NUM];
double adresult_F[SITE_NUM];
double adresult_BF[SITE_NUM];
double adresult_AF[SITE_NUM];

struct
{
	double P2P_leak_pre[SITE_NUM];
	double P2P_leak_post[SITE_NUM];
	double PIN_leak_pre[SITE_NUM];
	double PIN_leak_post[SITE_NUM];
	double ABS_LEAK[SITE_NUM];
} MEAS_VBUS, MEAS_DP, MEAS_DM, MEAS_PD_nINT, MEAS_SCL, MEAS_SDA, MEAS_INT, MEAS_CC1, MEAS_CE, MEAS_CC2, MEAS_TSBAT, MEAS_QON, MEAS_VBAT, MEAS_VSYS, MEAS_SW, MEAS_BTST, MEAS_REGN, MEAS_PMID;

struct IQ
{
	double IQ_VBAT[SITE_NUM];
	double IQ_BATFETOFF[SITE_NUM];
	double IQ_BAT_DRP[SITE_NUM];
	double IQ_EOC[SITE_NUM];
	double IQ_VBUS5V_HIZ[SITE_NUM];
	double IQ_VBUS9V_HIZ[SITE_NUM];
	double IQ_VBUS[SITE_NUM];
	double IQ_VBUS_UVLO[SITE_NUM];
	double IBOOST[SITE_NUM];
} IQ_PRE, IQ_POST;

struct MOSFET
{
	double Rdson[SITE_NUM];
	double Ioff_pre[SITE_NUM];
	double Ioff_post[SITE_NUM];
	double Idrive_pre[SITE_NUM];
	double Idrive_post[SITE_NUM];
} Q1, Q2, Q3, Q4;


/*The following code was created for STS PinPlanner,don't modify*/ 
/****STS_PINPLANNER_CODE_BEGIN****/
#define STS_SITE_NUM 16
/****PIN GROUP DEFINITION****/
FOVIe VBUS(_PIN_CHANNEL_DEFINE_VBUS_, "VBUS");
ACM PMID(_PIN_CHANNEL_DEFINE_PMID_, "PMID");
ACM CC1(_PIN_CHANNEL_DEFINE_CC1_, "CC1");
ACM CC2(_PIN_CHANNEL_DEFINE_CC2_, "CC2");
ACM DP(_PIN_CHANNEL_DEFINE_DP_, "DP");
ACM DM(_PIN_CHANNEL_DEFINE_DM_, "DM");
ACM TSBAT(_PIN_CHANNEL_DEFINE_TSBAT_, "TSBAT");
FOVIe VBAT(_PIN_CHANNEL_DEFINE_VBAT_, "VBAT");
FOVIe VSYS(_PIN_CHANNEL_DEFINE_VSYS_, "VSYS");
FOVIe SW(_PIN_CHANNEL_DEFINE_SW_, "SW");
ACM REGN(_PIN_CHANNEL_DEFINE_REGN_, "REGN");
ACM BTST(_PIN_CHANNEL_DEFINE_BTST_, "BTST");
FOVIe FOVI_ALL(_GROUP_CHANNEL_DEFINE_FOVI_ALL_,"FOVI_ALL");
ACM ACM_ALL(_GROUP_CHANNEL_DEFINE_ACM_ALL_,"ACM_ALL");
/****multisite settings should be included here****/
DUT_API void HardWareCfg()
{
	STSSetMultiSiteBindEx(MD_ACM,0,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE1_);
	STSSetMultiSiteBindEx(MD_ACM,1,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE2_);
	STSSetMultiSiteBindEx(MD_ACM,2,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE3_);
	STSSetMultiSiteBindEx(MD_ACM,3,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE4_);
	STSSetMultiSiteBindEx(MD_ACM,4,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE5_);
	STSSetMultiSiteBindEx(MD_ACM,5,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE6_);
	STSSetMultiSiteBindEx(MD_ACM,6,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE7_);
	STSSetMultiSiteBindEx(MD_ACM,7,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE8_);
	STSSetMultiSiteBindEx(MD_ACM,8,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE9_);
	STSSetMultiSiteBindEx(MD_ACM,9,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE10_);
	STSSetMultiSiteBindEx(MD_ACM,10,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE11_);
	STSSetMultiSiteBindEx(MD_ACM,11,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE12_);
	STSSetMultiSiteBindEx(MD_ACM,12,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE13_);
	STSSetMultiSiteBindEx(MD_ACM,13,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE14_);
	STSSetMultiSiteBindEx(MD_ACM,14,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE15_);
	STSSetMultiSiteBindEx(MD_ACM,15,_PIN_SITE_BIND_DEFINE_MD_ACM_SITE16_);
	STSSetMultiSiteBindEx(MD_FOVIe,0,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE1_);
	STSSetMultiSiteBindEx(MD_FOVIe,1,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE2_);
	STSSetMultiSiteBindEx(MD_FOVIe,2,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE3_);
	STSSetMultiSiteBindEx(MD_FOVIe,3,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE4_);
	STSSetMultiSiteBindEx(MD_FOVIe,4,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE5_);
	STSSetMultiSiteBindEx(MD_FOVIe,5,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE6_);
	STSSetMultiSiteBindEx(MD_FOVIe,6,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE7_);
	STSSetMultiSiteBindEx(MD_FOVIe,7,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE8_);
	STSSetMultiSiteBindEx(MD_FOVIe,8,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE9_);
	STSSetMultiSiteBindEx(MD_FOVIe,9,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE10_);
	STSSetMultiSiteBindEx(MD_FOVIe,10,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE11_);
	STSSetMultiSiteBindEx(MD_FOVIe,11,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE12_);
	STSSetMultiSiteBindEx(MD_FOVIe,12,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE13_);
	STSSetMultiSiteBindEx(MD_FOVIe,13,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE14_);
	STSSetMultiSiteBindEx(MD_FOVIe,14,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE15_);
	STSSetMultiSiteBindEx(MD_FOVIe,15,_PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE16_);
}
/****STS_PINPLANNER_CODE_END****/


DUT_API void UserLoad()
{
	cbit.SetOn(KPIN_USE, -1);

	ScanLoad_flag = dcm.LoadVectorFile("SCV89601P.acvec", false);
	dcm.SetPinGroup("SCL", "SCL_D");
	dcm.SetPinGroup("SDA", "SDA_D");
	dcm.SetPinGroup("QON", "QON_D");
	dcm.SetPinGroup("INT_AMUX", "INT_AMUX_D");
	dcm.SetPinGroup("CE", "CE_D");
	dcm.SetPinGroup("PD_nINT", "PD_nINT_D");

	dcm.SetPinGroup("I2C", "SCL_D,SDA_D");
	dcm.SetPinGroup("SCAN", "SCL_D,SDA_D,QON_D,INT_AMUX_D,CE_D");
	dcm.SetPinGroup("DCM_ALL", "SCL_D,SDA_D,QON_D,INT_AMUX_D,CE_D,PD_nINT_D");

	STSSetMultiSiteBindEx(MD_ACM, 0, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE1_);
	STSSetMultiSiteBindEx(MD_ACM, 1, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE2_);
	STSSetMultiSiteBindEx(MD_ACM, 2, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE3_);
	STSSetMultiSiteBindEx(MD_ACM, 3, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE4_);
	STSSetMultiSiteBindEx(MD_ACM, 4, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE5_);
	STSSetMultiSiteBindEx(MD_ACM, 5, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE6_);
	STSSetMultiSiteBindEx(MD_ACM, 6, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE7_);
	STSSetMultiSiteBindEx(MD_ACM, 7, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE8_);
	STSSetMultiSiteBindEx(MD_ACM, 8, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE9_);
	STSSetMultiSiteBindEx(MD_ACM, 9, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE10_);
	STSSetMultiSiteBindEx(MD_ACM, 10, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE11_);
	STSSetMultiSiteBindEx(MD_ACM, 11, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE12_);
	STSSetMultiSiteBindEx(MD_ACM, 12, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE13_);
	STSSetMultiSiteBindEx(MD_ACM, 13, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE14_);
	STSSetMultiSiteBindEx(MD_ACM, 14, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE15_);
	STSSetMultiSiteBindEx(MD_ACM, 15, _PIN_SITE_BIND_DEFINE_MD_ACM_SITE16_);
	STSSetMultiSiteBindEx(MD_FOVIe, 0, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE1_);
	STSSetMultiSiteBindEx(MD_FOVIe, 1, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE2_);
	STSSetMultiSiteBindEx(MD_FOVIe, 2, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE3_);
	STSSetMultiSiteBindEx(MD_FOVIe, 3, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE4_);
	STSSetMultiSiteBindEx(MD_FOVIe, 4, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE5_);
	STSSetMultiSiteBindEx(MD_FOVIe, 5, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE6_);
	STSSetMultiSiteBindEx(MD_FOVIe, 6, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE7_);
	STSSetMultiSiteBindEx(MD_FOVIe, 7, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE8_);
	STSSetMultiSiteBindEx(MD_FOVIe, 8, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE9_);
	STSSetMultiSiteBindEx(MD_FOVIe, 9, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE10_);
	STSSetMultiSiteBindEx(MD_FOVIe, 10, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE11_);
	STSSetMultiSiteBindEx(MD_FOVIe, 11, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE12_);
	STSSetMultiSiteBindEx(MD_FOVIe, 12, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE13_);
	STSSetMultiSiteBindEx(MD_FOVIe, 13, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE14_);
	STSSetMultiSiteBindEx(MD_FOVIe, 14, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE15_);
	STSSetMultiSiteBindEx(MD_FOVIe, 15, _PIN_SITE_BIND_DEFINE_MD_FOVIE_SITE16_);

	AstBindingAllModule();
	STSSetSiteStatus(0xFFFFFFFF);

	//BC
	if (DO_BoardCheck)	run_diags();


}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initialize function will be called before all the test functions.
DUT_API void InitBeforeTestFlow()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()
{		
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//Fail site hardware set function will be called after failed params, it can be called for serveral times. 
DUT_API void SetupFailSite(const unsigned char*byFailSite)
{			
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
 
DUT_API int T00_Info(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Part_No = StsGetParam(funcindex, "Part_No");
	CParam *ScanLoad = StsGetParam(funcindex, "ScanLoad");
	CParam *X_Coordinate = StsGetParam(funcindex, "X_Coordinate");
	CParam *Y_Coordinate = StsGetParam(funcindex, "Y_Coordinate");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here


	int X[SITE_NUM], Y[SITE_NUM];
	char waferid[255];


	GET_WAFER_INFO(waferid, X, Y);

	SERIAL Part_No->SetTestResult(SITE, 0, 8960150);
	SERIAL ScanLoad->SetTestResult(SITE, 0, ScanLoad_flag);
	SERIAL X_Coordinate->SetTestResult(SITE, 0, X[SITE]);
	SERIAL Y_Coordinate->SetTestResult(SITE, 0, Y[SITE]);




    return 0;
}
 
DUT_API int T01_OS(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *OS_SHORT = StsGetParam(funcindex, "OS_SHORT");
    CParam *OS_SOME_OPEN = StsGetParam(funcindex, "OS_SOME_OPEN");
    CParam *OS_ALL_OPEN = StsGetParam(funcindex, "OS_ALL_OPEN");
    CParam *OS_VBUS = StsGetParam(funcindex, "OS_VBUS");
    CParam *OS_DP = StsGetParam(funcindex, "OS_DP");
    CParam *OS_DM = StsGetParam(funcindex, "OS_DM");
    CParam *OS_PD_nINT = StsGetParam(funcindex, "OS_PD_nINT");
    CParam *OS_SCL = StsGetParam(funcindex, "OS_SCL");
    CParam *OS_SDA = StsGetParam(funcindex, "OS_SDA");
    CParam *OS_INT = StsGetParam(funcindex, "OS_INT");
    CParam *OS_CC1 = StsGetParam(funcindex, "OS_CC1");
    CParam *OS_CE = StsGetParam(funcindex, "OS_CE");
    CParam *OS_CC2 = StsGetParam(funcindex, "OS_CC2");
    CParam *OS_TSBAT = StsGetParam(funcindex, "OS_TSBAT");
    CParam *OS_QON = StsGetParam(funcindex, "OS_QON");
    CParam *OS_VBAT = StsGetParam(funcindex, "OS_VBAT");
    CParam *OS_VSYS = StsGetParam(funcindex, "OS_VSYS");
    CParam *OS_SW = StsGetParam(funcindex, "OS_SW");
    CParam *OS_BTST = StsGetParam(funcindex, "OS_BTST");
    CParam *OS_REGN = StsGetParam(funcindex, "OS_REGN");
    CParam *OS_PMID = StsGetParam(funcindex, "OS_PMID");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here

	const int PIN_NUM = 18;
	const int LEAK_NUM = 0;
	const int exclude_NUM = 0;
	double os_result[PIN_NUM][SITE_NUM] = { 0 };
	vector<string> vec_exclude;
	string pin_str_array[PIN_NUM] = { "OS_VBUS", "OS_DP", "OS_DM", "OS_PD_nINT", "OS_SCL", "OS_SDA", "OS_INT", "OS_CC1", "OS_CE", "OS_CC2", "OS_TSBAT", "OS_QON", "OS_VBAT", "OS_VSYS", "OS_SW", "OS_BTST", "OS_REGN", "OS_PMID" };



	FOVI_ALL.Set(FV, 0, FOVIe_2V, FOVIe_10MA, FOVIe_RELAY_ON);
	ACM_ALL.Set(FV, 0, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);

	dcm.Connect("DCM_ALL");
	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);


	FOVI_ALL.Set(FI, -1e-3, FOVIe_2V, FOVIe_10MA, FOVIe_RELAY_ON);
	delay_us(500);
	FOVI_ALL.MeasureVI(10, 10);
	FOVI_ALL.Set(FV, 0, FOVIe_2V, FOVIe_10MA, FOVIe_RELAY_ON);


	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FIMV, -1e-3, DCM_PPMUIRANGE_2MA);
	delay_ms(1);
	dcm.PPMUMeasure("DCM_ALL", 10, 10);


	ACM_ALL.SetClamp(10, 100);
	ACM_ALL.Set(FI, -1e-3, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);
	delay_ms(1);
	ACM_ALL.MeasureVI(ACM_MV, 10, 10);
	ACM_ALL.Set(FV, 0, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);
	

	//VBUS
	SERIAL os_result[0][SITE] = VBUS.GetMeasResult(SITE, MVRET);
	SERIAL OS_VBUS->SetTestResult(SITE, 0, os_result[0][SITE]);
	//PD_nINT
	SERIAL os_result[3][SITE] = dcm.GetPPMUMeasResult("PD_nINT_D", SITE);
	SERIAL OS_PD_nINT->SetTestResult(SITE, 0, os_result[3][SITE]);
	//SCL
	SERIAL os_result[4][SITE] = dcm.GetPPMUMeasResult("SCL_D", SITE);
	SERIAL OS_SCL->SetTestResult(SITE, 0, os_result[4][SITE]);
	//SDA
	SERIAL os_result[5][SITE] = dcm.GetPPMUMeasResult("SDA_D", SITE);
	SERIAL OS_SDA->SetTestResult(SITE, 0, os_result[5][SITE]);
	//INT_AMUX
	SERIAL os_result[6][SITE] = dcm.GetPPMUMeasResult("INT_AMUX_D", SITE);
	SERIAL OS_INT->SetTestResult(SITE, 0, os_result[6][SITE]);
	//CE
	SERIAL os_result[8][SITE] = dcm.GetPPMUMeasResult("CE_D", SITE);
	SERIAL OS_CE->SetTestResult(SITE, 0, os_result[8][SITE]);
	//TSBAT
	SERIAL os_result[10][SITE] = TSBAT.GetMeasResult(SITE);
	SERIAL OS_TSBAT->SetTestResult(SITE, 0, os_result[10][SITE]);
	//QON
	SERIAL os_result[11][SITE] = dcm.GetPPMUMeasResult("QON_D", SITE);
	SERIAL OS_QON->SetTestResult(SITE, 0, os_result[11][SITE]);
	//VBAT
	SERIAL os_result[12][SITE] = VBAT.GetMeasResult(SITE, MVRET);
	SERIAL OS_VBAT->SetTestResult(SITE, 0, os_result[12][SITE]);
	//VSYS
	SERIAL os_result[13][SITE] = VSYS.GetMeasResult(SITE, MVRET);
	SERIAL OS_VSYS->SetTestResult(SITE, 0, os_result[13][SITE]);
	//REGN
	SERIAL os_result[16][SITE] = REGN.GetMeasResult(SITE);
	SERIAL OS_REGN->SetTestResult(SITE, 0, os_result[16][SITE]);
	//PMID
	SERIAL os_result[17][SITE] = PMID.GetMeasResult(SITE);
	SERIAL OS_PMID->SetTestResult(SITE, 0, os_result[17][SITE]);



	//SW
	SERIAL os_result[14][SITE] = SW.GetMeasResult(SITE, MVRET);
	SERIAL OS_SW->SetTestResult(SITE, 0, os_result[14][SITE]);
	//BTST
	SERIAL os_result[15][SITE] = BTST.GetMeasResult(SITE);
	SERIAL OS_BTST->SetTestResult(SITE, 0, os_result[15][SITE]);
	//CC1
	SERIAL os_result[7][SITE] = CC1.GetMeasResult(SITE);
	SERIAL OS_CC1->SetTestResult(SITE, 0, os_result[7][SITE]);
	//CC2
	SERIAL os_result[9][SITE] = CC2.GetMeasResult(SITE);
	SERIAL OS_CC2->SetTestResult(SITE, 0, os_result[9][SITE]);


	//DP
	SERIAL os_result[1][SITE] = DP.GetMeasResult(SITE);
	SERIAL OS_DP->SetTestResult(SITE, 0, os_result[1][SITE]);
	//DM
	SERIAL os_result[2][SITE] = DM.GetMeasResult(SITE);
	SERIAL OS_DM->SetTestResult(SITE, 0, os_result[2][SITE]);

	//************************************ Add OS classify delog ***********************************************
	//test_method.OS_Classify(funcindex, pin_str_array, PIN_NUM, NULL, LEAK_NUM, os_result, NULL, vec_exclude);
	//************************************ Add OS classify delog end *******************************************

	
    return 0;
}
 
DUT_API int T02_P2P_Leakage_pre(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *P2P_LEAK_VBUS_pre = StsGetParam(funcindex, "P2P_LEAK_VBUS_pre");
    CParam *P2P_LEAK_DP_pre = StsGetParam(funcindex, "P2P_LEAK_DP_pre");
    CParam *P2P_LEAK_DM_pre = StsGetParam(funcindex, "P2P_LEAK_DM_pre");
    CParam *P2P_LEAK_PD_nINT_pre = StsGetParam(funcindex, "P2P_LEAK_PD_nINT_pre");
    CParam *P2P_LEAK_SCL_pre = StsGetParam(funcindex, "P2P_LEAK_SCL_pre");
    CParam *P2P_LEAK_SDA_pre = StsGetParam(funcindex, "P2P_LEAK_SDA_pre");
    CParam *P2P_LEAK_INT_pre = StsGetParam(funcindex, "P2P_LEAK_INT_pre");
    CParam *P2P_LEAK_CC1_pre = StsGetParam(funcindex, "P2P_LEAK_CC1_pre");
    CParam *P2P_LEAK_CE_pre = StsGetParam(funcindex, "P2P_LEAK_CE_pre");
    CParam *P2P_LEAK_CC2_pre = StsGetParam(funcindex, "P2P_LEAK_CC2_pre");
    CParam *P2P_LEAK_TSBAT_pre = StsGetParam(funcindex, "P2P_LEAK_TSBAT_pre");
    CParam *P2P_LEAK_QON_pre = StsGetParam(funcindex, "P2P_LEAK_QON_pre");
    CParam *P2P_LEAK_VBAT_pre = StsGetParam(funcindex, "P2P_LEAK_VBAT_pre");
    CParam *P2P_LEAK_VSYS_pre = StsGetParam(funcindex, "P2P_LEAK_VSYS_pre");
    CParam *P2P_LEAK_SW_pre = StsGetParam(funcindex, "P2P_LEAK_SW_pre");
    CParam *P2P_LEAK_BTST_pre = StsGetParam(funcindex, "P2P_LEAK_BTST_pre");
    CParam *P2P_LEAK_REGN_pre = StsGetParam(funcindex, "P2P_LEAK_REGN_pre");
    CParam *P2P_LEAK_PMID_pre = StsGetParam(funcindex, "P2P_LEAK_PMID_pre");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here


	//PD_nINT
	p2p_leakage_test("PD_nINT", 50e-3, DCM_PPMUIRANGE_2MA, 300, MEAS_PD_nINT.P2P_leak_pre);
	//SCL
	p2p_leakage_test("SCL", 50e-3, DCM_PPMUIRANGE_2MA, 300, MEAS_SCL.P2P_leak_pre);
	//SDA
	p2p_leakage_test("SDA", 50e-3, DCM_PPMUIRANGE_2MA, 300, MEAS_SDA.P2P_leak_pre);
	//INT_AMUX
	p2p_leakage_test("INT_AMUX", 50e-3, DCM_PPMUIRANGE_2MA, 300, MEAS_INT.P2P_leak_pre);
	//CE
	p2p_leakage_test("CE", 50e-3, DCM_PPMUIRANGE_2MA, 300, MEAS_CE.P2P_leak_pre);
	//QON
	p2p_leakage_test("QON", 50e-3, DCM_PPMUIRANGE_2MA, 300, MEAS_QON.P2P_leak_pre);

	//TSBAT
	p2p_leakage_test(TSBAT, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_TSBAT.P2P_leak_pre);
	//REGN
	p2p_leakage_test(REGN, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_REGN.P2P_leak_pre);
	//PMID
	p2p_leakage_test(PMID, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_PMID.P2P_leak_pre);
	//BTST
	p2p_leakage_test(BTST, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_BTST.P2P_leak_pre);
	//CC1
	p2p_leakage_test(CC1, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_CC1.P2P_leak_pre);
	//CC2
	p2p_leakage_test(CC2, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_CC2.P2P_leak_pre);
	//DP
	p2p_leakage_test(DP, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_DP.P2P_leak_pre);
	//DM
	p2p_leakage_test(DM, 50e-3, ACM_N2P18V, ACM_200UA, 300, MEAS_DM.P2P_leak_pre);


	//VBAT
	p2p_leakage_test(VBAT, 50e-3, FOVIe_1V, FOVIe_100UA, 300, MEAS_VBAT.P2P_leak_pre);
	//VSYS
	p2p_leakage_test(VSYS, 50e-3, FOVIe_1V, FOVIe_100UA, 300, MEAS_VSYS.P2P_leak_pre);
	//VBUS
	p2p_leakage_test(VBUS, 50e-3, FOVIe_1V, FOVIe_100UA, 300, MEAS_VBUS.P2P_leak_pre);
	//SW
	p2p_leakage_test(SW, 50e-3, FOVIe_1V, FOVIe_100UA, 300, MEAS_SW.P2P_leak_pre);




	SERIAL P2P_LEAK_VBUS_pre->SetTestResult(SITE, 0, MEAS_VBUS.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_DP_pre->SetTestResult(SITE, 0, MEAS_DP.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_DM_pre->SetTestResult(SITE, 0, MEAS_DM.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_PD_nINT_pre->SetTestResult(SITE, 0, MEAS_PD_nINT.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_SCL_pre->SetTestResult(SITE, 0, MEAS_SCL.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_SDA_pre->SetTestResult(SITE, 0, MEAS_SDA.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_INT_pre->SetTestResult(SITE, 0, MEAS_INT.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_CC1_pre->SetTestResult(SITE, 0, MEAS_CC1.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_CE_pre->SetTestResult(SITE, 0, MEAS_CE.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_CC2_pre->SetTestResult(SITE, 0, MEAS_CC2.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_TSBAT_pre->SetTestResult(SITE, 0, MEAS_TSBAT.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_QON_pre->SetTestResult(SITE, 0, MEAS_QON.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_VBAT_pre->SetTestResult(SITE, 0, MEAS_VBAT.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_VSYS_pre->SetTestResult(SITE, 0, MEAS_VSYS.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_SW_pre->SetTestResult(SITE, 0, MEAS_SW.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_BTST_pre->SetTestResult(SITE, 0, MEAS_BTST.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_REGN_pre->SetTestResult(SITE, 0, MEAS_REGN.P2P_leak_pre[SITE] * 1e9);
	SERIAL P2P_LEAK_PMID_pre->SetTestResult(SITE, 0, MEAS_PMID.P2P_leak_pre[SITE] * 1e9);



    return 0;
}
 
DUT_API int T03_PIN_Leakage_pre(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *PIN_LEAK_VBUS_pre = StsGetParam(funcindex, "PIN_LEAK_VBUS_pre");
    CParam *PIN_LEAK_DP_pre = StsGetParam(funcindex, "PIN_LEAK_DP_pre");
    CParam *PIN_LEAK_DM_pre = StsGetParam(funcindex, "PIN_LEAK_DM_pre");
    CParam *PIN_LEAK_PD_nINT_pre = StsGetParam(funcindex, "PIN_LEAK_PD_nINT_pre");
    CParam *PIN_LEAK_SCL_pre = StsGetParam(funcindex, "PIN_LEAK_SCL_pre");
    CParam *PIN_LEAK_SDA_pre = StsGetParam(funcindex, "PIN_LEAK_SDA_pre");
    CParam *PIN_LEAK_INT_pre = StsGetParam(funcindex, "PIN_LEAK_INT_pre");
    CParam *PIN_LEAK_CC1_pre = StsGetParam(funcindex, "PIN_LEAK_CC1_pre");
    CParam *PIN_LEAK_CE_pre = StsGetParam(funcindex, "PIN_LEAK_CE_pre");
    CParam *PIN_LEAK_CC2_pre = StsGetParam(funcindex, "PIN_LEAK_CC2_pre");
    CParam *PIN_LEAK_TSBAT_pre = StsGetParam(funcindex, "PIN_LEAK_TSBAT_pre");
    CParam *PIN_LEAK_QON_pre = StsGetParam(funcindex, "PIN_LEAK_QON_pre");
    CParam *PIN_LEAK_VBAT_pre = StsGetParam(funcindex, "PIN_LEAK_VBAT_pre");
    CParam *PIN_LEAK_VSYS_pre = StsGetParam(funcindex, "PIN_LEAK_VSYS_pre");
    CParam *PIN_LEAK_SW_pre = StsGetParam(funcindex, "PIN_LEAK_SW_pre");
    CParam *PIN_LEAK_BTST_pre = StsGetParam(funcindex, "PIN_LEAK_BTST_pre");
    CParam *PIN_LEAK_REGN_pre = StsGetParam(funcindex, "PIN_LEAK_REGN_pre");
    CParam *PIN_LEAK_PMID_pre = StsGetParam(funcindex, "PIN_LEAK_PMID_pre");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here
    return 0;
}
 
DUT_API int T04_EE_INIT(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *EEPROM0_READ = StsGetParam(funcindex, "EEPROM0_READ");
    CParam *EEPROM1_READ = StsGetParam(funcindex, "EEPROM1_READ");
    CParam *EEPROM2_READ = StsGetParam(funcindex, "EEPROM2_READ");
    CParam *EEPROM3_READ = StsGetParam(funcindex, "EEPROM3_READ");
    CParam *EEPROM4_READ = StsGetParam(funcindex, "EEPROM4_READ");
    CParam *EEPROM5_READ = StsGetParam(funcindex, "EEPROM5_READ");
    CParam *EEPROM6_READ = StsGetParam(funcindex, "EEPROM6_READ");
    CParam *EEPROM7_READ = StsGetParam(funcindex, "EEPROM7_READ");
    CParam *EEPROM8_READ = StsGetParam(funcindex, "EEPROM8_READ");
    CParam *EEPROM9_READ = StsGetParam(funcindex, "EEPROM9_READ");
    CParam *EEPROM10_READ = StsGetParam(funcindex, "EEPROM10_READ");
    CParam *EEPROM11_READ = StsGetParam(funcindex, "EEPROM11_READ");
    CParam *EEPROM12_READ = StsGetParam(funcindex, "EEPROM12_READ");
    CParam *EEPROM13_READ = StsGetParam(funcindex, "EEPROM13_READ");
    CParam *EEPROM14_READ = StsGetParam(funcindex, "EEPROM14_READ");
    CParam *EEPROM15_READ = StsGetParam(funcindex, "EEPROM15_READ");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here


	BYTE EE_READ[BANK_NUM][SITE_NUM] = { 0 };



	VBAT.Set(FV, 4, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBUS.Set(FV, 5, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(6);
	Enter_TM();
	I2C_WR(DEV_ADDR_BUCK, 0xD2, 0x01);//dcdc no switch

	for (int reg = 0; reg < BANK_NUM; ++reg)
	{
		I2C_RD(DEV_ADDR_BUCK, EE_BANK_ST + reg, EE_READ[reg]);
		string EE_str = "EEPROM" + to_string(reg) + "_READ";
		SERIAL StsGetParam(funcindex, EE_str.c_str())->SetTestResult(SITE, 0, EE_READ[reg][SITE]);
	}




	VBAT.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);
	VBUS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);


	return 0;
}
 
DUT_API int T05_IQ_pre(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *IQ_VBAT_pre = StsGetParam(funcindex, "IQ_VBAT_pre");
    CParam *IQ_BATFETOFF_pre = StsGetParam(funcindex, "IQ_BATFETOFF_pre");
    CParam *IQ_BAT_DRP_pre = StsGetParam(funcindex, "IQ_BAT_DRP_pre");
    CParam *IQ_EOC_pre = StsGetParam(funcindex, "IQ_EOC_pre");
    CParam *IQ_VBUS5V_HIZ_pre = StsGetParam(funcindex, "IQ_VBUS5V_HIZ_pre");
    CParam *IQ_VBUS9V_HIZ_pre = StsGetParam(funcindex, "IQ_VBUS9V_HIZ_pre");
    CParam *IQ_VBUS_pre = StsGetParam(funcindex, "IQ_VBUS_pre");
    CParam *IQ_VBUS_UVLO_pre = StsGetParam(funcindex, "IQ_VBUS_UVLO_pre");
    CParam *IBOOST_pre = StsGetParam(funcindex, "IBOOST_pre");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here


	dcm.Connect("CE");
	dcm.SetPPMU("CE", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);

	/////////////////////////////////////////////IQ_VBAT
	VBAT.Set(FV, 4, FOVIe_10V, FOVIe_1MA, FOVIe_RELAY_ON);
	delay_ms(5);
	Enter_TM();
	I2C_WR(DEV_ADDR_BUCK, 0xDA, 0x10);//speed up
	I2C_WR(DEV_ADDR_BUCK, 0x3B, 0x00);//BATFET_EN
	//I2C_WR(DEV_ADDR_BUCK, 0xDD, 0x53);//BATONLY
	delay_ms(10);
	VBAT.Set(FV, 4, FOVIe_10V, FOVIe_100UA, FOVIe_RELAY_ON);
	VBAT.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_VBAT[SITE] = VBAT.GetMeasResult(SITE, MIRET);


	/////////////////////////////////////////////IQ_BATFETOFF
	I2C_WR(DEV_ADDR_BUCK, 0x3B, 0x02);//BATFET_DIS
	delay_ms(1);
	VBAT.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_BATFETOFF[SITE] = VBAT.GetMeasResult(SITE, MIRET);


	/////////////////////////////////////////////IQ_VBAT_DRP
	VBAT.Set(FV, 4, FOVIe_10V, FOVIe_1MA, FOVIe_RELAY_ON);
	I2C_WR(DEV_ADDR_PD, 0x9B, 0xA0);//Non-Shutdown mode
	I2C_WR(DEV_ADDR_PD, 0x1A, 0x45);//DRP,CC=RP
	I2C_WR(DEV_ADDR_PD, 0x23, 0x99);//Start DRP
	I2C_WR(DEV_ADDR_PD, 0x90, 0x08);//LOW_POWER_MODE
	I2C_WR(DEV_ADDR_BUCK, 0x3B, 0x00);//BATFET_EN
	delay_ms(10);
	VBAT.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_BAT_DRP[SITE] = VBAT.GetMeasResult(SITE, MIRET);


	/////////////////////////////////////////////IQ_EOC
	VBAT.Set(FV, 0, FOVIe_10V, FOVIe_1MA, FOVIe_RELAY_ON);
	delay_ms(1);

	VBUS.Set(FV, 5, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBAT.Set(FV, 4.4, FOVIe_10V, FOVIe_1MA, FOVIe_RELAY_ON);
	REGN.Set(FV, 4.4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	delay_ms(5);
	I2C_WR(DEV_ADDR_BUCK, 0x31, 0x57);//4.4V VBAT REG
	delay_ms(5);
	VBAT.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_EOC[SITE] = VBAT.GetMeasResult(SITE, MIRET);

	VBAT.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBUS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	REGN.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	delay_ms(1);
	REGN.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);


	/////////////////////////////////////////////IQ_VBUS5V_HIZ
	VBUS.Set(FV, 5, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBAT.Set(FV, 4, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(10);

	I2C_WR(DEV_ADDR_BUCK, 0x3C, 0x80);//HIZ
	I2C_WR(DEV_ADDR_BUCK, 0x3B, 0x02);//BATFET_DIS
	VBUS.Set(FV, 5, FOVIe_10V, FOVIe_10MA, FOVIe_RELAY_ON);
	delay_ms(1);
	VBUS.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_VBUS5V_HIZ[SITE] = VBUS.GetMeasResult(SITE, MIRET);

	/////////////////////////////////////////////IQ_VBUS9V_HIZ
	VBUS.Set(FV, 9, FOVIe_10V, FOVIe_10MA, FOVIe_RELAY_ON);
	delay_ms(1);
	VBUS.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_VBUS9V_HIZ[SITE] = VBUS.GetMeasResult(SITE, MIRET);

	////////////////IQ_VBUS_pre ////////////////
	VBUS.Set(FV, 5, FOVIe_10V, FOVIe_10MA, FOVIe_RELAY_ON);
	VBAT.Set(FV, 4, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	I2C_WR(DEV_ADDR_BUCK, 0x3C, 0x04);//CHG DIS
	delay_ms(1);
	VBUS.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_VBUS[SITE] = VBUS.GetMeasResult(SITE, MIRET);

	////////////////IQ_VBUS_UVLO_pre ////////////////
	VBUS.Set(FV, 3, FOVIe_10V, FOVIe_10MA, FOVIe_RELAY_ON);
	delay_ms(1);
	VBUS.MeasureVI(100, 10);
	SERIAL IQ_PRE.IQ_VBUS_UVLO[SITE] = VBUS.GetMeasResult(SITE, MIRET);



	/////////////////////////////////////////////IBOOST
	VBUS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBAT.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(1);
	VBUS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);
	VBAT.Set(FV, 4.5, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(2);
	I2C_WR(DEV_ADDR_BUCK, 0x90, 0x00);//dis D+/D- det
	I2C_WR(DEV_ADDR_PD, 0x9B, 0xC0);//shutdown off vac ctrl
	Enter_TM();
	I2C_WR(DEV_ADDR_INTERNAL, 0xB2, 0x33);//OTG-PMID_UV DIS
	I2C_WR(DEV_ADDR_BUCK, 0x3A, 0x00);//DIS JEITA
	I2C_WR(DEV_ADDR_BUCK, 0xDA, 0x1C);//speed up
	I2C_WR(DEV_ADDR_BUCK, 0xD2, 0x01);//dcdc no switch

	I2C_WR(DEV_ADDR_BUCK, 0x3C, 0x06);//tm en qb boost
	I2C_WR(DEV_ADDR_BUCK, 0xDD, 0x54);//OTG-state
	delay_ms(25);
	VBAT.MeasureVI(200, 10);
	SERIAL IQ_PRE.IBOOST[SITE] = VBAT.GetMeasResult(SITE, MIRET);





	SERIAL IQ_VBAT_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_VBAT[SITE] * 1e6);
	SERIAL IQ_BATFETOFF_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_BATFETOFF[SITE] * 1e6);
	SERIAL IQ_BAT_DRP_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_BAT_DRP[SITE] * 1e6);
	SERIAL IQ_EOC_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_EOC[SITE] * 1e6);
	SERIAL IQ_VBUS5V_HIZ_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_VBUS5V_HIZ[SITE] * 1e6);
	SERIAL IQ_VBUS9V_HIZ_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_VBUS9V_HIZ[SITE] * 1e6);
	SERIAL IQ_VBUS_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_VBUS[SITE] * 1e3);
	SERIAL IQ_VBUS_UVLO_pre->SetTestResult(SITE, 0, IQ_PRE.IQ_VBUS_UVLO[SITE] * 1e3);
	SERIAL IBOOST_pre->SetTestResult(SITE, 0, IQ_PRE.IBOOST[SITE] * 1e3);


	return 0;
}
 
DUT_API int T06_Ioff_pre(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Ioff_Q1_pre = StsGetParam(funcindex, "Ioff_Q1_pre");
    CParam *Ioff_Q2_pre = StsGetParam(funcindex, "Ioff_Q2_pre");
    CParam *Ioff_Q3_pre = StsGetParam(funcindex, "Ioff_Q3_pre");
    CParam *Ioff_Q4_pre = StsGetParam(funcindex, "Ioff_Q4_pre");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here

		//Q1:5V   Q2:12V  Q3:12V  Q4:5V

	VBUS.Set(FV, 5, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBAT.Set(FV, 4.2, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(2);
	Enter_TM();
	I2C_WR(DEV_ADDR_BUCK, 0x3C, 0x80);//HIZ-state
	I2C_WR(DEV_ADDR_BUCK, 0x3B, 0x02);//DIS BATFET
	I2C_WR(DEV_ADDR_BUCK, 0xDD, 0x59);//HIZ-state

	PMID.Set(FV, 5.0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	VBUS.Set(FV, 0, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(2);
	VBUS.Set(FV, 0, FOVIe_20V, FOVIe_100UA, FOVIe_RELAY_ON);
	delay_ms(2);
	VBUS.MeasureVI(100, 10);
	SERIAL Q1.Ioff_pre[SITE] = VBUS.GetMeasResult(SITE, MIRET);


	VBUS.Set(FV, 12, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	PMID.Set(FV, 12, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);

	SW.Set(FV, 0, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	BTST.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);

	delay_ms(2);
	SW.Set(FV, 0, FOVIe_20V, FOVIe_10UA, FOVIe_RELAY_ON);
	delay_ms(2);
	SW.MeasureVI(100, 10);
	SERIAL Q2.Ioff_pre[SITE] = SW.GetMeasResult(SITE, MIRET);



	VBUS.Set(FV, 6, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	PMID.Set(FV, 6, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	BTST.Set(FV, 6, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	SW.Set(FV, 6, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBUS.Set(FV, 12, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	PMID.Set(FV, 12, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	BTST.Set(FV, 12, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	SW.Set(FV, 12, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(2);
	SW.Set(FV, 12, FOVIe_20V, FOVIe_10UA, FOVIe_RELAY_ON);
	delay_ms(2);
	SW.MeasureVI(100, 10);
	SERIAL Q3.Ioff_pre[SITE] = SW.GetMeasResult(SITE, MIRET);


	VBUS.Set(FV, 6, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	PMID.Set(FV, 6, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	BTST.Set(FV, 6, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	SW.Set(FV, 6, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(1);
	VBUS.Set(FV, 0, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_OFF);
	PMID.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	BTST.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	SW.Set(FV, 0, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_OFF);
	//FORCE_ALLPIN_0V_OFF(1);


	VBAT.Set(FV, 5, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(2);
	Enter_TM();
	I2C_WR(DEV_ADDR_BUCK, 0x3C, 0x80);//HIZ-state
	I2C_WR(DEV_ADDR_BUCK, 0x3B, 0x02);//DIS BATFET

	VSYS.Set(FV, 0, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_ON);
	delay_ms(2);
	VSYS.Set(FV, 0, FOVIe_20V, FOVIe_10UA, FOVIe_RELAY_ON);
	delay_ms(2);
	VSYS.MeasureVI(100, 10);
	SERIAL Q4.Ioff_pre[SITE] = VSYS.GetMeasResult(SITE, MIRET);

	VSYS.Set(FV, 0, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_OFF);
	VBAT.Set(FV, 0, FOVIe_20V, FOVIe_100MA, FOVIe_RELAY_OFF);


	SERIAL Ioff_Q1_pre->SetTestResult(SITE, 0, Q1.Ioff_pre[SITE] * 1e9);
	SERIAL Ioff_Q2_pre->SetTestResult(SITE, 0, Q2.Ioff_pre[SITE] * 1e9);
	SERIAL Ioff_Q3_pre->SetTestResult(SITE, 0, Q3.Ioff_pre[SITE] * 1e9);
	SERIAL Ioff_Q4_pre->SetTestResult(SITE, 0, Q4.Ioff_pre[SITE] * 1e9);










    return 0;
}
 
DUT_API int T07_Idrive(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Idrive_Q1_pre = StsGetParam(funcindex, "Idrive_Q1_pre");
    CParam *Idrive_Q2_pre = StsGetParam(funcindex, "Idrive_Q2_pre");
    CParam *Idrive_Q3_pre = StsGetParam(funcindex, "Idrive_Q3_pre");
    CParam *Idrive_Q4_pre = StsGetParam(funcindex, "Idrive_Q4_pre");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here
	int OVST_TIME = 100;

	//////////////////////////////////////Q1
	VBAT.Set(FV, 6, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBUS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	PMID.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.Connect("INT_AMUX");
	dcm.SetPPMU("INT_AMUX", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);
	delay_ms(2);
	Enter_TM();
	I2C_WR(DEV_ADDR_BUCK, 0xD0, 0x03);//AMUX_EN + OVST Q1

	dcm.SetPPMU("INT_AMUX", DCM_PPMU_FVMI, 5, DCM_PPMUIRANGE_2MA);
	delay_ms(1);
	dcm.PPMUMeasure("INT_AMUX", 100, 10);

	SERIAL Q1.Idrive_pre[SITE] = dcm.GetPPMUMeasResult("INT_AMUX", SITE);

	//////////////////////////////////////Q4
	PMID.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	VBUS.Set(FV, 5, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBAT.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VSYS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);

	I2C_WR(DEV_ADDR_BUCK, 0xD0, 0x09);//AMUX_EN + OVST Q4
	
	dcm.SetPPMU("INT_AMUX", DCM_PPMU_FVMI, 5, DCM_PPMUIRANGE_2MA);
	delay_ms(1);
	dcm.PPMUMeasure("INT_AMUX", 100, 10);

	SERIAL Q4.Idrive_pre[SITE] = dcm.GetPPMUMeasResult("INT_AMUX", SITE);

	I2C_WR(DEV_ADDR_BUCK, 0xD0, 0x00);//recover
	dcm.SetPPMU("INT_AMUX", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);
	dcm.Disconnect("INT_AMUX");

	VSYS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);
	//////////////////////////////////////Q2
	BTST.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	VSYS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);

	BTST.Set(FV, 5, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);
	delay_ms(3);
	BTST.MeasureVI(ACM_MI, 100, 10);
	SERIAL adresult[SITE] = BTST.GetMeasResult(SITE);

	I2C_WR(DEV_ADDR_BUCK, 0xD2, 0x01);//DCDC_NO_SWITCH
	I2C_WR(DEV_ADDR_BUCK, 0xD1, 0xB0);//HS = 1 DCDC_EN
	BTST.Set(FV, 5, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);
	delay_ms(3);
	BTST.MeasureVI(ACM_MI, 100, 10);

	SERIAL Q2.Idrive_pre[SITE] = BTST.GetMeasResult(SITE) - adresult[SITE];
	I2C_WR(DEV_ADDR_BUCK, 0xD1, 0x00);//hs_off
	BTST.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	//////////////////////////////////////Q3
	//Q2ON Q3OFF
	REGN.Set(FV, 5, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);
	delay_ms(10);
	REGN.MeasureVI(ACM_MI, 100, 10);

	SERIAL adresult[SITE] = REGN.GetMeasResult(SITE);


	//Q2OFF Q3ON
	I2C_WR(DEV_ADDR_BUCK, 0xD1, 0xD0);//LS = 1 DCDC_EN
	REGN.Set(FV, 5, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);
	delay_ms(1);
	REGN.MeasureVI(ACM_MI, 100, 10);

	SERIAL Q3.Idrive_pre[SITE] = adresult[SITE] - REGN.GetMeasResult(SITE);




	SERIAL Idrive_Q1_pre->SetTestResult(SITE, 0, Q1.Idrive_pre[SITE] * 1e6);
	SERIAL Idrive_Q2_pre->SetTestResult(SITE, 0, Q2.Idrive_pre[SITE] * 1e6);
	SERIAL Idrive_Q3_pre->SetTestResult(SITE, 0, Q3.Idrive_pre[SITE] * 1e6);
	SERIAL Idrive_Q4_pre->SetTestResult(SITE, 0, Q4.Idrive_pre[SITE] * 1e6);
    return 0;
}
 
DUT_API int T08_ABS_LEAK(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *VBUS_ABS_LEAK = StsGetParam(funcindex, "VBUS_ABS_LEAK");
    CParam *DP_ABS_LEAK = StsGetParam(funcindex, "DP_ABS_LEAK");
    CParam *DM_ABS_LEAK = StsGetParam(funcindex, "DM_ABS_LEAK");
    CParam *PD_nINT_ABS_LEAK = StsGetParam(funcindex, "PD_nINT_ABS_LEAK");
    CParam *SCL_ABS_LEAK = StsGetParam(funcindex, "SCL_ABS_LEAK");
    CParam *SDA_ABS_LEAK = StsGetParam(funcindex, "SDA_ABS_LEAK");
    CParam *INT_ABS_LEAK = StsGetParam(funcindex, "INT_ABS_LEAK");
    CParam *CC1_ABS_LEAK = StsGetParam(funcindex, "CC1_ABS_LEAK");
    CParam *CE_ABS_LEAK = StsGetParam(funcindex, "CE_ABS_LEAK");
    CParam *CC2_ABS_LEAK = StsGetParam(funcindex, "CC2_ABS_LEAK");
    CParam *TSBAT_ABS_LEAK = StsGetParam(funcindex, "TSBAT_ABS_LEAK");
    CParam *QON_ABS_LEAK = StsGetParam(funcindex, "QON_ABS_LEAK");
    CParam *VBAT_ABS_LEAK = StsGetParam(funcindex, "VBAT_ABS_LEAK");
    CParam *VSYS_ABS_LEAK = StsGetParam(funcindex, "VSYS_ABS_LEAK");
    CParam *SW_ABS_LEAK = StsGetParam(funcindex, "SW_ABS_LEAK");
    CParam *BTST_ABS_LEAK = StsGetParam(funcindex, "BTST_ABS_LEAK");
    CParam *REGN_ABS_LEAK = StsGetParam(funcindex, "REGN_ABS_LEAK");
    CParam *PMID_ABS_LEAK = StsGetParam(funcindex, "PMID_ABS_LEAK");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here
    return 0;
}
 
DUT_API int T09_DIGITAL_TEST(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *SCAN_CHAIN_TS1 = StsGetParam(funcindex, "SCAN_CHAIN_TS1");
    CParam *SCAN_CHAIN_TS2 = StsGetParam(funcindex, "SCAN_CHAIN_TS2");
    CParam *Vbox_high = StsGetParam(funcindex, "Vbox_high");
    CParam *Vbox_low = StsGetParam(funcindex, "Vbox_low");
    CParam *IDDQ = StsGetParam(funcindex, "IDDQ");
    CParam *IDDQ_0 = StsGetParam(funcindex, "IDDQ_0");
    CParam *IDDQ_1 = StsGetParam(funcindex, "IDDQ_1");
    CParam *IDDQ_2 = StsGetParam(funcindex, "IDDQ_2");
    CParam *IDDQ_3 = StsGetParam(funcindex, "IDDQ_3");
    CParam *IDDQ_4 = StsGetParam(funcindex, "IDDQ_4");
    CParam *IDDQ_5 = StsGetParam(funcindex, "IDDQ_5");
    CParam *IDDQ_6 = StsGetParam(funcindex, "IDDQ_6");
    CParam *IDDQ_7 = StsGetParam(funcindex, "IDDQ_7");
    CParam *IDDQ_8 = StsGetParam(funcindex, "IDDQ_8");
    CParam *IDDQ_9 = StsGetParam(funcindex, "IDDQ_9");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here

	bool get_result_flag[SITE_NUM];
	int FAIL_FLAG[SITE_NUM];
	ULONG FAIL_COUNT[SITE_NUM];

	dcm.Connect("SCAN");
	dcm.SetPinLevel("SCAN", 3, 0, 1.0, 0.8);
	SERIAL dcm.SetDynamicLoad("INT_AMUX", SITE, DCM_OPEN_CLAMP_OPEN_LOAD, 0.015, 0.015, 3, 5, -2);

	VBAT.Set(FV, 3.9, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	delay_ms(2);
	Enter_TM();
	I2C_WR(DEV_ADDR_BUCK, 0xFE, 0x5A);//SCAN MODE



	/////////////////////////////VBOXH///////////////////////////////
	REGN.Set(FV, 8, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "Scan_ts1_start", "Scan_ts1_stop", TRUE);

	SERIAL FAIL_COUNT[SITE] = 9999;
	SERIAL dcm.GetFailCount("INT_D", SITE, FAIL_COUNT[SITE]);

	SERIAL Vbox_high->SetTestResult(SITE, 0, FAIL_COUNT[SITE]);

	delay_ms(10);
	/////////////////////////////IDDQ_TP///////////////////////////////
	int iddq_dly_time = 2;
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_start", "iddq_tp_1", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_0->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_1", "iddq_tp_2", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_1->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_2", "iddq_tp_3", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_2->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_3", "iddq_tp_4", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_3->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_4", "iddq_tp_5", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_4->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_5", "iddq_tp_6", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_5->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_6", "iddq_tp_7", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_6->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_7", "iddq_tp_8", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_7->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_8", "iddq_tp_9", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_8->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_9", "iddq_tp_10", TRUE);
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200UA, ACM_RELAY_ON);
	delay_ms(iddq_dly_time);
	REGN.MeasureVI(ACM_MI, 100, 10);
	SERIAL IDDQ_9->SetTestResult(SITE, 0, REGN.GetMeasResult(SITE) * 1e6);




	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "iddq_tp_10", "iddq_stop", TRUE);


	/////////////////////////////TS1///////////////////////////////
	REGN.Set(FV, 5, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "Scan_ts1_start", "Scan_ts1_stop", TRUE);

	SERIAL FAIL_COUNT[SITE] = 9999;
	SERIAL dcm.GetFailCount("INT_D", SITE, FAIL_COUNT[SITE]);

	SERIAL SCAN_CHAIN_TS1->SetTestResult(SITE, 0, FAIL_COUNT[SITE]);


	/////////////////////////////VBOXL///////////////////////////////
	REGN.Set(FV, 4, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "Scan_ts1_start", "Scan_ts1_stop", TRUE);

	SERIAL FAIL_COUNT[SITE] = 9999;
	SERIAL dcm.GetFailCount("INT_D", SITE, FAIL_COUNT[SITE]);

	SERIAL Vbox_low->SetTestResult(SITE, 0, FAIL_COUNT[SITE]);

	/////////////////////////////IDDQ///////////////////////////////
	dcm.RunVectorWithGroup("SCAN", "iddq_start", "iddq_stop", TRUE);

	SERIAL FAIL_COUNT[SITE] = 9999;
	SERIAL dcm.GetFailCount("INT_D", SITE, FAIL_COUNT[SITE]);

	SERIAL IDDQ->SetTestResult(SITE, 0, FAIL_COUNT[SITE]);


	/////////////////////////////TS2///////////////////////////////
	REGN.Set(FV, 5, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	dcm.RunVectorWithGroup("SCAN", "Scan_ts2_start", "Scan_ts2_stop", TRUE);

	SERIAL FAIL_COUNT[SITE] = 9999;
	SERIAL dcm.GetFailCount("INT_D", SITE, FAIL_COUNT[SITE]);

	SERIAL SCAN_CHAIN_TS2->SetTestResult(SITE, 0, FAIL_COUNT[SITE]);









    return 0;
}
 
DUT_API int T10_Ioff_post(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Ioff_Q1_post = StsGetParam(funcindex, "Ioff_Q1_post");
    CParam *Ioff_Q2_post = StsGetParam(funcindex, "Ioff_Q2_post");
    CParam *Ioff_Q3_post = StsGetParam(funcindex, "Ioff_Q3_post");
    CParam *Ioff_Q4_post = StsGetParam(funcindex, "Ioff_Q4_post");
    CParam *Ioff_Q1_delta = StsGetParam(funcindex, "Ioff_Q1_delta");
    CParam *Ioff_Q2_delta = StsGetParam(funcindex, "Ioff_Q2_delta");
    CParam *Ioff_Q3_delta = StsGetParam(funcindex, "Ioff_Q3_delta");
    CParam *Ioff_Q4_delta = StsGetParam(funcindex, "Ioff_Q4_delta");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here


	SERIAL Ioff_Q1_post->SetTestResult(SITE, 0, Q1.Ioff_post[SITE] * 1e9);
	SERIAL Ioff_Q2_post->SetTestResult(SITE, 0, Q2.Ioff_post[SITE] * 1e9);
	SERIAL Ioff_Q3_post->SetTestResult(SITE, 0, Q3.Ioff_post[SITE] * 1e9);
	SERIAL Ioff_Q4_post->SetTestResult(SITE, 0, Q4.Ioff_post[SITE] * 1e9);
	SERIAL Ioff_Q1_delta->SetTestResult(SITE, 0, (Q1.Ioff_post[SITE] - Q1.Ioff_pre[SITE]) * 1e9);
	SERIAL Ioff_Q2_delta->SetTestResult(SITE, 0, (Q2.Ioff_post[SITE] - Q2.Ioff_pre[SITE]) * 1e9);
	SERIAL Ioff_Q3_delta->SetTestResult(SITE, 0, (Q3.Ioff_post[SITE] - Q3.Ioff_pre[SITE]) * 1e9);
	SERIAL Ioff_Q4_delta->SetTestResult(SITE, 0, (Q4.Ioff_post[SITE] - Q4.Ioff_pre[SITE]) * 1e9);



    return 0;
}
 
DUT_API int T11_IQ_post(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *IQ_VBAT_post = StsGetParam(funcindex, "IQ_VBAT_post");
    CParam *IQ_BATFETOFF_post = StsGetParam(funcindex, "IQ_BATFETOFF_post");
    CParam *IQ_BAT_DRP_post = StsGetParam(funcindex, "IQ_BAT_DRP_post");
    CParam *IQ_EOC_post = StsGetParam(funcindex, "IQ_EOC_post");
    CParam *IQ_VBUS5V_HIZ_post = StsGetParam(funcindex, "IQ_VBUS5V_HIZ_post");
    CParam *IQ_VBUS9V_HIZ_post = StsGetParam(funcindex, "IQ_VBUS9V_HIZ_post");
    CParam *IQ_VBUS_post = StsGetParam(funcindex, "IQ_VBUS_post");
    CParam *IQ_VBUS_UVLO_post = StsGetParam(funcindex, "IQ_VBUS_UVLO_post");
    CParam *IBOOST_post = StsGetParam(funcindex, "IBOOST_post");
    CParam *IQ_VBAT_delta = StsGetParam(funcindex, "IQ_VBAT_delta");
    CParam *IQ_BATFETOFF_delta = StsGetParam(funcindex, "IQ_BATFETOFF_delta");
    CParam *IQ_BAT_DRP_delta = StsGetParam(funcindex, "IQ_BAT_DRP_delta");
    CParam *IQ_EOC_delta = StsGetParam(funcindex, "IQ_EOC_delta");
    CParam *IQ_VBUS5V_HIZ_delta = StsGetParam(funcindex, "IQ_VBUS5V_HIZ_delta");
    CParam *IQ_VBUS9V_HIZ_delta = StsGetParam(funcindex, "IQ_VBUS9V_HIZ_delta");
    CParam *IQ_VBUS_delta = StsGetParam(funcindex, "IQ_VBUS_delta");
    CParam *IQ_VBUS_UVLO_delta = StsGetParam(funcindex, "IQ_VBUS_UVLO_delta");
    CParam *IBOOST_delta = StsGetParam(funcindex, "IBOOST_delta");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here


	SERIAL IQ_VBAT_post->SetTestResult(SITE, 0, IQ_POST.IQ_VBAT[SITE] * 1e6);
	SERIAL IQ_BATFETOFF_post->SetTestResult(SITE, 0, IQ_POST.IQ_BATFETOFF[SITE] * 1e6);
	SERIAL IQ_BAT_DRP_post->SetTestResult(SITE, 0, IQ_POST.IQ_BAT_DRP[SITE] * 1e6);
	SERIAL IQ_EOC_post->SetTestResult(SITE, 0, IQ_POST.IQ_EOC[SITE] * 1e6);
	SERIAL IQ_VBUS5V_HIZ_post->SetTestResult(SITE, 0, IQ_POST.IQ_VBUS5V_HIZ[SITE] * 1e6);
	SERIAL IQ_VBUS9V_HIZ_post->SetTestResult(SITE, 0, IQ_POST.IQ_VBUS9V_HIZ[SITE] * 1e6);
	SERIAL IQ_VBUS_post->SetTestResult(SITE, 0, IQ_POST.IQ_VBUS[SITE] * 1e3);
	SERIAL IQ_VBUS_UVLO_post->SetTestResult(SITE, 0, IQ_POST.IQ_VBUS_UVLO[SITE] * 1e3);
	SERIAL IBOOST_post->SetTestResult(SITE, 0, IQ_POST.IBOOST[SITE] * 1e3);

	SERIAL IQ_VBAT_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_VBAT[SITE] - IQ_PRE.IQ_VBAT[SITE]) * 1e6);
	SERIAL IQ_BATFETOFF_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_BATFETOFF[SITE] - IQ_PRE.IQ_BATFETOFF[SITE]) * 1e6);
	SERIAL IQ_BAT_DRP_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_BAT_DRP[SITE] - IQ_PRE.IQ_BAT_DRP[SITE]) * 1e6);
	SERIAL IQ_EOC_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_EOC[SITE] - IQ_PRE.IQ_EOC[SITE]) * 1e6);
	SERIAL IQ_VBUS5V_HIZ_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_VBUS5V_HIZ[SITE] - IQ_PRE.IQ_VBUS5V_HIZ[SITE]) * 1e6);
	SERIAL IQ_VBUS9V_HIZ_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_VBUS9V_HIZ[SITE] - IQ_PRE.IQ_VBUS9V_HIZ[SITE]) * 1e6);
	SERIAL IQ_VBUS_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_VBUS[SITE] - IQ_PRE.IQ_VBUS[SITE]) * 1e3);
	SERIAL IQ_VBUS_UVLO_delta->SetTestResult(SITE, 0, (IQ_POST.IQ_VBUS_UVLO[SITE] - IQ_PRE.IQ_VBUS_UVLO[SITE]) * 1e3);
	SERIAL IBOOST_delta->SetTestResult(SITE, 0, (IQ_POST.IBOOST[SITE] - IQ_PRE.IBOOST[SITE]) * 1e3);
    return 0;
}
 
DUT_API int T12_PIN_Leakage_post(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *PIN_LEAK_VBUS_post = StsGetParam(funcindex, "PIN_LEAK_VBUS_post");
    CParam *PIN_LEAK_DP_post = StsGetParam(funcindex, "PIN_LEAK_DP_post");
    CParam *PIN_LEAK_DM_post = StsGetParam(funcindex, "PIN_LEAK_DM_post");
    CParam *PIN_LEAK_PD_nINT_post = StsGetParam(funcindex, "PIN_LEAK_PD_nINT_post");
    CParam *PIN_LEAK_SCL_post = StsGetParam(funcindex, "PIN_LEAK_SCL_post");
    CParam *PIN_LEAK_SDA_post = StsGetParam(funcindex, "PIN_LEAK_SDA_post");
    CParam *PIN_LEAK_INT_post = StsGetParam(funcindex, "PIN_LEAK_INT_post");
    CParam *PIN_LEAK_CC1_post = StsGetParam(funcindex, "PIN_LEAK_CC1_post");
    CParam *PIN_LEAK_CE_post = StsGetParam(funcindex, "PIN_LEAK_CE_post");
    CParam *PIN_LEAK_CC2_post = StsGetParam(funcindex, "PIN_LEAK_CC2_post");
    CParam *PIN_LEAK_TSBAT_post = StsGetParam(funcindex, "PIN_LEAK_TSBAT_post");
    CParam *PIN_LEAK_QON_post = StsGetParam(funcindex, "PIN_LEAK_QON_post");
    CParam *PIN_LEAK_VBAT_post = StsGetParam(funcindex, "PIN_LEAK_VBAT_post");
    CParam *PIN_LEAK_VSYS_post = StsGetParam(funcindex, "PIN_LEAK_VSYS_post");
    CParam *PIN_LEAK_SW_post = StsGetParam(funcindex, "PIN_LEAK_SW_post");
    CParam *PIN_LEAK_BTST_post = StsGetParam(funcindex, "PIN_LEAK_BTST_post");
    CParam *PIN_LEAK_REGN_post = StsGetParam(funcindex, "PIN_LEAK_REGN_post");
    CParam *PIN_LEAK_PMID_post = StsGetParam(funcindex, "PIN_LEAK_PMID_post");
    CParam *PIN_LEAK_VBUS_delta = StsGetParam(funcindex, "PIN_LEAK_VBUS_delta");
    CParam *PIN_LEAK_DP_delta = StsGetParam(funcindex, "PIN_LEAK_DP_delta");
    CParam *PIN_LEAK_DM_delta = StsGetParam(funcindex, "PIN_LEAK_DM_delta");
    CParam *PIN_LEAK_PD_nINT_delta = StsGetParam(funcindex, "PIN_LEAK_PD_nINT_delta");
    CParam *PIN_LEAK_SCL_delta = StsGetParam(funcindex, "PIN_LEAK_SCL_delta");
    CParam *PIN_LEAK_SDA_delta = StsGetParam(funcindex, "PIN_LEAK_SDA_delta");
    CParam *PIN_LEAK_INT_delta = StsGetParam(funcindex, "PIN_LEAK_INT_delta");
    CParam *PIN_LEAK_CC1_delta = StsGetParam(funcindex, "PIN_LEAK_CC1_delta");
    CParam *PIN_LEAK_CE_delta = StsGetParam(funcindex, "PIN_LEAK_CE_delta");
    CParam *PIN_LEAK_CC2_delta = StsGetParam(funcindex, "PIN_LEAK_CC2_delta");
    CParam *PIN_LEAK_TSBAT_delta = StsGetParam(funcindex, "PIN_LEAK_TSBAT_delta");
    CParam *PIN_LEAK_QON_delta = StsGetParam(funcindex, "PIN_LEAK_QON_delta");
    CParam *PIN_LEAK_VBAT_delta = StsGetParam(funcindex, "PIN_LEAK_VBAT_delta");
    CParam *PIN_LEAK_VSYS_delta = StsGetParam(funcindex, "PIN_LEAK_VSYS_delta");
    CParam *PIN_LEAK_SW_delta = StsGetParam(funcindex, "PIN_LEAK_SW_delta");
    CParam *PIN_LEAK_BTST_delta = StsGetParam(funcindex, "PIN_LEAK_BTST_delta");
    CParam *PIN_LEAK_REGN_delta = StsGetParam(funcindex, "PIN_LEAK_REGN_delta");
    CParam *PIN_LEAK_PMID_delta = StsGetParam(funcindex, "PIN_LEAK_PMID_delta");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here
    return 0;
}
 
DUT_API int T13_P2P_Leakage_post(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *P2P_LEAK_VBUS_post = StsGetParam(funcindex, "P2P_LEAK_VBUS_post");
    CParam *P2P_LEAK_DP_post = StsGetParam(funcindex, "P2P_LEAK_DP_post");
    CParam *P2P_LEAK_DM_post = StsGetParam(funcindex, "P2P_LEAK_DM_post");
    CParam *P2P_LEAK_PD_nINT_post = StsGetParam(funcindex, "P2P_LEAK_PD_nINT_post");
    CParam *P2P_LEAK_SCL_post = StsGetParam(funcindex, "P2P_LEAK_SCL_post");
    CParam *P2P_LEAK_SDA_post = StsGetParam(funcindex, "P2P_LEAK_SDA_post");
    CParam *P2P_LEAK_INT_post = StsGetParam(funcindex, "P2P_LEAK_INT_post");
    CParam *P2P_LEAK_CC1_post = StsGetParam(funcindex, "P2P_LEAK_CC1_post");
    CParam *P2P_LEAK_CE_post = StsGetParam(funcindex, "P2P_LEAK_CE_post");
    CParam *P2P_LEAK_CC2_post = StsGetParam(funcindex, "P2P_LEAK_CC2_post");
    CParam *P2P_LEAK_TSBAT_post = StsGetParam(funcindex, "P2P_LEAK_TSBAT_post");
    CParam *P2P_LEAK_QON_post = StsGetParam(funcindex, "P2P_LEAK_QON_post");
    CParam *P2P_LEAK_VBAT_post = StsGetParam(funcindex, "P2P_LEAK_VBAT_post");
    CParam *P2P_LEAK_VSYS_post = StsGetParam(funcindex, "P2P_LEAK_VSYS_post");
    CParam *P2P_LEAK_SW_post = StsGetParam(funcindex, "P2P_LEAK_SW_post");
    CParam *P2P_LEAK_BTST_post = StsGetParam(funcindex, "P2P_LEAK_BTST_post");
    CParam *P2P_LEAK_REGN_post = StsGetParam(funcindex, "P2P_LEAK_REGN_post");
    CParam *P2P_LEAK_PMID_post = StsGetParam(funcindex, "P2P_LEAK_PMID_post");
    CParam *P2P_LEAK_VBUS_delta = StsGetParam(funcindex, "P2P_LEAK_VBUS_delta");
    CParam *P2P_LEAK_DP_delta = StsGetParam(funcindex, "P2P_LEAK_DP_delta");
    CParam *P2P_LEAK_DM_delta = StsGetParam(funcindex, "P2P_LEAK_DM_delta");
    CParam *P2P_LEAK_PD_nINT_delta = StsGetParam(funcindex, "P2P_LEAK_PD_nINT_delta");
    CParam *P2P_LEAK_SCL_delta = StsGetParam(funcindex, "P2P_LEAK_SCL_delta");
    CParam *P2P_LEAK_SDA_delta = StsGetParam(funcindex, "P2P_LEAK_SDA_delta");
    CParam *P2P_LEAK_INT_delta = StsGetParam(funcindex, "P2P_LEAK_INT_delta");
    CParam *P2P_LEAK_CC1_delta = StsGetParam(funcindex, "P2P_LEAK_CC1_delta");
    CParam *P2P_LEAK_CE_delta = StsGetParam(funcindex, "P2P_LEAK_CE_delta");
    CParam *P2P_LEAK_CC2_delta = StsGetParam(funcindex, "P2P_LEAK_CC2_delta");
    CParam *P2P_LEAK_TSBAT_delta = StsGetParam(funcindex, "P2P_LEAK_TSBAT_delta");
    CParam *P2P_LEAK_QON_delta = StsGetParam(funcindex, "P2P_LEAK_QON_delta");
    CParam *P2P_LEAK_VBAT_delta = StsGetParam(funcindex, "P2P_LEAK_VBAT_delta");
    CParam *P2P_LEAK_VSYS_delta = StsGetParam(funcindex, "P2P_LEAK_VSYS_delta");
    CParam *P2P_LEAK_SW_delta = StsGetParam(funcindex, "P2P_LEAK_SW_delta");
    CParam *P2P_LEAK_BTST_delta = StsGetParam(funcindex, "P2P_LEAK_BTST_delta");
    CParam *P2P_LEAK_REGN_delta = StsGetParam(funcindex, "P2P_LEAK_REGN_delta");
    CParam *P2P_LEAK_PMID_delta = StsGetParam(funcindex, "P2P_LEAK_PMID_delta");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here



	SERIAL P2P_LEAK_VBUS_post->SetTestResult(SITE, 0, MEAS_VBUS.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_DP_post->SetTestResult(SITE, 0, MEAS_DP.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_DM_post->SetTestResult(SITE, 0, MEAS_DM.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_PD_nINT_post->SetTestResult(SITE, 0, MEAS_PD_nINT.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_SCL_post->SetTestResult(SITE, 0, MEAS_SCL.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_SDA_post->SetTestResult(SITE, 0, MEAS_SDA.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_INT_post->SetTestResult(SITE, 0, MEAS_INT.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_CC1_post->SetTestResult(SITE, 0, MEAS_CC1.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_CE_post->SetTestResult(SITE, 0, MEAS_CE.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_CC2_post->SetTestResult(SITE, 0, MEAS_CC2.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_TSBAT_post->SetTestResult(SITE, 0, MEAS_TSBAT.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_QON_post->SetTestResult(SITE, 0, MEAS_QON.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_VBAT_post->SetTestResult(SITE, 0, MEAS_VBAT.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_VSYS_post->SetTestResult(SITE, 0, MEAS_VSYS.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_SW_post->SetTestResult(SITE, 0, MEAS_SW.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_BTST_post->SetTestResult(SITE, 0, MEAS_BTST.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_REGN_post->SetTestResult(SITE, 0, MEAS_REGN.P2P_leak_post[SITE] * 1e9);
	SERIAL P2P_LEAK_PMID_post->SetTestResult(SITE, 0, MEAS_PMID.P2P_leak_post[SITE] * 1e9);

	SERIAL P2P_LEAK_VBUS_delta->SetTestResult(SITE, 0, (MEAS_VBUS.P2P_leak_post[SITE] - MEAS_VBUS.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_DP_delta->SetTestResult(SITE, 0, (MEAS_DP.P2P_leak_post[SITE] - MEAS_DP.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_DM_delta->SetTestResult(SITE, 0, (MEAS_DM.P2P_leak_post[SITE] - MEAS_DM.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_PD_nINT_delta->SetTestResult(SITE, 0, (MEAS_PD_nINT.P2P_leak_post[SITE] - MEAS_PD_nINT.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_SCL_delta->SetTestResult(SITE, 0, (MEAS_SCL.P2P_leak_post[SITE] - MEAS_SCL.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_SDA_delta->SetTestResult(SITE, 0, (MEAS_SDA.P2P_leak_post[SITE] - MEAS_SDA.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_INT_delta->SetTestResult(SITE, 0, (MEAS_INT.P2P_leak_post[SITE] - MEAS_INT.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_CC1_delta->SetTestResult(SITE, 0, (MEAS_CC1.P2P_leak_post[SITE] - MEAS_CC1.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_CE_delta->SetTestResult(SITE, 0, (MEAS_CE.P2P_leak_post[SITE] - MEAS_CE.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_CC2_delta->SetTestResult(SITE, 0, (MEAS_CC2.P2P_leak_post[SITE] - MEAS_CC2.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_TSBAT_delta->SetTestResult(SITE, 0, (MEAS_TSBAT.P2P_leak_post[SITE] - MEAS_TSBAT.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_QON_delta->SetTestResult(SITE, 0, (MEAS_QON.P2P_leak_post[SITE] - MEAS_QON.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_VBAT_delta->SetTestResult(SITE, 0, (MEAS_VBAT.P2P_leak_post[SITE] - MEAS_VBAT.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_VSYS_delta->SetTestResult(SITE, 0, (MEAS_VSYS.P2P_leak_post[SITE] - MEAS_VSYS.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_SW_delta->SetTestResult(SITE, 0, (MEAS_SW.P2P_leak_post[SITE] - MEAS_SW.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_BTST_delta->SetTestResult(SITE, 0, (MEAS_BTST.P2P_leak_post[SITE] - MEAS_BTST.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_REGN_delta->SetTestResult(SITE, 0, (MEAS_REGN.P2P_leak_post[SITE] - MEAS_REGN.P2P_leak_pre[SITE]) * 1e9);
	SERIAL P2P_LEAK_PMID_delta->SetTestResult(SITE, 0, (MEAS_PMID.P2P_leak_post[SITE] - MEAS_PMID.P2P_leak_pre[SITE]) * 1e9);

    return 0;
}
 
DUT_API int T14_OS(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *REOS_VBUS = StsGetParam(funcindex, "REOS_VBUS");
    CParam *REOS_DP = StsGetParam(funcindex, "REOS_DP");
    CParam *REOS_DM = StsGetParam(funcindex, "REOS_DM");
    CParam *REOS_PD_nINT = StsGetParam(funcindex, "REOS_PD_nINT");
    CParam *REOS_SCL = StsGetParam(funcindex, "REOS_SCL");
    CParam *REOS_SDA = StsGetParam(funcindex, "REOS_SDA");
    CParam *REOS_INT = StsGetParam(funcindex, "REOS_INT");
    CParam *REOS_CC1 = StsGetParam(funcindex, "REOS_CC1");
    CParam *REOS_CE = StsGetParam(funcindex, "REOS_CE");
    CParam *REOS_CC2 = StsGetParam(funcindex, "REOS_CC2");
    CParam *REOS_TSBAT = StsGetParam(funcindex, "REOS_TSBAT");
    CParam *REOS_QON = StsGetParam(funcindex, "REOS_QON");
    CParam *REOS_VBAT = StsGetParam(funcindex, "REOS_VBAT");
    CParam *REOS_VSYS = StsGetParam(funcindex, "REOS_VSYS");
    CParam *REOS_SW = StsGetParam(funcindex, "REOS_SW");
    CParam *REOS_BTST = StsGetParam(funcindex, "REOS_BTST");
    CParam *REOS_REGN = StsGetParam(funcindex, "REOS_REGN");
    CParam *REOS_PMID = StsGetParam(funcindex, "REOS_PMID");
    //}}AFX_STS_PARAM_PROTOTYPES
    // TODO: Add your function code here
    return 0;
}
