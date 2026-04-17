#include "stdafx.h"
#include "Test_Method.h"



// use for open short test classify to different bin
// pgs must include "OS_SHORT" "OS_SOME_OPEN" "OS_ALL_OPEN" "OS_LEAK"
BOOL Test_Method::OS_Classify(short funcindex, string pin_str_array[], unsigned int OS_NUM, string leak_str_array[], unsigned int LEAK_NUM, double os_result[][SITE_NUM], double leak_result[][SITE_NUM], vector<string>& vec_exclude)
{
	if (OS_NUM == 0) return FALSE;
	int open_count[SITE_NUM] = { 0 };
	int SHORT_FLAG[SITE_NUM] = { 0 };
	int ALAM_FLAG[SITE_NUM] = { 0 };//Bill
	int SOME_OPEN_FLAG[SITE_NUM] = { 0 };
	int ALL_OPEN_FLAG[SITE_NUM] = { 0 };
	int LEAK_FLAG[SITE_NUM] = { 0 };
	CParam *OS_Param;

	for (unsigned int os_count = 0; os_count < OS_NUM; ++os_count)
	{
		if (find(vec_exclude.begin(), vec_exclude.end(), pin_str_array[os_count]) != vec_exclude.end()) continue;
		OS_Param = StsGetParam(funcindex, pin_str_array[os_count].c_str());
		double spec_l = OS_Param->GetMinLimit();
		double spec_h = OS_Param->GetMaxLimit();
		SERIAL{
			if (spec_l > 0)
			{
				if (os_result[os_count][SITE] < spec_l)
					SHORT_FLAG[SITE] = 1;
				if (SHORT_FLAG[SITE] != 1 && (os_result[os_count][SITE] > spec_h))
					open_count[SITE]++;
			}
			if (spec_h < 0)
			{
				if (os_result[os_count][SITE] >spec_h)
					SHORT_FLAG[SITE] = 1;
				if (os_result[os_count][SITE] >0.2)//Bill
					ALAM_FLAG[SITE] = 1;
				if (SHORT_FLAG[SITE] != 1 && (os_result[os_count][SITE] < spec_l))
					open_count[SITE]++;
			}
		}
	}

	for (unsigned int leak_count = 0; leak_count < LEAK_NUM; ++leak_count)
	{
		OS_Param = StsGetParam(funcindex, leak_str_array[leak_count].c_str());
		double spec_l = OS_Param->GetMinLimit();
		double spec_h = OS_Param->GetMaxLimit();
		SERIAL{
			if (leak_result[leak_count][SITE] < spec_l || leak_result[leak_count][SITE] > spec_h)
			LEAK_FLAG[SITE] = 1;
		}
	}

	SERIAL    //judge some open or all open 
	{
		if (SHORT_FLAG[SITE] != 1 && open_count[SITE] == (OS_NUM - vec_exclude.size()))
		ALL_OPEN_FLAG[SITE] = 1;
		else if (SHORT_FLAG[SITE] != 1 && open_count[SITE] < (int)(OS_NUM - vec_exclude.size()) && open_count[SITE] > 0)
			SOME_OPEN_FLAG[SITE] = 1;
		if (ALAM_FLAG[SITE] == 1)
			SHORT_FLAG[SITE] = 0;
	}

	SERIAL	StsGetParam(funcindex, "OS_SHORT")->SetTestResult(SITE, 0, SHORT_FLAG[SITE]);
	SERIAL	StsGetParam(funcindex, "OS_SOME_OPEN")->SetTestResult(SITE, 0, SOME_OPEN_FLAG[SITE]);
	SERIAL	StsGetParam(funcindex, "OS_ALL_OPEN")->SetTestResult(SITE, 0, ALL_OPEN_FLAG[SITE]);
	SERIAL	StsGetParam(funcindex, "OS_ALAM")->SetTestResult(SITE, 0, ALAM_FLAG[SITE]);
	if (LEAK_NUM != 0)
	{
		SERIAL	StsGetParam(funcindex, "OS_LEAK")->SetTestResult(SITE, 0, LEAK_FLAG[SITE]);
	}

	return TRUE;
}


FOVIe_VRNG Test_Method::get_fovi_v_range(double v)
{
	if (v < 0)
		v = -v;
	if (v <= 1)
		return FOVIe_1V;
	else if (v <= 5)
		return FOVIe_5V;
	else if (v <= 10)
		return FOVIe_10V;
	else if (v <= 20)
		return FOVIe_20V;
	else if (v <= 40)
		return FOVIe_40V;
	else
		return FOVIe_5V;
}


