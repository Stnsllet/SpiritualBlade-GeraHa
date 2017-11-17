// TAG.h: TAG クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAG_H__35513B5E_A6D0_4F59_95F2_C70D61D61A15__INCLUDED_)
#define AFX_TAG_H__35513B5E_A6D0_4F59_95F2_C70D61D61A15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct TAG  
{
public:
	static CComBSTR LEV, HP, MP, ATC, DEF, MAG, SPD, MOV;
	static CComBSTR NAME, JOB, PASON, KILL;
	static CComBSTR WEP, BODY, ACS;
	static CComBSTR X, Y, Z, W, H, PX, PY, A, DIR;
	static CComBSTR ON, OFF, FLG, UNFLG;
	static CComBSTR MSG, STR, HELP, TYPE;
	static CComBSTR POW, COUNT, RANGE;
	static CComBSTR PARAMPOS, NO, FORM, ARMY;
	static CComBSTR MEMBER, PLANE, FILE, PATH, SRC;

};

#endif // !defined(AFX_TAG_H__35513B5E_A6D0_4F59_95F2_C70D61D61A15__INCLUDED_)
