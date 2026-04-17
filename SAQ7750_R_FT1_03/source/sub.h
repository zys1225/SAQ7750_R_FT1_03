#define MAX_SAMPLES 300
#define ERROR_RES	9999
#define START_DELAY	0


void ALL_FPVI_Power_Down(void);

void ALL_FOVI_Power_Down(void);

void ALL_FOVI_Power_Off(bool debug);

void Meas_P2P_LEAK(FOVI fovi_pin, CParam* P2P_LKG, double Pre_P2P_LEAK[][SITE_NUM], int PIN_NUM);

void Meas_P2P_LEAK(FPVI10 fpvi_pin, CParam* P2P_LKG, double Pre_P2P_LEAK[][SITE_NUM], int PIN_NUM);

void Meas_IN_Vth(FOVI fovi_ramp, FPVI10 fpvi_cap, FOVI_VRNG v_range, FOVI_IRNG i_range, double start_point, double stop_point, double step, int interval, double trig_level, TRIG_MODE trig_mode, double* result);

void Enter_TM();

void EE_Read(int bank_num, map<DWORD, map<DWORD, int>>& EE_READ, bool& ok);

void EE_Preview(int site_num, int bank_num, int pre_data, int& nErr);

void FindRowMins(double preview_value[SITE_NUM][32], int min_step[SITE_NUM]);

void EE_no_Write(int site_num, int bank_num, int& nErr);

void EE_Write(int site_num, int bank_num, int burn_data, int& nErr);