// Ramp acm current, capture acm voltage, trig stop
// Can set acm_ramp i_range 
// Good for IBAT_SNS
// fovi_cap no current loading
BOOL Test_Method::ramp(ACM acm_ramp, ACM_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result)
{
	if (interval < 10)	return FALSE;
	if (step == 0)	return FALSE;

	int samples;
	double pat[MAX_SAMPLES];
	samples = int(fabs((stop_point - start_point) / step));
	if ((samples <= 1) || (samples > MAX_SAMPLES))
	{
		samples = MAX_SAMPLES;
	}

	acm_ramp.Set(FI, start_point, ACM_N2P18V, i_range, ACM_RELAY_ON, 1);
	acm_cap.Set(FI, 0, ACM_N2P18V, ACM_5UA, ACM_RELAY_ON);

	STSAWGCreateRampData(&pat[0], samples, 1, start_point, stop_point);
	acm_ramp.AwgClear();
	acm_ramp.AwgLoader("awg", FI, ACM_N2P18V, i_range, pat, samples);
	acm_ramp.AwgSelect("awg", 0, samples - 1, samples - 1, interval);
	acm_cap.SetMeasVTrig(trig_level, trig_mode);

	if (!START_DELAY)
		delay_ms(START_DELAY);

	acm_ramp.MeasureVI(ACM_MI, samples, interval, MEAS_AWG);
	acm_cap.MeasureVI(ACM_MV, samples, interval, MEAS_AWG);

	STSEnableAWG(&acm_ramp);
	STSEnableMeas(&acm_ramp, &acm_cap);
	STSAWGRunTriggerStop(&acm_cap, &acm_ramp, &acm_cap);
	delay_us(TRIG_DELAY);

	int Trig_Point[SITE_NUM];
	SERIAL{
		Trig_Point[SITE] = (int)acm_cap.GetMeasResult(SITE, TRIG_RESULT);
		if (((Trig_Point[SITE]) > 2) && ((Trig_Point[SITE]) < samples - 2))
		{
			//result[SITE] = start_point + (Trig_Point[SITE] - 1) * step;
			result[SITE] = acm_ramp.GetMeasResult(SITE, Trig_Point[SITE] - 1);
		}
		else
		{
			result[SITE] = ERROR_RES;
		}
	}
	acm_cap.Set(FI, 0, ACM_N2P18V, ACM_5UA, ACM_RELAY_OFF);
	return TRUE;
}

// Ramp acm voltage, capture acm voltage, trig stop
// Can set acm_ramp i_range 
// Good for DPDM VTH
BOOL Test_Method::ramp_v(ACM acm_ramp, ACM_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result)
{
	if (interval < 10)	return FALSE;
	if (step == 0)	return FALSE;

	int samples;
	double pat[MAX_SAMPLES];
	samples = int(fabs((stop_point - start_point) / step));
	if ((samples <= 1) || (samples > MAX_SAMPLES))
	{
		samples = MAX_SAMPLES;
	}

	acm_ramp.Set(FV, start_point, ACM_N2P18V, i_range, ACM_RELAY_ON);
	acm_cap.Set(FI, 0, ACM_N2P18V, ACM_5UA, ACM_RELAY_ON);

	STSAWGCreateRampData(&pat[0], samples, 1, start_point, stop_point);
	acm_ramp.AwgClear();
	acm_ramp.AwgLoader("awg", FV, ACM_N2P18V, i_range, pat, samples);
	acm_ramp.AwgSelect("awg", 0, samples - 1, samples - 1, interval);
	acm_cap.SetMeasVTrig(trig_level, trig_mode);

	if (!START_DELAY)
		delay_ms(START_DELAY);

	acm_ramp.MeasureVI(ACM_MV, samples, interval, MEAS_AWG);
	acm_cap.MeasureVI(ACM_MV, samples, interval, MEAS_AWG);

	STSEnableAWG(&acm_ramp);
	STSEnableMeas(&acm_ramp, &acm_cap);
	STSAWGRunTriggerStop(&acm_cap, &acm_ramp, &acm_cap);
	delay_us(TRIG_DELAY);

	int Trig_Point[SITE_NUM];
	SERIAL{
		Trig_Point[SITE] = (int)acm_cap.GetMeasResult(SITE, TRIG_RESULT);
		if (((Trig_Point[SITE]) > 2) && ((Trig_Point[SITE]) < samples - 2))
		{
			//result[SITE] = start_point + (Trig_Point[SITE] - 1) * step;
			result[SITE] = acm_ramp.GetMeasResult(SITE, Trig_Point[SITE] - 1);
		}
		else
		{
			result[SITE] = ERROR_RES;
		}
	}

	return TRUE;
}

