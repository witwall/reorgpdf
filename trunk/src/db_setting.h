#ifndef __DEF_SETTING_H__
#define __DEF_SETTING_H__

#include <wx/wx.h>
#include "db_pdfpage.h"

#ifndef wxAUTOORIENT
#define wxAUTOORIENT 0
#endif

typedef enum {
	small_ts = 10,
	normal_ts = 15,
	big_ts = 20
} ThumbnailSize;
typedef enum {
	lefttop_psp = 0,
	lefttopwith_psp,
	center_psp
} PageSetPos;

class db_setting
{
public:
	db_setting(void);
	db_setting(const db_setting &db);
	~db_setting(void) { }
	db_setting &operator=(const db_setting &db);
	void Clear(void);

	//getter
	bool IsMaximized(void) { return winmaximized; }
	int GetSashPosition(void) { return sashpos; }
	bool GetUseOriginalInfo(void) { return useinfo; }

	bool GetUseThumbnail(void) { return usethumbnail; }
	int GetDefaultPageOrientation(void) { return defpageorient; }
	bool GetUseCollapseOriginal(void) { return usecollapse_inorg; }
	bool GetUseCollapseNew(void) { return usecollapse_innew; }
	int GetCollapseCountOriginal(void) { return collapsecnt_inorg; }
	int GetCollapseCountNew(void) { return collapsecnt_innew; }
	wxPaperSize GetDefaultPaper(void) { return defpapersize; }
	ThumbnailSize GetThumbnailSize(void) { return thumbsize; }
	PageSetPos GetPagePosition(void) { return pageposition; }
	wxPoint GetPageMargin(void) { return pagemargin; }
	bool GetUseFrame(void) { return useframe; }
	bool GetFitPaper(void) { return fitpaper; }
	bool GetExtraction(void) { return useextract; }
	bool GetUseBackup(void) { return usebackup; }
	bool GetReloadPDF(void) { return reloadpdf; }
	bool GetOverwriteOriginal(void) { return overwriteori; }
	int TranslateThumnailSize(void);
	//setter
	void SetMaximize(bool flag) { winmaximized = flag; }
	void SetSashPosition(int pos) { sashpos = pos; }
	void SetUseOriginalInfo(bool flag) { useinfo = flag; }

	void SetUseThumbnail(bool flag) { usethumbnail = flag; }
	void SetDefaultPageOrientation(int o) { defpageorient = o; }
	void SetUseCollapseOriginal(bool flag) { usecollapse_inorg = flag; }
	void SetUseCollapseNew(bool flag) { usecollapse_innew = flag; }
	void SetCollapseCountOriginal(int c) { collapsecnt_inorg = c; }
	void SetCollapseCountNew(int c) { collapsecnt_innew = c; }
	void SetDefaultPaper(wxPaperSize ps) { defpapersize = ps; }
	void SetThumbnailSize(ThumbnailSize ts) { thumbsize = ts; }
	void SetPagePosition(PageSetPos psp) { pageposition = psp; }
	void SetPageMargin(wxPoint pt) { pagemargin = pt; }
	void SetUseFrame(bool flag) { useframe = flag; }
	void SetFitPaper(bool flag) { fitpaper = flag; }
	void SetExtraction(bool flag) { useextract = flag; }
	void SetUseBackup(bool flag) { usebackup = flag; }
	void SetReloadPDF(bool flag) { reloadpdf = flag; }
	void SetOverwriteOriginal(bool flag) { overwriteori = flag; }
private:
	bool winmaximized;
	int sashpos;
	bool useinfo;

	bool usethumbnail;
	int defpageorient;
	bool usecollapse_inorg;
	bool usecollapse_innew;
	int collapsecnt_inorg;
	int collapsecnt_innew;
	wxPaperSize defpapersize;
	ThumbnailSize thumbsize;
	PageSetPos pageposition;
	wxPoint pagemargin;
	bool useframe;
	bool fitpaper;
	bool useextract;
	bool usebackup;
	bool reloadpdf;
	bool overwriteori;
};

#endif