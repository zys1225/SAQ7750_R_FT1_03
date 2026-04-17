#include "stdafx.h"


void GET_WAFER_INFO(char *waferid, int *x, int *y);

void p2p_leakage_test(FOVIe fovi, double force_v, FOVIe_VRNG v_range, FOVIe_IRNG i_range, int delay_time, double *result);
void p2p_leakage_test(ACM acm, double force_v, ACM_VRNG v_range, ACM_IRNG i_range, int delay_time, double *result);
void p2p_leakage_test(const char* lpszPinGroup, double force_v, PPMUIRange i_range, int delay_time, double *result);

void I2CSetup(I2C_REG_ADDR_MODE REGMode, bool dynamicload, double VIH, double VIL, int period);
void I2C_RD(BYTE SAddress, BYTE RegAddress, BYTE *data);
void I2C_WR(BYTE SAddress, BYTE RegAddress, int data);
void Enter_TM();

void FORCE_ALL_PIN_0V();
void FORCE_ALL_PIN_0V_OFF();
void POWER_OFF();