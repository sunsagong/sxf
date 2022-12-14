//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFFeatureChildC.cpp                  作成日：2000.04.20
//
//	１．クラス概要
//      Feature Child Dataを管理する
//      
//	２．履歴
//	    履歴番号    日付け         担当者    概要
//      作成		2000.04.20    周
//
//------------------------------------------------------------

#include "stdafx.h"
#include "SXFFeatureChildC.h"
#include "SXFReadMapManagerC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFFeatureChildC.cpp                  作成日：2000.04.20
//
//  １．機能概要
//      コンストラクタ
//
//  ２．インターフェース
//		void	SXFFeatureChildC::SXFFeatureChildC()
//		
//	引数	：	型		引数名				内容
//		なし
//
//  ３．履歴
//	    履歴番号    日付け         担当者    概要
//      作成		2000.04.20    周
//
//------------------------------------------------------------

SXFFeatureChildC::SXFFeatureChildC()
{
	ChildID=-1;
	ChildMapManager=NULL;

}
//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFFeatureChildC.cpp                  作成日：2000.04.20
//
//  １．機能概要
//      デストラクタ
//  ２．インターフェース
//		void	SXFFeatureChildC::~SXFFeatureChildC()
//		
//	引数	：	型		引数名				内容
//		なし
//
//  ３．履歴
//	    履歴番号    日付け         担当者    概要
//      作成		2000.04.20    周
//
//------------------------------------------------------------
SXFFeatureChildC::~SXFFeatureChildC()
{
	if(ChildMapManager!=NULL)
		delete ChildMapManager;
}
