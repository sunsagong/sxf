//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFFeatureC.h                         ì¬úF2000.04.17
//
//	PDNXTv
//      êÊvftB[`[f[^ðÛ·é     
//
//	QDð
//	    ðÔ	út¯			SÒ			Tv
//      ì¬		2000.04.17		ü
//		XV		2015.04.27		T.Matsushima	JPCERT/CC wE Æã«Î
//
//------------------------------------------------------------
#ifndef _SXFFEATUREC_H_
#define _SXFFEATUREC_H_

#include <afx.h>
#include "SXFDataStructC.h"

class SXFInstanceIDC;

class SXFFeatureC  
{
public:
	void SetReturnFlag();
	SXFFeatureC();
	SXFFeatureC(SXFInstanceIDC* IN_InstanceIDManager);
	virtual ~SXFFeatureC();
	//read
    BOOL SetStructData(INSTANCEDATA* IN_Feature);
	LPVOID GetAssemblyStructData();
//	LPVOID GetStructData(char*& OUT_Type);					JPCERT/CC wE Æã«Î	T.Matsushima	2015.04.27
	LPVOID GetStructData(char*& OUT_Type,size_t TypeSize);	// obt@TCYðn·æ¤ÉÏX
	//write
    void SetFeatureData(LPCTSTR IN_TypeName,LPVOID IN_Struct);
	void SetAssemblyData(int IN_Type,LPVOID IN_Struct);
    INSTANCEDATA* GetFeatureData( );
	int GetSheetX(int IN_Type);
	int GetSheetY(int IN_Type);
private:
	int				m_ReturnFlag;			/* Ì¨°Á¬\¢ÌÔpÏÝÌ×¸Þ */
    INSTANCEDATA	m_FeatureData;			/* êÊvfÌ¨°Á¬²Ý½ÀÝ½ÃÞ°À */
    char			m_TypeName[30];			/* Ì¨°Á¬^ */
    LPVOID			m_StructData;			/* êÊvf\¢Ì */
	SXFInstanceIDC* m_InstanceIDManager;	/* ²Ý½ÀÝ½IDÇµÌÞ¼Þª¸Ä */
};

#endif

