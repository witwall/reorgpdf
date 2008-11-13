#include "wxpopp_ppm.h"
#include <wx/filename.h>
#include <wx/paper.h>


wxPopplerConvPPM::wxPopplerConvPPM(void)
{
	Initialize();
	if (!globalParams) {
		globalParams = new GlobalParams();
	}
	globalParams->setErrQuiet(gTrue);
#ifdef _MSC_VER
	//globalParams->setupBaseFonts(NULL);
#endif
	doc = (PDFDoc*)NULL;
}
wxPopplerConvPPM::wxPopplerConvPPM(const wxString &name, const wxString &usr, const wxString &own)
{
	Initialize();
	if (!globalParams) {
		globalParams = new GlobalParams();
	}
	SetTargetFile(name);
	SetUserPassword(usr);
	SetOwnerPassword(own);
	
	isopen = AssignFile();
}
wxPopplerConvPPM::~wxPopplerConvPPM(void)
{
	//clean up
	//if (doc) delete doc;
	if (splashOut)	delete splashOut;
	//delete globalParams;
	//check for memory leaks
	//Object::memCheck(stderr);
	//gMemReport(stderr);
}
void wxPopplerConvPPM::SetPageRange(int begin, int end)
{
	if (begin < 0) {
		firstpage = 1;
	}else{
		firstpage = begin;
	}
	if (end < -1) {
		lastpage = -1;
	}else{
		lastpage = end;
	}
}
void wxPopplerConvPPM::SetResolution(int res)
{
	if (res < 0) {
		resolution = 1;
	}else{
		resolution = res;
	}
}
bool wxPopplerConvPPM::Prepare(void)
{
	if (isready) return true;
	if (!isopen) {
		isopen = AssignFile();
		if (!isopen) return false;
	}
	//get page range
	InitPageRange();
	//setup splash
	switch (color) {
	case full:
		splashOut = new SplashOutputDev(splashModeRGB8, 1, gFalse, paperColor);
		break;
	case mono:
		splashOut = new SplashOutputDev(splashModeMono1, 1, gFalse, paperColor);
		break;
	case gray:
		splashOut = new SplashOutputDev(splashModeMono8, 1, gFalse, paperColor);
		break;
	}
	splashOut->startDoc(doc->getXRef());
	curpage = firstpage;
	isready = true;
	return true;
}
int wxPopplerConvPPM::Output(void)
{
	if (!isready) return -1;
	if (!isopen) {
		isopen = AssignFile();
		if (!isopen) return -1;
	}
	//output one page
	doc->displayPage(splashOut, curpage, resolution, resolution, 0,
		gFalse, gTrue, gFalse);
	wxString fname, extname;
	switch (color) {
	case full:
		extname = _T("ppm"); break;
	case mono:
		extname = _T("pbm"); break;
	case gray:
		extname = _T("pgm"); break;
	}
	
	fname.Printf(_T("%s-%06d.%s"),rootName.c_str(),curpage,extname.c_str());
	lastpath = wxFileName(outputDir,fname).GetFullPath();
	wxCharBuffer cfile = wxConvUTF8.cWC2MB(lastpath.wc_str(*wxConvCurrent));
	splashOut->getBitmap()->writePNMFile(cfile.data());
	return ++curpage;
}
int wxPopplerConvPPM::OutputAll(void)
{
	if (!isready) return -1;
	if (!isopen) {
		isopen = AssignFile();
		if (!isopen) return false;
	}
	//output one page
	wxString fname, extname;
	switch (color) {
	case full:
		extname = _T("ppm"); break;
	case mono:
		extname = _T("pbm"); break;
	case gray:
		extname = _T("pgm"); break;
	}
	for (curpage = firstpage; curpage <= lastpage; curpage++) {
		doc->displayPage(splashOut, curpage, resolution, resolution, 0,
			gFalse, gTrue, gFalse);
		fname.Printf(_T("%s-%06d.%s"),rootName.c_str(),curpage,extname.c_str());
		wxCharBuffer cfile = wxConvUTF8.cWC2MB(fname.wc_str(*wxConvCurrent));
		splashOut->getBitmap()->writePNMFile(cfile.data());
	}
	return curpage;
}
void wxPopplerConvPPM::Initialize(void)
{
	firstpage = 1;
	lastpage = -1;
	resolution = 150;
	color = full;
	enableFreeType = true;
	enableAntiAlias = true;
	enableVector = true;
	ownerPasswd.Clear();
	userPasswd.Clear();
	fileName.Clear();
	rootName.Clear();
	paperColor[0] = 255;
	paperColor[1] = 255;
	paperColor[2] = 255;
	errors.Clear();
	isopen = false;
	isready = false;
	curpage = 0;
	srcpdf_lastpage = -1;

}
void wxPopplerConvPPM::InitPageRange(void)
{
	if (isopen) {
		if ((lastpage < 1) || (lastpage > srcpdf_lastpage)) {
			lastpage = srcpdf_lastpage;
		}
	}
}
bool wxPopplerConvPPM::AssignFile(void)
{
	GooString *goofile, *goousr, *gooown;
	wxCharBuffer cfile = wxConvLocal.cWC2MB(fileName.wc_str(*wxConvCurrent));
	wxCharBuffer cusr = wxConvLocal.cWC2MB(userPasswd.wc_str(*wxConvCurrent));
	wxCharBuffer cown = wxConvLocal.cWC2MB(ownerPasswd.wc_str(*wxConvCurrent));
	if (fileName.IsEmpty()) {
		return false;
	}else{
		goofile = new GooString(cfile);
	}
	if (userPasswd.IsEmpty()) {
		goousr = NULL;
	}else{
		goousr = new GooString(cusr);
	}
	if (ownerPasswd.IsEmpty()) {
		gooown = NULL;
	}else{
		gooown = new GooString(cown);
	}
	doc = new PDFDoc(goofile, goousr, gooown);
	if (goofile) delete goofile;
	if (goousr) delete goousr;
	if (gooown) delete gooown;
	isopen = true;
	return true;
}
void wxPopplerConvPPM::Close(void)
{
	Initialize();
	if (doc->isOk()) delete doc;
}
int wxPopplerConvPPM::GetRotatedValue(int page)
{
	if (!isready) return -1;
	if (page < 1) return -1;
	return doc->getPageRotate(page);
}
double wxPopplerConvPPM::GetPageWidth(int page)
{
	if (!isready) return 0.0;
	if (page < 1) return 0.0;
	return doc->getPageMediaWidth(page);
}
double wxPopplerConvPPM::GetPageHeight(int page)
{
	if (!isready) return 0.0;
	if (page < 1) return 0.0;
	return doc->getPageMediaHeight(page);
}
//Get paper type of page
wxPaperSize wxPopplerConvPPM::GetPagePaperSize(int page)
{
	if (!isready) return wxPAPER_NONE;
	if (page < 1) return wxPAPER_NONE;
	// Convert to wxPaperSize
	wxSize ps;
	double scalef = 254.0 / 72.0;
	// Change size from point to real size (mm)
	ps.SetWidth((int)(GetPageWidth(page) * scalef));
	ps.SetHeight((int)(GetPageHeight(page) * scalef));
	// Page format
	wxPrintPaperDatabase* printPaperDatabase = new wxPrintPaperDatabase;
	printPaperDatabase->CreateDatabase();
	wxPrintPaperType* paperType = printPaperDatabase->FindPaperType(ps);
	if (paperType == NULL) {
		return wxPAPER_NONE;
	}
	return paperType->GetId();
}

