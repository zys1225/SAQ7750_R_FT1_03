#include "stdafx.h"
#include "sub.h"


void ALL_FPVI_Power_Down(void)
{
	KELVIN_OUT0.Set(FV, 0, FPVI10_100V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	KELVIN_OUT1.Set(FV, 0, FPVI10_100V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
}

void ALL_FOVI_Power_Down(void)
{
	KELVIN_VS.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IN1_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	DEN_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	NC_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IN0_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IS_DESL_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	delay_ms(2);
}




void ALL_FOVI_Power_Off(bool debug)
{
	if (debug)
	{
		KELVIN_VS.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		IN1_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		DEN_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		NC_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		IN0_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		IS_DESL_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		delay_ms(2);

		KELVIN_VS.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		IN1_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		DEN_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		NC_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		IN0_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		IS_DESL_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
		delay_ms(2);

	}
}



void Meas_P2P_LEAK(FOVI fovi_pin, CParam* P2P_LKG, double Pre_P2P_LEAK[][SITE_NUM], int PIN_NUM)
{
	double Results[SITE_NUM] = { 0.0 };
	fovi_pin.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
	fovi_pin.Set(FV, 50e-3, FOVI_1V, FOVI_10MA, RELAY_ON);
	fovi_pin.Set(FV, 50e-3, FOVI_1V, FOVI_10UA, RELAY_ON);
	delay_us(1000);

	fovi_pin.MeasureVI(20, 10);
	SERIAL Results[SITE] = fovi_pin.GetMeasResult(SITE, MIRET);
	SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
	SERIAL Pre_P2P_LEAK[PIN_NUM][SITE] = Results[SITE];
	SERIAL P2P_LKG->SetTestResult(SITE, 0, Results[SITE]);
	fovi_pin.Set(FV, 0, FOVI_1V, FOVI_10MA, RELAY_ON);
	delay_us(100);
}

void Meas_P2P_LEAK(FPVI10 fpvi_pin, CParam* P2P_LKG, double Pre_P2P_LEAK[][SITE_NUM], int PIN_NUM)
{
	double Results[SITE_NUM] = { 0.0 };
	fpvi_pin.Set(FV, 0, FPVI10_1V, FPVI10_10MA, RELAY_ON);
	fpvi_pin.Set(FV, 50e-3, FPVI10_1V, FPVI10_10MA, RELAY_ON);
	fpvi_pin.Set(FV, 50e-3, FPVI10_1V, FPVI10_10UA, RELAY_ON);
	delay_us(1000);

	fpvi_pin.MeasureVI(20, 10);
	SERIAL Results[SITE] = fpvi_pin.GetMeasResult(SITE, MIRET);
	SERIAL Results[SITE] = Results[SITE] * 1e9; // nA
	SERIAL Pre_P2P_LEAK[PIN_NUM][SITE] = Results[SITE];
	SERIAL P2P_LKG->SetTestResult(SITE, 0, Results[SITE]);
	fpvi_pin.Set(FV, 0, FPVI10_1V, FPVI10_10MA, RELAY_ON);
	delay_us(100);
}

void Meas_IN_Vth(FOVI fovi_ramp, FPVI10 fpvi_cap, FOVI_VRNG v_range, FOVI_IRNG i_range,
	double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double* result)
{
	//MV@V_TRIG
	int samples;
	double pat[MAX_SAMPLES];
	samples = int(fabs((stop_point - start_point) / step));
	if ((samples <= 1) || (samples > MAX_SAMPLES))
	{
		samples = MAX_SAMPLES;
	}

	fovi_ramp.Set(FV, start_point, v_range, i_range, RELAY_ON, 1);
	fpvi_cap.Set(FI, 0, FPVI10_20V, FPVI10_1MA, RELAY_ON);
	delay_ms(2);

	STSAWGCreateRampData(&pat[0], samples, 1, start_point, stop_point);
	fovi_ramp.AwgClear();
	fovi_ramp.AwgLoader("awg", FV, v_range, i_range, pat, samples);
	fovi_ramp.AwgSelect("awg", 0, samples - 1, samples - 1, interval);
	fpvi_cap.SetMeasVTrig(trig_level, trig_mode);

	//if (!START_DELAY)
	//	delay_ms(START_DELAY);

	fovi_ramp.MeasureVI(samples, interval, MEAS_AWG);
	fpvi_cap.MeasureVI(samples, interval, MEAS_AWG);

	//STSEnableAWG(&fovi_ramp);//cap 不要跑awg!!!
	//STSEnableMeas(&fovi_ramp, &fpvi_cap);
	//STSAWGRun();

	STSEnableAWG(&fovi_ramp, &fpvi_cap);
	STSEnableMeas(&fovi_ramp, &fpvi_cap);
	STSAWGRunTriggerStop(&fpvi_cap, &fovi_ramp, &fpvi_cap);

	int Trig_Point[SITE_NUM];
	SERIAL
	{
		Trig_Point[SITE] = (int)fpvi_cap.GetMeasResult(SITE, MVRET,TRIG_RESULT); // 跳变point_num
		if (((Trig_Point[SITE]) > 2) && ((Trig_Point[SITE]) < samples - 2))
		{
			////result[SITE] = start_point + (Trig_Point[SITE] - 1) * step;
			result[SITE] = fovi_ramp.GetMeasResult(SITE, MVRET, Trig_Point[SITE] - 1);

		}
		else
		{
			result[SITE] = ERROR_RES;
		}
	}
}


void Enter_TM()
{

	KELVIN_VS.Set(FV, 5, FOVI_10V, FOVI_1A, RELAY_ON);//芯片正常上电 从VS， IN0_PAD 上force 5V 电压, 芯片内部使能，芯片正常工作， 然后量测 IN0_PAD 电流（此时电流应该较小）。
	IN0_VI.Set(FV, 5.0, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_us(500);

	DEN_VI.Set(FV, -0.4, FOVI_10V, FOVI_10MA, RELAY_ON); //Pull SEN_PAD 电压为 -0.4V always 
	delay_ms(5);

	IS_DESL_VI.Set(FV, 6.6, FOVI_10V, FOVI_10MA, RELAY_ON); //force SEL0_PAD和SEL1_PAD加高电平为6.6V， t>550uS（不分先后，但是每个PAD都需拉到6.6V550us以上）
	IN1_VI.Set(FV, 6.6, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(5);

	DEN_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON); //SEN_PAD 释放电压 从-0.4V  to 5V， 此时芯片会从正常模式进入测试模式，IN0_PAD上吃电电流增加到>50uA)
	delay_ms(1);

	IS_DESL_VI.Set(FI, 0, FOVI_10V, FOVI_100UA, RELAY_ON); 
	IN1_VI.Set(FI, 0, FOVI_10V, FOVI_100UA, RELAY_ON); //进入测试模式后，SEL1_PAD释放（确保进入测试模式后，在不同的测试项下，必须接电阻/不接电阻）6.6-->0
	DEN_VI.Set(FI, 0, FOVI_10V, FOVI_100UA, RELAY_ON);
	delay_us(500);
}


DIO_PLUS_PINGROUP_DEF_REG(dioplus_efuse_grp)
"SDA"
DIO_PLUS_PINGROUP_DEF_REG_END(dioplus_efuse_grp);


void EE_Read(int bank_num, map<DWORD, map<DWORD, int>>& EE_READ, bool& ok)
{
	char* modifyPattern[3] = {"0"};
	SITE_NO efsite1[1] = { SITE_1 };
	SITE_NO efsite2[1] = { SITE_2 };
	SITE_NO efsite3[1] = { SITE_3 };
	SITE_NO efsite4[1] = { SITE_4 };

	SITE_NO efsite_all[SITE_NUM] = { SITE_1, SITE_2, SITE_3, SITE_4 };
	int nErr = 1;

	// 检查bank_num是否在有效范围(0-7)
	if (bank_num < 0 || bank_num > 7)
	{
		return;
	}

	// 通过位运算提取3位二进制值，modifyPattern[0]为最高位(bit2)
	modifyPattern[0] = (bank_num & 0x4) ? "1" : "0";  // 提取bit2 (0x4 = 100b)
	modifyPattern[1] = (bank_num & 0x2) ? "1" : "0";  // 提取bit1 (0x2 = 010b)
	modifyPattern[2] = (bank_num & 0x1) ? "1" : "0";  // 提取bit0 (0x1 = 001b)

	// DEN --->data
	// IN1 --->clk
	nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Read_Bank_str", 3, modifyPattern, efsite_all, SITE_NUM, 1);
	delay_us(200);

	nErr = dio_plus.Run("Read_Bank_str", "Read_Bank_stp");
	// 从bank的bit7开始读取8位数据，MSB格式
	SERIAL EE_READ[SITE][bank_num] = dio_plus.GetSerialPatternResult("SDA", SITE, "bank_bit7", 0, 8, MSB, &ok);
}


void EE_Preview(int site_num, int bank_num, int pre_data, int& nErr)
{

	SITE_NO efsite1[1] = { SITE_1 };
	SITE_NO efsite2[1] = { SITE_2 };
	SITE_NO efsite3[1] = { SITE_3 };
	SITE_NO efsite4[1] = { SITE_4 };
	SITE_NO efsite_all[SITE_NUM] = { SITE_1, SITE_2, SITE_3, SITE_4 };

	// 定义3个元素的数组用于选择bank num二进制值
	char* modifyPattern_bank[3] = { "0", "0", "0" };  // 初始化所有位为0
	// 检查bank_num是否在有效范围(0-7)
	if (bank_num < 0 || bank_num > 7)
	{
		return;
	}

	// 通过位运算提取3位二进制值，modifyPattern[0]为最高位(bit2)
	modifyPattern_bank[0] = (bank_num & 0x4) ? "1" : "0";  // 提取bit2 (0x4 = 100b)
	modifyPattern_bank[1] = (bank_num & 0x2) ? "1" : "0";  // 提取bit1 (0x2 = 010b)
	modifyPattern_bank[2] = (bank_num & 0x1) ? "1" : "0";  // 提取bit0 (0x1 = 001b)

	// 定义8个元素的数组用于存储8位二进制值
	char* modifyPattern_data[8] = { "0", "0", "0", "0", "0", "0", "0", "0" };  // 初始化所有位为0
	modifyPattern_data[0] = (pre_data & 0x80) ? "1" : "0";  // 提取bit7 (0x80 = 10000000b)
	modifyPattern_data[1] = (pre_data & 0x40) ? "1" : "0";  // 提取bit6 (0x40 = 01000000b)
	modifyPattern_data[2] = (pre_data & 0x20) ? "1" : "0";  // 提取bit5 (0x20 = 00100000b)
	modifyPattern_data[3] = (pre_data & 0x10) ? "1" : "0";  // 提取bit4 (0x10 = 00010000b)
	modifyPattern_data[4] = (pre_data & 0x08) ? "1" : "0";  // 提取bit3 (0x08 = 00001000b)
	modifyPattern_data[5] = (pre_data & 0x04) ? "1" : "0";  // 提取bit2 (0x04 = 00000100b)
	modifyPattern_data[6] = (pre_data & 0x02) ? "1" : "0";  // 提取bit1 (0x02 = 00000010b)
	modifyPattern_data[7] = (pre_data & 0x01) ? "1" : "0";  // 提取bit0 (0x01 = 00000001b)


	if (site_num == 0)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_Bank_str", 3, modifyPattern_bank, efsite1, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_bank_bit7", 8, modifyPattern_data, efsite1, 1, 0);
	}
	else if (site_num == 1)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_Bank_str", 3, modifyPattern_bank, efsite2, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_bank_bit7", 8, modifyPattern_data, efsite2, 1, 0);
	}
	else if (site_num == 2)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_Bank_str", 3, modifyPattern_bank, efsite3, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_bank_bit7", 8, modifyPattern_data, efsite3, 1, 0);
	}
	else
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_Bank_str", 3, modifyPattern_bank, efsite4, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Pre_bank_bit7", 8, modifyPattern_data, efsite4, 1, 0);
	}
	delay_us(200);
}

