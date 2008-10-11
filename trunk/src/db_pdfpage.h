#ifndef __DEF_PDFPAGE_H__
#define __DEF_PDFPAGE_H__

#include <wx/wx.h>

typedef enum {
	page_rt = 0,
	content_rt
} RotateType;

class db_pdfpage
{
public:
	db_pdfpage(void)
	{
		page = 0;
		srcpage = 0;
		isrotate = false;
		rotangle = 0.0;
	}
	db_pdfpage(int src, int p)
	{
		page = p;
		srcpage = src;
		isrotate = false;
		rotangle = 0.0;
	}
	~db_pdfpage(void) { };

	//getter
	int GetPage(void) { return page; }
	int GetSourcePage(void) { return srcpage; }
	bool IsRotate(void) { return isrotate; }
	double GetRotateAngle(void) { return rotangle; }
	RotateType GetRotateType(void) { return rottype; }
	int GetOrientation(void) { return orientation; }
	wxString GetPageString(int i);
	//setter
	void SetPage(int p) { page = p; }
	void SetSourcePage(int p) { srcpage = p; }
	void SetRotate(bool flag) { isrotate = flag; }
	void SetRotateAngle(double a) { rotangle = a; }
	void SetRotateType(RotateType rt) { rottype = rt; }
	void SetOrientation(int o);
private:
	int page;
	int srcpage;
	bool isrotate;
	double rotangle;
	RotateType rottype;
	int orientation;

};

#endif