// Ramp fovie voltage, capture acm voltage, trig stop
// Can set fovi_ramp v_range and i_range 
// Good for UVLO, threshold, power good measurement
// fovi_cap no current loading
BOOL Test_Method::ramp(FOVIe fovi_ramp, FOVIe_VRNG v_range, FOVIe_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result)
{
	if (interval < 10)	return FALSE;
	if (step == 0)	return FALSE;

	int samples;
	double pat[MAX_SAMPLES];
	samples = int(fabs((stop_point - start_point) / step));
	if ((samples <= 1) || (samples > MAX_SAMPLES))
	{
		samples = MAX_SAMPLES;
	}

	fovi_ramp.Set(FV, start_point, v_range, i_range, FOVIe_RELAY_ON, 1);
	acm_cap.Set(FI, 0, ACM_N2P18V, ACM_5UA, ACM_RELAY_ON);

	STSAWGCreateRampData(&pat[0], samples, 1, start_point, stop_point);
	fovi_ramp.AwgClear();
	fovi_ramp.AwgLoader("awg", FV, v_range, i_range, pat, samples);
	fovi_ramp.AwgSelect("awg", 0, samples - 1, samples - 1, interval);
	acm_cap.SetMeasVTrig(trig_level, trig_mode);

	if (!START_DELAY)
		delay_ms(START_DELAY);

	fovi_ramp.MeasureVI(samples, interval, FOVIe_MI_X1, MEAS_AWG);
	acm_cap.MeasureVI(ACM_MV, samples, interval, MEAS_AWG);

	STSEnableAWG(&fovi_ramp);
	STSEnableMeas(&fovi_ramp, &acm_cap);
	STSAWGRunTriggerStop(&acm_cap, &acm_cap, &fovi_ramp);
	delay_us(TRIG_DELAY);

	int Trig_Point[SITE_NUM];
	SERIAL{
		Trig_Point[SITE] = (int)acm_cap.GetMeasResult(SITE, TRIG_RESULT);
		if (((Trig_Point[SITE]) > 2) && ((Trig_Point[SITE]) < samples - 2))
		{
			//result[SITE] = start_point + (Trig_Point[SITE] - 1) * step;
			result[SITE] = fovi_ramp.GetMeasResult(SITE, MVRET, Trig_Point[SITE] - 1);
		}
		else
		{
			result[SITE] = ERROR_RES;
		}
	}
	acm_cap.Set(FI, 0, ACM_N2P18V, ACM_5UA, ACM_RELAY_OFF);
	return TRUE;
}

