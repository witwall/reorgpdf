#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/aboutdlg.h>
#include <wx/progdlg.h>
#include <wx/imaglist.h>
#include <vector>
#include "myapp.h"
#include "edpdf.h"
#include "appconstants.h"

#ifdef __LINUX__
#include "reorgpdf.xpm"
#endif

DECLARE_APP(edpdfApp);

BEGIN_EVENT_TABLE(EDFrame, wxFrame)
	EVT_MENU(edpdf_fm_new,EDFrame::fileMenu_New_clicked)
	EVT_MENU(edpdf_fm_open,EDFrame::fileMenu_Open_clicked)
	EVT_MENU(edpdf_fm_save,EDFrame::fileMenu_Save_clicked)
	EVT_MENU(edpdf_fm_saveas,EDFrame::fileMenu_Save_clicked)
	EVT_MENU(edpdf_fm_exit,EDFrame::fileMenu_Exit_clicked)
	EVT_MENU(edpdf_em_cut,EDFrame::editMenu_CutCopy_clicked)
	EVT_MENU(edpdf_em_copy,EDFrame::editMenu_CutCopy_clicked)
	EVT_MENU(edpdf_em_paste,EDFrame::editMenu_Paste_clicked)
	EVT_MENU(edpdf_em_delete,EDFrame::editMenu_Delete_clicked)
	EVT_MENU(edpdf_em_rotleft,EDFrame::editMenu_Rotate_clicked)
	EVT_MENU(edpdf_em_rotright,EDFrame::editMenu_Rotate_clicked)
	EVT_MENU(edpdf_em_rot180,EDFrame::editMenu_Rotate_clicked)
	EVT_MENU(edpdf_em_selall,EDFrame::editMenu_Select_clicked)
	EVT_MENU(edpdf_em_seleven,EDFrame::editMenu_Select_clicked)
	EVT_MENU(edpdf_em_selodd,EDFrame::editMenu_Select_clicked)
	EVT_MENU(edpdf_em_selspec,EDFrame::editMenu_Select_clicked)
	EVT_MENU(edpdf_em_inslast,EDFrame::editMenu_Insert_clicked)
	EVT_MENU(edpdf_em_insspec,EDFrame::editMenu_Insert_clicked)
	EVT_MENU(edpdf_tm_setting,EDFrame::toolMenu_Setting_clicked)
	EVT_MENU(edpdf_hm_about,EDFrame::helpMenu_About_clicked)
	EVT_TREE_BEGIN_DRAG(edpdf_orilist,EDFrame::ed_OriThumbnails_dragged)
	EVT_TREE_BEGIN_DRAG(edpdf_newlist,EDFrame::ed_NewThumbnails_dragged)
	EVT_TREE_END_DRAG(edpdf_newlist,EDFrame::ed_NewThumbnails_dragend)
	EVT_TREE_KEY_DOWN(edpdf_newlist,EDFrame::ed_NewThumbnails_keydown)
	EVT_TREE_ITEM_MENU(edpdf_newlist,EDFrame::ed_NewThumbnails_menu)
	EVT_TREE_ITEM_MENU(edpdf_orilist,EDFrame::ed_OriThumbnails_menu)
	EVT_LIST_ITEM_RIGHT_CLICK(edpdf_newinfo,EDFrame::ed_NewDocumentInfo_rightclicked)
	EVT_MENU(edpdf_origetthumb,EDFrame::ed_OriThumbnails_GetThumb_clicked)
	EVT_TREE_SEL_CHANGED(edpdf_orilist,EDFrame::ed_AnyThumbnails_selected)
	EVT_TREE_SEL_CHANGED(edpdf_newlist,EDFrame::ed_AnyThumbnails_selected)
	EVT_LIST_ITEM_ACTIVATED(edpdf_newinfo,EDFrame::ed_NewDocumentInfo_dblclicked)
	EVT_MENU(edpdf_useinfo,EDFrame::ed_NewDocMenu_UseInfo_clicked)
	EVT_MENU(edpdf_oriinsnew,EDFrame::editMenu_Insert_clicked)
END_EVENT_TABLE() 

