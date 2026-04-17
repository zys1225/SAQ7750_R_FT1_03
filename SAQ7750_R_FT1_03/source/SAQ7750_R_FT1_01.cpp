// SAQ7750_R_FT1_01.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
//if needed, you can move the following functions to the file of test.cpp.
//Do not change the definition of these functions, or the pgs can not be loaded.
//DUT_API void UserLoad()
//{		
//}
///************************************************************************/
///*                                                                      */
///************************************************************************/
//DUT_API void UserInitAfterLoad()
//{
//}
///************************************************************************/
///*                                                                      */
///************************************************************************/
DUT_API void UserExit()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void OnSot()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

DUT_API void BinOutDut()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void OnNewLot(const char* Lotid)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void OnWaferEnd(const char* Lotid)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
