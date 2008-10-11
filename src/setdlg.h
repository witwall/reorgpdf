#ifndef __DEF_SETDLG_H__
#define __DEF_SETDLG_H__

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include "db_setting.h"

class SettingDialog: public wxDialog
{
public:
	SettingDialog(wxWindow *parent);
	~SettingDialog(void);

	void BindData_Setting(db_setting &db);
	db_setting GetData_Setting(void);
protected:
	wxNotebook *set_mainPanel;
		wxPanel *set_filePanel;
			wxChoice *set_defpaper;
			wxChoice *set_pageposition;
			wxSpinCtrl *set_leftmargin;
			wxSpinCtrl *set_topmargin;
			wxCheckBox *set_useframe;
			wxCheckBox *set_fitpaper;
			wxCheckBox *set_extraction;
			wxCheckBox *set_onbackup;
			wxCheckBox *set_overwriteori;
		wxPanel *set_viewPanel;
			wxCheckBox *set_usethumb;
			wxChoice *set_defpageorient;
			wxCheckBox *set_usecol_org;
			wxSpinCtrl *set_colcnt_org;
			wxCheckBox *set_usecol_new;
			wxSpinCtrl *set_colcnt_new;
			wxChoice *set_thumbsize;
			wxCheckBox *set_reload_onsave;
			
	wxButton *set_OkBtn;
	wxButton *set_CanBtn;
	
	void set_OkBtn_clicked(wxCommandEvent &event);
	void set_CanBtn_clicked(wxCommandEvent &event);
	void set_pageposition_changed(wxCommandEvent &event);
	void set_fitpaper_clicked(wxCommandEvent &event);
protected:
	enum EvtCode {
		setdlg_ok = 10,
		setdlg_can,
		setdlg_pagepos,
		setdlg_fitpaper
	};
private:
	DECLARE_EVENT_TABLE()
	wxPanel *generate_filepage(wxNotebook *book);
	wxPanel *generate_viewpage(wxNotebook *book);
};
#endif