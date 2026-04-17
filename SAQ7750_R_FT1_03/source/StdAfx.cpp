// stdafx.cpp : source file that includes just the standard includes
//	DUT.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

GLOBAL int g_x_coords[SITE_NUM] = { -99999 }, g_y_Coords[SITE_NUM] = { -99999 };

/////////////////FOVI resource///////////////////////
FOVI fovi0(0);
FOVI fovi1(1);
FOVI fovi2(2);
FOVI fovi3(3);
FOVI fovi4(4);
FOVI fovi5(5);
FOVI fovi6(6);
FOVI fovi7(7);


///////////////////FPVI resource//////////////////////
//site1
FPVI10 fpvi0(0);
FPVI10 fpvi1(1);
//site2
FPVI10 fpvi2(2);
FPVI10 fpvi3(3);
//site3
FPVI10 fpvi4(4);
FPVI10 fpvi5(5);
//site4
FPVI10 fpvi6(6);
FPVI10 fpvi7(7);


////////////////////DIO//////////////////////////
//DIO dio(0);


//////////////////////HVI1K//////////////////////////
//HVI1K hvi1k0(0);


///////////////////////QTMU///////////////////////////
QTMU_PLUS qtmu0(0);
QTMU_PLUS qtmu1(1);


///////////////////////QVM/////////////////////////////
QVM   qvm0(0);
QVM   qvm1(1);
QVM   qvm2(2);
QVM   qvm3(3);


////////////////////////// GPVI /////////////////////////
//CBIT128 cbit;







GLOBAL void cbitclose(BYTE k1, BYTE k2/*=-1*/, BYTE k3/*=-1*/, BYTE k4/*=-1*/, BYTE k5/*=-1*/, BYTE k6/*=-1*/, BYTE k7/*=-1*/, BYTE k8/*=-1*/, BYTE k9/*=-1*/, BYTE k10/*=-1*/,
	BYTE k11/*=-1*/, BYTE k12/*=-1*/, BYTE k13/*=-1*/, BYTE k14/*=-1*/, BYTE k15/*=-1*/, BYTE k16/*=-1*/, BYTE k17/*=-1*/, BYTE k18/*=-1*/, BYTE k19/*=-1*/, BYTE k20/*=-1*/,
	BYTE k21/*=-1*/, BYTE k22/*=-1*/, BYTE k23/*=-1*/, BYTE k24/*=-1*/, BYTE k25/*=-1*/, BYTE k26/*=-1*/, BYTE k27/*=-1*/, BYTE k28/*=-1*/, BYTE k29/*=-1*/, BYTE k30/*=-1*/,
	BYTE k31/*=-1*/, BYTE k32/*=-1*/, BYTE k33/*=-1*/, BYTE k34/*=-1*/, BYTE k35/*=-1*/, BYTE k36/*=-1*/, BYTE k37/*=-1*/, BYTE k38/*=-1*/, BYTE k39/*=-1*/, BYTE k40/*=-1*/,
	BYTE k41/*=-1*/, BYTE k42/*=-1*/, BYTE k43/*=-1*/, BYTE k44/*=-1*/, BYTE k45/*=-1*/, BYTE k46/*=-1*/, BYTE k47/*=-1*/, BYTE k48/*=-1*/, BYTE k49/*=-1*/, BYTE k50/*=-1*/,
	BYTE k51/*=-1*/, BYTE k52/*=-1*/, BYTE k53/*=-1*/, BYTE k54/*=-1*/, BYTE k55/*=-1*/, BYTE k56/*=-1*/, BYTE k57/*=-1*/, BYTE k58/*=-1*/, BYTE k59/*=-1*/, BYTE k60/*=-1*/,
	BYTE k61/*=-1*/, BYTE k62/*=-1*/, BYTE k63/*=-1*/, BYTE k64/*=-1*/, BYTE k65/*=-1*/, BYTE k66/*=-1*/, BYTE k67/*=-1*/, BYTE k68/*=-1*/, BYTE k69/*=-1*/, BYTE k70/*=-1*/,
	BYTE k71/*=-1*/, BYTE k72/*=-1*/, BYTE k73/*=-1*/, BYTE k74/*=-1*/, BYTE k75/*=-1*/, BYTE k76/*=-1*/, BYTE k77/*=-1*/, BYTE k78/*=-1*/, BYTE k79/*=-1*/, BYTE k80/*=-1*/,
	BYTE k81/*=-1*/, BYTE k82/*=-1*/, BYTE k83/*=-1*/, BYTE k84/*=-1*/, BYTE k85/*=-1*/, BYTE k86/*=-1*/, BYTE k87/*=-1*/, BYTE k88/*=-1*/, BYTE k89/*=-1*/, BYTE k90/*=-1*/,
	BYTE k91/*=-1*/, BYTE k92/*=-1*/, BYTE k93/*=-1*/, BYTE k94/*=-1*/, BYTE k95/*=-1*/, BYTE k96/*=-1*/, BYTE k97/*=-1*/, BYTE k98/*=-1*/, BYTE k99/*=-1*/, BYTE k100/*=-1*/,
	BYTE k101/*=-1*/, BYTE k102/*=-1*/, BYTE k103/*=-1*/, BYTE k104/*=-1*/, BYTE k105/*=-1*/, BYTE k106/*=-1*/, BYTE k107/*=-1*/, BYTE k108/*=-1*/, BYTE k109/*=-1*/, BYTE k110/*=-1*/,
	BYTE k111/*=-1*/, BYTE k112/*=-1*/, BYTE k113/*=-1*/, BYTE k114/*=-1*/, BYTE k115/*=-1*/, BYTE k116/*=-1*/, BYTE k117/*=-1*/, BYTE k118/*=-1*/, BYTE k119/*=-1*/, BYTE k120/*=-1*/,
	BYTE k121/*=-1*/, BYTE k122/*=-1*/, BYTE k123/*=-1*/, BYTE k124/*=-1*/, BYTE k125/*=-1*/, BYTE k126/*=-1*/, BYTE k127/*=-1*/, BYTE k128/*=-1*/)
{
	BYTE parameter = 0;
	int parameterCount = 0;
	BYTE k[128] = { -1 };	// -1 is end flag, init k[0]=-1

	BYTE sitestatus[SITE_NUM] = { 0 };
	StsGetSiteStatus(sitestatus, SITE_NUM);

	va_list arg_ptr;
	va_start(arg_ptr, k1);
	parameter = k1;

	while (parameter != 255 && parameterCount < 128)	// 255 is '-1': end flag
	{
		k[parameterCount++] = parameter;
		parameter = va_arg(arg_ptr, BYTE);
	}
	va_end(arg_ptr);

	for (int index = 0; index < parameterCount; index++)
	{
		for (int site = SITE_1; site < SITE_NUM; site++)
			if ((site == index % SITE_NUM) && sitestatus[site])
			{
				if (k[index] != 255)	// 255 is '-1': end flag
				{
					cbit.SetCBITOn(k[index]);
					delay_us(100);
				}
			}

	}


}

