/*****************************************************************************
*                                                                            *
*       Source title:   BoardCheck.h                                         *
*                       (board checker for AccoTest programs)                *
*         Written by:						                                 *
*        Description:					                                     *
*                                                                            *
*   Revision History:                                                        *
*                                                                            *
*     mm/dd/yy  r.rr  - Original coding.                                     *
*                                                                            *
*****************************************************************************/

/*
 REVISION BLOCK:
 -Rev. -- Date -------------------------------------------------------------------------------
   1  12/24/20   upgrade to 8300
 ----------------------------------------------------------------------------------------------
*/


#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
using namespace std;

#define BTN_REDO 2000
#define BTN_EXIT 2001
#define BTN_NONE 2002
#define BTN_CANCEL 2003

class DialogTemplate;

class DialogTemplate
{
public:
DialogTemplate(LPCSTR caption, DWORD style);

    DialogTemplate(LPCSTR caption, DWORD style, int x, int y, int w, int h,
        LPCSTR font = NULL, WORD fontSize = 8);
    void AddComponent(LPCSTR type, LPCSTR caption, DWORD style, DWORD exStyle,
        int x, int y, int w, int h, WORD id);
    void AddButton(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y,
        int w, int h, WORD id);
    void AddEditBox(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y,
        int w, int h, WORD id);
    void AddStatic(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y,
        int w, int h, WORD id);
    void AddListBox(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y,
        int w, int h, WORD id);
    void AddScrollBar(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y,
        int w, int h, WORD id);
    void AddComboBox(LPCSTR caption, DWORD style, DWORD exStyle, int x, int y,
        int w, int h, WORD id);
    operator const DLGTEMPLATE*() const;
    virtual ~DialogTemplate();

protected:
    // Prevent copying and assignments (by private definition)
    DialogTemplate operator=(const DialogTemplate &x) {return *this;}
    DialogTemplate(DialogTemplate &x) {}

    void AddStandardComponent(WORD type, LPCSTR caption, DWORD style,
        DWORD exStyle, int x, int y, int w, int h, WORD id);
    void AlignData(int size);
    void AppendString(LPCSTR string);
    void AppendData(void* data, int dataLength);
    void EnsureSpace(int length);

private:

    DLGTEMPLATE* dialogTemplate;
    int totalBufferLength;
    int usedBufferLength;

};


////////////////
//
// Base classes For Windows interface
//
////////////////
class BoardCheckElement
{
public:
    virtual ~BoardCheckElement() {};
    virtual void get(HWND hDlg)=0;
    virtual void set(HWND hDlg)=0;
    virtual void dlg_file(HWND hDlg,char *fname,int len)=0;
    virtual int width()=0;
    virtual int height()=0;
    virtual void create_dialog(DialogTemplate *dialog,int x,int y,int *newid)=0;
    virtual void command(HWND hDlg,int code,int target_id) {};
    int id;
    static int tagged_filename;     // Allow tags names in file name components
};
typedef BoardCheckElement *LPBoardCheckElement;


class BoardCheckButton : public BoardCheckElement
{
public:
    BoardCheckButton(char *aname,int aid,int def_style=BS_PUSHBUTTON);
    virtual ~BoardCheckButton();
    virtual int width();
    virtual int height();
    virtual void get(HWND hDlg) {};
    virtual void set(HWND hDlg) {};
    virtual void dlg_file(HWND hDlg,char *fname,int len) {};
    virtual void create_dialog(DialogTemplate *dialog,int x,int y,int *newid);
protected:
    // Prevent copying and assignments (by private definition)
    BoardCheckButton operator=(const BoardCheckButton &x) {return *this;}
    BoardCheckButton(BoardCheckButton &x) {}
    char *name;
    int style;
};


class BoardCheckListBox : public BoardCheckElement
{
public:
    BoardCheckListBox(char *aname,int aid,int def_style=LBS_NOTIFY | WS_VSCROLL | WS_BORDER);
    virtual ~BoardCheckListBox();
    virtual int width();
    virtual int height();
    virtual void get(HWND hDlg) {};
    virtual void set(HWND hDlg) {};
    virtual void dlg_file(HWND hDlg,char *fname,int len) {};
    virtual void create_dialog(DialogTemplate *dialog,int x,int y,int *newid);
    //virtual void command(HWND hDlg,int code,int target_id);
protected:
    // Prevent copying and assignments (by private definition)
    BoardCheckListBox operator=(const BoardCheckListBox &x) {return *this;}
    BoardCheckListBox(BoardCheckListBox &x) {}
    char *name;
    int style;
};

