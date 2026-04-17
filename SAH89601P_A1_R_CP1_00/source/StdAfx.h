// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
#define AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define DEBUG_MODE 0
#define SITE_NUM 16
#define BANK_NUM 16//What is bank number?--It seems used in trim function

extern int globalsite;
//#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
#define SITE globalsite
#define SERIAL \
        for(globalsite=0;globalsite<SITE_NUM;globalsite++) \

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#define DUT_API extern "C" __declspec(dllexport)
#include <string>
using namespace std;
#include "usertype.h"
#include "userres.h"

////add

#include "sub.h"
#include <vector>
#include "Test_Method.h"



extern DCM dcm;
extern CBITe cbit;
extern Test_Method test_method;



extern FOVIe VBUS;
extern ACM PMID;
extern ACM CC1;
extern ACM CC2;
extern ACM DP;
extern ACM DM;
extern ACM TSBAT;
extern FOVIe VBAT;
extern FOVIe VSYS;
extern FOVIe SW;
extern ACM REGN;
extern ACM BTST;
extern FOVIe FOVI_ALL;
extern ACM ACM_ALL;
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.






//PO
#define K20  				    	106,107
#define K23				    	    114,115,116,117
#define K25				    	    122,123,124,125

#define KPIN_USE					K20,K23,K25



/***********************************************************************
Register address											          //
***********************************************************************/
#define DEV_ADDR_BUCK	         0x6B<<1
#define DEV_ADDR_PD  	         0x62<<1
#define DEV_ADDR_INTERNAL        0x64<<1

#define Reg_TM_MODE			 0x5D
#define Reg_SCAN_Mode	     0xFE
#define Reg_EFUSE_BURN	     0xFF
#define Part_Number         89601

/***********************************************************************
Register address
***********************************************************************/
#define EE_BANK_ST			0xB0
#define Reg_EFUSE0			0xB0
#define Reg_EFUSE1			0xB1
#define Reg_EFUSE2			0xB2
#define Reg_EFUSE3			0xB3
#define Reg_EFUSE4			0xB4
#define Reg_EFUSE5			0xB5
#define Reg_EFUSE6			0xB6
#define Reg_EFUSE7			0xB7
#define Reg_EFUSE8			0xB8
#define Reg_EFUSE9			0xB9
#define Reg_EFUSE10			0xBA
#define Reg_EFUSE11			0xBB
#define Reg_EFUSE12			0xBC
#define Reg_EFUSE13			0xBD
#define Reg_EFUSE14			0xBE
#define Reg_EFUSE15			0xBF

#endif // !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
