/**
	All Rights Reserved,(c) JACIC 2001
	SXFread_table.cpp								作成日：2000.4.29

	１．機能概要
	定義テーブルフィーチャの読込

	２．インターフェース
	int SXFread_table(int IN_TableType, 
						void *&OUT_Struct,
						int &OUT_EndFlag)

	引数:	型				引数名				内容
			int&			IN_TableType		定義テーブル型
			void*&			OUT_Struct			構造体
			int&			OUT_EndFlag			終了フラグ（０：継続　１：後続なし）

	復帰値	正常時：コード
			異常時：エラー番号

	３．履歴
	履歴番号		日付け				担当者				概要
	作成			2000.4.29			宮園(FQS)			なし。

**/

#include "stdafx.h"
#include "SXF_EXT.h"
#include "SXFManageC.h"
#include "SXFErrorMsgC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

__declspec(dllexport) int WINAPI SXFread_table(int IN_TableType, 
											   void *OUT_Struct, 
											   int *OUT_EndFlag)
{
	//ファイルがオープンされていなければエラー
	if (gl_Man == NULL){
		if (gl_log_file == NULL){
			gl_log_file = new ofstream("out.log",ios::out);
			if(gl_log_file->fail()){
				delete gl_log_file;
				gl_log_file = NULL;
				cerr << "Error opening out.log" << endl;
				*OUT_EndFlag = 1;
				return -1;
			}
		}
		SXFErrorMsgC::RecordError(SXFSTEPFILENOTOPEN,"",0);
		gl_log_file->close();
		delete gl_log_file;
		gl_log_file = NULL;
		*OUT_EndFlag = 1;
		return SXFOPENSTEPFILE;
	}
	int code = gl_Man->Read_table(IN_TableType,OUT_Struct,OUT_EndFlag);
	return code;
}
