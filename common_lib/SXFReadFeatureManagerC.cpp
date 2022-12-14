//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2000.04.23
//
//	PDNXTv
//      e CommentAZuvfA
//      êÊvfÌeqÖWðÇ·é
//
//	QDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.23     ü
//
//------------------------------------------------------------

#include "stdafx.h"
#include "SXFReadFeatureManagerC.h"
#include "SXFReadMapManagerC.h"
#include "SXFFeatureAssemblyC.h"
#include "SXFFeatureChildC.h"
#include "SXFSplitFeatureC.h"
#include "SXFErrorMsgC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2000.04.23
//
//  PD@\Tv
//      NX\z
//
//  QDC^[tF[X
//		void	SXFReadFeatureManagerC::SXFReadFeatureManagerC()
//		
//	ø	F	^		ø¼				àe
//		Èµ
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.23     ü
//
//------------------------------------------------------------
SXFReadFeatureManagerC::SXFReadFeatureManagerC()
{
    //initialization
    m_SheetAssembly = NULL;
    m_CurveAssemblyList = NULL;
    m_SymbolAssemblyList = NULL;
	m_SymbolInfoTable.InitHashTable(5000);
    m_CurrentAssembly = NULL;
    m_CurrentChildList = NULL;
    m_CurrentChild=NULL;
    m_CurrentMapManager=NULL;
    m_CurrentAssemblyType=0;
    m_AssemblyPos = NULL;
    m_ChildPos = NULL;
	m_code = 1;			/* AZuR[h */
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2000.04.23
//
//  PD@\Tv
//      NXÁÅ
//
//  QDC^[tF[X
//		void	SXFReadFeatureManagerC::~SXFReadFeatureManagerC()
//		
//	ø	F	^		ø¼				àe
//		Èµ
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.23     ü
//
//------------------------------------------------------------
SXFReadFeatureManagerC::~SXFReadFeatureManagerC()
{
    if(m_SheetAssembly != NULL )
        delete m_SheetAssembly;
    if(m_CurveAssemblyList != NULL){
        m_AssemblyPos = m_CurveAssemblyList->GetHeadPosition();
        while(m_AssemblyPos!=NULL){
            m_CurrentAssembly = m_CurveAssemblyList->GetAt(m_AssemblyPos);
            if(m_CurrentAssembly!=NULL){
                delete m_CurrentAssembly;
            }
            m_CurveAssemblyList->GetNext(m_AssemblyPos);
        }
        if(m_CurveAssemblyList != NULL)
            delete  m_CurveAssemblyList;
    }
	
    if(m_SymbolAssemblyList != NULL){
        m_AssemblyPos = m_SymbolAssemblyList->GetHeadPosition();
        while(m_AssemblyPos!=NULL){
            m_CurrentAssembly = m_SymbolAssemblyList->GetAt(m_AssemblyPos);
            if(m_CurrentAssembly!=NULL){
                delete m_CurrentAssembly;
            }
            m_SymbolAssemblyList->GetNext(m_AssemblyPos);
        }
        if(m_SymbolAssemblyList != NULL)
            delete  m_SymbolAssemblyList;
    }
	/*
	CString					strKey ;
	SXFFeatureAssemblyC*	pSymValue ;
	for( POSITION pos = m_SymbolInfoTable.GetStartPosition(); pos; ) {
		m_SymbolInfoTable.GetNextAssoc(pos, strKey, (void*&)pSymValue) ;
		delete pSymValue ;
	}
	*/
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2000.04.23
//
//  PD@\Tv
//      vfÇTableÉêÂÌFeature Comment Assembly Instanceðo^·é
//      
//  QDC^[tF[X
//      BOOL RegisterFeatureAssembly(INSTANCEDATA *IN_Instance)
//
//	    ø: ^			  ø¼			    àe
//            INSTANCEDATA*  IN_Instance           Feature Comment Assembly
//                                                 Instance\¢ÌÌAhX
//      
//	    Al	
//            ³íFTRUE
//			  ÙíFFALSE
//      
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.23     ü
//
//------------------------------------------------------------
BOOL SXFReadFeatureManagerC::RegisterFeatureAssembly(INSTANCEDATA *IN_Instance)
{

	/** PÂOÌAZuIuWFNgðÞð **/
	SXFFeatureAssemblyC* LastAssembly = m_CurrentAssembly;
	
	/** ±¾ÝÌÞØµÌÞ¼Þª¸Ä¶¬ */
    m_CurrentAssembly=new SXFFeatureAssemblyC;
    if(m_CurrentAssembly == NULL){
		Delete_m_CurrentChildList();
        return FALSE;//memory error
    }
	/** CX^XIDÝè **/
    m_CurrentAssembly->AssemblyID = IN_Instance->InstanceID;
	/** AZuR[hÝè **/
    m_CurrentAssembly->AssemblyCode = m_code;
	/** tB[`XgÝè **/
    m_CurrentAssembly->ChildList = m_CurrentChildList;
    m_CurrentChildList = NULL;

	/** MappingÇµÌÞ¼Þª¸Ä¶¬ */
    m_CurrentMapManager =  new SXFReadMapManagerC;
    if(m_CurrentMapManager == NULL){
		delete m_CurrentAssembly;
		m_CurrentAssembly=NULL;
        return FALSE;//memory error
    }
	/** CX^Xf[^Ýè **/
    if (!m_CurrentMapManager->SetFeatureData(IN_Instance)){
		delete m_CurrentAssembly;
		m_CurrentAssembly=NULL;
		delete m_CurrentMapManager;
		m_CurrentMapManager=NULL;
        return FALSE;//memory error
	}
	/** MappingÇµÌÞ¼Þª¸Ä¾¯Ä */
    m_CurrentAssembly->AssemblyMapManager = m_CurrentMapManager;

	/** p **/
    if(IN_Instance->EntityName == "drawing_sheet_feature"){
		/* pª·ÅÉÝèÏÝ */
        if(m_SheetAssembly != NULL){
			/* pÍêÂÌÝ */
			SXFErrorMsgC::RecordError(SXFSHEETONE,"SHEET",0);
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
			return FALSE;
		}
        CStringArray* array;
        SXFSplitFeatureC split(IN_Instance->EntityData);
        array = split.Split();
        if(array == NULL){
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
        if (array->GetSize() != 5){ // syntax err
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
		/* p¼ */
        m_CurrentAssembly->AssemblyName = (*array)[0];
		/* pTCYíÊ */
        m_CurrentAssembly->AssemSheetType = atoi((*array)[1]);
		//pTCYíÊªO`SÜ½ÍXÅÍÈ¢Æ«
		if ( (m_CurrentAssembly->AssemSheetType < 0) 
					|| (m_CurrentAssembly->AssemSheetType > 4) ){
			if (m_CurrentAssembly->AssemSheetType != 9){
				SXFErrorMsgC::RecordError(SXFSHEETSIZE,
											"SHEET",
											1,
											m_CurrentAssembly->AssemSheetType);
				delete m_CurrentAssembly;
				m_CurrentAssembly=NULL;
				return FALSE;
			}
		}
		/* pTCY */
        m_CurrentAssembly->AssemSheetX = atoi((*array)[3]);
        m_CurrentAssembly->AssemSheetY = atoi((*array)[4]);
		//pTCYíÊªXÅ©Rp¡·,c·ªÝè³êÄÈ¢Æ«
		if (m_CurrentAssembly->AssemSheetType == 9){
			if (m_CurrentAssembly->AssemSheetX <= 0){
				SXFErrorMsgC::RecordError(SXFSHEETLENGTH,
											"SHEET",
											1,
											m_CurrentAssembly->AssemSheetX);
				delete m_CurrentAssembly;
				m_CurrentAssembly=NULL;
				return FALSE;
			}
			if (m_CurrentAssembly->AssemSheetY <= 0){
				SXFErrorMsgC::RecordError(SXFSHEETLENGTH,
											"SHEET",
											1,
											m_CurrentAssembly->AssemSheetY);
				delete m_CurrentAssembly;
				m_CurrentAssembly=NULL;
				return FALSE;
			}
		}
		/* c^¡æª */
        m_CurrentAssembly->AssemSheetOrient = atoi((*array)[2]);
		//pTCYíÊªXÅÍÈ­Ac/¡æªªOÜ½ÍPÅÍÈ¢Æ«
		//pTCYíÊªXÌÆ«Ac/¡æªÍ³ø
		if (m_CurrentAssembly->AssemSheetType != 9){
			if ((m_CurrentAssembly->AssemSheetOrient < 0) 
					|| (m_CurrentAssembly->AssemSheetOrient > 1)){
				SXFErrorMsgC::RecordError(SXFORIENTERROR,
											"SHEET",
											1,
											m_CurrentAssembly->AssemSheetOrient);
				delete m_CurrentAssembly;
				m_CurrentAssembly=NULL;
				return FALSE;
			}
		}
		/* pAZuIuWFNgZbg */
        m_SheetAssembly = m_CurrentAssembly;
    }
	/** ¡Èüè` **/
	else if(IN_Instance->EntityName == "composite_curve_org_feature"){
        /** PÂÚÌ¡ÈüÌê**/
        if(m_CurveAssemblyList == NULL){
            m_CurveAssemblyList = new SXFASSEMBLYLIST;
        }
        /** QÂÚÈ~Ì¡ÈüÌÉÄAPÂOÌAZuvfªp¾Á½ê **/
		if(LastAssembly==m_SheetAssembly && LastAssembly!=NULL ){
			/* pÍ¡Èüè`ÉzuÅ«È¢ */
			SXFErrorMsgC::RecordError(SXFCURVELOCERR,"COPOSITE_CURVE_ORG",0);
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
        CStringArray* array;
        SXFSplitFeatureC split(IN_Instance->EntityData);
        array = split.Split();
        if(array == NULL){
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
        if (array->GetSize() != 4){ // syntax err
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
		/* ®« */
		m_CurrentAssembly->m_CurveColour = atoi((*array)[0]);
		m_CurrentAssembly->m_CurveType = atoi((*array)[1]);
		m_CurrentAssembly->m_CurveWidth = atoi((*array)[2]);
		/* \¦^ñ\¦tO */
        m_CurrentAssembly->AssemCurveFlag = atoi((*array)[3]);
        //\¦^ñ\¦tOªOÜ½ÍPÈOÌÆ«
        if ((m_CurrentAssembly->AssemCurveFlag < 0) 
				|| (m_CurrentAssembly->AssemCurveFlag > 1)){
            SXFErrorMsgC::RecordError(SXFCURVEFLAG,
										"COPOSITE_CURVE_ORG",
										1,
										m_CurrentAssembly->AssemCurveFlag);
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
        }
		/* ¡Èüe[uÉÇÁ */
        m_CurveAssemblyList->AddTail(m_CurrentAssembly);
    }
	/** ¡}`è` **/
	else if(IN_Instance->EntityName == "sfig_org_feature"){
		/** QÂÚÈ~Ì¡}`ÉÄAPÂOÌAZuvfªp¾Á½ê **/
		if(LastAssembly==m_SheetAssembly && LastAssembly!=NULL ){
			/* pÍ¡}`è`ÉzuÅ«È¢ */
			SXFErrorMsgC::RecordError(SXFSFIGLOCERR,"SFIG_ORG",0);
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
		/** PÂÚÌ¡}`Ìê **/
		
        if(m_SymbolAssemblyList == NULL){
            m_SymbolAssemblyList = new SXFASSEMBLYLIST;
        }
		
        CStringArray* array;
        SXFSplitFeatureC split(IN_Instance->EntityData);
        array = split.Split();
        if(array == NULL){
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
        if (array->GetSize() != 2){ // syntax err
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
            return FALSE;
		}
		/* ¡}`¼ */
        m_CurrentAssembly->AssemblyName = (*array)[0];
		//Name Unique check

		/*	2010.08
		for(POSITION pos = m_SymbolAssemblyList->GetHeadPosition();
						pos != NULL;m_SymbolAssemblyList->GetNext(pos)){
			// ùÉ¼Oª¼Oªo^ÏÝ
			if (m_SymbolAssemblyList->GetAt(pos)->AssemblyName == (*array)[0]){
				SXFErrorMsgC::RecordError(SXFSAMENAME,"SFIG_ORG",1,(*array)[0]);
				delete m_CurrentAssembly;
				m_CurrentAssembly=NULL;
				return FALSE;
			}
		}
		*/

		SXFFeatureAssemblyC* pValue ;	// 2010.08
		if( m_SymbolInfoTable.Lookup(m_CurrentAssembly->AssemblyName, (void*&)pValue) ) {
			// ùÉ¼Oª¼Oªo^ÏÝ
			SXFErrorMsgC::RecordError(SXFSAMENAME,"SFIG_ORG",1,(*array)[0]);
			delete m_CurrentAssembly;
			m_CurrentAssembly=NULL;
			return FALSE;
		}
		/* ¡}`íÊ */
		m_CurrentAssembly->AssemSymbolType = atoi((*array)[1]);
		if (m_CurrentAssembly->AssemSymbolType < 1 
				|| m_CurrentAssembly->AssemSymbolType > 4){
			CString AssemID;
			AssemID.Format("#%d",m_CurrentAssembly->AssemblyID);
			SXFErrorMsgC::RecordError(SXFSFIGFLAG,
										"SFIG_ORG",
										3,
										AssemID,
										m_CurrentAssembly->AssemSymbolType,
										"SFIG_ORG");
				delete m_CurrentAssembly;
				m_CurrentAssembly=NULL;
				return FALSE;
		}
		/* ¡}`Ã°ÌÞÙÉÇÁ */
        m_SymbolAssemblyList->AddTail(m_CurrentAssembly);
		m_SymbolInfoTable.SetAt(m_CurrentAssembly->AssemblyName, m_CurrentAssembly);	// 2010.08
    }
    m_code++;	/* ±¾ÝÌÞØº°ÄÞ */
    return TRUE;
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2000.04.23
//
//  PD@\Tv
//      vfÇTableÉêÂÌFeature Comment Entity Instanceðo^·é
//      
//  QDC^[tF[X
//      BOOL RegisterFeatureEntity(INSTANCEDATA *IN_Instance)
//
//	    ø: ^			  ø¼			    àe
//            INSTANCEDATA*  IN_Instance           Feature Comment Entity
//                                                 Instance\¢ÌÌAhX
//      
//	    Al	
//            ³íFTRUE
//			  ÙíFFALSE
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.23     ü
//
//------------------------------------------------------------
BOOL SXFReadFeatureManagerC::RegisterFeatureEntity(INSTANCEDATA *IN_Instance)
{
    if(m_CurrentChildList == NULL){
		/* êÊvfÃ°ÌÞÙ¶¬ */
        m_CurrentChildList = new SXFCHILDLIST; 
    }
	/* êÊvfÌ¨°Á¬µÌÞ¼Þª¸Ä¶¬ */
    m_CurrentChild = new SXFFeatureChildC;
    if(m_CurrentChild == NULL)
        return FALSE;//memory error
	/** CX^XIDÝè **/
    m_CurrentChild->ChildID = IN_Instance->InstanceID;
	/* Ì¨°Á¬MappingÇµÌÞ¼Þª¸Ä¶¬ */
    m_CurrentMapManager =  new SXFReadMapManagerC;
    if(m_CurrentMapManager == NULL){
		delete m_CurrentChild;
		m_CurrentChild=NULL;
        return FALSE;//memory error
	}
	/** CX^Xf[^Ýè **/
    if (!m_CurrentMapManager->SetFeatureData(IN_Instance)){
		delete m_CurrentChild;
		m_CurrentChild=NULL;
		delete m_CurrentMapManager;
		m_CurrentMapManager=NULL;
		return FALSE;
	}
	/* Ì¨°Á¬MappingÇµÌÞ¼Þª¸ÄÝè */
    m_CurrentChild->ChildMapManager = m_CurrentMapManager;
	/* êÊvfÃ°ÌÞÙÉÇÁ */
    m_CurrentChildList->AddTail( m_CurrentChild );
    return TRUE;
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2000.04.23
//
//  PD@\Tv
//      vfÇTable©çêÂÌvfðoÍ·é
//      
//  QDC^[tF[X
//      int GetNextFeature(char* OUT_TypeName,
//                         LPVOID& OUT_Struct,
//                        int& OUT_EndFlg)
//
//	    ø: ^			  ø¼			   àe
//            char*			  OUT_TypeName         \¢ÌÌType Name
//			  size_t		  TypeNameSize		   Type Nameobt@TCY
//            LPVOID&         OUT_Struct           \¢ÌÌAhX
//            int&            OUT_EndFlg           ÅãÌvfÌFlag
//                                                  1: ÅãÌvf
//                                                  0: ±«
//      
//	    Al	
//            ³íF Feature ID
//			  ÙíF (G[R[h)
//      
//
//  RDð
//		ðÔ	út¯			SÒ			Tv
//		ì¬		2000.04.23		ü
//		XV		2015.04.27		T.Matsushima	JPCERT/CC wE Æã«Î
//
//------------------------------------------------------------
int SXFReadFeatureManagerC::GetNextFeature(char* OUT_TypeName, 
										   size_t TypeNameSize,	// obt@TCYðn·æ¤ÉÏX JPCERT/CC wE Æã«Î	T.Matsushima	2015.04.27
										   LPVOID& OUT_Struct,
										   int* OUT_EndFlg)
{
    OUT_Struct = NULL;
    *OUT_EndFlg = 1;
	int ReturnID = -1;
    m_CurrentChild=NULL;
	/** AZuªJ©êÄ¢È¢ **/
    if(m_CurrentAssembly == NULL){
		SXFErrorMsgC::RecordError(SXFNOTASSEMBLY,OUT_TypeName,0);
		return SXFNOTASSEMBLY;
	}
	/** AZuãÉvfª¶ÝµÈ¢ **/
	if(m_CurrentChildList == NULL){
		SXFErrorMsgC::RecordError(SXFNOTFEATURE,"",0);
		return SXFNOTFEATURE;
	}
	/** AZuãÉvfª¶ÝµÈ¢ **/
    if(m_ChildPos == NULL){
		SXFErrorMsgC::RecordError(SXFNOTFEATURE,"",0);
		return SXFNOTFEATURE;
	}
	/** êÊvfµÌÞ¼Þª¸Ä¾¯Ä */
    m_CurrentChild = m_CurrentChildList->GetAt(m_ChildPos);
    if(m_CurrentChild == NULL){
		/** AZuãÉvfª¶ÝµÈ¢ **/
		SXFErrorMsgC::RecordError(SXFNOTFEATURE,"",0);
		return SXFNOTFEATURE;
	}
	/** MappingÇµÌÞ¼Þª¸Ä¾¯Ä */
    m_CurrentMapManager = m_CurrentChild->ChildMapManager;
    if(m_CurrentMapManager == NULL){
		/* àG[ª­¶µÜµ½ */
		SXFErrorMsgC::RecordError(SXFERROR,"",0);
		return SXFERROR;
	}
	/** CX^XIDÝè **/
	ReturnID = m_CurrentChild->ChildID;
	/** tB[`\¢Ìæ¾ **/
	//	JPCERT/CC wE Æã«Î	T.Matsushima	2015.04.27
	//OUT_Struct = m_CurrentMapManager->GetStructData( OUT_TypeName);
	OUT_Struct = m_CurrentMapManager->GetStructData( OUT_TypeName, TypeNameSize);
	if(OUT_Struct == NULL){
		/* s³ÈGeBeB¼Å· */
 		SXFErrorMsgC::RecordError(SXFERRENTITY,"",0);
		return SXFERRENTITY;
	}
	m_CurrentChildList->GetNext(m_ChildPos);
	/** \¢ÌÔpÏÝtOðON **/
	m_CurrentMapManager->SetReturnFlag();
	/** êÊvfÃ°ÌÞÙÌÅãÌvfÌÆ« **/
    if(m_ChildPos == NULL){
        *OUT_EndFlg = 1;		/* EndFlg ðON */
    }
	else{
        *OUT_EndFlg = 0;		/* EndFlg ðOFF */
    }
    return ReturnID;
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2000.04.23
//
//  PD@\Tv
//      AssemblyvfÇTable©çêÂÌAssemblyvfðoÍ·é
//      
//  QDC^[tF[X
//      int GetNextAssembly(int IN_Type,
//                          LPVOID& OUT_Struct,
//                          int& OUT_EndFlg,
//                          int& OUT_InstanceID)
//
//	    ø: ^			  ø¼			    àe
//            int             IN_Type              Assembly\¢ÌÌType
//                                                  1:sheet
//                                                  2:symbol
//                                                  3:curve
//            LPVOID&         OUT_Struct           Assembly\¢ÌÌAhX
//            int&            OUT_EndFlg           ÅãÌAssemblyvfÌFlag
//                                                  1: ÅãÌAssemblyvf
//                                                  0: ±«
//            int&            OUT_InstanceID        Assembly Feature ID
//      
//	    Al	
//            ³íFAssembly Code(Assembly TableÉo^µ½ÌÔj
//			  ÙíF(G[R[h)
//      
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2000.04.23     ü
//
//------------------------------------------------------------
int SXFReadFeatureManagerC::GetNextAssembly(int IN_Type, 
												LPVOID& OUT_Struct, 
												int* OUT_EndFlg,
												int& OUT_InstanceID)
{
    OUT_Struct = NULL;
    *OUT_EndFlg = 1;
	OUT_InstanceID = -1;
    m_CurrentAssembly = NULL;
    m_CurrentChildList = NULL;    
    m_ChildPos = NULL;

	/* ±¾ÝÌÞØ^ªÙí */
	if (IN_Type != 1 && IN_Type != 2 && IN_Type != 3){
		SXFErrorMsgC::RecordError(SXFASSEMTYPEERR,"",1,IN_Type);
		return SXFASSEMTYPEERR;
	}
	/* VKÃ°ÌÞÙÌÇÝoµwè */
	if(m_CurrentAssemblyType != IN_Type){
		m_CurrentAssemblyType = 0;
	}
	/* VKÃ°ÌÞÙÌÇÝoµwè */
	if(m_CurrentAssemblyType != IN_Type){
		m_CurrentAssemblyType = 0;
	}

	/** ÅÌAZuvfÌÆ« **/
    if(m_CurrentAssemblyType == 0){
		/** p **/
        if(IN_Type == 1){
			/** pªÈ¢ **/
            if(m_SheetAssembly == NULL){
				SXFErrorMsgC::RecordError(SXFNOTSETSHEET,"SHEET",0);
                return SXFNOTSETSHEET;//no sheet assembly
			}
            m_CurrentAssembly = m_SheetAssembly;
            m_AssemblyPos = NULL;
			/* »ÝÌ±¾ÝÌÞØ^ðZbg */
            m_CurrentAssemblyType = 1;
        }
		/** ¡}`è` **/
		else if(IN_Type == 2){//symbol

            if(m_SymbolAssemblyList == NULL){
				// ÇÝo·AZuvfª èÜ¹ñ
// MOD(S)	bZ[WC³	K.Naono	04.03.05
//				SXFErrorMsgC::RecordError(SXFNOASSEMBLY,"SFIGORG",0);
				SXFErrorMsgC::RecordError2(SXFNOASSEMBLY,"SFIGORG",0);
				return SXFNOASSEMBLY;
// MOD(E)	bZ[WC³	K.Naono	04.03.05
			}

			/** ¡}`XgÌæªAhXÝè **/
			m_AssemblyPos=m_SymbolAssemblyList->GetHeadPosition();
            // m_AssemblyPos=m_SymbolInfoTable.GetStartPosition(); // 2010.08
            if(m_AssemblyPos == NULL){
				/* ÇÝo·AZuvfª èÜ¹ñB*/
// MOD(S)	bZ[WC³	K.Naono	04.03.05
//				SXFErrorMsgC::RecordError(SXFNOASSEMBLY,"SFIGORG",0);
				SXFErrorMsgC::RecordError2(SXFNOASSEMBLY,"SFIGORG",0);
				return SXFNOASSEMBLY;
// MOD(E)	bZ[WC³	K.Naono	04.03.05
			}
            m_CurrentAssembly = m_SymbolAssemblyList->GetAt(m_AssemblyPos);
			// CString	strKey ;									// 2010.08
			// m_SymbolInfoTable.GetNextAssoc(m_AssemblyPos, strKey, (void*&)m_CurrentAssembly);
			/* »ÝÌ±¾ÝÌÞØ^ðZbg */
            m_CurrentAssemblyType = 2;
        }
		/** ¡Èüè` **/
		else if(IN_Type == 3){//curve
            if(m_CurveAssemblyList == NULL){
				/* ÇÝo·AZuvfª èÜ¹ñB*/
// MOD(S)	bZ[WC³	K.Naono	04.03.05
//				SXFErrorMsgC::RecordError(SXFNOASSEMBLY,"CCURVE_ORG",0);
				SXFErrorMsgC::RecordError2(SXFNOASSEMBLY,"CCURVE_ORG",0);
				return SXFNOASSEMBLY;
// MOD(E)	bZ[WC³	K.Naono	04.03.05
			}
			/** ¡ÈüXgÌæªAhXÝè **/
            m_AssemblyPos=m_CurveAssemblyList->GetHeadPosition();
            if(m_AssemblyPos == NULL){
				/* ÇÝo·AZuvfª èÜ¹ñB*/
// MOD(S)	bZ[WC³	K.Naono	04.03.05
//				SXFErrorMsgC::RecordError(SXFNOASSEMBLY,"CCURVE_ORG",0);
				SXFErrorMsgC::RecordError2(SXFNOASSEMBLY,"CCURVE_ORG",0);
				return SXFNOASSEMBLY;
// MOD(E)	bZ[WC³	K.Naono	04.03.05
			}
            m_CurrentAssembly = m_CurveAssemblyList->GetAt(m_AssemblyPos);
			/* »ÝÌ±¾ÝÌÞØ^ðZbg */
            m_CurrentAssemblyType = 3;
        }
    }
	/** QÂÚÈ~ÌAZuvfÌÆ« **/
	else{
		/* ÅIf[^æ¾ãÌÇÝoµ`FbN */
		if (m_AssemblyPos == NULL){
			/* ÇÝo·AZuvfª èÜ¹ñB*/
			SXFErrorMsgC::RecordError(SXFNOASSEMBLY,"",0);
			return SXFNOASSEMBLY;
		}
		/* ¡}` */
		if(m_CurrentAssemblyType == 2) {
		    m_CurrentAssembly = m_SymbolAssemblyList->GetAt(m_AssemblyPos);
			// CString	strKey ;									// 2010.08
			// m_SymbolInfoTable.GetNextAssoc(m_AssemblyPos, strKey, (void*&)m_CurrentAssembly);
		}
		/* ¡Èü */
        else if(m_CurrentAssemblyType == 3)
            m_CurrentAssembly = m_CurveAssemblyList->GetAt(m_AssemblyPos);
    }
    if(m_CurrentAssembly == NULL){
		/* ÇÝo·AZuvfª èÜ¹ñB*/
		SXFErrorMsgC::RecordError(SXFNOASSEMBLY,"",0);
		return SXFNOASSEMBLY;
	}
	/** MappingÇµÌÞ¼Þª¸Ä¾¯Ä **/
    m_CurrentMapManager = m_CurrentAssembly->AssemblyMapManager;
    if(m_CurrentMapManager == NULL){
		/* àG[ª­¶µÜµ½ */
		SXFErrorMsgC::RecordError(SXFERROR,"",0);
		return SXFERROR;
	}
	/** AZutB[`Ì\¢Ìæ¾ **/
    OUT_Struct = m_CurrentMapManager->GetAssemblyStructData( );
	/** CX^XIDÝè **/
	OUT_InstanceID = m_CurrentAssembly->AssemblyID ;//G[bZ[WÉKvI
	/** tB[`XgÝè **/
    m_CurrentChildList=m_CurrentAssembly->ChildList;
	if(m_CurrentChildList == NULL){
		/** G[tOð½Äé **/
		m_CurrentAssembly->m_AssemblyErrFlag=FALSE;
	}
	else{
		/** tB[`XgÌæªAhXðÝè **/
		m_ChildPos = m_CurrentChildList->GetHeadPosition();
		    if(m_ChildPos == NULL){
				/** G[tOð½Äé **/
				m_CurrentAssembly->m_AssemblyErrFlag=FALSE;
		    }
	}

	/** p **/
    if(IN_Type == 1)
        m_AssemblyPos = NULL;
	/** ¡}` **/
	else if(IN_Type == 2) {
        m_SymbolAssemblyList->GetNext(m_AssemblyPos);
		// CString	strKey ;
		// SXFFeatureAssemblyC* pSymValue ;
		// m_SymbolInfoTable.GetNextAssoc(m_AssemblyPos, strKey, (void*&)pSymValue);
	}
    /** ¡Èü **/
	else if(IN_Type == 3)
        m_CurveAssemblyList->GetNext(m_AssemblyPos);
	/** \¢ÌÔpÏÝtOðON **/
	m_CurrentMapManager->SetReturnFlag();
	/** AZue[uÌÅãÌvfÌÆ« **/
    if(m_AssemblyPos == NULL){
        *OUT_EndFlg = 1;	/** END flg ðON **/
    }
	else{
        *OUT_EndFlg = 0;	/** END flg ðOFF **/
    }
    return m_CurrentAssembly->AssemblyCode;
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2001.2.23
//
//  PD@\Tv
//      ¡}`¼ÉÎ·é¡}`íÊÌæ¾
//
//  QDC^[tF[X
//      BOOL GetSfigFlg( LPCTSTR IN_Name,
//						int* OUT_SfigFlg,
//						int* OUT_LocFlg)
//
//	    ø: ^			  ø¼			àe
//            LPCTSTR         IN_Name           Sfig Name
//			  int*			  OUT_SfigFlg		¡}`íÊ
//			  int*			  OUT_LocFlg		zutO
//												(1:zuÏ 0:¢zu)
//
//	    Al	
//            ³íFTRUE		¡}`¼ÉÎ·éíÊ è
//			  ÙíFFALSE		¡}`¼ÉÎ·éíÊÈµ
//
//  RDð
//	    ðÔ    út¯         SÒ		 Tv
//      ì¬		2001.2.23      {(FQS)
//
//------------------------------------------------------------
BOOL SXFReadFeatureManagerC::GetSfigFlg( LPCTSTR IN_Name,
											int* OUT_SfigFlg,
											int* OUT_LocFlg)
{
	*OUT_SfigFlg = -1;
	*OUT_LocFlg = -1;
/*
	if (m_SymbolAssemblyList == NULL)
		return FALSE;
	//Name check 
	for(POSITION pos = m_SymbolAssemblyList->GetHeadPosition();
					pos != NULL;m_SymbolAssemblyList->GetNext(pos)){
		if (m_SymbolAssemblyList->GetAt(pos)->AssemblyName == IN_Name){
			// ¡}`íÊ
			*OUT_SfigFlg = m_SymbolAssemblyList->GetAt(pos)->AssemSymbolType;
			// ¡}`zuÌ×¸Þ
			*OUT_LocFlg = m_SymbolAssemblyList->GetAt(pos)->m_SfigLocateFlag;
			return TRUE;
        }
	}
*/
	SXFFeatureAssemblyC* pValue ;
	if( m_SymbolInfoTable.Lookup(IN_Name, (void*&)pValue) ) {
		// ¡}`íÊ
		*OUT_SfigFlg= pValue->AssemSymbolType ;
		// ¡}`zuÌ×¸Þ
		*OUT_LocFlg	= pValue->m_SfigLocateFlag ;
		return TRUE ;
	}
	return FALSE;
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2001.2.23
//
//  PD@\Tv
//      ¡}`¼ÉÎ·é¡}`íÊÌÝè
//
//  QDC^[tF[X
//      BOOL SetLocFlg( LPCTSTR IN_Name)
//
//	    ø: ^			  ø¼			àe
//            LPCTSTR         IN_Name           Sfig Name
//
//	    Al	
//            ³íFTRUE		¡}`¼ÉÎ·éíÊ è
//			  ÙíFFALSE		¡}`¼ÉÎ·éíÊÈµ
//
//  RDð
//	    ðÔ    út¯         SÒ    Tv
//      ì¬		2001.2.23	   {(FQS)
//
//------------------------------------------------------------
BOOL SXFReadFeatureManagerC::SetLocFlg( LPCTSTR IN_Name)
{
/*
	if (m_SymbolAssemblyList == NULL)
		return FALSE;
	for(POSITION pos = m_SymbolAssemblyList->GetHeadPosition();
					pos != NULL;m_SymbolAssemblyList->GetNext(pos)){
		if (m_SymbolAssemblyList->GetAt(pos)->AssemblyName == IN_Name){
			// ¡}`zuÌ×¸ÞðON 
			m_SymbolAssemblyList->GetAt(pos)->m_SfigLocateFlag = 1;
			return TRUE;
        }
	}
*/

/*
	for(int i=0;i<m_SymbolInfoTable.GetSize();i++){
		if (m_SymbolInfoTable[i].name == IN_Name){
			// ¡}`zuÌ×¸ÞðON
			m_SymbolInfoTable[i].locateflag = 1;
			return TRUE;
		}
	}
*/
	SXFFeatureAssemblyC* pValue ;
	if( m_SymbolInfoTable.Lookup(IN_Name, (void*&)pValue) ) {
		// ¡}`zuÌ×¸ÞðON
		pValue->m_SfigLocateFlag = 1 ;
		return TRUE ;
	}
	return FALSE;  
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2001.2.23
//
//  PD@\Tv
//      nb`OÌO`E²«IDªè`³êÄ¢é©`FbN·é
//
//  QDC^[tF[X
//      BOOL CheckHatchID( int IN_ID)
//
//	    ø: ^	ø¼	    àe
//            int   IN_ID       Ê¯ÁÝ¸ÞÌO`²«ÌID
//								(¡ÈüÌ±¾ÝÌÞØº°ÄÞ)
//
//	    Al	
//            ³íFTRUE		Ê¯ÁÝ¸ÞÌO`ªè`³êÄ¢é
//			  ÙíFFALSE		Ê¯ÁÝ¸ÞÌO`ªè`³êÄ¢È¢
//      
//  RDð
//	    ðÔ    út¯         SÒ		Tv
//      ì¬		2001.2.23	   {(FQS)
//
//------------------------------------------------------------
BOOL SXFReadFeatureManagerC::CheckHatchID( int IN_ID){
	if (m_CurveAssemblyList == NULL)
		return FALSE;
	for(POSITION pos = m_CurveAssemblyList->GetHeadPosition();
				pos != NULL;m_CurveAssemblyList->GetNext(pos)){
		if (m_CurveAssemblyList->GetAt(pos)->AssemblyCode == IN_ID)
			// ³ðÉTRUEÉ·éÌÅÍÈ­AerrorÅ é©Ç¤©à²×é
			if (m_CurveAssemblyList->GetAt(pos)->m_AssemblyErrFlag == TRUE) {
				return TRUE;
			}
			else {
				return FALSE;
			}
	}
	return FALSE;			
}

//------------------------------------------------------------
//	All Rights Reserved,(c) JACIC 2001
//	SXFReadFeatureManagerC.cpp               ì¬úF2001.2.23
//
//  PD@\Tv
//      m_CurrentChildListðdelete·é
//
//  QDC^[tF[X
//      void Delete_m_CurrentChildList()
//
//	    ø: ^	ø¼	    àe
//
//	    Al	
//      
//  RDð
//	    ðÔ    út¯         SÒ		Tv
//      ì¬		2001.2.23	   {(FQS)
//
//------------------------------------------------------------
void SXFReadFeatureManagerC::Delete_m_CurrentChildList(){
	if(m_CurrentChildList != NULL){
		POSITION childpos = m_CurrentChildList->GetHeadPosition();
		while(childpos!=NULL){
			SXFFeatureChildC* child = m_CurrentChildList->GetAt(childpos);
			if(child!=NULL)
				delete child;
			m_CurrentChildList->GetNext(childpos);
		}
		delete m_CurrentChildList;
		m_CurrentChildList = NULL;
	}
}