EDFrame::EDFrame(const wxString &title)
:wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300,300),wxDEFAULT_FRAME_STYLE)
{
	this->SetClientSize(640,480);
	this->SetMinSize(wxSize(320,240));

	this->SetIcon(wxICON(reorgpdf));

	ed_MenuBar = new wxMenuBar;
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(edpdf_fm_new, _("New"));
	fileMenu->Append(edpdf_fm_open, StrF(_T("%s\tCtrl+O"),_("Open")));
	fileMenu->Append(edpdf_fm_save, StrF(_T("%s\tCtrl+S"),_("Save")));
	fileMenu->Append(edpdf_fm_saveas, _("Save as"));
	fileMenu->AppendSeparator();
	fileMenu->Append(edpdf_fm_exit, StrF(_T("%s\tAlt+X"),_("Exit")));
	wxMenu *editMenu = new wxMenu;
	editMenu->Append(edpdf_em_cut, StrF(_T("%s\tCtrl+X"),_("Cut")));
	editMenu->Append(edpdf_em_copy, StrF(_T("%s\tCtrl+C"),_("Copy")));
	editMenu->Append(edpdf_em_paste, StrF(_T("%s\tCtrl+V"),_("Paste")));
	editMenu->Append(edpdf_em_delete, StrF(_T("%s\tDelete"),_("Delete")));
		wxMenu *editRotate = new wxMenu;
		editRotate->Append(edpdf_em_rotleft, _("To left 90"));
		editRotate->Append(edpdf_em_rotright, _("To right 90"));
		editRotate->Append(edpdf_em_rot180, _("180 degrees"));
	editMenu->AppendSubMenu(editRotate, _("Rotate"));
		wxMenu *editSelection = new wxMenu;
		editSelection->AppendCheckItem(edpdf_em_seltype, _("On: Original, Off:New"));
		editSelection->Check(edpdf_em_seltype,true);
		editSelection->AppendSeparator();
		editSelection->Append(edpdf_em_selall,_("all pages"));
		editSelection->Append(edpdf_em_seleven, _("Even number pages"));
		editSelection->Append(edpdf_em_selodd, _("Odd number pages"));
		editSelection->Append(edpdf_em_selspec, _("Specified number pages"));
	editMenu->AppendSubMenu(editSelection, _("Selection"));
		wxMenu *editInsert = new wxMenu;
		editInsert->Append(edpdf_em_inslast, _("Last page"));
		editInsert->Append(edpdf_em_insspec, _("Specified page"));
	editMenu->AppendSubMenu(editInsert, _("Insert"));
	wxMenu *toolMenu = new wxMenu;
	toolMenu->Append(edpdf_tm_setting, _("Setting"));
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(edpdf_hm_help, _("Help"));
	helpMenu->Append(edpdf_hm_about, _("About"));

	ed_MenuBar->Append(fileMenu, _("&File(&F)"));
	ed_MenuBar->Append(editMenu, _("&Edit(&E)"));
	ed_MenuBar->Append(toolMenu, _("&Tool(&T)"));
	ed_MenuBar->Append(helpMenu, _("&Help(&H)"));
	SetMenuBar(ed_MenuBar);
	ed_MenuBar->Enable(edpdf_fm_save,false);
	ed_MenuBar->Enable(edpdf_fm_saveas,false);

	
	ed_StatusBar = CreateStatusBar();
	
	//OriImages = new wxImageList(120,120,true);
	ImgSize = wxGetApp().GetConfig().TranslateThumnailSize();
	Resolution = (int)wxGetApp().GetConfig().GetThumbnailSize();
	LoadIconImage(wxGetApp().GetConfig().GetUseThumbnail());
	
	ed_SplitWin = new wxSplitterWindow(this, wxID_ANY,wxDefaultPosition, wxDefaultSize,wxSP_3D | wxSP_LIVE_UPDATE |wxCLIP_CHILDREN);
	ed_SplitWin->SetMinimumPaneSize(180);
	setup_originalpage(ed_SplitWin);
	setup_newpage(ed_SplitWin);
	ed_SplitWin->SplitVertically(ed_LeftPanel,ed_RightPanel,320);
	ed_SplitWin->SetSashGravity(1.0);
	ed_SplitWin->SetSashPosition(wxGetApp().GetConfig().GetSashPosition());

	pdf = new wxPdfDocument;
	setdlg = new SettingDialog(this);
}
EDFrame::~EDFrame(void)
{
	/*if (pdf) {
		delete pdf;
	}*/
	setdlg->Close(true);
	ClosePDF();
	ed_OriThumbnails->DeleteChildren(ed_OriThumbnails->GetRootItem());
	ed_NewThumbnails->DeleteChildren(ed_NewThumbnails->GetRootItem());

	wxGetApp().SaveIni();
}
void EDFrame::setup_originalpage(wxSplitterWindow *parent)
{
	ed_LeftPanel = new wxPanel(parent,wxID_ANY);
	ed_OriDocumentInfo = new wxListCtrl(ed_LeftPanel, edpdf_oriinfo, wxPoint(-1,-1),wxSize(-1,130),wxLC_REPORT|wxLC_SINGLE_SEL);
	wxListItem itemcol;
	itemcol.SetText(_("Heading"));
	ed_OriDocumentInfo->InsertColumn(0,itemcol);
	ed_OriDocumentInfo->SetColumnWidth(0, 130);
	itemcol.SetText(_("Value"));
	ed_OriDocumentInfo->InsertColumn(1,itemcol);
	ed_OriDocumentInfo->SetColumnWidth(1, 190);
	ed_OriDocumentInfo->InsertItem(0,_("File name"));
	ed_OriDocumentInfo->InsertItem(1,_("Total page"));
	ed_OriDocumentInfo->InsertItem(2,_("Title"));
	ed_OriDocumentInfo->InsertItem(3,_("Subtitle"));
	ed_OriDocumentInfo->InsertItem(4,_("Author"));
	ed_OriDocumentInfo->InsertItem(5,_("Keyword"));
	ed_OriThumbnails = new wxTreeCtrl(ed_LeftPanel, edpdf_orilist, wxPoint(-1,-1),wxDefaultSize,wxTR_NO_BUTTONS|wxTR_MULTIPLE|wxTR_NO_LINES|wxTR_FULL_ROW_HIGHLIGHT);
	
	ed_OriThumbnails->SetImageList(OriImages);
	wxTreeItemId root = ed_OriThumbnails->AddRoot(_T("Original document"),-1);
	ed_OriThumbnails->ExpandAll();
	//ed_OriThumbnails->SetIndent(-5);

	wxBoxSizer *bsizer1 = new wxBoxSizer(wxVERTICAL);
	bsizer1->Add(ed_OriDocumentInfo,0,wxEXPAND|wxLEFT,0);
	bsizer1->Add(ed_OriThumbnails,1,wxEXPAND|wxTOP,5);
	ed_LeftPanel->SetSizer(bsizer1);
	ed_LeftPanel->Layout();
}
void EDFrame::setup_newpage(wxSplitterWindow *parent)
{
	ed_RightPanel = new wxPanel(parent,wxID_ANY);
	ed_NewDocumentInfo = new wxListCtrl(ed_RightPanel, edpdf_newinfo, wxPoint(-1,-1),wxSize(-1,130),wxLC_REPORT|wxLC_SINGLE_SEL);
	wxListItem itemcol;
	itemcol.SetText(_("Heading"));
	ed_NewDocumentInfo->InsertColumn(0,itemcol);
	ed_NewDocumentInfo->SetColumnWidth(0, 130);
	itemcol.SetText(_("Value"));
	ed_NewDocumentInfo->InsertColumn(1,itemcol);
	ed_NewDocumentInfo->SetColumnWidth(1, 190);
	ed_NewDocumentInfo->InsertItem(0,_("File name"));
	ed_NewDocumentInfo->InsertItem(1,_("Total page"));
	ed_NewDocumentInfo->InsertItem(2,_("Title"));
	ed_NewDocumentInfo->InsertItem(3,_("Subtitle"));
	ed_NewDocumentInfo->InsertItem(4,_("Author"));
	ed_NewDocumentInfo->InsertItem(5,_("Keyword"));
	ed_NewThumbnails = new wxTreeCtrl(ed_RightPanel, edpdf_newlist, wxPoint(-1,-1),wxDefaultSize,wxTR_NO_BUTTONS|wxTR_MULTIPLE|wxTR_NO_LINES|wxTR_FULL_ROW_HIGHLIGHT);
	ed_NewDocMenu = new wxMenu;
	ed_NewDocMenu->AppendCheckItem(edpdf_useinfo,_("Use infos of original document"));
	ed_NewDocMenu->Check(edpdf_useinfo,wxGetApp().GetConfig().GetUseOriginalInfo());
	
	ed_NewThumbnails->SetImageList(OriImages);
	wxTreeItemId root = ed_NewThumbnails->AddRoot(_T("New document"),-1);
	ed_NewThumbnails->SetDropTarget(new DDText(this,ed_OriThumbnails,ed_NewThumbnails));
	ed_NewThumbnails->ExpandAll();
	//ed_NewThumbnails->SetIndent(-5);


	wxBoxSizer *bsizer1 = new wxBoxSizer(wxVERTICAL);
	bsizer1->Add(ed_NewDocumentInfo,0,wxEXPAND|wxLEFT,0);
	bsizer1->Add(ed_NewThumbnails,1,wxEXPAND|wxTOP,5);
	ed_RightPanel->SetSizer(bsizer1);
	ed_RightPanel->Layout();
	
}
void EDFrame::fileMenu_New_clicked(wxCommandEvent &event)
{
	/*if (pdf) {
		delete pdf;
		pdf = new wxPdfDocument;
	}*/
	ClosePDF();
	ed_OriThumbnails->DeleteChildren(ed_OriThumbnails->GetRootItem());
	ed_NewThumbnails->DeleteChildren(ed_NewThumbnails->GetRootItem());
}
void EDFrame::fileMenu_Open_clicked(wxCommandEvent &event)
{
	wxString passmes = _("It is possible necessary password. Please input password.\nIf password is not necessary, input space.");
	wxString errmes = _("This PDF could not open by access permission or other");

	wxFileDialog dialog(this,_("Please choose a PDF file as source."),_T(""),_T(""),_("PDF file (*.pdf)|*.pdf;*.PDF;"));
	if (dialog.ShowModal() == wxID_OK) {
		ClosePDF();
		issecure = false;
		//try to open PDF without password
		if (OpenPDF(dialog.GetPath()) == 0) {
			wxPasswordEntryDialog passdlg(this,passmes);
			if (passdlg.ShowModal() == wxID_OK) {
				//try to open with password
				if (OpenPDF(dialog.GetPath(),passdlg.GetValue()) == 0) {
					wxMessageBox(errmes,APPTITLE,wxOK|wxICON_ERROR,this);
					return;
				}
				issecure = true;
			}
		}
		ed_MenuBar->Enable(edpdf_fm_save,true);
		ed_MenuBar->Enable(edpdf_fm_saveas,true);
		SetListInformation(srcpdfname,srcpdfnum,srcpdfinfo);
		/*ed_OriDocumentInfo->SetItem(0,1,wxFileName(srcpdfname).GetFullName());
		ed_OriDocumentInfo->SetItem(1,1,StrF(_T("%d pages"),srcpdfnum));
		ed_OriDocumentInfo->SetItem(2,1,srcpdfinfo.GetTitle());
		ed_OriDocumentInfo->SetItem(3,1,srcpdfinfo.GetSubject());
		ed_OriDocumentInfo->SetItem(4,1,srcpdfinfo.GetAuthor());
		ed_OriDocumentInfo->SetItem(5,1,srcpdfinfo.GetKeywords());

		ed_NewDocumentInfo->SetItem(1,1,StrF(_("%d pages"),0));
		if (ed_NewDocMenu->IsChecked(edpdf_useinfo)) {
			ed_NewDocumentInfo->SetItem(2,1,srcpdfinfo.GetTitle());
			ed_NewDocumentInfo->SetItem(3,1,srcpdfinfo.GetSubject());
			ed_NewDocumentInfo->SetItem(4,1,srcpdfinfo.GetAuthor());
			ed_NewDocumentInfo->SetItem(5,1,srcpdfinfo.GetKeywords());
		}*/
	}
}
void EDFrame::fileMenu_Save_clicked(wxCommandEvent &event)
{
	wxString mes = _("The original file takes a password. You can not overwrite save.");
	if (event.GetId() == edpdf_fm_save) {
		if (newdocDB.GetFileName().IsEmpty()) {
			newdocDB.SetFileName(srcpdfname);
			//option: Overwrite original file
			if (wxGetApp().GetConfig().GetOverwriteOriginal()) {
				//If the original file has password, no overwrite save.
				if (issecure) {
					wxMessageBox(mes,APPTITLE,wxOK|wxICON_ERROR,this);
				}else{
					SavePDF(srcpdfname);
					return;
				}
			}
		}else{
			SavePDF(newdocDB.GetFileName());
			return;
		}
	}
	wxFileDialog dialog(this,_("Please input file name for save"),_T(""),_T(""),_("PDF file (*.pdf)|*.pdf;*.PDF;"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (dialog.ShowModal() == wxID_OK) {
		newdocDB.SetFileName(dialog.GetPath());
		//option: Do backup on overwrite
		if (srcpdfname.Cmp(newdocDB.GetFileName()) == 0) {
			wxCopyFile(srcpdfname, newdocDB.GetFileName()+_T("~"));
		}
		SavePDF(dialog.GetPath());
		ed_NewDocumentInfo->SetItem(0,1,dialog.GetFilename());
	}
}
void EDFrame::fileMenu_Exit_clicked(wxCommandEvent &event)
{
	Close(true);
}
void EDFrame::editMenu_CutCopy_clicked(wxCommandEvent &event)
{
	clipbd.Clear();
	ed_NewThumbnails->GetSelections(clipbd);
	//exclude root
	for (int i = 0; i < (int)clipbd.GetCount(); i++) {
		if (clipbd.Item(i) == ed_NewThumbnails->GetRootItem()) {
			clipbd.RemoveAt(i);
			return;
		}
	}
	for (int i = 0; i < (int)clipbd.GetCount(); i++) {
		int page, src;
		wxString label = ed_NewThumbnails->GetItemText(clipbd.Item(i));
		GetPageNumber(label,&page,&src);
		clipbd_db.Add(newdocDB.GetPage(page-1));
	}
	if (event.GetId() == edpdf_em_cut) {
		clipcut = true;
	}else if (event.GetId() == edpdf_em_copy) {
		clipcut = false;
	}
}
void EDFrame::editMenu_Paste_clicked(wxCommandEvent &event)
{
	wxArrayTreeItemIds arr;
	if (ed_NewThumbnails->GetSelections(arr) > 1) {
		return;
	}
	wxString label;
	int imgid, page, src;
	wxTreeItemId posId = arr.Item(0);
	GetPageNumber(ed_NewThumbnails->GetItemText(posId),&page,&src);
	page--;
	wxTreeItemId root = ed_NewThumbnails->GetRootItem();
	for (int i = (int)clipbd.GetCount()-1; i >= 0; i--) {
		//AFTER posId(page)
		newdocDB.InsertAfterPage(clipbd_db.Item(i),page);
		label = ed_NewThumbnails->GetItemText(clipbd.Item(i));
		imgid = ed_NewThumbnails->GetItemImage(clipbd.Item(i));
		//AFTER posId
		ed_NewThumbnails->InsertItem(root,posId,newdocDB.GetPageString(page),imgid);
	}
	//refresh page label
	for (int i = page; i < (int)newdocDB.GetPagesCount(); i++) {
		wxTreeItemId id = GetTreeItem(ed_NewThumbnails,i);
		label = newdocDB.GetPageString(i);
		ed_NewThumbnails->SetItemText(id,label);
	}
	SetPageCount();
	clipbd.Clear();
	clipbd_db.Clear();
}
void EDFrame::editMenu_Delete_clicked(wxCommandEvent &event)
{
	if (this->FindFocus()->GetId() != edpdf_newlist) {
		return;
	}
	int ans = wxMessageBox(_("Do you really remove this page?"),APPTITLE,wxYES_NO|wxICON_QUESTION,this);
	if (ans == wxYES) {
		wxString label;
		wxArrayTreeItemIds arr;
		int page, src, i = 0;
		wxTreeItemId root = ed_NewThumbnails->GetRootItem();
		wxTreeItemId tmpitem;
		wxTreeItemIdValue cookie;
		ed_NewThumbnails->GetSelections(arr);
		for (int i = (int)arr.GetCount()-1; i >= 0; i--) {
			tmpitem = arr.Item(i);
			if ((tmpitem != ed_NewThumbnails->GetRootItem())) {
				//internal data
				GetPageNumber(ed_NewThumbnails->GetItemText(tmpitem),&page,&src);
				page--;
				if (page >= 0) newdocDB.RemovePage(page);
				//visible data
				ed_NewThumbnails->Delete(tmpitem);
			}
		}
		//refresh page label
		for (wxTreeItemId item = ed_NewThumbnails->GetFirstChild(root,cookie); item.IsOk(); item = ed_NewThumbnails->GetNextChild(root,cookie)) {
			//GetPageNumber(ed_NewThumbnails->GetItemText(item),&page,&src);
			//page--;
			//Msgbox(StrF(_T("page=%d\nsource=%d"),i,newdocDB.GetPage(i).GetSourcePage()),this);
			label = newdocDB.GetPageString(i);
			ed_NewThumbnails->SetItemText(item,label);
			i++;
		}
	}
	SetPageCount();
}
void EDFrame::editMenu_Rotate_clicked(wxCommandEvent &event)
{
	wxArrayTreeItemIds items;
	int cnt = (int)ed_NewThumbnails->GetSelections(items);
	if (cnt == 0) {
		wxMessageBox(_("No page choose!"),APPTITLE,wxOK|wxICON_EXCLAMATION,this);
		return;
	}
	wxString label;
	int page, src_page;
	double angle;
	wxTreeItemId root = ed_NewThumbnails->GetRootItem();
	for (int i = 0; i < (int)items.GetCount(); i++) {
		int imgid = ed_NewThumbnails->GetItemImage(items.Item(i));
		label = ed_NewThumbnails->GetItemText(items.Item(i));
		wxImage img = OriImages->GetBitmap(imgid).ConvertToImage();
		wxImage cp;
		switch (event.GetId()) {
		case edpdf_em_rotleft:
			cp = img.Rotate90(false);
			angle = 90.0;
			break;
		case edpdf_em_rotright:
			cp = img.Rotate90(true);
			angle = -90.0;
			break;
		case edpdf_em_rot180:
			cp = img.Rotate90(true);
			cp = cp.Rotate90(true);
			angle = 180.0;
			break;
		}
		//change DB
		GetPageNumber(label,&page,&src_page);
		db_pdfpage db = newdocDB.GetPage(page-1);
		db.SetRotate(true);
		db.SetRotateAngle(angle);
		newdocDB.SetPage(db,page-1);
		//change view
		int newindex = OriImages->Add(wxBitmap(cp));
		ed_NewThumbnails->SetItemImage(items.Item(i),newindex);
	}
}
void EDFrame::editMenu_Select_clicked(wxCommandEvent &event)
{
	wxTreeCtrl *tree;
	if (ed_MenuBar->IsChecked(edpdf_em_seltype)) {
		tree = ed_OriThumbnails;
	}else{
		tree = ed_NewThumbnails;
	}
	//execute selection
	tree->UnselectAll();
	wxTreeItemId root = tree->GetRootItem();
	switch (event.GetId()) {
	case edpdf_em_selall:
		for (int i = 0; i < (int)tree->GetChildrenCount(root); i++) {
			wxTreeItemId item = GetTreeItem(tree,i);
			if (item != root) {
				tree->SelectItem(item);
			}
		}
		break;
	case edpdf_em_seleven:
		for (int i = 0; i < (int)tree->GetChildrenCount(root); i++) {
			if (((i+1)%2) == 0) {
				wxTreeItemId item = GetTreeItem(tree,i);
				if (item != root) {
					tree->SelectItem(item);
				}
			}
		}
		break;
	case edpdf_em_selodd:
		for (int i = 0; i < (int)tree->GetChildrenCount(root); i++) {
			if (((i+1)%2) == 1) {
				wxTreeItemId item = GetTreeItem(tree,i);
				if (item != root) {
					tree->SelectItem(item);
				}
			}
		}
		break;
	case edpdf_em_selspec:
		{
			wxString mes = _("Please choose page number.\nExsample: 1,3,5 ...");
			wxString selstr = wxGetTextFromUser(mes,APPTITLE);
			if (!selstr.IsEmpty()) {
				wxStringTokenizer tkz(selstr,_T(","));
				while (tkz.HasMoreTokens()) {
					wxString token = tkz.GetNextToken();
					if (token.IsNumber()) {
						long i;
						token.ToLong(&i);
						wxTreeItemId item = GetTreeItem(tree,i-1);
						if (item != root) {
							tree->SelectItem(item);
						}
					}
				}
			}
		}
		break;
	}
}
void EDFrame::editMenu_Insert_clicked(wxCommandEvent &event)
{
	wxArrayTreeItemIds arr;
	wxTreeItemId root = ed_OriThumbnails->GetRootItem();
	int cnt = (int)ed_OriThumbnails->GetSelections(arr);
	wxArrayString pageStr;
	switch (event.GetId()) {
	case edpdf_em_inslast:
	case edpdf_oriinsnew:
		for (int i = 0; i < cnt; i++) {
			if (arr.Item(i) != root) {
				AppendNewPage(ed_OriThumbnails->GetItemText(arr.Item(i)),arr.Item(i));
			}
		}
		break;
	case edpdf_em_insspec:
		{
			for (int i = 0; i < (int)newdocDB.GetPagesCount(); i++) {
				pageStr.Add(StrF(_T("%d"),i+1));
			}
			wxSingleChoiceDialog dialog(this,_("Please choose page number as target"),APPTITLE,pageStr);
			if (dialog.ShowModal() == wxID_OK) {
				for (int i = cnt-1; i >= 0; i--) {
					if (arr.Item(i) != root) {
						AppendNewPage(ed_OriThumbnails->GetItemText(arr.Item(i)),arr.Item(i),dialog.GetSelection());
					}
				}
			}
		}
		break;
	}
}
void EDFrame::toolMenu_Setting_clicked(wxCommandEvent &event)
{
	db_setting olddb(wxGetApp().GetConfig());
	setdlg->BindData_Setting(wxGetApp().GetConfig());
	if (setdlg->ShowModal() == wxID_OK) {
		wxGetApp().SetConfig(setdlg->GetData_Setting());
	}
}
void EDFrame::helpMenu_About_clicked(wxCommandEvent &event)
{
	wxAboutDialogInfo info;
	info.SetName(APPTITLE);
	info.SetIcon(wxICON(reorgpdf));
	info.AddDeveloper(CREATOR);
	info.SetVersion(APPVER);
	info.SetDescription(_("This program is an editor to reorganize a PDF file."));
	info.SetCopyright(wxString::Format(_T("(C) 2008 %s < %s >"),CREATOR,EMAIL));

	wxAboutBox(info);
}
void EDFrame::ed_OriThumbnails_dragged(wxTreeEvent &event)
{
	wxString str;
	if (event.GetItem() == ed_OriThumbnails->GetRootItem()) return;
	wxArrayTreeItemIds arr;
	ed_OriThumbnails->GetSelections(arr);
	for (int i = 0; i < (int)arr.GetCount(); i++) {
		str += ed_OriThumbnails->GetItemText(arr.Item(i));
		if (i < (int)arr.GetCount()-1) {
			str.Append(_T(","));
		}
	}
	wxTextDataObject dragdata(str);
	wxDropSource dragsrc(dragdata,this);
	
	wxDragResult result = dragsrc.DoDragDrop(wxDrag_CopyOnly);
	
}
void EDFrame::ed_NewThumbnails_dragged(wxTreeEvent &event)
{
	if (event.GetItem() == ed_NewThumbnails->GetRootItem()) return;
	NewDragItem = event.GetItem();
	//event.Allow();
}
void EDFrame::ed_NewThumbnails_dragend(wxTreeEvent &event)
{
	if (NewDragItem.IsOk()) {
		if (event.GetItem() == NewDragItem) return;
		wxString label = ed_NewThumbnails->GetItemText(NewDragItem);
		int page, srcpage;
		GetPageNumber(label,&page,&srcpage);
		int image = ed_NewThumbnails->GetItemImage(NewDragItem);
		
		wxTreeItemId id = ed_NewThumbnails->GetPrevSibling(event.GetItem());
		wxTreeItemId eid = ed_NewThumbnails->GetNextSibling(event.GetItem());
		int pp, psp;
		db_pdfpage pdfpage = newdocDB.GetPage(page-1);
		//to bottom
		if (id == NewDragItem) {
			GetPageNumber(ed_NewThumbnails->GetItemText(event.GetItem()),&pp,&psp);
			newdocDB.InsertPage(pdfpage,pp-1);
			newdocDB.RemovePage(page-1);
		}else if (eid == NewDragItem) {
			if (id.IsOk()) {
				GetPageNumber(ed_NewThumbnails->GetItemText(id),&pp,&psp);
			}else{
				GetPageNumber(ed_NewThumbnails->GetItemText(event.GetItem()),&pp,&psp);
			}
			newdocDB.InsertPage(pdfpage,pp-1);
			newdocDB.RemovePage(page-1);
		}else{
			if (!id.IsOk()) {
				GetPageNumber(ed_NewThumbnails->GetItemText(event.GetItem()),&pp,&psp);
				newdocDB.InsertPage(pdfpage,pp-1);
				newdocDB.RemovePage(page-1);
			}else{
				GetPageNumber(ed_NewThumbnails->GetItemText(id),&pp,&psp);
				newdocDB.InsertPage(pdfpage,pp);
				newdocDB.RemovePage(page-1);
			}
		}
		ed_NewThumbnails->Freeze();
		ed_NewThumbnails->DeleteChildren(ed_NewThumbnails->GetRootItem());
		for (int i = 0; i < (int)newdocDB.GetPagesCount(); i++) {
			ed_NewThumbnails->AppendItem(ed_NewThumbnails->GetRootItem(),
				StrF(_T("%dP(%dP)"),i+1,newdocDB.GetPage(i).GetSourcePage()),image);
		}
		ed_NewThumbnails->Thaw();
	}
}
void EDFrame::ed_NewThumbnails_keydown(wxTreeEvent &event)
{
	switch (event.GetKeyEvent().GetUnicodeKey()) {
	case WXK_DELETE:
		break;
	}
}
void EDFrame::ed_OriThumbnails_menu(wxTreeEvent &event)
{
	wxMenu menu;
	menu.Append(edpdf_origetthumb,_("Get thumbnail"));
	if (wxGetApp().GetConfig().GetUseThumbnail()) {
		menu.Enable(edpdf_origetthumb,false);
	}
	menu.AppendSeparator();
	menu.Append(edpdf_oriinsnew,_("Insert to new document"));
	if (event.GetItem() == ed_OriThumbnails->GetRootItem()) {
		menu.Enable(edpdf_oriinsnew,false);
	}
	wxPoint clientpt = event.GetPoint();
	ed_OriThumbnails->PopupMenu(&menu,clientpt);

}
void EDFrame::ed_NewThumbnails_menu(wxTreeEvent &event)
{
	wxMenu menu, smenu1;
	menu.Append(edpdf_em_cut,_("Cut"));
	menu.Append(edpdf_em_copy,_("Copy"));
	menu.Append(edpdf_em_paste,_("Paste"));
	menu.Append(edpdf_em_delete,_("Delete"));
	menu.AppendSeparator();
	menu.Append(edpdf_em_rotleft,_("To left 90"));
	menu.Append(edpdf_em_rotright,_("To right 90"));
	menu.Append(edpdf_em_rot180,_("180 degrees"));

	wxPoint clientpt = event.GetPoint();
	ed_NewThumbnails->PopupMenu(&menu,clientpt);
}
void EDFrame::ed_NewDocumentInfo_rightclicked(wxListEvent &event)
{
	wxPoint clientpt = event.GetPoint();
	ed_NewDocumentInfo->PopupMenu(ed_NewDocMenu,clientpt);
}
void EDFrame::ed_OriThumbnails_GetThumb_clicked(wxCommandEvent &event)
{
	wxArrayTreeItemIds arrs;
	wxTreeItemId root = ed_OriThumbnails->GetRootItem();
	
	ed_OriThumbnails->GetSelections(arrs);
	for (int i = 0; i < (int)arrs.GetCount(); i++) {
		int page, src;
		if (arrs.Item(i) != root) {
			GetPageNumber(ed_NewThumbnails->GetItemText(arrs.Item(i)),&page,&src);
			GenerateThumbnail(srcpdfname,page,page,srcpdfpass);
			ed_OriThumbnails->SetItemImage(arrs.Item(i),OriImages->GetImageCount()-1);
		}
	}
}
void EDFrame::ed_AnyThumbnails_selected(wxTreeEvent &event)
{
	wxTreeCtrl *tree;
	bool flag[2];
	if (event.GetId() == edpdf_orilist) {
		tree = ed_OriThumbnails;
		flag[0] = false;
		flag[1] = true;
	}else if (event.GetId() == edpdf_newlist) {
		tree = ed_NewThumbnails;
		flag[0] = true;
		flag[1] = false;
	}else{
		return;
	}
	//toggle enable/disable
	for (int i = edpdf_em_cut; i < edpdf_em_seltype; i++) {
		ed_MenuBar->Enable(i,flag[0]);
	}
	ed_MenuBar->Enable(edpdf_em_inslast,flag[1]);
	ed_MenuBar->Enable(edpdf_em_insspec,flag[1]);
	//if focus && not root select
	wxArrayTreeItemIds arr;
	if (tree->GetSelections(arr) > 0) {
		for (int i = 0; i < (int)arr.GetCount(); i++) {
			if (arr.Item(i) == tree->GetRootItem()) {
				for (int i = edpdf_em_cut; i < edpdf_em_seltype; i++) {
					ed_MenuBar->Enable(i,false);
				}
				ed_MenuBar->Enable(edpdf_em_inslast,false);
				ed_MenuBar->Enable(edpdf_em_insspec,false);
				return;
			}
		}
	}
}
void EDFrame::ed_NewDocumentInfo_dblclicked(wxListEvent &event)
{
	if (ed_NewDocMenu->IsChecked(edpdf_useinfo)) {
		return;
	}
	if (event.GetIndex() <= 1) {
		wxMessageBox(_("You can not edit File name and Total page."),APPTITLE,wxOK|wxICON_EXCLAMATION,this);
		return;
	}
	wxListItem item;
	item.SetId(event.GetIndex());
	item.SetColumn(0);
	item.SetMask(wxLIST_MASK_TEXT);
	ed_NewDocumentInfo->GetItem(item);
	wxString label = ed_NewDocumentInfo->GetItemText(item);

	item.SetColumn(1);
	ed_NewDocumentInfo->GetItem(item);
	wxString defvalue = item.GetText();
	
	wxTextEntryDialog dialog(this,label,APPTITLE,defvalue);
	if (dialog.ShowModal() == wxID_OK) {
		ed_NewDocumentInfo->SetItem(event.GetIndex(),1,dialog.GetValue());
		switch (event.GetIndex()) {
		case 2: newpdfinfo.SetTitle(dialog.GetValue()); break;
		case 3: newpdfinfo.SetSubject(dialog.GetValue()); break;
		case 4: newpdfinfo.SetAuthor(dialog.GetValue()); break;
		case 5: newpdfinfo.SetKeywords(dialog.GetValue()); break;
		}
	}
}
void EDFrame::ed_NewDocMenu_UseInfo_clicked(wxCommandEvent &event)
{
	if (ed_NewDocMenu->IsChecked(edpdf_useinfo)) {
		//ed_NewDocumentInfo->SetItem(1,1,StrF(_("%d pages"),0));
		ed_NewDocumentInfo->SetItem(2,1,srcpdfinfo.GetTitle());
		ed_NewDocumentInfo->SetItem(3,1,srcpdfinfo.GetSubject());
		ed_NewDocumentInfo->SetItem(4,1,srcpdfinfo.GetAuthor());
		ed_NewDocumentInfo->SetItem(5,1,srcpdfinfo.GetKeywords());
		newpdfinfo = srcpdfinfo;
	}/*else{
		for (int i = 2; i < 6; i++) {
			ed_NewDocumentInfo->SetItem(i,1,wxEmptyString);
		}
		
	}*/
	wxGetApp().GetConfig().SetUseOriginalInfo(ed_NewDocMenu->IsChecked(edpdf_useinfo));
}
void EDFrame::LoadIconImage(bool loadreal)
{
	wxFileName fpath;
	//set image path
	fpath.AssignDir(wxGetApp().GetAppPath().GetResourcesDir());
#ifdef __WXMSW__
	fpath.AppendDir(wxGetApp().GetPathElement(P_SHARE));
	fpath.AppendDir(wxGetApp().GetPathElement(P_APPNAME));
#endif
	fpath.AppendDir(wxGetApp().GetPathElement(P_ICON));

	//set image
	OriImages = new wxImageList(ImgSize,ImgSize,true);

	if (!loadreal) {
		wxImage img1, img2;
		img1.LoadFile(wxFileName(fpath.GetFullPath(),_T("thumbv.bmp")).GetFullPath());
		img1.Resize(wxSize(ImgSize,ImgSize),wxPoint(12,0),255,255,255);
		OriImages->Add(wxBitmap(img1),*wxWHITE);
		img2.LoadFile(wxFileName(fpath.GetFullPath(),_T("thumbh.bmp")).GetFullPath());
		img2.Resize(wxSize(ImgSize,ImgSize),wxPoint(0,12),255,255,255);
		OriImages->Add(wxBitmap(img2),*wxWHITE);
	}
}
int EDFrame::OpenPDF(const wxString &name, const wxString &pass)
{
	wxLogNull logNo;
	srcpdfnum = pdf->SetSourceFile(name,pass);
	if (srcpdfnum > 0) {
		srcpdfname = name;
		srcpdfpass = pass;
		pdf->GetSourceInfo(srcpdfinfo);
		pdf->GetSourceInfo(newpdfinfo);
		//option: Use real thumbnails
		if (wxGetApp().GetConfig().GetUseThumbnail()) {
			OriImages->RemoveAll();
			GenerateThumbnail(name,1,srcpdfnum,pass);
		}
		EnumThumbnail(srcpdfnum,wxGetApp().GetConfig().GetUseThumbnail());

		newdocDB.Clear();
		/*if (pdf) {
			delete pdf;
			pdf = new wxPdfDocument;
		}*/
	}
	this->SetTitle(StrF(_T("%s - %s"),APPTITLE,wxFileName(srcpdfname).GetFullName().c_str()));
	return srcpdfnum;
}
int EDFrame::SavePDF(const wxString &name)
{
	int cnt = (int)newdocDB.GetPagesCount();
	int tpl;
	double pwid, phei;
	if (cnt == 0) {
		wxMessageBox(_("There is not a page in a new PDF file."),APPTITLE,wxOK|wxICON_ERROR,this);
		return 0;
	}
	//re-create PDF
	if (pdf) {
		delete pdf;
		//option: Default paper type
		if (wxGetApp().GetConfig().GetDefaultPaper() == wxPAPER_NONE) {
			pdf = new wxPdfDocument;
		}else{
			pdf = new wxPdfDocument(wxPORTRAIT,wxString(_T("mm")),wxGetApp().GetConfig().GetDefaultPaper());
		}
	}
	pwid = pdf->GetPageWidth();
	phei = pdf->GetPageHeight();
	pdf->SetSourceFile(srcpdfname,srcpdfpass);
	for (int i = 0; i < cnt; i++) {
		double x = 0.0,y = 0.0,w,h;
		db_pdfpage page = newdocDB.GetPage(i);
		tpl = pdf->ImportPage(page.GetSourcePage());
		pdf->GetTemplateBBox(tpl,x,y,w,h);
		//---enable page rotation
		if (page.IsRotate()) {
			int pageorient;
			//option: Default page orientation
			if (wxGetApp().GetConfig().GetDefaultPageOrientation() == wxAUTOORIENT) {
				if (page.GetOrientation() == wxPORTRAIT) {
					if (page.GetRotateAngle() != 180.0) {
						pageorient = wxLANDSCAPE;
					}else{
						pageorient = wxPORTRAIT;
					}
				}else{
					if (page.GetRotateAngle() != 180.0) {
						pageorient = wxPORTRAIT;
					}else{
						pdf->AddPage(wxLANDSCAPE);
					}
				}
			}else{
				pageorient = wxGetApp().GetConfig().GetDefaultPageOrientation();
			}
			pdf->AddPage(pageorient);
			pwid = pdf->GetPageWidth();
			phei = pdf->GetPageHeight();
			//pdf->SetDrawColor(0);
			pdf->StartTransform();
			double xdiv = 0, ydiv = 0;
			//---start roration of page
			if (page.GetRotateAngle() == 180.0) {
				pdf->Rotate(page.GetRotateAngle(),w/2.0,h/2.0);
			}else if (page.GetRotateAngle() == 90.0){//To left
				x = 0.0;
				y = w;
				pdf->Rotate(page.GetRotateAngle(),x,y);
			}else{//To right
				x = h;
				y = 0;
				pdf->Rotate(page.GetRotateAngle(),x,y);
			}
			//pdf->UseTemplate(tpl,x,y);
			//pdf->StopTransform();
		}else{
		//---nothing page rotation
			//option: Default page orientation
			switch (wxGetApp().GetConfig().GetDefaultPageOrientation()) {
			case wxAUTOORIENT:
				pdf->AddPage(page.GetOrientation());
				break;
			case wxPORTRAIT:	
				pdf->AddPage(wxPORTRAIT);
				break;
			case wxLANDSCAPE:	
				pdf->AddPage(wxLANDSCAPE);
				break;
			}
			pwid = pdf->GetPageWidth();
			phei = pdf->GetPageHeight();
			//option: Positionning of page -> Position
			switch (wxGetApp().GetConfig().GetPagePosition()) {
			case lefttop_psp:
				x = 0.0;
				y = 0.0;
				break;
			case lefttopwith_psp:
				x = (double)wxGetApp().GetConfig().GetPageMargin().x;
				y = (double)wxGetApp().GetConfig().GetPageMargin().y;
				break;
			case center_psp:
				if (pwid > w) {
					double difx = pwid / w;
					x = difx / 2.0;
				}
				if (phei > h) {
					double dify = phei / h;
					y = dify / 2.0;
				}
				break;
			default:
				x = 0.0;
				y = 0.0;
			}
			//get current page size
			pdf->StartTransform();
			//option: Fit in paper
			if (wxGetApp().GetConfig().GetFitPaper()) {
				double newwid, newhei;
				newwid = pwid / w;
				newhei = phei / h;
				//option: Fit with extraction
				if (wxGetApp().GetConfig().GetExtraction()) {
					pdf->Scale(newwid*100,newhei*100);
				}else{
					if (pwid < phei) {
						pdf->ScaleXY(newwid*100);
					}else{
						pdf->ScaleXY(newhei*100);
					}
				}
				//conflict with [option: Positionning of page]
				x = 0.0;
				y = 0.0;
			}
		}
		//option: Use frame around page
		if (wxGetApp().GetConfig().GetUseFrame()) {
			pdf->SetDrawColor(0);
			pdf->Rect(x,y,w,h,wxPDF_STYLE_DRAW);
		}
		pdf->UseTemplate(tpl,x,y,w,h);
		pdf->StopTransform();
	}
	//set PDF information
	pdf->SetTitle(newpdfinfo.GetTitle());
	pdf->SetSubject(newpdfinfo.GetSubject());
	pdf->SetAuthor(newpdfinfo.GetAuthor());
	pdf->SetKeywords(newpdfinfo.GetKeywords());
	pdf->SetCreator(newpdfinfo.GetCreator());
	pdf->SaveAsFile(name);
	SetStatusText(_("Save finished."),0);
	//option: Reload thumbnails after overwrite
	if (wxGetApp().GetConfig().GetReloadPDF()) {
		wxString tmpname, tmppass;
		wxString mes = _("Because an original file overwrites, and it was saved, I read it again and do it.");
		wxCommandEvent evt;
		if (newdocDB.GetFileName().Cmp(srcpdfname) == 0) {
			tmpname = newdocDB.GetFileName();
			tmppass = wxEmptyString;
			fileMenu_New_clicked(evt);
			wxMessageBox(mes,APPTITLE,wxOK|wxICON_INFORMATION,this);
			OpenPDF(tmpname,tmppass);
			SetListInformation(srcpdfname,srcpdfnum,srcpdfinfo);
		}
	}
	return cnt;
}
void EDFrame::ClosePDF(void)
{
	if (pdf) {
		delete pdf;
		pdf = new wxPdfDocument;
	}
	if (!wxGetApp().GetConfig().GetUseThumbnail()) {
		OriImages->RemoveAll();
		delete OriImages;
		LoadIconImage(wxGetApp().GetConfig().GetUseThumbnail());
	}
	if (!srcpdfname.IsEmpty()) {
		wxFileName rootname(wxGetApp().GetAppPath().GetTempDir(),BINTITLE);
		for (int i = 1; i < srcpdfnum+1; i++) {
			wxRemoveFile(StrF(_T("%s-%06d.ppm"),rootname.GetFullPath().c_str(),i));
		}
	}
	srcpdfnum = 0;
	srcpdfname.Clear();
	srcpdfpass.Clear();
	ed_MenuBar->Enable(edpdf_fm_save,false);
	ed_MenuBar->Enable(edpdf_fm_saveas,false);
	for (int i = 0; i < 6; i++) {
		ed_OriDocumentInfo->SetItem(i,1,wxEmptyString);
		ed_NewDocumentInfo->SetItem(i,1,wxEmptyString);
	}
	this->SetTitle(StrF(_T("%s"),APPTITLE));
}
void EDFrame::GenerateThumbnail(const wxString &name, int firstp, int lastp, const wxString &pass)
{
	if (srcpdfname.IsEmpty()) return;
	wxProgressDialog prog(APPTITLE,_("Now Loading..."),srcpdfnum,this,wxPD_AUTO_HIDE|wxPD_APP_MODAL|wxPD_CAN_ABORT);
	int curmeter = 1;
	int cp = firstp;
	wxFileName fpath,rootname;
	//set image path
	fpath.AssignDir(wxGetApp().GetAppPath().GetResourcesDir());
#ifdef __WXMSW__
	fpath.AppendDir(wxGetApp().GetPathElement(P_SHARE));
	fpath.AppendDir(wxGetApp().GetPathElement(P_APPNAME));
#endif
	fpath.AppendDir(wxGetApp().GetPathElement(P_ICON));
	
	rootname = wxFileName(wxGetApp().GetAppPath().GetTempDir(),BINTITLE);

	wxPopplerConvPPM *popp = new wxPopplerConvPPM();
	popp->SetTargetFile(name);
	popp->SetFileRootName(BINTITLE);
	popp->SetOutputDir(wxGetApp().GetAppPath().GetTempDir());
	popp->SetOwnerPassword(pass);
	popp->SetUserPassword(pass);
	popp->SetPageRange(firstp,lastp);
	popp->SetPageTotal(srcpdfnum);
	popp->SetResolution(Resolution);
	//OriImages->RemoveAll();
	if (popp->Prepare()) {
		while (cp <= lastp) {
			cp = popp->Output();
			if (!prog.Update(curmeter)) {
				break;
			}
			wxImage img;
			if (!img.LoadFile(popp->GetLastOutputPath())) {
				img.LoadFile(_T("thumbv.bmp"));
			}
			img.Resize(wxSize(ImgSize,ImgSize),wxPoint(0,0),255,255,255);
			OriImages->Add(wxBitmap(img),*wxWHITE);
			curmeter++;
		}
	}
	delete popp;
}
void EDFrame::EnumThumbnail(int pagesize, bool fromfile)
{
	if (srcpdfname.IsEmpty()) return;
	vector <int> pageno;
	for (int i = 0; i < pagesize; i++) {
		pageno.push_back(pdf->ImportPage(i+1));
	}

	vector <int>::iterator it;
	wxTreeItemId root = ed_OriThumbnails->GetRootItem();
	for (it = pageno.begin(); it != pageno.end(); it++) {
		double x,y,w,h;
		pdf->GetTemplateBBox(*it,x,y,w,h);
		//if not use real thumbnail, use dummy thumbnail
		if (!fromfile) {
			if (w > h) {
				ed_OriThumbnails->AppendItem(root,StrF(_T("%dP"),*it),1);
			}else{
				ed_OriThumbnails->AppendItem(root,StrF(_T("%dP"),*it),0);
			}
		}else{
			ed_OriThumbnails->AppendItem(root,StrF(_T("%dP"),*it),(*it)-1);
		}
	}
	ed_OriThumbnails->ExpandAll();
	SetStatusText(_("Finished to enumerate thumbnails."),0);
}
void EDFrame::GetPageNumber(const wxString &str, int *page, int *srcpage)
{
	int stq = str.Find(_T("("));
	wxString str_page = str.Mid(0,stq-1);
	wxString str_srcpage = str.Mid(stq+1,str.Len());
	str_srcpage.RemoveLast();
	str_srcpage.RemoveLast();
	
	long tmp[2];
	str_page.ToLong(&tmp[0]);
	str_srcpage.ToLong(&tmp[1]);
	*page = tmp[0];
	*srcpage = tmp[1];

}
wxTreeItemId EDFrame::GetTreeItem(wxTreeCtrl *tree, int index)
{
	wxTreeItemId root = tree->GetRootItem();
	int cnt = (int)tree->GetChildrenCount(root);
	wxTreeItemIdValue cookie;
	int i = 0;

	for (wxTreeItemId id = tree->GetFirstChild(root,cookie); id.IsOk(); id = tree->GetNextChild(root,cookie)) {
		if (i == index) {
			return id;
		}
		i++;
	}
	return root;
}
void EDFrame::SetPageCount(void)
{
	ed_NewDocumentInfo->SetItem(1,1,StrF(_("%d pages"),newdocDB.GetPagesCount()));
}
void EDFrame::AppendNewPage(const wxString &label, wxTreeItemId id, int pos)
{
	wxTreeItemId newroot = ed_NewThumbnails->GetRootItem();
	db_pdfpage page;
	long tmp;
	double x, y, w, h;
	wxString tmptoken = label;
	tmptoken = tmptoken.RemoveLast();
	tmptoken.ToLong(&tmp);
	page.SetSourcePage(tmp);
	pdf->GetTemplateBBox(tmp,x, y, w,h);
	if (w > h) {
		page.SetOrientation(wxLANDSCAPE);
	}else{
		page.SetOrientation(wxPORTRAIT);
	}
	if (pos < 0) {
		//add DB
		int p = newdocDB.AddPage(page);
		//add control
		ed_NewThumbnails->AppendItem(newroot,newdocDB.GetPageString(p),ed_OriThumbnails->GetItemImage(id));
	}else{
		//insert DB
		newdocDB.InsertPage(page,pos);
		//insert control
		ed_NewThumbnails->InsertItem(newroot,pos+1,newdocDB.GetPageString(pos-1),ed_OriThumbnails->GetItemImage(id));
	}

}
void EDFrame::SetListInformation(const wxString &name, int num, wxPdfInfo &info)
{
	ed_OriDocumentInfo->SetItem(0,1,wxFileName(name).GetFullName());
	ed_OriDocumentInfo->SetItem(1,1,StrF(_("%d pages"),num));
	ed_OriDocumentInfo->SetItem(2,1,info.GetTitle());
	ed_OriDocumentInfo->SetItem(3,1,info.GetSubject());
	ed_OriDocumentInfo->SetItem(4,1,info.GetAuthor());
	ed_OriDocumentInfo->SetItem(5,1,info.GetKeywords());

	ed_NewDocumentInfo->SetItem(1,1,StrF(_("%d pages"),0));
	if (ed_NewDocMenu->IsChecked(edpdf_useinfo)) {
		ed_NewDocumentInfo->SetItem(2,1,info.GetTitle());
		ed_NewDocumentInfo->SetItem(3,1,info.GetSubject());
		ed_NewDocumentInfo->SetItem(4,1,info.GetAuthor());
		ed_NewDocumentInfo->SetItem(5,1,info.GetKeywords());
	}
}

bool DDText::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxStringTokenizer tkz(data,_T(","));
	wxTreeItemId root = oldowner->GetRootItem();
	wxTreeItemId newroot = newowner->GetRootItem();
	wxTreeItemIdValue cookie;
	int newp = 1;
	
	while (tkz.HasMoreTokens()) {
		wxString token = tkz.GetNextToken();
		for (wxTreeItemId id = oldowner->GetFirstChild(root,cookie);
			 id.IsOk();
			 id = oldowner->GetNextChild(root,cookie)) {
			//wxString str;
			//str += token + _T("\n") + oldowner->GetItemText(id);
			if (token == oldowner->GetItemText(id)) {
				parent->AppendNewPage(token,id);
			}
		}
		newp++;
	}
	parent->SetPageCount();
	newowner->ExpandAll();
	return true;
}
