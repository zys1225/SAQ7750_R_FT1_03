// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
#define AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_

//#if _MSC_VER > 1000
#define _CRT_SECURE_NO_WARNINGS 
#pragma warning( disable: 4996)
//#endif // _MSC_VER > 1000


#define DEBUG_MODE 0
#define SITE_NUM 4
#define BANK_NUM 8

extern int globalsite;
#define SITE globalsite
#define SERIAL for(globalsite=0;globalsite<SITE_NUM;globalsite++)
// Insert your headers here
#define SERIAL_1 for(globalsite=0;globalsite<SITE_NUM;globalsite++)  if ( StsGetsSiteStatus() & (1 << globalsite))


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#define DUT_API extern "C" __declspec(dllexport)
#include <string>
using namespace std;
#include "usertype.h"
#include "userres.h"

//#include "dio_plus.h" 

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include <map>
#include <string>
#include <iostream>
#include<cstdlib>



//////////////FOVI resource//////////////////////
extern FOVI KELVIN_VS; // Logic channel 0
extern FOVI IN1_VI;
extern FOVI DEN_VI;
extern FOVI NC_VI;
extern FOVI IN0_VI;
extern FOVI IS_DESL_VI;
extern FOVI KELVIN_GND;
extern FOVI PullUp_VI;

//////////////FPVI resource//////////////////////
extern FPVI10 KELVIN_OUT1;
extern FPVI10 KELVIN_OUT0;

//////////////QTMU//////////////////////////////
extern QTMU_PLUS QTMU_MEAS;

//////////////DIO///////////////////////////////
extern DIO dio;
extern DIO_PLUS dio_plus;

//////////////CBIT///////////////////////////////
extern CBIT128 cbit;

////////////CBIT Define//////////////////////////
#define K11_OutPullDown					1, 1 + 32, 1 + 32 + 32, 1 + 32 + 32 + 32
#define K8_VS_FPVI0						2, 2 + 32, 2 + 32 + 32, 2 + 32 + 32 + 32
#define K6_QTMU							3, 3 + 32, 3 + 32 + 32, 3 + 32 + 32 + 32
#define K5_QTMU_SWITCH					4, 4 + 32, 4 + 32 + 32, 4 + 32 + 32 + 32
#define K9_VS_FPVI1						5, 5 + 32, 5 + 32 + 32, 5 + 32 + 32 + 32
#define K7_DenPullUp					6, 6 + 32, 6 + 32 + 32, 6 + 32 + 32 + 32
#define K4_NC_IS_DESL_VI				7, 7 + 32, 7 + 32 + 32, 7 + 32 + 32 + 32
#define K1_K2_VS_OUT1_KelvinCheck		8, 8 + 32, 8 + 32 + 32, 8 + 32 + 32 + 32
#define K3_K10_OUT0_GND_KelvinCheck     9, 9 + 32, 9 + 32 + 32, 9 + 32 + 32 + 32
#define K12_NC_IN1						10, 10 + 32, 10 + 64, 10 + 96
#define K30_QTMU_AB                     30, 30 + 32, 30 + 32 + 32, 30 + 32 + 32 + 32


#define GLOBAL

GLOBAL void cbitclose(BYTE k1, BYTE k2 = -1, BYTE k3 = -1, BYTE k4 = -1, BYTE k5 = -1, BYTE k6 = -1, BYTE k7 = -1, BYTE k8 = -1, BYTE k9 = -1, BYTE k10 = -1,
	BYTE k11 = -1, BYTE k12 = -1, BYTE k13 = -1, BYTE k14 = -1, BYTE k15 = -1, BYTE k16 = -1, BYTE k17 = -1, BYTE k18 = -1, BYTE k19 = -1, BYTE k20 = -1,
	BYTE k21 = -1, BYTE k22 = -1, BYTE k23 = -1, BYTE k24 = -1, BYTE k25 = -1, BYTE k26 = -1, BYTE k27 = -1, BYTE k28 = -1, BYTE k29 = -1, BYTE k30 = -1,
	BYTE k31 = -1, BYTE k32 = -1, BYTE k33 = -1, BYTE k34 = -1, BYTE k35 = -1, BYTE k36 = -1, BYTE k37 = -1, BYTE k38 = -1, BYTE k39 = -1, BYTE k40 = -1,
	BYTE k41 = -1, BYTE k42 = -1, BYTE k43 = -1, BYTE k44 = -1, BYTE k45 = -1, BYTE k46 = -1, BYTE k47 = -1, BYTE k48 = -1, BYTE k49 = -1, BYTE k50 = -1,
	BYTE k51 = -1, BYTE k52 = -1, BYTE k53 = -1, BYTE k54 = -1, BYTE k55 = -1, BYTE k56 = -1, BYTE k57 = -1, BYTE k58 = -1, BYTE k59 = -1, BYTE k60 = -1,
	BYTE k61 = -1, BYTE k62 = -1, BYTE k63 = -1, BYTE k64 = -1, BYTE k65 = -1, BYTE k66 = -1, BYTE k67 = -1, BYTE k68 = -1, BYTE k69 = -1, BYTE k70 = -1,
	BYTE k71 = -1, BYTE k72 = -1, BYTE k73 = -1, BYTE k74 = -1, BYTE k75 = -1, BYTE k76 = -1, BYTE k77 = -1, BYTE k78 = -1, BYTE k79 = -1, BYTE k80 = -1,
	BYTE k81 = -1, BYTE k82 = -1, BYTE k83 = -1, BYTE k84 = -1, BYTE k85 = -1, BYTE k86 = -1, BYTE k87 = -1, BYTE k88 = -1, BYTE k89 = -1, BYTE k90 = -1,
	BYTE k91 = -1, BYTE k92 = -1, BYTE k93 = -1, BYTE k94 = -1, BYTE k95 = -1, BYTE k96 = -1, BYTE k97 = -1, BYTE k98 = -1, BYTE k99 = -1, BYTE k100 = -1,
	BYTE k101 = -1, BYTE k102 = -1, BYTE k103 = -1, BYTE k104 = -1, BYTE k105 = -1, BYTE k106 = -1, BYTE k107 = -1, BYTE k108 = -1, BYTE k109 = -1, BYTE k110 = -1,
	BYTE k111 = -1, BYTE k112 = -1, BYTE k113 = -1, BYTE k114 = -1, BYTE k115 = -1, BYTE k116 = -1, BYTE k117 = -1, BYTE k118 = -1, BYTE k119 = -1, BYTE k120 = -1,
	BYTE k121 = -1, BYTE k122 = -1, BYTE k123 = -1, BYTE k124 = -1, BYTE k125 = -1, BYTE k126 = -1, BYTE k127 = -1, BYTE k128 = -1);	// close relay.

