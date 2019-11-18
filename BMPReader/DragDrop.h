//---------------------------------------------------------------------------
// DragSource Lib.
// Copyright (c) 2006, 2007 by Mao yongjun
// All rights reserved
// Add "WINOLEAPI OleInitialize (LPVOID pvReserved)" before use DragDropLib on each thread
// Don't forget "WINOLEAPI OleUninitialize ()" on exit the thread
//---------------------------------------------------------------------------

#ifndef DragDropH
#define DragDropH
#include <oleidl.h>
#include <objidl.h>
#include <vector>
#include <string>

struct THDROP_Files :
    std::vector<std::string>{
    void *Create_HDROP() const;
};


struct TDropResult {
    DWORD Result;
    /*
    * DRAGDROP_S_DROP
    *     The OLE drag-and-drop operation was successful.
    * DRAGDROP_S_CANCEL
    *     The OLE drag-and-drop operation was canceled.
    * E_UNSPEC
    *     Unexpected error occurred.
    */
    DWORD Effect;
    /*
    * typedef enum tagDROPEFFECT
    * {
    *     DROPEFFECT_NONE   = 0,
    *     DROPEFFECT_COPY   = 1,
    *     DROPEFFECT_MOVE   = 2,
    *     DROPEFFECT_LINK   = 4,
    *     DROPEFFECT_SCROLL = 0x80000000
    * }DROPEFFECT;
    */
};
//---------------------------------------------------------------------------
TDropResult DoDragDrop(FORMATETC *fmtetc, STGMEDIUM *stgmeds,UINT count=1,
        DWORD dwOKEffect=DROPEFFECT_COPY|DROPEFFECT_MOVE);  //Drag fms,
TDropResult DoDragDrop(CLIPFORMAT ctFormat,DWORD tymed,void *hStorage,
        DWORD dwOKEffect=DROPEFFECT_COPY|DROPEFFECT_MOVE);  //Drag simple type, hStorage will be freed
TDropResult DoDragDrop(char *szText,
        DWORD dwOKEffect=DROPEFFECT_COPY|DROPEFFECT_MOVE);  //Drag Text
TDropResult DoDragDropURL(char *URL,
        DWORD dwOKEffect=DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_LINK);  //Drag URL
TDropResult DoDragDrop(wchar_t *wszText,
        DWORD dwOKEffect=DROPEFFECT_COPY|DROPEFFECT_MOVE);  //Drag Unicode Text
TDropResult DoDragDrop(HBITMAP hBitmap,
        DWORD dwOKEffect=DROPEFFECT_COPY|DROPEFFECT_MOVE);  //Drag Bitmap
TDropResult DoDragDrop(const THDROP_Files &files,
        DWORD dwOKEffect=DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_LINK);  //Drag Files

HRESULT CreateDataObject (FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count, IDataObject **ppDataObject);
HRESULT CreateEnumFormatEtc (UINT cfmt, FORMATETC *afmt, IEnumFORMATETC **ppEnumFormatEtc);
HANDLE MemToHandle (void *pMem, int nSize);
//----------------TDropSource-----------------------------------------------------------
class TDropSource
    :public IDropSource
{
private:
    ULONG _iRefCount;
public:
    // IUnknown members
    HRESULT __stdcall QueryInterface(REFIID iid, void **ppv);
    ULONG __stdcall AddRef(void) { return ++_iRefCount; }
    ULONG __stdcall Release(void) { if(--_iRefCount==0){delete this; return 0;} return _iRefCount; }

    // IDropSource
    HRESULT __stdcall QueryContinueDrag(BOOL fEscapePressed,DWORD grfKeyState);
    HRESULT __stdcall GiveFeedback(DWORD dwEffect);
    
    //
    TDropSource():_iRefCount(1){;}
    virtual ~TDropSource() {;}
};

//----------------TDataObject-----------------------------------------------------------
class TDataObject : public IDataObject
{
public:
    // IUnknown members
    HRESULT __stdcall QueryInterface (REFIID iid, void ** ppv);
    ULONG   __stdcall AddRef (void) { return ++_iRefCount; }
    ULONG   __stdcall Release (void)  { if(--_iRefCount==0){delete this;  return 0;} return _iRefCount; }