// 函数：计算二维数组每一行的最小值及对应的列索引（step）
void FindRowMins(double preview_value[SITE_NUM][32], int min_step[SITE_NUM])
{
	// 遍历每一行（每个site）
	for (int site = 0; site < SITE_NUM; site++)
	{
		// 初始化当前行的最小值为第一个元素
		double row_mins = preview_value[site][0];
		min_step[site] = 0;  // 初始索引为0（第0列）

		// 遍历当前行的每一列（每个step），寻找最小值
		for (int step = 1; step < 32; step++)
		{
			// 发现更小的值时更新
			if (preview_value[site][step] < row_mins)
			{
				row_mins = preview_value[site][step];
				min_step[site] = step;  // 记录当前列索引（step）
			}
		}
	}
}




void EE_no_Write(int site_num, int bank_num, int& nErr)
{
	SITE_NO efsite1[1] = { SITE_1 };
	SITE_NO efsite2[1] = { SITE_2 };
	SITE_NO efsite3[1] = { SITE_3 };
	SITE_NO efsite4[1] = { SITE_4 };
	SITE_NO efsite_all[SITE_NUM] = { SITE_1, SITE_2, SITE_3, SITE_4 };

	// 定义3个元素的数组用于选择bank num二进制值
	char* modifyPattern_bank[3] = { "0", "0", "0" };  // 初始化所有位为0
	char* modifyPattern_burn =  "0" ;
	// 检查bank_num是否在有效范围(0-7)
	if (bank_num < 0 || bank_num > 7)
	{
		return;
	}

	// 通过位运算提取3位二进制值，modifyPattern[0]为最高位(bit2)
	modifyPattern_bank[0] = (bank_num & 0x4) ? "1" : "0";  // 提取bit2 (0x4 = 100b)
	modifyPattern_bank[1] = (bank_num & 0x2) ? "1" : "0";  // 提取bit1 (0x2 = 010b)
	modifyPattern_bank[2] = (bank_num & 0x1) ? "1" : "0";  // 提取bit0 (0x1 = 001b)

	if (site_num == 0)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite1, 1, 1);
		nErr = dio_plus.ModifyLinePattern(dioplus_efuse_grp, "Burn_enable", modifyPattern_burn, efsite1, 1, 0);
	}
	else if (site_num == 1)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite2, 1, 1);
		nErr = dio_plus.ModifyLinePattern(dioplus_efuse_grp, "Burn_enable", modifyPattern_burn, efsite1, 1, 0);
	}
	else if (site_num == 2)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite3, 1, 1);
		nErr = dio_plus.ModifyLinePattern(dioplus_efuse_grp, "Burn_enable", modifyPattern_burn, efsite1, 1, 0);
	}
	else
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite4, 1, 1);
		nErr = dio_plus.ModifyLinePattern(dioplus_efuse_grp, "Burn_enable", modifyPattern_burn, efsite1, 1, 0);
	}
	delay_us(200);
}