// Ramp 2 fovie voltage, capture acm voltage, trig stop
// Can set fovi_ramp v_range and i_range 
// Good for UVLO, threshold, power good measurement
// fovi_cap no current loading
BOOL Test_Method::ramp(FOVIe fovi_ramp, FOVIe fovi_ramp1, FOVIe_VRNG v_range, FOVIe_IRNG i_range, ACM acm_cap, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result)
{
	if (interval < 10)	return FALSE;
	if (step == 0)	return FALSE;

	int samples;
	double pat[MAX_SAMPLES];
	samples = int(fabs((stop_point - start_point) / step));
	if ((samples <= 1) || (samples > MAX_SAMPLES))
	{
		samples = MAX_SAMPLES;
	}

	fovi_ramp.Set(FV, start_point, v_range, i_range, FOVIe_RELAY_ON, 1);
	fovi_ramp1.Set(FV, start_point, v_range, i_range, FOVIe_RELAY_ON, 1);
	acm_cap.Set(FI, 0, ACM_N2P18V, ACM_5UA, ACM_RELAY_ON);

	STSAWGCreateRampData(&pat[0], samples, 1, start_point, stop_point);
	fovi_ramp.AwgClear();
	fovi_ramp1.AwgClear();
	fovi_ramp.AwgLoader("awg", FV, v_range, i_range, pat, samples);
	fovi_ramp1.AwgLoader("awg", FV, v_range, i_range, pat, samples);
	fovi_ramp.AwgSelect("awg", 0, samples - 1, samples - 1, interval);
	fovi_ramp1.AwgSelect("awg", 0, samples - 1, samples - 1, interval);
	acm_cap.SetMeasVTrig(trig_level, trig_mode);

	if (!START_DELAY)
		delay_ms(START_DELAY);

	fovi_ramp.MeasureVI(samples, interval, FOVIe_MI_X1, MEAS_AWG);
	fovi_ramp1.MeasureVI(samples, interval, FOVIe_MI_X1, MEAS_AWG);
	acm_cap.MeasureVI(ACM_MV, samples, interval, MEAS_AWG);

	STSEnableAWG(&fovi_ramp, &fovi_ramp1);
	STSEnableMeas(&fovi_ramp, &fovi_ramp1, &acm_cap);
	STSAWGRunTriggerStop(&acm_cap, &fovi_ramp, &fovi_ramp1, &acm_cap);
	delay_us(TRIG_DELAY);

	int Trig_Point[SITE_NUM];
	SERIAL{
		Trig_Point[SITE] = (int)acm_cap.GetMeasResult(SITE, TRIG_RESULT);
		if (((Trig_Point[SITE]) > 2) && ((Trig_Point[SITE]) < samples - 2))
		{
			//result[SITE] = start_point + (Trig_Point[SITE] - 1) * step;
			result[SITE] = fovi_ramp.GetMeasResult(SITE, MVRET, Trig_Point[SITE] - 1);
		}
		else
		{
			result[SITE] = ERROR_RES;
		}
	}

	return TRUE;
}

// Ramp fovie voltage, capture acm voltage, trig stop
// Can set fovi_ramp v_range and i_range 
// Good for UVLO, threshold, power good measurement
// capture acm has current loading
BOOL Test_Method::ramp(FOVIe fovi_ramp, FOVIe_VRNG v_range, FOVIe_IRNG i_range, ACM acm_cap, ACM_IRNG acm_i_range, double loading, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double *result)
{
	if (interval < 10)	return FALSE;
	if (step == 0)	return FALSE;

	int samples;
	double pat[MAX_SAMPLES];
	samples = int(fabs((stop_point - start_point) / step));
	if ((samples <= 1) || (samples > MAX_SAMPLES))
	{
		samples = MAX_SAMPLES;
	}

	fovi_ramp.Set(FV, start_point, v_range, i_range, FOVIe_RELAY_ON, 1);
	acm_cap.Set(FI, loading, ACM_N2P18V, acm_i_range, ACM_RELAY_ON);

	STSAWGCreateRampData(&pat[0], samples, 1, start_point, stop_point);
	fovi_ramp.AwgClear();
	fovi_ramp.AwgLoader("awg", FV, v_range, i_range, pat, samples);
	fovi_ramp.AwgSelect("awg", 0, samples - 1, samples - 1, interval);
	acm_cap.SetMeasVTrig(trig_level, trig_mode);

	if (!START_DELAY)
		delay_ms(START_DELAY);

	fovi_ramp.MeasureVI(samples, interval, FOVIe_MI_X1, MEAS_AWG);
	acm_cap.MeasureVI(ACM_MV, samples, interval, MEAS_AWG);

	STSEnableAWG(&fovi_ramp);
	STSEnableMeas(&fovi_ramp, &acm_cap);
	STSAWGRunTriggerStop(&acm_cap, &fovi_ramp, &acm_cap);
	delay_us(TRIG_DELAY);

	int Trig_Point[SITE_NUM];
	SERIAL{
		Trig_Point[SITE] = (int)acm_cap.GetMeasResult(SITE, TRIG_RESULT);
		if (((Trig_Point[SITE]) > 2) && ((Trig_Point[SITE]) < samples - 2))
		{
			//result[SITE] = start_point + (Trig_Point[SITE] - 1) * step;
			result[SITE] = fovi_ramp.GetMeasResult(SITE, MVRET, Trig_Point[SITE] - 1);
		}
		else
		{
			result[SITE] = ERROR_RES;
		}
	}
	acm_cap.Set(FI, 0, ACM_N2P18V, acm_i_range, ACM_RELAY_ON);

	return TRUE;
}

