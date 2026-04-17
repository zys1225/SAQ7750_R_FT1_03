#include "stdafx.h"
#include "BoardCheck.h"
#include "sub.h"
#include <chrono>
#define		YES 	6
#define		NO		7
#define		DEBUG_MODE		0
extern string int2str(DWORD n);

/* No repeat: board_check_repeat = -1 */
int board_check_repeat = -1; // set repeat count

void connected_site_check(BoardCheck& bc)
{

    //update for 8300
    double site_check_specl = 0;
    double site_check_spech = 0;
    double result[SITE_NUM] = { 0 };
    //enable all sites
    for (int site = 0; site<SITE_NUM; ++site) bc.SetSiteConnected(site, 1); // inital for site check component measure


    //can only enable valid site for engineering use
    if (DEBUG_MODE)
    {
        //unsigned int bc_status_set = 0x3333;
        //unsigned int bc_status_set = 0xCCCC;
        unsigned int bc_status_set = 0xFFFFFFFF;
        SERIAL bc.SetSiteConnected(SITE, 0);

        STSSetSiteStatus(bc_status_set);

        for (int site = 0; site < 16; ++site)
        {
            if ((bc_status_set >> site) & 0x0001) bc.SetSiteConnected(site, 1);
            else  bc.SetSiteConnected(site, 0);
        }
    }


}


