#ifndef __DEF_WXPOPPLER_H__
#define __DEF_WXPOPPLER_H__

#include <wx/wx.h>
//#include "config.h"
#include <poppler/poppler-config.h>
#include <stdio.h>
#include <math.h>
//#include "parseargs.h"
#include "goo/gmem.h"
#include "goo/GooString.h"
#include "GlobalParams.h"
#include "Object.h"
#include "PDFDoc.h"
#include "splash/SplashBitmap.h"
#include "splash/Splash.h"
#include "SplashOutputDev.h"


bool WxToGString(GooString *src, const wxString &str);
wxString GStringToWx(GooString *str);
GBool WxToGBool(bool flag);
bool GBoolToWx(GBool flag);


class wxPopplerConvPPM
{
public:
	wxPopplerConvPPM(void);
	wxPopplerConvPPM(const wxString &name, const wxString &usr, const wxString &own);
	~wxPopplerConvPPM(void);
	
	//Specifies range of pages
	void SetPageRange(int begin, int end = -1);
	//Specifies resolution, in DPI.
	void SetResolution(int res = 150);
	//Specifies PDF-file
	void SetTargetFile(const wxString &name) { fileName = name; }
	//Specifies root name of file for output
	void SetFileRootName(const wxString &name) { rootName = name; }
	//Enable  or  disable  FreeType
	void SetFreeType(bool flag) { enableFreeType = flag; }
	//Enable or disable font anti-aliasing.
	void SetAntiAlias(bool flag) { enableAntiAlias = flag; }
	//Enable or disable vector anti-aliasing.
	void SetVectorAntiAlias(bool flag) { enableVector = flag; }
	//Specify the user password for the PDF file.
	void SetUserPassword(const wxString &pass) { userPasswd = pass; }
	//Specify the owner password for the  PDF  file.
	void SetOwnerPassword(const wxString &pass) { ownerPasswd = pass; }
	void SetPageTotal(int last) { srcpdf_lastpage = last; }
	//Specify output directory of pages images
	void SetOutputDir(const wxString &name) { outputDir = name; }

	int GetFirstPage(void) { return firstpage; }
	int GetLastPage(void) { return lastpage; }
	int GetResolution(void) { return resolution; }
	//Get last outputted file path
	wxString GetLastOutputPath(void) { return lastpath; }
	//Get rotated value of specified page
	int GetRotatedValue(int page);
	//Get width of specified page
	double GetPageWidth(int page);
	//Get height of specified page
	double GetPageHeight(int page);
	//Get paper type of page
	wxPaperSize GetPagePaperSize(int page);
	
	//Do prepare for output
	bool Prepare(void);
	//Output one page of PDF-file as Portable Pixmap(PPM)
	int Output(void);
	//Output all pages of PDF-file as Portable Pixmap(PPM)
	int OutputAll(void);
	void Close(void);
	bool IsOk(void) { return (doc->isOk()) ? true : false; }
public:
	enum PPMColor {
		full = 0, mono, gray
	};
protected:
	void Initialize(void);
	void InitPageRange(void);
	bool AssignFile(void);
private:
	int firstpage;
	int lastpage;
	int resolution;
	PPMColor color;
	bool enableFreeType;
	bool enableAntiAlias;
	bool enableVector;
	wxString ownerPasswd;
	wxString userPasswd;
	wxString fileName;
	wxString rootName;
	wxString outputDir;

	PDFDoc *doc;
	SplashColor paperColor;
	SplashOutputDev *splashOut;

	wxArrayString errors;
	bool isopen;
	bool isready;
	int curpage;
	wxString lastpath;
	int srcpdf_lastpage;
};

#endif