//GLOBAL void cbitopen(BYTE k1, ...)
GLOBAL void cbitopen(BYTE k1, BYTE k2/*=-1*/, BYTE k3/*=-1*/, BYTE k4/*=-1*/, BYTE k5/*=-1*/, BYTE k6/*=-1*/, BYTE k7/*=-1*/, BYTE k8/*=-1*/, BYTE k9/*=-1*/, BYTE k10/*=-1*/,
	BYTE k11/*=-1*/, BYTE k12/*=-1*/, BYTE k13/*=-1*/, BYTE k14/*=-1*/, BYTE k15/*=-1*/, BYTE k16/*=-1*/, BYTE k17/*=-1*/, BYTE k18/*=-1*/, BYTE k19/*=-1*/, BYTE k20/*=-1*/,
	BYTE k21/*=-1*/, BYTE k22/*=-1*/, BYTE k23/*=-1*/, BYTE k24/*=-1*/, BYTE k25/*=-1*/, BYTE k26/*=-1*/, BYTE k27/*=-1*/, BYTE k28/*=-1*/, BYTE k29/*=-1*/, BYTE k30/*=-1*/,
	BYTE k31/*=-1*/, BYTE k32/*=-1*/, BYTE k33/*=-1*/, BYTE k34/*=-1*/, BYTE k35/*=-1*/, BYTE k36/*=-1*/, BYTE k37/*=-1*/, BYTE k38/*=-1*/, BYTE k39/*=-1*/, BYTE k40/*=-1*/,
	BYTE k41/*=-1*/, BYTE k42/*=-1*/, BYTE k43/*=-1*/, BYTE k44/*=-1*/, BYTE k45/*=-1*/, BYTE k46/*=-1*/, BYTE k47/*=-1*/, BYTE k48/*=-1*/, BYTE k49/*=-1*/, BYTE k50/*=-1*/,
	BYTE k51/*=-1*/, BYTE k52/*=-1*/, BYTE k53/*=-1*/, BYTE k54/*=-1*/, BYTE k55/*=-1*/, BYTE k56/*=-1*/, BYTE k57/*=-1*/, BYTE k58/*=-1*/, BYTE k59/*=-1*/, BYTE k60/*=-1*/,
	BYTE k61/*=-1*/, BYTE k62/*=-1*/, BYTE k63/*=-1*/, BYTE k64/*=-1*/, BYTE k65/*=-1*/, BYTE k66/*=-1*/, BYTE k67/*=-1*/, BYTE k68/*=-1*/, BYTE k69/*=-1*/, BYTE k70/*=-1*/,
	BYTE k71/*=-1*/, BYTE k72/*=-1*/, BYTE k73/*=-1*/, BYTE k74/*=-1*/, BYTE k75/*=-1*/, BYTE k76/*=-1*/, BYTE k77/*=-1*/, BYTE k78/*=-1*/, BYTE k79/*=-1*/, BYTE k80/*=-1*/,
	BYTE k81/*=-1*/, BYTE k82/*=-1*/, BYTE k83/*=-1*/, BYTE k84/*=-1*/, BYTE k85/*=-1*/, BYTE k86/*=-1*/, BYTE k87/*=-1*/, BYTE k88/*=-1*/, BYTE k89/*=-1*/, BYTE k90/*=-1*/,
	BYTE k91/*=-1*/, BYTE k92/*=-1*/, BYTE k93/*=-1*/, BYTE k94/*=-1*/, BYTE k95/*=-1*/, BYTE k96/*=-1*/, BYTE k97/*=-1*/, BYTE k98/*=-1*/, BYTE k99/*=-1*/, BYTE k100/*=-1*/,
	BYTE k101/*=-1*/, BYTE k102/*=-1*/, BYTE k103/*=-1*/, BYTE k104/*=-1*/, BYTE k105/*=-1*/, BYTE k106/*=-1*/, BYTE k107/*=-1*/, BYTE k108/*=-1*/, BYTE k109/*=-1*/, BYTE k110/*=-1*/,
	BYTE k111/*=-1*/, BYTE k112/*=-1*/, BYTE k113/*=-1*/, BYTE k114/*=-1*/, BYTE k115/*=-1*/, BYTE k116/*=-1*/, BYTE k117/*=-1*/, BYTE k118/*=-1*/, BYTE k119/*=-1*/, BYTE k120/*=-1*/,
	BYTE k121/*=-1*/, BYTE k122/*=-1*/, BYTE k123/*=-1*/, BYTE k124/*=-1*/, BYTE k125/*=-1*/, BYTE k126/*=-1*/, BYTE k127/*=-1*/, BYTE k128/*=-1*/)

