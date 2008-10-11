#include "setdlg.h"

BEGIN_EVENT_TABLE(SettingDialog, wxDialog)
	EVT_BUTTON(setdlg_ok,SettingDialog::set_OkBtn_clicked)
	EVT_BUTTON(setdlg_can,SettingDialog::set_CanBtn_clicked)
	EVT_CHOICE(setdlg_pagepos,SettingDialog::set_pageposition_changed)
	EVT_CHECKBOX(setdlg_fitpaper,SettingDialog::set_fitpaper_clicked)
END_EVENT_TABLE() 

SettingDialog::SettingDialog(wxWindow *parent)
:wxDialog(parent, wxID_ANY, _("Setting"), wxDefaultPosition, wxDefaultSize)
{
	SetClientSize(wxSize(510,385));

	set_mainPanel = new wxNotebook(this,wxID_ANY,wxPoint(0,0),wxSize(500,340),wxNB_TOP);
	set_mainPanel->AddPage(generate_filepage(set_mainPanel), _("File"), false);
	set_mainPanel->AddPage(generate_viewpage(set_mainPanel), _("View"), false);
	
	set_OkBtn = new wxButton(this, setdlg_ok, _T("OK"), wxPoint(-1,-1),wxSize(85,-1));
	set_CanBtn = new wxButton(this, setdlg_can, _("Cancel"), wxPoint(-1,-1),wxSize(85,-1));

	wxBoxSizer *bsizer1 = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *bsizer2 = new wxBoxSizer(wxHORIZONTAL);
		bsizer2->Add(set_OkBtn,0,wxTOP|wxLEFT,0);
		bsizer2->Add(set_CanBtn,0,wxLEFT,5);
	bsizer1->Add(set_mainPanel,0,wxBOTTOM,5);
	bsizer1->Add(bsizer2,0,wxRIGHT|wxALIGN_RIGHT,10);
	this->SetSizer(bsizer1);
	this->Layout();
	
}
SettingDialog::~SettingDialog(void)
{
	
}
wxPanel *SettingDialog::generate_filepage(wxNotebook *book)
{
	set_filePanel = new wxPanel(book,wxID_ANY);
	
	wxString pos_items[] = {
		_("Left-top(default)"),_("Left-top(with margin)"),_("Center")
	};
	wxString paper_items[] = {
		_T("None"),_T("A3"),_T("A4"),_T("A5"),_T("B4"),_T("B5")
	};

	
	wxStaticText *label1 = new wxStaticText(set_filePanel,wxID_ANY,_("Default paper type:"));
	set_defpaper = new wxChoice(set_filePanel, wxID_ANY,wxDefaultPosition,wxSize(220,-1),6,paper_items);
	set_defpaper->SetSelection(2);
	set_defpaper->SetToolTip(_("Specify a kind of default paper."));
	
	wxStaticBox *sbox1 = new wxStaticBox(set_filePanel, wxID_ANY, _("Positionning of page"));
	wxStaticBoxSizer *sbsizer1 = new wxStaticBoxSizer(sbox1,wxVERTICAL);
	wxStaticText *label2 = new wxStaticText(set_filePanel,wxID_ANY,_("Position:"));
	set_pageposition = new wxChoice(set_filePanel, setdlg_pagepos,wxDefaultPosition,wxSize(150,-1),3,pos_items);
	set_pageposition->SetSelection(0);
	set_pageposition->SetToolTip(_("Specify a placement position to the paper of the document."));
	wxBoxSizer *sb1_child1 = new wxBoxSizer(wxHORIZONTAL);
	sb1_child1->Add(label2,0,wxTOP|wxLEFT,3);
	sb1_child1->Add(set_pageposition,0,wxLEFT,5);
		wxStaticBox *sbox_margin = new wxStaticBox(set_filePanel, wxID_ANY, _("Margin"));
		wxStaticBoxSizer *sbsizer_margin = new wxStaticBoxSizer(sbox_margin,wxHORIZONTAL);
		wxStaticText *label3 = new wxStaticText(set_filePanel,wxID_ANY,_("Left:"));
		set_leftmargin = new wxSpinCtrl(set_filePanel,wxID_ANY,_T("0"),wxDefaultPosition,wxSize(65,-1),wxSP_ARROW_KEYS,0,100,0);
		set_leftmargin->SetToolTip(_("Specify number of left margin"));
		wxStaticText *label4 = new wxStaticText(set_filePanel,wxID_ANY,_("Top:"));
		set_topmargin = new wxSpinCtrl(set_filePanel,wxID_ANY,_T("0"),wxDefaultPosition,wxSize(65,-1),wxSP_ARROW_KEYS,0,100,0);
		set_topmargin->SetToolTip(_("Specify number of top margin"));
		sbsizer_margin->Add(label3,0,wxTOP|wxLEFT,0);
		sbsizer_margin->Add(set_leftmargin,0,wxLEFT,5);
		sbsizer_margin->Add(label4,0,wxLEFT,10);
		sbsizer_margin->Add(set_topmargin,0,wxLEFT,5);
	set_useframe = new wxCheckBox(set_filePanel, wxID_ANY, _("Use frame around page"));
	set_useframe->SetToolTip(_("Surround the document with the black frames, if a document does not reach size of the paper"));
	set_fitpaper = new wxCheckBox(set_filePanel, setdlg_fitpaper, _("Fit in paper"));
	set_fitpaper->SetToolTip(_("Match a document with size of the paper, if a document does not reach size of the paper"));
	set_extraction = new wxCheckBox(set_filePanel, wxID_ANY, _("Fit with extraction"));
	set_extraction->SetToolTip(_("Change the ratio of the length and breadth and match a document with size of the paper."));
	sbsizer1->Add(sb1_child1,0,wxTOP|wxLEFT,0);
	sbsizer1->Add(sbsizer_margin,0,wxTOP,5);
	sbsizer1->Add(set_useframe,0,wxTOP,5);
	sbsizer1->Add(set_fitpaper,0,wxTOP|wxLEFT,0);
	sbsizer1->Add(set_extraction,0,wxTOP|wxLEFT,0);
	
	set_onbackup = new wxCheckBox(set_filePanel, wxID_ANY, _("Do backup"));
	set_onbackup->SetToolTip(_("Back up the file, if a file path is the same as an original file."));
	set_overwriteori = new wxCheckBox(set_filePanel, wxID_ANY, _("Overwrite original file"));
	set_overwriteori->SetToolTip(_("Overwrite with an original file, if you never store a new file."));
	
	
	wxBoxSizer *bsizer1 = new wxBoxSizer(wxVERTICAL);
	bsizer1->Add(label1,0,wxTOP|wxLEFT,5);
	bsizer1->Add(set_defpaper,0,wxLEFT,5);
	bsizer1->Add(sbsizer1,0,wxTOP|wxLEFT,5);
	bsizer1->Add(set_onbackup,0,wxTOP|wxLEFT,5);
	bsizer1->Add(set_overwriteori,0,wxTOP|wxLEFT,5);
	set_filePanel->SetSizer(bsizer1);
	set_filePanel->Layout();
	return set_filePanel;

}
wxPanel *SettingDialog::generate_viewpage(wxNotebook *book)
{
	set_viewPanel = new wxPanel(book,wxID_ANY);
	
	wxString orient_items[] = {
		_("Auto"),_("Portrait"),_("Landscape")
	};
	wxString thumsize_items[] = {
		_("Small"),_("Normal"),_("Big")
	};
	
	set_usethumb = new wxCheckBox(set_viewPanel, wxID_ANY, _("Use real thumbnails"));
	set_usethumb->SetValue(true);
	set_usethumb->SetToolTip(_("Use the thumbnail of the real document"));
	
	wxStaticText *label5 = new wxStaticText(set_viewPanel,wxID_ANY,_("Thumbnail size:"));
	set_thumbsize = new wxChoice(set_viewPanel, wxID_ANY,wxDefaultPosition,wxSize(100,-1),3,thumsize_items);
	set_thumbsize->SetSelection(0);
	set_thumbsize->SetToolTip(_("Specify the size of the thumbnail to display"));
	wxStaticText *label51 = new wxStaticText(set_viewPanel,wxID_ANY,_("(enable after reboot application)"));

	wxStaticText *label1 = new wxStaticText(set_viewPanel,wxID_ANY,_("Default page orientation:"));
	set_defpageorient = new wxChoice(set_viewPanel, wxID_ANY,wxDefaultPosition,wxSize(120,-1),3,orient_items);
	set_defpageorient->SetSelection(0);
	set_defpageorient->SetToolTip(_("Specify a direction of the paper of the default."));

	wxStaticBox *sbox1 = new wxStaticBox(set_viewPanel, wxID_ANY, _("Original document"));
	wxStaticBoxSizer *sbsizer1 = new wxStaticBoxSizer(sbox1,wxVERTICAL);
	set_usecol_org = new wxCheckBox(set_viewPanel, wxID_ANY, _("Use collapse"));
	wxStaticText *label2 = new wxStaticText(set_viewPanel,wxID_ANY,_("Count:"));
	set_colcnt_org = new wxSpinCtrl(set_viewPanel,wxID_ANY,_T("2"),wxDefaultPosition,wxSize(65,-1),wxSP_ARROW_KEYS,2,50,2);
	set_colcnt_org->Enable(false);
	wxBoxSizer *sb1_child = new wxBoxSizer(wxHORIZONTAL);
	sb1_child->Add(label2,0,wxTOP,3);
	sb1_child->Add(set_colcnt_org,0,wxLEFT,5);
	sbsizer1->Add(set_usecol_org,0,wxTOP|wxLEFT,0);
	sbsizer1->Add(sb1_child,0,wxTOP,5);

	wxStaticBox *sbox2 = new wxStaticBox(set_viewPanel, wxID_ANY, _("New document"));
	wxStaticBoxSizer *sbsizer2 = new wxStaticBoxSizer(sbox2,wxVERTICAL);
	set_usecol_new = new wxCheckBox(set_viewPanel, wxID_ANY, _("Use collapse"));
	wxStaticText *label3 = new wxStaticText(set_viewPanel,wxID_ANY,_("Count:"));
	set_colcnt_new = new wxSpinCtrl(set_viewPanel,wxID_ANY,_T("2"),wxDefaultPosition,wxSize(65,-1),wxSP_ARROW_KEYS,2,50,2);
	set_colcnt_new->Enable(false);
	wxBoxSizer *sb2_child = new wxBoxSizer(wxHORIZONTAL);
	sb2_child->Add(label3,0,wxTOP,3);
	sb2_child->Add(set_colcnt_new,0,wxLEFT,5);
	sbsizer2->Add(set_usecol_new,0,wxTOP|wxLEFT,0);
	sbsizer2->Add(sb2_child,0,wxTOP,5);


	set_reload_onsave = new wxCheckBox(set_viewPanel, wxID_ANY, _("Reload thumbnails after overwrite"));
	set_reload_onsave->SetToolTip(_("Reload a file again after overwrite saving."));

	wxBoxSizer *bsizer1 = new wxBoxSizer(wxVERTICAL);
	bsizer1->Add(set_usethumb,0,wxTOP|wxLEFT,5);
	bsizer1->Add(label5,0,wxTOP|wxLEFT,5);
	bsizer1->Add(set_thumbsize,0,wxLEFT,5);
	bsizer1->Add(label51,0,wxTOP|wxLEFT,0);
	bsizer1->Add(label1,0,wxTOP|wxLEFT,5);
	bsizer1->Add(set_defpageorient,0,wxLEFT,5);
	bsizer1->Add(sbsizer1,0,wxTOP|wxLEFT,5);
	bsizer1->Add(sbsizer2,0,wxTOP|wxLEFT,5);
	bsizer1->Add(set_reload_onsave,0,wxTOP|wxLEFT,5);
	set_viewPanel->SetSizer(bsizer1);
	set_viewPanel->Layout();
	bsizer1->Hide(sbsizer1);
	bsizer1->Hide(sbsizer2);
	return set_viewPanel;

}
void SettingDialog::set_OkBtn_clicked(wxCommandEvent &event)
{
	this->SetReturnCode(wxID_OK);
	this->EndModal(wxID_OK);
	this->Hide();
}
void SettingDialog::set_CanBtn_clicked(wxCommandEvent &event)
{
	this->SetReturnCode(wxID_CANCEL);
	this->EndModal(wxID_CANCEL);
	this->Hide();
}
void SettingDialog::BindData_Setting(db_setting &db)
{
	//file tab
	int sel = 0;
	switch (db.GetDefaultPaper()) {
	case wxPAPER_NONE: sel = 0; break;
	case wxPAPER_A3: sel = 1; break;
	case wxPAPER_A4: sel = 2; break;
	case wxPAPER_A5: sel = 3; break;
	case wxPAPER_B4: sel = 4; break;
	case wxPAPER_B5: sel = 5; break;
	}
	set_defpaper->SetSelection(sel);
	set_pageposition->SetSelection((int)db.GetPagePosition());
	set_leftmargin->SetValue(db.GetPageMargin().x);
	set_topmargin->SetValue(db.GetPageMargin().y);
	set_useframe->SetValue(db.GetUseFrame());
	set_fitpaper->SetValue(db.GetFitPaper());
	set_extraction->SetValue(db.GetExtraction());
	set_onbackup->SetValue(db.GetUseBackup());
	set_overwriteori->SetValue(db.GetOverwriteOriginal());
	//view tab
	set_usethumb->SetValue(db.GetUseThumbnail());
	set_defpageorient->SetSelection(db.GetDefaultPageOrientation());
	set_usecol_org->SetValue(db.GetUseCollapseOriginal());
	set_colcnt_org->SetValue(db.GetCollapseCountOriginal());
	set_usecol_new->SetValue(db.GetUseCollapseNew());
	set_colcnt_new->SetValue(db.GetCollapseCountNew());
	switch (db.GetThumbnailSize()) {
	case small_ts:
		set_thumbsize->SetSelection(0);
		break;
	case normal_ts:
		set_thumbsize->SetSelection(1);
		break;
	case big_ts:
		set_thumbsize->SetSelection(2);
		break;
	}
	set_reload_onsave->SetValue(db.GetReloadPDF());

	wxCommandEvent evt;
	set_pageposition_changed(evt);
	set_fitpaper_clicked(evt);
}
db_setting SettingDialog::GetData_Setting(void)
{
	db_setting db;
	//file tab
	wxPaperSize paps;
	switch (set_defpaper->GetSelection()) {
	case 0: paps = wxPAPER_NONE; break;
	case 1: paps = wxPAPER_A3; break;
	case 2: paps = wxPAPER_A4; break;
	case 3: paps = wxPAPER_A5; break;
	case 4: paps = wxPAPER_B4; break;
	case 5: paps = wxPAPER_B5; break;
	}
	db.SetDefaultPaper(paps);
	db.SetPagePosition((PageSetPos)set_pageposition->GetSelection());
	wxPoint pos;
	pos.x = set_leftmargin->GetValue();
	pos.y = set_topmargin->GetValue();
	db.SetPageMargin(pos);
	db.SetUseFrame(set_useframe->GetValue());
	db.SetFitPaper(set_fitpaper->GetValue());
	db.SetExtraction(set_extraction->GetValue());
	db.SetUseBackup(set_onbackup->GetValue());
	db.SetOverwriteOriginal(set_overwriteori->GetValue());
	//view tab
	db.SetUseThumbnail(set_usethumb->GetValue());
	db.SetDefaultPageOrientation(set_defpageorient->GetSelection());
	db.SetUseCollapseOriginal(set_usecol_org->GetValue());
	db.SetCollapseCountOriginal(set_colcnt_org->GetValue());
	db.SetUseCollapseNew(set_usecol_new->GetValue());
	db.SetCollapseCountNew(set_colcnt_new->GetValue());
	switch (set_thumbsize->GetSelection()) {
	case 0:
		db.SetThumbnailSize(small_ts);
		break;
	case 1:
		db.SetThumbnailSize(normal_ts);
		break;
	case 2:
		db.SetThumbnailSize(big_ts);
		break;
	}
	db.SetReloadPDF(set_reload_onsave->GetValue());

	return db;
}
void SettingDialog::set_pageposition_changed(wxCommandEvent &event)
{
	bool flag;
	if (set_pageposition->GetSelection() == lefttopwith_psp) {
		flag = true;
	}else{
		flag = false;
	}
	set_leftmargin->Enable(flag);
	set_topmargin->Enable(flag);
}
void SettingDialog::set_fitpaper_clicked(wxCommandEvent &event)
{
	set_extraction->Enable(set_fitpaper->GetValue());
}
