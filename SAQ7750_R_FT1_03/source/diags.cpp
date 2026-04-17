#include "stdafx.h"
#include "BoardCheck.h"
#include "sub.h"
#define		YES 	6
#define		NO		7
extern string int2str(DWORD n);

/* No repeat: board_check_repeat = -1 */
int board_check_repeat = -1; // set repeat count
//int board_check_repeat = 100; // set repeat count


void connected_site_check(BoardCheck& bc) {
	double site_check_specl = 0;
	double site_check_spech = 0;
	double result[SITE_NUM] = { 0 };
	for (int site = 0; site < SITE_NUM; ++site) bc.SetSiteConnected(site, 1); // inital for site check component measure

	//////////////////////////////////////////////////////////////////
	/********************** User Define Begin ***********************/
	KELVIN_GND.Set(FI, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
	KELVIN_GND.Set(FI, 1e-3, FOVI_10V, FOVI_10MA, RELAY_ON);//FORCE 0.1MA
	delay_ms(5);
	KELVIN_GND.MeasureVI(30, 10);
	SERIAL result[SITE] = KELVIN_GND.GetMeasResult(SITE, 0, MVRET);//site1=1V site2_2K=2V
	KELVIN_GND.Set(FI, 0, FOVI_10V, FOVI_10MA, RELAY_ON); //
	KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON); //
	delay_ms(2);
	KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_OFF); //OFF VI

	/********************** User Define End ***********************/
	//////////////////////////////////////////////////////////////////

	for (int site = 0; site < SITE_NUM; ++site) bc.SetSiteConnected(site, 0); // clear site check flag
	for (int site = 0; site < SITE_NUM; ++site)
	{
		// reset site check flag base on component measure
		if (site == 0) { site_check_specl = 0.5; site_check_spech = 1.5; }
		if (site == 1) { site_check_specl = 1.5; site_check_spech = 2.5; }
		if (site == 2) { site_check_specl = 2.5; site_check_spech = 3.5; }
		if (site == 3) { site_check_specl = 3.5; site_check_spech = 4.5; }
		if ((result[site] > site_check_specl) && (result[site] < site_check_spech))
			bc.SetSiteConnected(site, 1);
		else
			bc.SetSiteConnected(site, 0);
	}
}


