#ifndef __DEF_EDPDF_H__
#define __DEF_EDPDF_H__

#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/dnd.h>
#include <vector>

#include "db_pdfdoc.h"
#include "setdlg.h"
#include "wxpopp_ppm.h"

//pre-define
class EDFrame;

using namespace std;

class DDText: public wxTextDropTarget
{
public:
	DDText(EDFrame *par, wxTreeCtrl *own1, wxTreeCtrl *own2)
	{
		parent = par;
		oldowner = own1;
		newowner = own2;
	}
	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);
private:
	wxTreeCtrl *oldowner, *newowner;
	EDFrame *parent;
};

class EDFrame: public wxFrame
{
public:
	EDFrame(const wxString &title);
	~EDFrame(void);

	db_pdfdoc &GetDocDB(void) { return newdocDB; }
	wxPdfDocument *GetPdfObj(void) { return pdf; }
	int GetSplitPos(void) { return ed_SplitWin->GetSashPosition(); }
	void SetPageCount(void);
	void AppendNewPage(const wxString &label, wxTreeItemId id, int pos = -1);
protected:
	wxMenuBar *ed_MenuBar;
	wxSplitterWindow *ed_SplitWin;
	wxStatusBar *ed_StatusBar;
	
	wxPanel *ed_LeftPanel;
		wxListCtrl *ed_OriDocumentInfo;
		wxTreeCtrl *ed_OriThumbnails;
	wxPanel *ed_RightPanel;
		wxListCtrl *ed_NewDocumentInfo;
			wxMenu *ed_NewDocMenu;
		wxTreeCtrl *ed_NewThumbnails;
		
	SettingDialog *setdlg;

	void fileMenu_New_clicked(wxCommandEvent &event);
	void fileMenu_Open_clicked(wxCommandEvent &event);
	void fileMenu_Save_clicked(wxCommandEvent &event);
	void fileMenu_Exit_clicked(wxCommandEvent &event);
	void editMenu_CutCopy_clicked(wxCommandEvent &event);
	void editMenu_Paste_clicked(wxCommandEvent &event);
	void editMenu_Delete_clicked(wxCommandEvent &event);
	void editMenu_Rotate_clicked(wxCommandEvent &event);
	void editMenu_Select_clicked(wxCommandEvent &event);
	void editMenu_Insert_clicked(wxCommandEvent &event);
	void toolMenu_Setting_clicked(wxCommandEvent &event);
	void helpMenu_About_clicked(wxCommandEvent &event);
	void ed_OriThumbnails_dragged(wxTreeEvent &event);
	void ed_OriThumbnails_menu(wxTreeEvent &event);
	void ed_NewThumbnails_dragged(wxTreeEvent &event);
	void ed_NewThumbnails_dragend(wxTreeEvent &event);
	void ed_NewThumbnails_keydown(wxTreeEvent &event);
	void ed_NewThumbnails_menu(wxTreeEvent &event);
	void ed_NewDocumentInfo_rightclicked(wxListEvent &event);
	void ed_OriThumbnails_GetThumb_clicked(wxCommandEvent &event);
	void ed_AnyThumbnails_selected(wxTreeEvent &event);
	void ed_NewDocumentInfo_dblclicked(wxListEvent &event);
	void ed_NewDocMenu_UseInfo_clicked(wxCommandEvent &event);
private:
	DECLARE_EVENT_TABLE()
	enum Evcode {
		edpdf_fm_new = 10,
		edpdf_fm_open,
		edpdf_fm_save,
		edpdf_fm_saveas,
		edpdf_fm_exit,
		edpdf_em_cut,
		edpdf_em_copy,
		edpdf_em_paste,
		edpdf_em_delete,
		edpdf_em_rotleft,
		edpdf_em_rotright,
		edpdf_em_rot180,
		edpdf_em_seltype,
		edpdf_em_selall,
		edpdf_em_seleven,
		edpdf_em_selodd,
		edpdf_em_selspec,
		edpdf_em_inslast,
		edpdf_em_insspec,
		edpdf_tm_setting,
		edpdf_hm_help,
		edpdf_hm_about,
		edpdf_orilist,
		edpdf_newlist,
		edpdf_oriinfo,
		edpdf_newinfo,
		edpdf_useinfo,
		edpdf_origetthumb,
		edpdf_oriinsnew
	};
	wxImageList *OriImages;
	wxTreeItemId OriDragItem;
	wxTreeItemId NewDragItem;
	db_pdfdoc newdocDB;
	wxPdfDocument *pdf;
	int srcpdfnum;
	wxPdfInfo srcpdfinfo;
	wxPdfInfo newpdfinfo;
	wxString srcpdfname;
	wxString srcpdfpass;
	int ImgSize;
	int Resolution;
	wxArrayTreeItemIds clipbd;
	db_page_array clipbd_db;
	bool clipcut;
	//wxPopplerConvPPM popp;
	bool issecure;
	
	void setup_originalpage(wxSplitterWindow *parent);
	void setup_newpage(wxSplitterWindow *parent);
	void LoadIconImage(bool loadreal);

	int OpenPDF(const wxString &name, const wxString &pass = wxEmptyString);
	int SavePDF(const wxString &name);
	void ClosePDF(void);
	
	void GenerateThumbnail(const wxString &name, int firstp, int lastp, const wxString &pass = wxEmptyString);
	void EnumThumbnail(int pagesize, bool fromfile);
	void GetPageNumber(const wxString &str, int *page, int *srcpage);
	wxTreeItemId GetTreeItem(wxTreeCtrl *tree, int index);
	void SetListInformation(const wxString &name, int num, wxPdfInfo &info);
};

#endif

