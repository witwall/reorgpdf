#include "myapp.h"
#include "edpdf.h"
#include "appconstants.h"
#include "../config.h"

IMPLEMENT_APP(edpdfApp)

bool edpdfApp::OnInit()
{
	SetAppName(_T(PACKAGE_NAME));
	SetVendorName(CREATOR);
	
#ifdef __UNIX__
	wxString pdd(_T(PACKAGE_DATA_DIR));
	pdd.Replace(_T("/share"),_T(" "));
	pdd.Trim();
	paths.SetInstallPrefix(pdd);
#endif
	locale.Init(wxLANGUAGE_DEFAULT);
	locale.AddCatalogLookupPathPrefix(_T("."));
#ifdef __UNIX__
#ifdef _DEBUG
	locale.AddCatalogLookupPathPrefix(_T("./share/locale/"));
#endif
#endif
	locale.AddCatalogLookupPathPrefix(_T(PACKAGE_LOCALE_DIR));
	locale.AddCatalog(BINTITLE);
	wxInitAllImageHandlers();
	
	wxFileName fontpath[2];
	fontpath[0].AssignDir(paths.GetResourcesDir());
	fontpath[0].AppendDir(_T("fonts"));
	fontpath[1].AssignDir(paths.GetUserDataDir());
	fontpath[1].AppendDir(_T("fonts"));
	wxSetEnv(_T("WXPDF_FONTPATH"),fontpath[0].GetFullPath()+_T(";")+
		fontpath[1].GetFullPath());

	//initialize path string
	pathstr.Add(_T("share"));
	pathstr.Add(_T("doc"));
	pathstr.Add(_T("locale"));
	pathstr.Add(BINTITLE);
	pathstr.Add(_T("icon"));

	//read initial setting file
	wxString filename = StrF(_T("%s.ini"),APPTITLE);
	wxFileName fullpath(paths.GetUserDataDir(),filename);
	if (!wxFileName::DirExists(fullpath.GetPath())) {
		wxFileName::Mkdir(fullpath.GetPath());
	}
	LoadIni(fullpath.GetFullPath());
	//create main window
	EDFrame *frame = new EDFrame(APPTITLE);
	SetTopWindow(frame);
	frame->Centre();
	frame->Maximize(cfgdb.IsMaximized());
	frame->Show(true);
	
	return true;
}
int edpdfApp::OnExit()
{
	return 0;
}
wxString edpdfApp::GetPathElement(size_t num)
{
	if (num >= pathstr.GetCount()) {
		return wxEmptyString;
	}else{
		return pathstr.Item(num);
	}
}
void edpdfApp::LoadIni(const wxString &path)
{
	if (!wxFileName::FileExists(path)) {
		wxFile fl;
		fl.Create(path.c_str());
		fl.Close();
	}
	wxFileInputStream fis(path);
	config = new wxFileConfig(fis,wxConvUTF8);
	inipath = path;
	
	bool flag;
	
	config->Read(_T("/common/windowmaximized"),&flag,false);
	cfgdb.SetMaximize(flag);
	cfgdb.SetSashPosition(config->Read(_T("/common/sashposition"),(long)0));
	config->Read(_T("/common/useoriginalinfo"),&flag,false);
	cfgdb.SetUseOriginalInfo(flag);
	//file tab
	cfgdb.SetDefaultPaper((wxPaperSize)config->Read(_T("/file/DefaultPaper"),(long)3));
	cfgdb.SetPagePosition((PageSetPos)config->Read(_T("/file/PagePosition"),(long)lefttop_psp));
	wxPoint pos;
	pos.x = config->Read(_T("/file/PageMarginLeft"),(long)0);
	pos.y = config->Read(_T("/file/PageMarginTop"),(long)0);
	cfgdb.SetPageMargin(pos);
	config->Read(_T("/file/UseFrame"),&flag,false);
	cfgdb.SetUseFrame(flag);
	config->Read(_T("/file/FitPaper"),&flag,false);
	cfgdb.SetFitPaper(flag);
	config->Read(_T("/file/Extraction"),&flag,false);
	cfgdb.SetExtraction(flag);
	config->Read(_T("/file/UseBackup"),&flag,false);
	cfgdb.SetUseBackup(flag);
	config->Read(_T("/file/OverwriteOriginal"),&flag,false);
	cfgdb.SetOverwriteOriginal(flag);
	//view tab
	config->Read(_T("/view/UseThumbnail"),&flag,true);
	cfgdb.SetUseThumbnail(flag);
	cfgdb.SetDefaultPageOrientation(config->Read(_T("/view/DefaultPageOrientation"),(long)0));
	cfgdb.SetThumbnailSize((ThumbnailSize)config->Read(_T("/view/ThumbnailSize"),(long)10));
	config->Read(_T("/view/ReloadPDF"),&flag,false);
	cfgdb.SetReloadPDF(flag);

}
void edpdfApp::SaveIni(void)
{
	wxFileOutputStream fos(inipath);
	EDFrame *frame = (EDFrame*)GetTopWindow();
	config->Write(_T("/common/windowmaximized"),frame->IsMaximized());
	config->Write(_T("/common/sashposition"),(long)frame->GetSplitPos());
	config->Write(_T("/common/useoriginalinfo"),cfgdb.GetUseOriginalInfo());
	//file tab
	config->Write(_T("/file/DefaultPaper"),(long)cfgdb.GetDefaultPaper());
	config->Write(_T("/file/PagePosition"),(long)cfgdb.GetPagePosition());
	config->Write(_T("/file/PageMarginLeft"),cfgdb.GetPageMargin().x);
	config->Write(_T("/file/PageMarginTop"),cfgdb.GetPageMargin().y);
	config->Write(_T("/file/UseFrame"),cfgdb.GetUseFrame());
	config->Write(_T("/file/FitPaper"),cfgdb.GetFitPaper());
	config->Write(_T("/file/Extraction"),cfgdb.GetExtraction());
	config->Write(_T("/file/UseBackup"),cfgdb.GetUseBackup());
	config->Write(_T("/file/OverwriteOriginal"),cfgdb.GetOverwriteOriginal());
	//view tab
	config->Write(_T("/view/UseThumbnail"),cfgdb.GetUseThumbnail());
	config->Write(_T("/view/DefaultPageOrientation"),cfgdb.GetDefaultPageOrientation());
	config->Write(_T("/view/ThumbnailSize"),(long)cfgdb.GetThumbnailSize());
	config->Write(_T("/view/ReloadPDF"),cfgdb.GetReloadPDF());
	
	config->Save(fos);
	delete config;
}