BOOL board_check_function(int& nBtn, BOOL& flag_pass)
{
	//////////////////////////////////////////////////////////////////
	/* Common usage format, don't change this if no specific need   */
	BoardCheck bc;
	double result[SITE_NUM];
	int io_ch[SITE_NUM];
	for (int site = 0; site < SITE_NUM; ++site) result[site] = 9999;
	DWORD tnum = 1;

	int sel_flag = NO;
	bc.ClearSiteConnected();
	while ((bc.IsNoSiteConnected() || (sel_flag == NO)) && (board_check_repeat == -1))
	{
		connected_site_check(bc);

		if (bc.IsNoSiteConnected()) {
			if (MessageBoxA(NULL, "没有检测到任何工位! \n请确认: 测试板和排线是否连接正确？\n\n如果继续，选择-是(Y)\n如果退出，选择-否(N)", "诊断提示对话框", MB_YESNO) == IDNO) {
				nBtn = BTN_EXIT;
				return FALSE;
			}
			continue;
		}

		string msg("当前使用工位编号：");
		for (int site = 0; site < SITE_NUM; ++site) {
			if (bc.GetSiteConnected(site))
				msg = msg + "工位(" + int2str(site + 1) + ") ？";
		}
		msg = msg + "\n\n如果正确: \n(第一步) 选择-是(Y)，程序将开始运行诊断\n\n如果错误:\n(第一步) 请检查排线连接\n(第二步) 选择-否(N)，程序将重新检测工位";
		sel_flag = MessageBoxA(NULL, msg.c_str(), "诊断提示对话框", MB_YESNO);
	}
	if (board_check_repeat != -1) connected_site_check(bc);	// repeat always check site connect
	/* Common usage format, don't change this if no specific need   */
	//////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////
	/********************** User Define Begin ***********************/

	Cvi_config vi; // default FV,3V,0A,FOVI_5V,FOVI_10MA,10ms,100 sample times,10 us interval

	/*** Connections and components ***/
	double spec_h = vi.v + 0.01;
	double spec_l = vi.v - 0.01;

	double leak_spel = -100; //nA
	double leak_speh = 100;  //nA

	cbit.SetOn(-1);
	delay_ms(3);


	///////////////////////RESOURCE INITIAL///////////////////////////
	KELVIN_VS.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IN1_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	DEN_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	NC_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IN0_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IS_DESL_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);

	KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	delay_ms(2);

	KELVIN_VS.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	KELVIN_GND.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IN1_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	DEN_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	NC_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IN0_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	IS_DESL_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);

	KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	delay_ms(2);

	double result_V[SITE_NUM];
	double result_I[SITE_NUM];
	double result_R[SITE_NUM];
	double result_QTMU[SITE_NUM];
	double result_DIO[4][SITE_NUM] = { 9999 };

	///////////////////////RESOURCE CHECK/////////////////////////////
	///////////////////////FPVI1-KELVIN_OUT0//////////////////////////
	//cbit.SetOn(K3_K10_OUT0_GND_KelvinCheck, -1);
	//delay_ms(2);

	KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT0.Set(FV, -2, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	KELVIN_OUT0.MeasureVI(30, 10);
	groupgetresults(KELVIN_OUT0, MVRET, result, SITE_NUM);
	KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	bc.log(tnum++, "FPVI1-KELVIN_OUT0", result, -2.2, -1.8, "V");

	///////////////////////FPVI0-KELVIN_OUT1//////////////////////////
	//cbit.SetOn(K1_K2_VS_OUT1_KelvinCheck, -1);
	//delay_ms(2);

	KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT1.Set(FV, -2, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	KELVIN_OUT1.MeasureVI(30, 10);
	groupgetresults(KELVIN_OUT1, MVRET, result, SITE_NUM);
	KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	bc.log(tnum++, "FPVI0-KELVIN_OUT0", result, -2.2, -1.8, "V");

	///////////////////////FOVI6-KELVIN_GND///////////////////////////
	
	//vi.mode = FV;
	//vi.v = -2;
	//vi.v_range = FOVI_10V;
	//vi.i_range = FOVI_10MA;
	//bc.test_v(KELVIN_GND, vi, tnum++, "FOVI6_KELVIN_GND", -2.2, -1.8, "V");
	//delay_ms(2);

	//vi.mode = FV;
	//vi.v = 6.5;
	//vi.v_range = FOVI_10V;
	//vi.i_range = FOVI_10UA;
	//bc.test_i(KELVIN_GND, vi, tnum++, "FOVI6_KELVIN_GND_LEAKAGE", leak_spel, leak_speh, "nA");//接地的源漏电过大
	//delay_ms(2);

	///////////////////////FOVI0-KELVIN_VS//////////////////////////////
	//cbit.SetOn(K1_K2_VS_OUT1_KelvinCheck, -1); // VS,VF在子板中已经短接，不需闭合k1，否则漏电过大
	//delay_ms(2);

	vi.mode = FV;
	vi.v = -2;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10MA;
	bc.test_v(KELVIN_VS, vi, tnum++, "FOVI0_KELVIN_VS", -2.2, -1.8, "V");
	delay_ms(2);

	vi.mode = FV;
	vi.v = 6.5;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10UA;
	bc.test_i(KELVIN_VS, vi, tnum++, "FOVI0_KELVIN_VS_LEAKAGE", leak_spel, leak_speh, "nA");
	delay_ms(2);

	cbit.SetOn(-1);
	delay_ms(2);

	///////////////////////FOVI1-IN1//////////////////////////////
	vi.mode = FV;
	vi.v = -2;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10MA;
	bc.test_v(IN1_VI, vi, tnum++, "FOVI1_IN1", -2.2, -1.8, "V");
	delay_ms(2);

	vi.mode = FV;
	vi.v = 6.5;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10UA;
	bc.test_i(IN1_VI, vi, tnum++, "FOVI1_IN1_LEAKAGE", leak_spel, leak_speh, "nA");
	delay_ms(2);

	///////////////////////FOVI2_DEN//////////////////////////////
	vi.mode = FV;
	vi.v = -2;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10MA;
	bc.test_v(DEN_VI, vi, tnum++, "FOVI2_DEN", -2.2, -1.8, "V");
	delay_ms(2);

	vi.mode = FV;
	vi.v = 6.5;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10UA;
	bc.test_i(DEN_VI, vi, tnum++, "FOVI2_DEN_LEAKAGE", leak_spel, leak_speh, "nA");
	delay_ms(2);

	///////////////////////FOVI3_NC//////////////////////////////
	vi.mode = FV;
	vi.v = -2;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10MA;
	bc.test_v(NC_VI, vi, tnum++, "FOVI3_NC", -2.2, -1.8, "V");
	delay_ms(2);

	vi.mode = FV;
	vi.v = 6.5;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10UA;
	bc.test_i(NC_VI, vi, tnum++, "FOVI3_NC_LEAKAGE", leak_spel, leak_speh, "nA");
	delay_ms(2);

	///////////////////////FOVI4_IN0//////////////////////////////
	vi.mode = FV;
	vi.v = -2;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10MA;
	bc.test_v(IN0_VI, vi, tnum++, "FOVI4_IN0", -2.2, -1.8, "V");
	delay_ms(2);

	vi.mode = FV;
	vi.v = 6.5;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10UA;
	bc.test_i(IN0_VI, vi, tnum++, "FOVI4_IN0_LEAKAGE", leak_spel, leak_speh, "nA");
	delay_ms(2);

	///////////////////////FOVI5_IS_DESL//////////////////////////////
	vi.mode = FV;
	vi.v = -2;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10MA;
	bc.test_v(IS_DESL_VI, vi, tnum++, "FOVI5_IS_DESL", -2.2, -1.8, "V");
	delay_ms(2);

	vi.mode = FV;
	vi.v = 6.5;
	vi.v_range = FOVI_10V;
	vi.i_range = FOVI_10UA;
	bc.test_i(IS_DESL_VI, vi, tnum++, "FOVI5_IS_DESL_LEAKAGE", leak_spel, leak_speh, "nA");
	delay_ms(2);


	///////////////////////  CAP CHECK//////////////////////////////
	delay_ms(2);
	bc.test_cap(KELVIN_VS, tnum++, "C2_10nF", 5, 15, "nF");

	cbit.SetOn(K7_DenPullUp, -1);
	delay_ms(2);

	bc.test_cap(KELVIN_VS, tnum++, "C1_1uF", 0.5, 1.5, "uF");
	cbit.SetOn(-1);
	delay_ms(2);

	///////////////////////// RES CHECK////////////////////////////
	//----------------------R4_4P7K_PULLUP
	PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	PullUp_VI.Set(FV, 5, FOVI_10V, FOVI_1MA, RELAY_ON);
	delay_ms(2);
	PullUp_VI.MeasureVI(20, 10);
	SERIAL result_V[SITE] = PullUp_VI.GetMeasResult(SITE, MVRET);
	SERIAL result_I[SITE] = PullUp_VI.GetMeasResult(SITE, MIRET);
	SERIAL result_R[SITE] = result_V[SITE] / ((result_I[SITE] + 1e-12) * 1e3);//KOHM
	bc.log(tnum++, "R4_4P7K_PULLUP", result_R, 4.6, 4.8, "KOhm");
	PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_1MA, RELAY_ON);
	PullUp_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(2);

	//---------------------R10/R11_10K_PULLDOWN
	cbit.SetOn(K3_K10_OUT0_GND_KelvinCheck, K1_K2_VS_OUT1_KelvinCheck, K11_OutPullDown, -1);
	delay_ms(2);
	//cbitclose(K11_OutPullDown);
	//delay_ms(2);

	KELVIN_OUT0.Set(FV, 5, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	delay_ms(5);
	KELVIN_OUT0.MeasureVI(20, 10);
	SERIAL result_V[SITE] = KELVIN_OUT0.GetMeasResult(SITE, MVRET);
	SERIAL result_I[SITE] = KELVIN_OUT0.GetMeasResult(SITE, MIRET);
	SERIAL result_R[SITE] = result_V[SITE] / ((result_I[SITE] + 1e-12) * 1e3);//KOHM
	bc.log(tnum++, "R10_10K_OUT0_PULLDOWN", result_R, 9.9, 10.1, "KOhm");
	KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	KELVIN_OUT0.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);

	KELVIN_OUT1.Set(FV, 5, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	delay_ms(2);
	KELVIN_OUT1.MeasureVI(20, 10);
	SERIAL result_V[SITE] = KELVIN_OUT1.GetMeasResult(SITE, MVRET);
	SERIAL result_I[SITE] = KELVIN_OUT1.GetMeasResult(SITE, MIRET);
	SERIAL result_R[SITE] = result_V[SITE] / ((result_I[SITE] + 1e-12) * 1e3);//KOHM
	bc.log(tnum++, "R10_10K_OUT1_PULLDOWN", result_R, 9.9, 10.1, "KOhm");
	KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_1MA, RELAY_ON);
	KELVIN_OUT1.Set(FV, 0, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);

	/////////////////////////QTMU CHECK///////////////////////////////
	QTMU_MEAS.Init();
	QTMU_MEAS.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_100KHz);
	QTMU_MEAS.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_100KHz);
	QTMU_MEAS.SetStartTrigger(2, QTMU_PLUS_POS_SLOPE);
	QTMU_MEAS.SetStopTrigger(2.5, QTMU_PLUS_NEG_SLOPE);
	QTMU_MEAS.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	QTMU_MEAS.Connect();
	delay_ms(1);
	cbitclose(K6_QTMU);
	delay_ms(2);

	QTMU_MEAS.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, 0);//返回us
	QTMU_MEAS.SetTimeOut(100); //waiting time is 10ms.

	IN0_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(2);
	IN0_VI.Set(FV, 5, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(5);
	IN0_VI.Set(FV, 0, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(2);
	QTMU_MEAS.SinglePlsMeas(0);
	SERIAL  result_QTMU[SITE] = QTMU_MEAS.GetMeasureResult(SITE) * 1e-3;//ms
	bc.log(tnum++, "QTMU_0_R", result_QTMU, 4, 6, "ms");//DELAY 5ms

	QTMU_MEAS.Disconnect();
	cbitclose(K6_QTMU);
	delay_ms(2);

	/////////////////////////DIO CHECK////////////////////////////////
	//----------------------DIO 0
	IN0_VI.Set(FI, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
	delay_ms(2);

	dio_plus.Connect(0,2,4,6);
	dio_plus.Run("CheckLow1", "CheckLow2", 10e-3);
	IN0_VI.MeasureVI(100, 10);//IN0_L
	SERIAL result_DIO[0][SITE] = IN0_VI.GetMeasResult(SITE, 0, MVRET);

	dio_plus.Run("CheckHigh1", "CheckHigh2", 10e-3);
	IN0_VI.MeasureVI(100, 10);//IN0_H
	SERIAL result_DIO[2][SITE] = IN0_VI.GetMeasResult(SITE, 0, MVRET);

	dio_plus.Stop();
	dio_plus.Disconnect();
	delay_ms(2);

	bc.log(tnum++, "IN0_Low", result_DIO[0], -0.1, 0.1, "V");
	bc.log(tnum++, "IN0_High", result_DIO[2], 4.7, 5.2, "V");

	//----------------------DIO 1
	IN1_VI.Set(FI, 0, FOVI_10V, FOVI_10UA, RELAY_ON);
	delay_ms(2);

	dio_plus.Connect(1,3,5,7);
	dio_plus.Run("CheckLow1", "CheckLow2", 10e-3);
	IN1_VI.MeasureVI(100, 10);//IN1_L
	SERIAL result_DIO[1][SITE] = IN1_VI.GetMeasResult(SITE, 0, MVRET);

	dio_plus.Run("CheckHigh1", "CheckHigh2", 10e-3);
	IN1_VI.MeasureVI(100, 10);//IN1_H
	SERIAL result_DIO[3][SITE] = IN1_VI.GetMeasResult(SITE, 0, MVRET);

	dio_plus.Stop();
	dio_plus.Disconnect();
	delay_ms(2);

	bc.log(tnum++, "IN1_Low", result_DIO[1], -0.1, 0.1, "V");
	bc.log(tnum++, "IN1_High", result_DIO[3], 4.7, 5.2, "V");

	ALL_FPVI_Power_Down();
	ALL_FOVI_Power_Down();
	cbit.SetOn(-1);
	delay_ms(3);

	/*********************** User Define End ************************/
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	/* Common usage format, don't change this if no specific need   */
	if (board_check_repeat < 0)
		bc.Display();
	nBtn = bc.get_nBtn();
	flag_pass = bc.IsCheckPass();
	bc.report();

	return TRUE;
}

BOOL run_diags(void)
{
	int nBtn = BTN_REDO;
	BOOL flag_pass = FALSE;

	if (MessageBoxA(NULL, "诊断即将开始! \n\n如果继续，请确认测试座里没有样品，选择-是(Y)\n如果退出，选择-否(N)", "诊断提示对话框", MB_YESNO) == IDNO) {
		PostQuitMessage(0);
		return FALSE;
	}

	if (board_check_repeat < 0) {
		while ((nBtn == BTN_REDO) && (flag_pass == FALSE))
		{
			board_check_function(nBtn, flag_pass);
		}
		if ((nBtn == BTN_EXIT) || (nBtn == BTN_CANCEL))
			PostQuitMessage(0);
		if (flag_pass == TRUE)
			MessageBoxA(NULL, "诊断成功！\n请点击确认，开始测试", "诊断提示对话框", MB_OK);
	}

	string message_str;
	message_str = "重复测试" + int2str(board_check_repeat) + "次\n请点击确定按钮开始\n结束后程序自动退出\n数据请查看 bc.csv";
	if (board_check_repeat > 0)
		MessageBoxA(NULL, message_str.c_str(), "诊断提示对话框", MB_OK);

	while (board_check_repeat-- > 0)
	{
		board_check_function(nBtn, flag_pass);
		delay_ms(100);

		if (board_check_repeat == 1)
			PostQuitMessage(0);
	}


	return TRUE;
}