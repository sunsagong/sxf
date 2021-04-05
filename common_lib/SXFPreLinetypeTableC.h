/**
	All Rights Reserved,(c) JACIC 2001
	SXFPreLineTypeTableC.h                ěŹúF2000.04.29

	PDNXTv
		ůč`üíe[uîńđÇˇé

	QDđ
	    đÔ    útŻ         SŇ		Tv
		ěŹ		2000.4.29     {(FQS)
**/
#ifndef _SXFPRELINETYPETABLEC_H_
#define _SXFPRELINETYPETABLEC_H_

#include <string.h>
#include <afxtempl.h>
#include "SXFDefaultPreLineTypeC.h"

typedef CArray<SXFDefaultPreLineTypeC, SXFDefaultPreLineTypeC&> SXFDEFAULTPRELINETYPE;

class SXFPreLineTypeTableC{  

public:
	SXFPreLineTypeTableC();
	~SXFPreLineTypeTableC();
	BOOL GetPreLinetype(int in_code, char*& out_name);
	int GetPreCode(const char* in_name);
private:
	SXFDEFAULTPRELINETYPE m_DefaultPreLineTypeTable;	/* FĂ°ĚŢŮ */
};

#endif // _SXFPRELINETYPETABLEC_H_