GLOBAL void cbitopen(BYTE k1, BYTE k2 = -1, BYTE k3 = -1, BYTE k4 = -1, BYTE k5 = -1, BYTE k6 = -1, BYTE k7 = -1, BYTE k8 = -1, BYTE k9 = -1, BYTE k10 = -1,
	BYTE k11 = -1, BYTE k12 = -1, BYTE k13 = -1, BYTE k14 = -1, BYTE k15 = -1, BYTE k16 = -1, BYTE k17 = -1, BYTE k18 = -1, BYTE k19 = -1, BYTE k20 = -1,
	BYTE k21 = -1, BYTE k22 = -1, BYTE k23 = -1, BYTE k24 = -1, BYTE k25 = -1, BYTE k26 = -1, BYTE k27 = -1, BYTE k28 = -1, BYTE k29 = -1, BYTE k30 = -1,
	BYTE k31 = -1, BYTE k32 = -1, BYTE k33 = -1, BYTE k34 = -1, BYTE k35 = -1, BYTE k36 = -1, BYTE k37 = -1, BYTE k38 = -1, BYTE k39 = -1, BYTE k40 = -1,
	BYTE k41 = -1, BYTE k42 = -1, BYTE k43 = -1, BYTE k44 = -1, BYTE k45 = -1, BYTE k46 = -1, BYTE k47 = -1, BYTE k48 = -1, BYTE k49 = -1, BYTE k50 = -1,
	BYTE k51 = -1, BYTE k52 = -1, BYTE k53 = -1, BYTE k54 = -1, BYTE k55 = -1, BYTE k56 = -1, BYTE k57 = -1, BYTE k58 = -1, BYTE k59 = -1, BYTE k60 = -1,
	BYTE k61 = -1, BYTE k62 = -1, BYTE k63 = -1, BYTE k64 = -1, BYTE k65 = -1, BYTE k66 = -1, BYTE k67 = -1, BYTE k68 = -1, BYTE k69 = -1, BYTE k70 = -1,
	BYTE k71 = -1, BYTE k72 = -1, BYTE k73 = -1, BYTE k74 = -1, BYTE k75 = -1, BYTE k76 = -1, BYTE k77 = -1, BYTE k78 = -1, BYTE k79 = -1, BYTE k80 = -1,
	BYTE k81 = -1, BYTE k82 = -1, BYTE k83 = -1, BYTE k84 = -1, BYTE k85 = -1, BYTE k86 = -1, BYTE k87 = -1, BYTE k88 = -1, BYTE k89 = -1, BYTE k90 = -1,
	BYTE k91 = -1, BYTE k92 = -1, BYTE k93 = -1, BYTE k94 = -1, BYTE k95 = -1, BYTE k96 = -1, BYTE k97 = -1, BYTE k98 = -1, BYTE k99 = -1, BYTE k100 = -1,
	BYTE k101 = -1, BYTE k102 = -1, BYTE k103 = -1, BYTE k104 = -1, BYTE k105 = -1, BYTE k106 = -1, BYTE k107 = -1, BYTE k108 = -1, BYTE k109 = -1, BYTE k110 = -1,
	BYTE k111 = -1, BYTE k112 = -1, BYTE k113 = -1, BYTE k114 = -1, BYTE k115 = -1, BYTE k116 = -1, BYTE k117 = -1, BYTE k118 = -1, BYTE k119 = -1, BYTE k120 = -1,
	BYTE k121 = -1, BYTE k122 = -1, BYTE k123 = -1, BYTE k124 = -1, BYTE k125 = -1, BYTE k126 = -1, BYTE k127 = -1, BYTE k128 = -1);	// open relay. 


GLOBAL BYTE G_GetSingleSiteStatus(short SiteID);	// return site status. 1: valid; 0: invalid
GLOBAL void msLogResult(CParam* Fdlg, short siteID, short subUnitID, double result, char* notes = "", ...); // data log results, single site function
GLOBAL void msLogResultAll(CParam* Fdlg, short subUnitID, double* result, int resultsSize = SITE_NUM, char* notes = "", ...);	// data log results, multi-site function
GLOBAL void groupgetresults(FPVI10 pin, MeasRet retTyp, double* result, int resultsSize = SITE_NUM);
GLOBAL void groupgetresults(FOVI pin, MeasRet retTyp, double* result, int resultsSize = SITE_NUM);

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