    // IDataObject members
    HRESULT __stdcall GetData (FORMATETC *pFormatEtc, STGMEDIUM *pStgMedium);
    HRESULT __stdcall QueryGetData (FORMATETC *pFormatEtc);
    HRESULT __stdcall EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);

    HRESULT __stdcall GetDataHere (FORMATETC *pFormatEtc, STGMEDIUM *pmedium){
        return DATA_E_FORMATETC;
    }
    HRESULT __stdcall GetCanonicalFormatEtc (FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut){
        // Apparently we have to set this field to NULL even though we don't do anything else
        pFormatEtcOut->ptd = NULL;
        return E_NOTIMPL;
    }
    HRESULT __stdcall SetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium,  BOOL fRelease){
        return E_NOTIMPL;
    }
    HRESULT __stdcall DAdvise (FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *, DWORD *){
        return OLE_E_ADVISENOTSUPPORTED;
    }
    HRESULT __stdcall DUnadvise (DWORD dwConnection){
        return OLE_E_ADVISENOTSUPPORTED;
    }
    HRESULT __stdcall EnumDAdvise (IEnumSTATDATA **ppEnumAdvise){
        return OLE_E_ADVISENOTSUPPORTED;
    }

    // Constructor / Destructor
    TDataObject():_iRefCount(1) {;}
    virtual ~TDataObject (){;}

    void Add(FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT count);
    void Add(CLIPFORMAT ctFormat,DWORD tymed,void *hStorage);
    size_t size(){return _pFormatEtc.size();}
    bool empty(){return _pFormatEtc.empty();}
    void Clear();
    void ClearAndRelease();

private:
    ULONG _iRefCount;
    std::vector<FORMATETC> _pFormatEtc;
    std::vector<STGMEDIUM> _pStgMedium;
    int LookupFormatEtc(FORMATETC *pFormatEtc);
};

//----------------TEnumFormatEtc-----------------------------------------------------------
class TEnumFormatEtc : public IEnumFORMATETC
{
public:
    // IUnknown members
    HRESULT __stdcall  QueryInterface (REFIID iid, void ** ppv)
    {
        if((iid==IID_IUnknown)||(iid==IID_IEnumFORMATETC))
        {
            *ppv=this;
            AddRef();
            return S_OK;
        }
        else
        {
            *ppv=NULL;
            return E_NOINTERFACE;
        }
    }
    ULONG   __stdcall AddRef (void) { return ++_iRefCount; }
    ULONG   __stdcall Release (void)  { if(--_iRefCount==0){delete this;  return 0;} return _iRefCount; }

    // IEnumFormatEtc members
    HRESULT __stdcall  Next  (ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched);
    HRESULT __stdcall  Skip  (ULONG celt)
    {
        _nIndex += celt;
        return (_nIndex <= _nNumFormats) ? S_OK : S_FALSE;
    }
    HRESULT __stdcall  Reset (void)
    {
        _nIndex = 0;
        return S_OK;
    }
    HRESULT __stdcall  Clone (IEnumFORMATETC ** ppEnumFormatEtc)
    {
        HRESULT hResult;
        hResult = CreateEnumFormatEtc(_nNumFormats, _pFormatEtc, ppEnumFormatEtc);
        if(hResult == S_OK)
        {
            ((TEnumFormatEtc *)*ppEnumFormatEtc)->_nIndex = _nIndex;
        }
        return hResult;
    }

    // Construction / Destruction
    TEnumFormatEtc(FORMATETC *pFormatEtc, ULONG nNumFormats);
    ~TEnumFormatEtc();

private:
    LONG        _iRefCount;        // Reference count for this COM interface
    ULONG       _nIndex;           // current enumerator index
    ULONG       _nNumFormats;      // number of FORMATETC members
    FORMATETC * _pFormatEtc;       // array of FORMATETC objects
};
//---------------------------------------------------------------------------
#endif