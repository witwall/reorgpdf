#ifndef __DEF_PDFDOC_H__
#define __DEF_PDFDOC_H__

#include <wx/wx.h>
#include <wx/pdfdoc.h>

#include "db_pdfpage.h"

WX_DECLARE_OBJARRAY(db_pdfpage, db_page_array);

class db_pdfdoc
{
public:
	db_pdfdoc(void);
	~db_pdfdoc(void) { };
	void Clear(void);
	wxString GetPageString(int i);

	//getter
	const wxString &GetFileName(void) { return filename; }
	db_page_array &GetPages(void) { return pages; }
	size_t GetPagesCount(void) { return pages.GetCount(); }
	db_pdfpage &GetPage(int i) { return pages.Item(i); }
	wxPaperSize GetPaperSize(void) { return docpaper; }
	int GetOrientation(void) { return orientation; }
	const wxString &GetTitle(void) { return doctitle; }
	const wxString &GetSubject(void) { return docsubject; }
	const wxString &GetAuthor(void) { return docauthor; }
	const wxString &GetKeyword(void) { return dockeyword; }
	const wxString &GetCreator(void) { return doccreator; }
	//setter
	void SetFileName(const wxString &name) { filename = name; }
	int AddPage(const db_pdfpage &p) { pages.Add(p); return (int)pages.GetCount()-1; }
	void InsertPage(const db_pdfpage &p, size_t n) { pages.Insert(p,n); }
	void InsertAfterPage(const db_pdfpage &p, size_t n);
	void RemovePage(size_t n) { pages.RemoveAt(n); }
	void SetPage(const db_pdfpage &p, size_t n);
	void SetPaperSize(wxPaperSize p) { docpaper = p; }
	void SetOrientation(int o);
	void SetTitle(const wxString &name) { doctitle = name; }
	void SetSubject(const wxString &name) { docsubject = name; }
	void SetAuthor(const wxString &name) { docauthor = name; }
	void SetKeyword(const wxString &name) { dockeyword = name; }
	void SetCreator(const wxString &name) { doccreator = name; }

private:
	wxString filename;
	db_page_array pages;
	wxPaperSize docpaper;
	int orientation;
	wxString doctitle;
	wxString docsubject;
	wxString docauthor;
	wxString dockeyword;
	wxString doccreator;
};

#endif