{
	BYTE parameter = 0;
	int parameterCount = 0;
	BYTE k[128] = { -1 };	// -1 is end flag, init k[0]=-1

	BYTE sitestatus[SITE_NUM] = { 0 };
	StsGetSiteStatus(sitestatus, SITE_NUM);

	va_list arg_ptr;
	va_start(arg_ptr, k1);
	parameter = k1;

	while (parameter != 255 && parameterCount < 128)	// 255 is '-1': end flag
	{
		k[parameterCount++] = parameter;
		parameter = va_arg(arg_ptr, BYTE);
	}
	va_end(arg_ptr);

	for (int index = 0; index < parameterCount; index++)
	{
		for (int site = SITE_1; site < SITE_NUM; site++)
			if ((site == index % SITE_NUM) && sitestatus[site])
			{
				if (k[index] != 255)	// 255 is '-1': end flag
					cbit.SetCBITOff(k[index]);
				delay_us(100);
			}

	}

}

GLOBAL BYTE G_GetSingleSiteStatus(short SiteID)
{
	BYTE sitesta[SITE_NUM] = { 0 };
	StsGetSiteStatus(sitesta, SITE_NUM);	// Get site status
	return sitesta[SiteID];
}


GLOBAL void msScaleDataAll(double scaleval, double* results, int resultsSize/*=NUM_SITES*/)
{
	for (int site = SITE_1; site < SITE_NUM; site++)
	{
		*(results + site) = *(results + site) * scaleval;
	}
}


GLOBAL void msLogResult(CParam* Fdlg, short siteID, short subUnitID, double result, char* notes/*=""*/, ...)
{
	char buffer[256];
	va_list argptr;
	va_start(argptr, notes);
	vsprintf(buffer, notes, argptr);
	va_end(argptr);
	notes = buffer;

	if (G_GetSingleSiteStatus(siteID))
	{
		Fdlg->SetTestResult(siteID, subUnitID, result);
		Fdlg->SetResultRemark(siteID, subUnitID, notes);
	}
}

GLOBAL void msLogResultAll(CParam* Fdlg, short subUnitID, double* result/*=NULL*/, int resultsSize/*=NUM_SITES*/, char* notes/*=""*/, ...)
{
	char buffer[256];
	va_list argptr;
	va_start(argptr, notes);
	vsprintf(buffer, notes, argptr);
	va_end(argptr);
	notes = buffer;

	for (int site = SITE_1; site < resultsSize; site++)
	{
		msLogResult(Fdlg, site, subUnitID, *(result + site), notes);
	}
}


GLOBAL void groupgetresults(FPVI10 pin, MeasRet retTyp, double* result, int resultsSize/*=NUM_SITES*/)
{
	for (int site = SITE_1; site < resultsSize; site++)
	{
		*(result + site) = pin.GetMeasResult(site, retTyp);
	}
}

GLOBAL void groupgetresults(FOVI pin, MeasRet retTyp, double* result, int resultsSize/*=NUM_SITES*/)
{
	for (int site = SITE_1; site < resultsSize; site++)
	{
		*(result + site) = pin.GetMeasResult(site, retTyp);
	}
}