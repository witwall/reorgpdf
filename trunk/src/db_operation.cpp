#include "db_pdfdoc.h"
#include "db_setting.h"
#include "appconstants.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(db_page_array);

db_pdfdoc::db_pdfdoc(void)
{
	Clear();
}
void db_pdfdoc::Clear(void)
{
	filename.Clear();
	pages.Clear();
	docpaper = wxPAPER_A4;
	orientation = wxPORTRAIT;
	doctitle.Clear();
	docsubject.Clear();
	docauthor.Clear();
	dockeyword.Clear();
	doccreator.Clear();
}
void db_pdfdoc::SetPage(const db_pdfpage &p, size_t n)
{
	pages.Insert(p,n);
	pages.RemoveAt(n+1);
}
void db_pdfdoc::SetOrientation(int o)
{
	if ((o == wxLANDSCAPE) || (o == wxPORTRAIT)) {
		orientation = o;
	}else{
		orientation = wxPORTRAIT;
	}
}
wxString db_pdfdoc::GetPageString(int i)
{
	wxString label;
	if (i > (int)GetPagesCount()-1) {
		return wxEmptyString;
	}
	label.Printf(_T("%dP(%dP)"),i+1,GetPage(i).GetSourcePage());
	return label;
}
void db_pdfdoc::InsertAfterPage(const db_pdfpage &p, size_t n)
{
	if ((n+1) >= pages.GetCount()) {
		pages.Add(p);
	}else{
		pages.Insert(p,n+1);
	}
}

void db_pdfpage::SetOrientation(int o)
{
	if ((o == wxLANDSCAPE) || (o == wxPORTRAIT)) {
		orientation = o;
	}else{
		orientation = wxPORTRAIT;
	}
}
wxString db_pdfpage::GetPageString(int i)
{
	wxString label;
	label.Printf(_T("%dP(%dP)"),page,srcpage);
	return label;
}

db_setting::db_setting(void)
{
	Clear();
}
db_setting::db_setting(const db_setting &db)
{
	winmaximized = db.winmaximized;
	sashpos = db.sashpos;
	useinfo = db.useinfo;

	usethumbnail = db.usethumbnail;
	defpageorient = db.defpageorient;
	usecollapse_inorg = db.usecollapse_inorg;
	usecollapse_innew = db.usecollapse_innew;
	collapsecnt_inorg = db.collapsecnt_inorg;
	collapsecnt_innew = db.collapsecnt_innew;
	defpapersize = db.defpapersize;
	thumbsize = db.thumbsize;
	pageposition = db.pageposition;
	pagemargin = db.pagemargin;
	useframe = db.useframe;
	fitpaper = db.fitpaper;
	useextract = db.useextract;
	usebackup = db.usebackup;
	reloadpdf = db.reloadpdf;
	overwriteori = db.overwriteori;
}
db_setting &db_setting::operator=(const db_setting &db)
{
	if (this == &db) return *this;
	winmaximized = db.winmaximized;
	sashpos = db.sashpos;
	useinfo = db.useinfo;

	usethumbnail = db.usethumbnail;
	defpageorient = db.defpageorient;
	usecollapse_inorg = db.usecollapse_inorg;
	usecollapse_innew = db.usecollapse_innew;
	collapsecnt_inorg = db.collapsecnt_inorg;
	collapsecnt_innew = db.collapsecnt_innew;
	defpapersize = db.defpapersize;
	thumbsize = db.thumbsize;
	pageposition = db.pageposition;
	pagemargin = db.pagemargin;
	useframe = db.useframe;
	fitpaper = db.fitpaper;
	useextract = db.useextract;
	usebackup = db.usebackup;
	reloadpdf = db.reloadpdf;
	overwriteori = db.overwriteori;
	return *this;
}
void db_setting::Clear(void)
{
	usethumbnail = true;
	defpageorient = wxAUTOORIENT;
	usecollapse_inorg = false;
	usecollapse_innew = false;
	collapsecnt_inorg = 0;
	collapsecnt_innew = 0;
	defpapersize = wxPAPER_A4;
	thumbsize = small_ts;
	pageposition = lefttop_psp;
	pagemargin = wxPoint(0,0);
	useframe = false;
	fitpaper = false;
	useextract = false;
	usebackup = false;
	reloadpdf = false;
	overwriteori = false;

	winmaximized = false;
	sashpos = 512;
	useinfo = true;
}
int db_setting::TranslateThumnailSize(void)
{
	switch (thumbsize) {
	case small_ts:
		return 120;
	case normal_ts:
		return 180;
	case big_ts:
		return 240;
	default:
		return 120;
	}
}