class BoardCheckGroup : public BoardCheckElement
{
public:
    BoardCheckGroup(int vertical,int aospace,int aispace);
    virtual ~BoardCheckGroup();
    void add(BoardCheckElement *el);
    virtual int width();
    virtual int height();
    virtual void get(HWND hDlg);
    virtual void set(HWND hDlg);
    virtual void dlg_file(HWND hDlg,char *fname,int len);
    virtual void create_dialog(DialogTemplate *dialog,int x,int y,int *newid);
    virtual void command(HWND hDlg,int code,int target_id);
    int count();
protected:
    // Prevent copying and assignments (by private definition)
    BoardCheckGroup operator=(const BoardCheckGroup &x) {return *this;}
    BoardCheckGroup(BoardCheckGroup &x) {}
    int is_vertical;
    int no;
    int ospace;     // outer space (space to other elements)
    int ispace;     // inner space (space between elements)
    LPBoardCheckElement *member;
};


class CBC_log
{
public:
	CBC_log(void) {}
	~CBC_log(void) {}
	void clear(void) {
		component_vec.clear();
		tnum_map.clear();
		lolim_map.clear();
		hilim_map.clear();
		unit_map.clear();
		data_map.clear();
		flag_map.clear();
	}

	vector<string> component_vec;
	map<string, DWORD> tnum_map;				// map<component, test_number>
	map<string, double> lolim_map;				// map<component, low_lim>
	map<string, double> hilim_map;				// map<component, high_lim>
	map<string, string> unit_map;				// map<component, unit>
	map<string, map<DWORD, double>> data_map;	// map<component, map<site, data>>
	map<string, map<DWORD, string>> flag_map;	// map<component, map<site, pass or fail>>
};

class Cvi_config
{
public:
	Cvi_config(){
		mode = FV;
		v = 3;
		i = 0;
		v_range = FOVIe_5V;
		i_range = FOVIe_10MA;
		miGain = FOVIe_MI_X1;
		acm_v_range = ACM_N2P18V;
		acm_i_range = ACM_200UA;
		delay = 10; // ms
		sample_times = 100;
		sample_period = 10;	// us
	}

	void init(void){
		mode = FV;
		v = 3;
		i = 0;
		v_range = FOVIe_5V;
		i_range = FOVIe_10MA;
		miGain = FOVIe_MI_X1;
		acm_v_range = ACM_N2P18V;
		acm_i_range = ACM_200UA;
		delay = 10; // ms
		sample_times = 100;
		sample_period = 10;	// us	
	}
	void fovi_set(int mode_val = FV, double v_val = 3, double i_val = 0, FOVIe_VRNG v_range_val = FOVIe_5V, FOVIe_IRNG i_range_val = FOVIe_10MA, int delay_val = 10, int sample_times_val = 100, int sample_period_val = 10){
		mode = mode_val;
		v = v_val;
		i = i_val;
		v_range = v_range_val;
		i_range = i_range_val;
		delay = delay_val;
		sample_times = sample_times_val;
		sample_period = sample_period_val;
	}
	void acm_set(int mode_val = FV, double v_val = 3, double i_val = 0, ACM_VRNG v_range_val = ACM_N2P18V, ACM_IRNG i_range_val = ACM_200UA, int delay_val = 10, int sample_times_val = 100, int sample_period_val = 10){
		mode = mode_val;
		v = v_val;
		i = i_val;
		acm_v_range = v_range_val;
		acm_i_range = i_range_val;
		delay = delay_val;
		sample_times = sample_times_val;
		sample_period = sample_period_val;
	}
	~Cvi_config() {}

	int mode;
	double v;
	double i;
	FOVIe_VRNG v_range;
	FOVIe_IRNG i_range;
	FOVIe_MI_GAIN miGain;
	ACM_VRNG acm_v_range;
	ACM_IRNG acm_i_range;

	int delay;
	int sample_times;
	int sample_period;
};


