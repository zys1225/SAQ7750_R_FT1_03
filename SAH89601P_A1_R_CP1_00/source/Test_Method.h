/*****************************************************************************
*                                                                            *
*       Source title:   Test_Method.h                                        *
*                       (Universal functions for all SC test projects)       *
*         Written by:   SC TE                                                *
*        Description:			                                             *
*                                                                            *
*   Revision History:                                                        *
*                                                                            *
*     mm/dd/yy  r.rr  - Original coding.                                     *
*                                                                            *
*****************************************************************************/

/*
 REVISION BLOCK:
 -Rev. -- Date --------------------------------------------------
   00   08/24/18  Initial.						Alan Luo
   01   09/04/18  Reset ERROR_RES to 9999		Alan Luo
				  Remove fpvi current ramp 1ms capload at start for EOS damage risk 
   02   10/31/18  Fix "Enable log" alarm issue  Alan
				  Change "fovi_cap.Set" from RELAY_SENSE_ON to RELAY_ON
   03   3/27/19   Add ramp with I2C flag trig, ramp with QVM measure
				  Add ADC_Statistic Parameters Calculation function (Includes FSR/LSB/INL/DNL...)
   04   07/23/19  Return measure current not calculated result, when use fpvi ramp current
----------------------------------------------------------------
*/

#pragma once
#include "stdafx.h"

#define MAX_SAMPLES 4096
#define ERROR_RES	9999
#define START_DELAY	0
#define TRIG_DELAY	250

class Test_Method {
public:
	Test_Method() {}
	~Test_Method() {}

	BOOL OS_Classify(short funcindex, string pin_str_array[], unsigned int OS_NUM, string leak_str_array[], unsigned int LEAK_NUM, double os_result[][SITE_NUM], double leak_result[][SITE_NUM], vector<string>& vec_exclude);
	FOVIe_VRNG get_fovi_v_range(double v);

	BOOL ramp(ACM acm_ramp, ACM_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result);
	BOOL ramp_v(ACM acm_ramp, ACM_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result);
	BOOL ramp(FOVIe fovi_ramp, FOVIe_VRNG v_range, FOVIe_IRNG i_range, ACM acm_cap, ACM_IRNG acm_i_range, double loading, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result);
	
	BOOL ramp(FOVIe fovi_ramp, FOVIe fovi_ramp1, FOVIe_VRNG v_range, FOVIe_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result);
	BOOL ramp(FOVIe fovi_ramp, FOVIe_VRNG v_range, FOVIe_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result);

};



