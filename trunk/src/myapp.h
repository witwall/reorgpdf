#ifndef __DEF_EDPDFAPP_H__
#define __DEF_EDPDFAPP_H__

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/imaglist.h>
#include "db_setting.h"

class edpdfApp: public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	wxImageList *sysicoList;
	
	wxStandardPaths &GetAppPath(void) { return paths; }
	db_setting &GetConfig(void) { return cfgdb; }
	void SetConfig(db_setting db) { cfgdb = db; }
	wxString GetPathElement(size_t num);
	void LoadIni(const wxString &path);
	void SaveIni(void);
	void LoadIconImage(const wxString &path, wxImageList *list, int ws, int hs);
protected:
	wxLocale locale;
	wxStandardPaths paths;
	db_setting cfgdb;
	wxArrayString pathstr;
	wxFileConfig *config;
	wxString inipath;
};

#endif