class BoardCheck
{
public:
    BoardCheck();
    ~BoardCheck();
    void Sot();
    friend BOOL CALLBACK  BoardCheckDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	FOVIe_VRNG get_fovi_v_range(double v){
		if(v < 0)
			v = -v;
		if(v <= 1)
			return FOVIe_1V;
		if (v <= 2)
			return FOVIe_2V;
		else if(v <= 5)
			return FOVIe_5V;
		else if(v <= 10)
			return FOVIe_10V;
		else if(v <= 20)
			return FOVIe_20V;
		else if(v <= 40)
			return FOVIe_40V;
		else 
			return FOVIe_5V;
	}
	FPVIe_VRNG get_fpvi_v_range(double v){
		if (v < 0)
			v = -v;
		if (v <= 0.1)
			return FPVIe_100MV;
		if (v <= 1)
			return FPVIe_1V;
		else if (v <= 2)
			return FPVIe_2V;
		else if (v <= 5)
			return FPVIe_5V;
		else if (v <= 10)
			return FPVIe_10V;
		else if (v <= 20)
			return FPVIe_20V;
		else if (v <= 40)
			return FPVIe_40V;
		else if (v <= 100)
			return FPVIe_100V;
		else
			return FPVIe_1V;
	}
	FOVIe_IRNG get_fovi_i_range(double i){
		if(i < 0)
			i = -i;
	    if(i <= 10e-6)
			return FOVIe_10UA;
		else if(i <= 100e-6)
			return FOVIe_100UA;
		else if(i <= 1e-3)
			return FOVIe_1MA;
		else if(i <= 10e-3)
			return FOVIe_10MA;
		else if(i <= 100e-3)
			return FOVIe_100MA;
		else if(i <= 1)
			return FOVIe_1A;
		else 
			return FOVIe_1MA;
	}
	ACM_IRNG get_acm_i_range(double i){
		if (i < 0)
			i = -i;
		if (i <= 5e-6)
			return ACM_5UA;
		else if (i <= 20e-6)
			return ACM_20UA;
		else if (i <= 200e-6)
			return ACM_200UA;
		else if (i <= 2e-3)
			return ACM_2MA;
		else if (i <= 20e-3)
			return ACM_20MA;
		else if (i <= 200e-3)
			return ACM_200MA;
		else if (i <= 0.5)
			return ACM_500MA;
		else
			return ACM_2MA;
	}
	PPMUIRange get_dcm_i_range(double i){
		if (i < 0)
			i = -i;
		if (i <= 2e-6)
			return DCM_PPMUIRANGE_2UA;
		else if (i <= 20e-6)
			return DCM_PPMUIRANGE_20UA;
		else if (i <= 200e-6)
			return DCM_PPMUIRANGE_200UA;
		else if (i <= 2e-3)
			return DCM_PPMUIRANGE_2MA;
		else if (i <= 32e-3)
			return DCM_PPMUIRANGE_32MA;
		else
			return DCM_PPMUIRANGE_2MA;
	}
	FPVIe_IRNG get_fpvi_i_range(double i){
		if(i < 0)
			i = -i;
		if(i <= 10e-6)
			return FPVIe_10UA;
		else if(i <= 100e-6)
			return FPVIe_100UA;
		else if(i <= 1e-3)
			return FPVIe_1MA;
		else if(i <= 10e-3)
			return FPVIe_10MA;
		else if(i <= 100e-3)
			return FPVIe_100MA;
		else if(i <= 1)
			return FPVIe_1A;
		else if (i <= 2)
			return FPVIe_2A;
		else if(i <= 10)
			return FPVIe_10A;
		else 
			return FPVIe_1MA;
	}


	void SetSiteConnected(int site, BOOL value) { site_connected[site] = value; }
	BOOL GetSiteConnected(int site) { return site_connected[site]; }
	BOOL IsNoSiteConnected(void) { for(int site=0; site<SITE_NUM; ++site) if(site_connected[site]) return FALSE; return TRUE;}
	void ClearSiteConnected(void) { for(int site=0; site<SITE_NUM; ++site) site_connected[site] = 0; }
	BOOL IsCheckPass(void);
	int get_nBtn(void) {return nBtn;}
    void Display(void);
	BOOL output_format(void);
	BOOL report(void);
	BOOL Board_ID_Check(const char* boardName, const char* rev, const char* SCLChannel, const char* SDAChannel);
	BOOL Board_ID_Write(const char* boardName, const char* rev, const char* number, const char* SCLChannel, const char* SDAChannel);
	BOOL Board_ID_Read(string* boardName, string* rev, string* number, const char* SCLChannel, const char* SDAChannel);