void EE_Write(int site_num, int bank_num, int burn_data, int& nErr)
{

	SITE_NO efsite1[1] = { SITE_1 };
	SITE_NO efsite2[1] = { SITE_2 };
	SITE_NO efsite3[1] = { SITE_3 };
	SITE_NO efsite4[1] = { SITE_4 };
	SITE_NO efsite_all[SITE_NUM] = { SITE_1, SITE_2, SITE_3, SITE_4 };

	// 定义3个元素的数组用于选择bank num二进制值
	char* modifyPattern_bank[3] = { "0", "0", "0" };  // 初始化所有位为0
	// 检查bank_num是否在有效范围(0-7)
	if (bank_num < 0 || bank_num > 7)
	{
		return;
	}

	// 通过位运算提取3位二进制值，modifyPattern[0]为最高位(bit2)
	modifyPattern_bank[0] = (bank_num & 0x4) ? "1" : "0";  // 提取bit2 (0x4 = 100b)
	modifyPattern_bank[1] = (bank_num & 0x2) ? "1" : "0";  // 提取bit1 (0x2 = 010b)
	modifyPattern_bank[2] = (bank_num & 0x1) ? "1" : "0";  // 提取bit0 (0x1 = 001b)

	// 定义8个元素的数组用于存储8位二进制值
	char* modifyPattern_data[8] = { "0", "0", "0", "0", "0", "0", "0", "0" };  // 初始化所有位为0
	modifyPattern_data[0] = (burn_data & 0x80) ? "1" : "0";  // 提取bit7 (0x80 = 10000000b)
	modifyPattern_data[1] = (burn_data & 0x40) ? "1" : "0";  // 提取bit6 (0x40 = 01000000b)
	modifyPattern_data[2] = (burn_data & 0x20) ? "1" : "0";  // 提取bit5 (0x20 = 00100000b)
	modifyPattern_data[3] = (burn_data & 0x10) ? "1" : "0";  // 提取bit4 (0x10 = 00010000b)
	modifyPattern_data[4] = (burn_data & 0x08) ? "1" : "0";  // 提取bit3 (0x08 = 00001000b)
	modifyPattern_data[5] = (burn_data & 0x04) ? "1" : "0";  // 提取bit2 (0x04 = 00000100b)
	modifyPattern_data[6] = (burn_data & 0x02) ? "1" : "0";  // 提取bit1 (0x02 = 00000010b)
	modifyPattern_data[7] = (burn_data & 0x01) ? "1" : "0";  // 提取bit0 (0x01 = 00000001b)



	if (site_num == 0)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite1, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_bank_bit7", 8, modifyPattern_data, efsite1, 1, 0);
	}
	else if (site_num == 1)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite2, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_bank_bit7", 8, modifyPattern_data, efsite2, 1, 0);
	}
	else if (site_num == 2)
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite3, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_bank_bit7", 8, modifyPattern_data, efsite3, 1, 0);
	}
	else
	{
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_str", 3, modifyPattern_bank, efsite4, 1, 1);
		nErr = dio_plus.ModifyMultiLinePatterns(dioplus_efuse_grp, "Burn_bank_bit7", 8, modifyPattern_data, efsite4, 1, 0);
	}
	delay_us(200);

}


