#ifndef __DEF_CONSTANT_H__
#define __DEF_CONSTANT_H__


#define APPTITLE _T("reorgPDF")

#define BINTITLE _T("reorgpdf")

#define APPVER _T("0.7.0")

#define CREATOR _T("ISHII Eiju")

#define EMAIL _T("lumidina@gmail.com")

#define StrF wxString::Format

#define Msgbox(MES,OBJ) (wxMessageBox(MES,APPTITLE,(long)wxOK,OBJ))

#define IntToStr(VAL) (wxString::Format(_T("%d"),VAL))

#define GetFocus(OBJ,EVT) (OBJ->FindFocus()->GetId() == EVT)

#define P_SHARE 0
#define P_DOC 1
#define P_LOCALE 2
#define P_APPNAME 3
#define P_ICON 4

#endif