	// single ch fovi, force v, measure v
	BOOL test_v(FOVIe &fovi_res, Cvi_config &vi, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);			

	// dual ch fovi shorted, one force v, one measure v
	BOOL test_v(FOVIe &fovi_force, FOVIe &fovi_measure, Cvi_config &vi, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);	
	// only measure
	BOOL test_v(ACM &acm_measure,  DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);
	// dual ch fovi shorted, one force v, one measure v
	BOOL test_v(FOVIe &fovi_force, ACM &acm_measure, Cvi_config &vi, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);

	// dual ch fovi, force v, fpvi measure the diff of fovis
	BOOL test_v(FOVIe &fovi_res1, FOVIe &fovi_res2, FPVIe &fpvi_res, DWORD tnum, const char* component, double *result1=NULL);										
	// QVM, measure v
	BOOL test_v(QVMe &qvm_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);
	BOOL test_v(const char* lpszGroupPinName, const char* lpszPinName, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);
	
	BOOL test_i(ACM &acm_res, Cvi_config &vi, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);
	BOOL test_i(FOVIe &fovi_res, Cvi_config &vi, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);	
	BOOL test_i(const char* lpszGroupPinName, const char* lpszPinName, DWORD tnum, const char* component, double fv, double lolim, double hilim, const char* unit, double *result1 = NULL);
	BOOL test_r(ACM &acm_res, DWORD delay, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);

	BOOL test_r(FOVIe &fovi_res, DWORD delay, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);	
	BOOL test_r(FPVIe &fpvi_res, DWORD delay, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);
	BOOL test_r(FPVIe *fpvi_res, DWORD delay, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);
	BOOL test_r(const char* lpszGroupPinName, const char* lpszPinName,  DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);

	BOOL contact_check(FOVIe &fovi_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);
	BOOL contact_check(FPVIe *fpvi_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);

	// use awg method to test cap with fovi
	BOOL test_cap(FOVIe &fovi_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);
	BOOL test_cap(ACM &acm_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);

	// not use awg to test cap with fovi
	BOOL test_cap1(FOVIe &fovi_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);	
	// use awg method to test cap with fpvi
	BOOL test_cap(FPVIe &fpvi_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);
	// not use awg to test cap with fpvi
	BOOL test_cap1(FPVIe &fpvi_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);	
	BOOL test_cap1(ACM &acm_res, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);

	//BOOL test_dcm_init();
	//BOOL test_dcm_L(FOVIe &fovi_res, int* dio_ch, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);	
	//BOOL test_dcm_H(FOVIe &fovi_res, int* dio_ch, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);
	//// Test dio high z leak
	//BOOL test_dcm_Z(FOVIe &fovi_res, int* dio_ch, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);	
	// Test fovi_res 1ms pulse
	BOOL test_qtmu(FOVIe &fovi_res, QTMUe *qtmu, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);
	BOOL test_qtmu(ACM &acm_res, QTMUe &qtmu, QTMUe_SOURCE_AB source, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1=NULL);
	BOOL test_qtmu_freq(QTMUe &qtmu, QTMUe_SOURCE_AB source, DWORD tnum, const char* component, double lolim, double hilim, const char* unit, double *result1 = NULL);

	BOOL log(DWORD tnum, const char* component, double* result, double lolim, double hilim, const char* unit);

private:
    // Prevent copying and assignments (by private definition)
    BoardCheck operator=(const BoardCheck &x) {return *this;}
    BoardCheck(BoardCheck &x) {}
    void create_input(void);
    void delete_input(void);
    BoardCheckGroup *input;
    BoardCheckGroup *buttons;    // deleted with input
    BoardCheckGroup *listbox;    // deleted with input

    DialogTemplate *dialog;
	CBC_log bc_log;
	vector<string> display_vec;
	//int nMaxExtent;
	BOOL CheckPass;				// result of all site
	BOOL check_result[SITE_NUM];// result of each site
	int nBtn;
	BOOL site_connected[SITE_NUM];
};

