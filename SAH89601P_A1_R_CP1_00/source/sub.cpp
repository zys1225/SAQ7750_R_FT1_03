#include "stdafx.h"
extern string int2str(DWORD n);




void p2p_leakage_test(FOVIe fovi, double force_v, FOVIe_VRNG v_range, FOVIe_IRNG i_range, int delay_time, double *result)
{
	fovi.Set(FV, force_v, v_range, i_range, FOVIe_RELAY_ON);
    delay_us(delay_time);
    fovi.MeasureVI(50, 10);
    SERIAL result[SITE] = fovi.GetMeasResult(SITE, MIRET);
	fovi.Set(FV, 0, FOVIe_10V, FOVIe_1MA, FOVIe_RELAY_ON);
}

void p2p_leakage_test(ACM acm, double force_v, ACM_VRNG v_range, ACM_IRNG i_range, int delay_time, double *result)
{
	acm.Set(FV, force_v, v_range, i_range, ACM_RELAY_ON);
	delay_us(delay_time);
	acm.MeasureVI(ACM_MV, 50, 10);
	SERIAL result[SITE] = acm.GetMeasResult(SITE);
	acm.Set(FV, 0, ACM_N2P18V, ACM_2MA, ACM_RELAY_ON);
}

void p2p_leakage_test(const char* lpszPinGroup, double force_v, PPMUIRange i_range, int delay_time, double *result)
{
	dcm.Connect(lpszPinGroup);
	dcm.SetPPMU(lpszPinGroup, DCM_PPMU_FVMI, force_v, i_range);
	delay_us(delay_time);
	dcm.PPMUMeasure(lpszPinGroup, 50, 10);
	SERIAL result[SITE] = dcm.GetPPMUMeasResult(lpszPinGroup, SITE);
	dcm.SetPPMU(lpszPinGroup, DCM_PPMU_FVMI, 0, i_range);
}


void GET_WAFER_INFO(char *waferid, int *x, int *y)
{
	int skip_x = 0;
	int skip_y = 0;

	StsGetWaferID(waferid, 255);
	StsGetSingleDieCorXY(0, x[0], y[0]);
	SERIAL
	{
		x[SITE] = x[0] + (skip_x + 1) * (SITE % 8);
		y[SITE] = y[0] + (skip_y + 1) * (SITE / 8);
	}
}


void I2CSetup(I2C_REG_ADDR_MODE REGMode, bool dynamicload, double VIH, double VIL, int period)
{
	float time_period = period;//ns

	dcm.Connect("I2C");
	dcm.I2CSet(time_period, SITE_NUM, REGMode, "S24_28,S24_20,S24_12,S24_4,S9_60,S9_52,S9_44,S9_36,S24_36,S24_44,S24_52,S24_60,S9_20,S9_28,S9_4,S9_12",
		                                       "S24_24,S24_16,S24_8,S24_0,S9_56,S9_48,S9_40,S9_32,S24_32,S24_40,S24_48,S24_56,S9_16,S9_24,S9_0,S9_8");
	dcm.I2CSetPinLevel(VIH, VIL, 1.0, 0.8);
	dcm.I2CSetSCLEdge(0, time_period*0.7);
	dcm.I2CSetSDAEdge(time_period*0.1, time_period*0.995, time_period*0.1, time_period*0.95);
	if (dynamicload)
	{
		SERIAL dcm.SetDynamicLoad("SDA_D", SITE, DCM_OPEN_CLAMP_OPEN_LOAD, 0.001, 0.001, 3.3, 3.3, -2);
	}
	delay_ms(1);

}
void I2C_RD(BYTE SAddress, BYTE RegAddress, BYTE *data)
{
	dcm.I2CReadData(SAddress, RegAddress, 1);
	SERIAL
	{
		data[SITE] = dcm.I2CGetBitData(SITE, 0, 8);
	}
}
void I2C_WR(BYTE SAddress, BYTE RegAddress, int data)
{
	BYTE temp_data[SITE_NUM] = { 0 };
	SERIAL temp_data[SITE] = data;
	dcm.I2CWriteData(SAddress, RegAddress, 1, temp_data);
}


void Enter_TM()
{

	I2C_WR(DEV_ADDR_BUCK, 0x07, 0x00);//watchdog
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x00);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x44);//DISCOVERYP
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x49);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x53);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x43);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x4F);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x56);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x45);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x52);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x59);
	I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x50);
	//I2C_WR(DEV_ADDR_BUCK, Reg_TM_MODE, 0x00);
	//EFUSE_Preview();
}



void FORCE_ALL_PIN_0V()
{
	VBUS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VBAT.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	VSYS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	SW.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_ON);
	PMID.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	CC1.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	CC2.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	DP.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	DM.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	TSBAT.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	REGN.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);
	BTST.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_ON);

	dcm.Connect("DCM_ALL");
	dcm.SetPPMU("SCL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("SDA", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("QON", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("INT_AMUX", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("CE", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("PD_nINT", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
}
void FORCE_ALL_PIN_0V_OFF()
{

	VBUS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);
	VBAT.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);
	VSYS.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);
	SW.Set(FV, 0, FOVIe_10V, FOVIe_100MA, FOVIe_RELAY_OFF);
	PMID.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	CC1.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	CC2.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	DP.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	DM.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	TSBAT.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	REGN.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);
	BTST.Set(FV, 0, ACM_N2P18V, ACM_200MA, ACM_RELAY_OFF);

	dcm.SetPPMU("SCL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("SDA", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("QON", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("INT_AMUX", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("CE", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.SetPPMU("PD_nINT", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_32MA);
	dcm.Disconnect("DCM_ALL");

}

void POWER_OFF()
{
	FORCE_ALL_PIN_0V();
	delay_ms(3);
	FORCE_ALL_PIN_0V_OFF();
}