BOOL board_check_function(int &nBtn, BOOL &flag_pass)
{
    //////////////////////////////////////////////////////////////////
    /* Common usage format, don't change this if no specific need   */
    BoardCheck bc;
    double result[SITE_NUM];
    string board_name;
    string board_rev;
    string board_number;
    for (int site = 0; site<SITE_NUM; ++site) result[site] = 9999;
    DWORD tnum = 1;
    bool stoponfail = 1;
    STSSetSiteStatus(0xFFFFFFFF);
    //bc.Board_ID_Write("SC5891", "A", "2", "S24_31", "S24_15");
    //------------Board ID Check---------------
    if (0)
    {


        if (bc.Board_ID_Check("SC5891", "A", "S24_31", "S24_15"))
        {
            bc.Board_ID_Read(&board_name, &board_rev, &board_number, "S24_31", "S24_15");
        }
        else
        {

            nBtn = BTN_EXIT;
            return FALSE;
        }
    }

    //BOOL Board_ID_Check(const char* boardName, const char* rev, const char* SCLChannel, const char* SDAChannel);
    //BOOL Board_ID_Write(const char* boardName, const char* rev, const char* number, const char* SCLChannel, const char* SDAChannel);
    //BOOL Board_ID_Read(const char* boardName, const char* rev, const char* number, const char* SCLChannel, const char* SDAChannel);


    int sel_flag = NO;
    bc.ClearSiteConnected();
    while ((bc.IsNoSiteConnected() || (sel_flag == NO)) && (board_check_repeat == -1))
    {
        connected_site_check(bc);

        if (bc.IsNoSiteConnected() && DEBUG_MODE)
        {
            if (MessageBoxA(NULL, "没有使能任何工位! \n请确认: 如果继续，选择-是(Y)\n如果退出，选择-否(N)", "诊断提示对话框", MB_YESNO) == IDNO)
            {
                nBtn = BTN_EXIT;
                return FALSE;
            }
            continue;
        }
        sel_flag = YES;
    }

    if ((board_check_repeat != -1)) connected_site_check(bc);
    /* Common usage format, don't change this if no specific need   */
    //////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////
    /********************** User Define Begin ***********************/

    Cvi_config vi; // default FV,3V,0A,FOVIe_5V,FOVI_10MA,10ms,100 sample times,10 us interval

    //	/*** Connections and components ***/
    double spec_h = vi.v + 0.01;
    double spec_l = vi.v - 0.01;
    double cap4p7uF_l = 1.5;
    double cap4p7uF_h = 7;
    double cap10nF_l = 5;
    double cap10nF_h = 15;
    double cap100nF_l = 50;
    double cap100nF_h = 150;
    double cap10uF_l = 5;
    double cap10uF_h = 15;


	bc.test_cap(VBUS, tnum++, "VBUS, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(VBAT, tnum++, "VBAT, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(VSYS, tnum++, "VSYS, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(SW, tnum++, "SW, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(PMID, tnum++, "PMID, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(CC1, tnum++, "CC1, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(CC2, tnum++, "CC2, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(DP, tnum++, "DP, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(DM, tnum++, "DM, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(TSBAT, tnum++, "TSBAT, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(REGN, tnum++, "REGN, 10nF", cap10nF_l, cap10nF_h, "nF");
	bc.test_cap(BTST, tnum++, "BTST, 10nF", cap10nF_l, cap10nF_h, "nF");



	///////////////////////*** Leakage ***/

	vi.init();		// default FV,3V,0A,FOVIe_5V,FOVI_10MA,10ms,100 sample times,10 us interval
	vi.v = 5;
	vi.v_range = FOVIe_10V;
	vi.i_range = FOVIe_100UA;
	vi.miGain = FOVIe_MI_X10;
	vi.acm_v_range = ACM_N2P18V;
	vi.acm_i_range = ACM_20UA;
	vi.delay = 25;//ms
	double leak_spel = -100; //nA
	double leak_speh = 100;  //nA

	FORCE_ALL_PIN_0V();
	bc.test_i(VBUS, vi, tnum++, "VBUS leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(VBAT, vi, tnum++, "VBAT leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(VSYS, vi, tnum++, "VSYS leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(SW, vi, tnum++, "SW leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(PMID, vi, tnum++, "PMID leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(CC1, vi, tnum++, "CC1 leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(CC2, vi, tnum++, "CC2 leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(DP, vi, tnum++, "DP leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(DM, vi, tnum++, "DM leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(TSBAT, vi, tnum++, "TSBAT leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(REGN, vi, tnum++, "REGN leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();
	bc.test_i(BTST, vi, tnum++, "BTST leakage", leak_spel, leak_speh, "nA");
	FORCE_ALL_PIN_0V();

	bc.test_i("QON_D", "QON_D", tnum++, "QON leakage", vi.v, leak_spel, leak_speh, "nA");
	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);
	bc.test_i("CE_D", "CE_D", tnum++, "CE leakage", vi.v, leak_spel, leak_speh, "nA");
	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);
	bc.test_i("SCL_D", "SCL_D", tnum++, "SCL leakage", vi.v, leak_spel, leak_speh, "nA");
	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);
	bc.test_i("SDA_D", "SDA_D", tnum++, "SDA leakage", vi.v, leak_spel, leak_speh, "nA");
	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);
	bc.test_i("INT_AMUX_D", "INT_AMUX_D", tnum++, "INT_AMUX leakage", vi.v, leak_spel, leak_speh, "nA");
	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);
	bc.test_i("PD_nINT_D", "PD_nINT_D", tnum++, "PD_nINT leakage", vi.v, leak_spel, leak_speh, "nA");
	dcm.SetPPMU("DCM_ALL", DCM_PPMU_FVMI, 0, DCM_PPMUIRANGE_2MA);

    //	/*********************** User Define End ************************/
    //	//////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    /* Common usage format, don't change this if no specific need   */
    if (board_check_repeat < 0)
        bc.Display();
    nBtn = bc.get_nBtn();
    flag_pass = bc.IsCheckPass();
    bc.report();
    //////////////////////////////////////////////////////////////////


    return TRUE;
}


///////////////////////////////////////////////////////////////////////////
/* run_diags() define the boardcheck flow, as a part of boardcheck library
don't change this function if no specific need       				 */
///////////////////////////////////////////////////////////////////////////
BOOL run_diags(void)
{
    int nBtn = BTN_REDO;
    BOOL flag_pass = FALSE;
    char oper_id[20];
    StsGetOperatorID(oper_id, 20);


    if (strcmp(oper_id, "admin") == 0)
    {
        if (MessageBoxA(NULL, "当前账号为admin，是否需要诊断？\n量产请务必进行诊断！！！！\n\n选择（否）将跳过诊断。", "诊断提示对话框", MB_YESNO) == IDNO)
        {
            return TRUE;
        }
    }
	/*
    if (MessageBoxA(NULL, "1.请确认windows时间是否准确！！！！\n2.请注意！量产更换针卡后需要重新load程序！！！！\n\n选择（否）将关闭程序，选择（是）继续。", "测试注意事项", MB_YESNO) == IDNO)
    {
        PostQuitMessage(0);
        return FALSE;
    }
	*/
    if (MessageBoxA(NULL, "诊断即将开始! \n\n如果继续，请确认测试座里没有样品，选择-是(Y)\n如果退出，选择-否(N)", "诊断提示对话框", MB_YESNO) == IDNO)
    {
        PostQuitMessage(0);
        return FALSE;
    }

    if (board_check_repeat < 0)
    {
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
