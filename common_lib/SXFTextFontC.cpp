//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFTextFontC.cpp                        ì¬úF2000.04.16
//
//	PDNXTv
//     TextFont Instance Table DataðÇ·é
// 
//	QDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.16     ü
//
//------------------------------------------------------------

#include "stdafx.h"
#include "SXFTextFontC.h"
#include "SXFTableMapManagerC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFTextFontC.cpp                        ì¬úF2000.04.16
//
//  PD@\Tv
//      NX\z
//
//  QDC^[tF[X
//		void	SXFTextFontC::SXFTextFontC( )
//		
//	ø	F	^		ø¼				àe
//		Èµ
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.16     ü
//
//------------------------------------------------------------
SXFTextFontC::SXFTextFontC( )
{
	//initialization 
	m_Code = -1;
	m_ID = -1;
	m_Name.Empty();
	m_MappingManager = NULL;
}
//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFTextFontC.cpp                        ì¬úF2000.04.16
//
//  PD@\Tv
//      NXÁÅ
//
//  QDC^[tF[X
//		void	SXFTextFontC::~SXFTextFontC( )
//		
//	ø	F	^		ø¼				àe
//		Èµ
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.16     ü
//
//------------------------------------------------------------
SXFTextFontC::~SXFTextFontC()
{
}
