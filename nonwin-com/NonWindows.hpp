#pragma once
/* Minimal subset of COM defines for compatibility with non-Windows platforms. */

#ifdef _WIN32
#error Header not intended for Windows platform
#endif

#define DECLSPEC_UUID(arg) 
#include <memory>
#include <cassert>
#include <atomic>
#include <vector>
#include <string>
#include <string.h> // for wcsdup
#include <codecvt>
#include <locale>
#include <map>

// trick to add quotes to preprocessor define
#define QUOTE1(x) #x

/** Taken from guiddef.h. */
struct GUID {
    unsigned int   Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
    
    bool operator == (const GUID & other) const {
        return memcmp(this, &other, sizeof(GUID)) == 0;
    }
};
static_assert(sizeof(GUID) == 16, "GUID not packed");

// __uuidof emulation
template<typename Q>
static GUID hold_uuidof () { return {}; }
#define DEFINE_UUIDOF_ID(Q, IID) template<> GUID hold_uuidof<Q>() { return IID; }
#define DEFINE_UUIDOF(Q) DEFINE_UUIDOF_ID(Q, IID_##Q)
#define __uuidof(Q) hold_uuidof<Q>()


typedef unsigned int   DWORD;   ///< 32bit unsigned
typedef bool           BOOL;
typedef unsigned char  BYTE;
typedef unsigned short USHORT;  ///< 16bit unsigned
typedef unsigned int   UINT;    ///< 32bit int
typedef unsigned int   ULONG;   ///< 32bit unsigned (cannot use 'long' since it's 64bit on 64bit Linux)
typedef int            LONG;    ///< 32bit int (cannot use 'long' since it can be 64bit)
typedef wchar_t*       BSTR;    ///< zero terminated double-byte text string
typedef int32_t        HRESULT; ///< 32bit signed int (negative values indicate failure)
static_assert(sizeof(int) == 4, "int size not 32bit");

// Common HRESULT codes
// REF: https://msdn.microsoft.com/en-us/library/windows/desktop/aa378137.aspx
#define S_OK           static_cast<int32_t>(0L)
#define E_BOUNDS       static_cast<int32_t>(0x8000000BL)
#define E_NOTIMPL      static_cast<int32_t>(0x80004001L)
#define E_NOINTERFACE  static_cast<int32_t>(0x80004002L)
#define E_POINTER      static_cast<int32_t>(0x80004003L)
#define E_ABORT        static_cast<int32_t>(0x80004004L)
#define E_FAIL         static_cast<int32_t>(0x80004005L)
#define E_UNEXPECTED   static_cast<int32_t>(0x8000FFFFL)
#define E_ACCESSDENIED static_cast<int32_t>(0x80070005L)
#define E_HANDLE       static_cast<int32_t>(0x80070006L)
#define E_OUTOFMEMORY  static_cast<int32_t>(0x8007000EL)
#define E_INVALIDARG   static_cast<int32_t>(0x80070057L)
#define E_NOT_SET      static_cast<int32_t>(0x80070490L)


enum CLSCTX { 
  CLSCTX_INPROC_SERVER           = 0x1,
  CLSCTX_INPROC_HANDLER          = 0x2,
  CLSCTX_LOCAL_SERVER            = 0x4,
  CLSCTX_REMOTE_SERVER           = 0x10,
  CLSCTX_NO_CODE_DOWNLOAD        = 0x400,
  CLSCTX_NO_CUSTOM_MARSHAL       = 0x1000,
  CLSCTX_ENABLE_CODE_DOWNLOAD    = 0x2000,
  CLSCTX_NO_FAILURE_LOG          = 0x4000,
  CLSCTX_DISABLE_AAA             = 0x8000,
  CLSCTX_ENABLE_AAA              = 0x10000,
  CLSCTX_FROM_DEFAULT_CONTEXT    = 0x20000,
  CLSCTX_ACTIVATE_32_BIT_SERVER  = 0x40000,
  CLSCTX_ACTIVATE_64_BIT_SERVER  = 0x80000,
  CLSCTX_ENABLE_CLOAKING         = 0x100000,
  CLSCTX_APPCONTAINER            = 0x400000,
  CLSCTX_ACTIVATE_AAA_AS_IU      = 0x800000,
  CLSCTX_PS_DLL                  = 0x80000000
};
#define CLSCTX_ALL              (CLSCTX_INPROC_SERVER| \
                                 CLSCTX_INPROC_HANDLER| \
                                 CLSCTX_LOCAL_SERVER| \
                                 CLSCTX_REMOTE_SERVER)


static void CHECK (HRESULT hr) {
    if (hr < 0)
        throw std::logic_error("HRESULT error");
}


class CComBSTR {
public:
    CComBSTR () {
    }
    CComBSTR (const wchar_t* str) {
        if (str)
            m_str = wcsdup(str);
    }    
    CComBSTR (int /*size*/, const wchar_t* str) {
        m_str = wcsdup(str);
    }
    CComBSTR (const CComBSTR & other) {
        m_str = other.Copy();
    }

    ~CComBSTR() {
        Empty();
    }
    
    void operator = (const CComBSTR & other) {
        Empty();
        m_str = other.Copy();
    }

    /** Returns string length excluding null termination. */
    unsigned int Length () const {
        if (!m_str)
            return 0;

        return static_cast<unsigned int>(wcslen(m_str));
    }

    operator wchar_t* () const {
        return m_str;
    }
    
    wchar_t** operator & () {
        return &m_str;
    }
    
    wchar_t* Detach () {
        wchar_t* tmp = m_str;
        m_str = nullptr;
        return tmp;
    }
    
    wchar_t* Copy () const {
        if (!m_str)
            return nullptr;
        
        return wcsdup(m_str);
    }
    
    void Empty() {
        if (!m_str)
            return;
        
        free(m_str);
        m_str = nullptr;
    }

    wchar_t* m_str = nullptr;
};
static_assert(sizeof(CComBSTR) == sizeof(wchar_t*), "CComBSTR size mismatch");


extern "C" {
// REF: https://msdn.microsoft.com/en-us/library/windows/desktop/ms682521.aspx
static constexpr GUID IID_IUnknown = {0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};

/** IUnknown base-class for Non-Windows platforms. */
struct IUnknown {
    IUnknown () : m_ref(0) {}
    
    virtual ~IUnknown() {
        assert(!m_ref && "IUnknown::dtor non-zero ref count.");
    }
    
    /** Cast method. */
    virtual HRESULT QueryInterface (const GUID & iid, /*[out]*/void **obj) = 0;

    virtual ULONG AddRef () {
        assert((m_ref < MAX_REF_COUNT) && "IUnknown::AddRef negative ref count.");
        return ++m_ref;
    }

    virtual ULONG Release () {
        ULONG ref = --m_ref;
        if (!ref)
            delete this;
        assert((m_ref < MAX_REF_COUNT) && "IUnknown::Release negative ref count.");
        return ref;
    }
    
    void _AssertRefCount () const {
        assert((m_ref > 0) && (m_ref < MAX_REF_COUNT) && "IUnknown::_CheckRefCount non-positive ref count.");
    }
    
private:
    std::atomic<ULONG> m_ref;
    static constexpr ULONG MAX_REF_COUNT = 0xFFFF; // 64k ought to be enough
};
} // extern "C"
DEFINE_UUIDOF(IUnknown)


class IUnknownFactory {
public:
    typedef IUnknown* (*Factory)();

    static IUnknown* CreateInstance (std::string class_name) {
        // remove "HostSupport." prefix if present
        size_t idx = class_name.find('.');
        if (idx != std::string::npos)
            class_name = class_name.substr(idx+1);
        
        return Factories().find(class_name)->second();
    }
    
    template <class CLS>
    static bool RegisterClass(const char * class_name) {
        Factories()[class_name] = CreateClass<CLS>;
        return true; // dummy return
    }
    
private:
    template <class CLS>
    static IUnknown* CreateClass () {
        return new CLS;
    }
    
    static std::map<std::string, Factory> & Factories ();
};

#define OBJECT_ENTRY_AUTO(clsid, cls) \
    static bool tmp_factory_##cls = IUnknownFactory::RegisterClass<cls>(#cls);


/** COM smart-pointer object. */
template <class T>
class CComPtr {
public:
    CComPtr (T * ptr = nullptr) : p(ptr) {
        if (p)
            p->AddRef();
    }
    CComPtr (const CComPtr & other) : p(other.p) {
        if (p)
            p->AddRef();
    }
    
    ~CComPtr () {
        if (p)
            p->Release();
        p = nullptr;
    }
    
    void operator = (T * other) {
        if (p != other)
            CComPtr(other).Swap(*this);
    }
    void operator = (const CComPtr & other) {
        if (p != other.p)
            CComPtr(other).Swap(*this);
    }
    template <typename U>
    void operator = (CComPtr<U> & other) {
        CComPtr tmp;
        other->QueryInterface(__uuidof(T), reinterpret_cast<void**>(&tmp));
        Swap(tmp);
    }
    
    template <class Q>
    HRESULT QueryInterface (Q** arg) const {
        if (!p)
            return E_POINTER;
        if (!arg)
            return E_POINTER;
        if (*arg)
            return E_POINTER;
        
        return p->QueryInterface(__uuidof(Q), reinterpret_cast<void**>(arg));
    }
    
    bool IsEqualObject (IUnknown * other) const {
        CComPtr<IUnknown> this_obj = *this;
        return this_obj == other;
    }
    
    HRESULT CopyTo (T** arg) {
        if (!arg)
            return E_POINTER;
        if (*arg)
            return E_POINTER; // input must be pointer to nullptr
                
        *arg = p;
        if (p)
            p->AddRef();
        
        return S_OK;
    }
    
    HRESULT CoCreateInstance (std::string name, IUnknown* outer = NULL, DWORD context = CLSCTX_ALL) {
        CComPtr<IUnknown> tmp1(IUnknownFactory::CreateInstance(name));
        if (!tmp1)
            return E_FAIL;
        
        CComPtr tmp2;
        tmp2 = tmp1; // cast
        if (!tmp2)
            return E_FAIL;
        
        Swap(tmp2);
        return S_OK;
    }
    
    HRESULT CoCreateInstance (std::wstring name, IUnknown* outer = NULL, DWORD context = CLSCTX_ALL) {
        return CoCreateInstance(ToAscii(name), outer, context);
    }
    
    /** Take over ownership (does not incr. ref-count). */
    void Attach (T * ptr) {
        if (p)
            p->Release();
        p = ptr;
        // no AddRef
    }
    /** Release ownership (does not decr. ref-count). */
    T* Detach () {
        T * tmp = p;
        p = nullptr;
        // no Release
        return tmp;
    }
    
    void Release () {
        if (!p)
            return;
        
        p->Release();
        p = nullptr;
    }
    
    operator T*() const {
        return p;
    }
    T* operator -> () const {
        assert(p && "CComPtr::operator -> nullptr.");
        p->_AssertRefCount();
        return p;
    }
    T** operator & () {
        return &p;
    }
    
    T * p = nullptr;

protected:
    void Swap (CComPtr & other) {
        T* tmp = p;
        p = other.p;
        other.p = tmp;
    }    
};

template <class T>
using CComQIPtr = CComPtr<T>;


struct SAFEARRAY {
    enum TYPE {
        TYPE_EMPTY,
        TYPE_DATA,
        TYPE_STRINGS,
        TYPE_POINTERS,
    };
    
    SAFEARRAY (TYPE t) : type(t) {
        assert(t == TYPE_STRINGS || t == TYPE_POINTERS);
    }
    SAFEARRAY (unsigned int _elm_size, unsigned int count) : type(TYPE_DATA), data(_elm_size*count), elm_size(_elm_size) {
    }
    
    ~SAFEARRAY() {
    }
    
    SAFEARRAY () = delete;
    SAFEARRAY (const SAFEARRAY&) = delete;
    SAFEARRAY& operator = (const SAFEARRAY&) = delete;

    const TYPE                     type = TYPE_EMPTY; ///< \todo: Replace with std::variant when upgrading to C++17
    std::vector<unsigned char>     data;
    std::vector<CComBSTR>          strings;
    std::vector<CComPtr<IUnknown>> pointers;
    const unsigned int             elm_size = 0;
};


template <typename T>
struct CComTypeWrapper {
    typedef T type; // default mapping
};
template <>
struct CComTypeWrapper<BSTR> {
    typedef CComBSTR type; // map BSTR/wchar_t* to CComBSTR
};
template <>
struct CComTypeWrapper<IUnknown*> {
    typedef CComPtr<IUnknown> type; // wrap IUnknown* in CComPtr
};

template <class T>
struct CComSafeArray {
    CComSafeArray (UINT size = 0) {
        if (size)
            m_ptr.reset(new SAFEARRAY(sizeof(T), size));
    }
    
    CComSafeArray (SAFEARRAY * obj) {
        m_ptr.reset(obj);
    }
    
    ~CComSafeArray () {
    }
    
    CComSafeArray (const CComSafeArray&) = delete;
    CComSafeArray (CComSafeArray&&) = default;
    CComSafeArray& operator = (const CComSafeArray&) = delete;
    CComSafeArray& operator = (CComSafeArray&&) = default;
    
    HRESULT Attach (SAFEARRAY * obj) {
        m_ptr.reset(obj);
        return S_OK;
    }
    SAFEARRAY* Detach () {
        return m_ptr.release();
    }
    
    operator SAFEARRAY* () {
		return m_ptr.get();
	}
    
    typename CComTypeWrapper<T>::type& GetAt (int idx) const {
        assert(m_ptr);
        assert(m_ptr->type == SAFEARRAY::TYPE_DATA);
        unsigned char * ptr = &m_ptr->data[idx*m_ptr->elm_size];
        return reinterpret_cast<T&>(*ptr);
    }
    
    HRESULT SetAt (int idx, const T& val, bool copy = true) {
        (void)copy; // mute unreferenced argument warning
        assert(m_ptr);
        assert(m_ptr->type == SAFEARRAY::TYPE_DATA);
        assert(sizeof(T) == m_ptr->elm_size);
        unsigned char * ptr = &m_ptr->data[idx*m_ptr->elm_size];
        reinterpret_cast<T&>(*ptr) = val;
        return S_OK;
    }
    
    HRESULT Add (const typename CComTypeWrapper<T>::type& t, BOOL copy = true) {
        (void)copy; // mute unreferenced argument warning
        assert(m_ptr);
        assert(m_ptr->type == SAFEARRAY::TYPE_DATA);
        assert(sizeof(T) == m_ptr->elm_size);
        const size_t prev_size = m_ptr->data.size();
        for (size_t i = 0; i < sizeof(T); ++i)
            m_ptr->data.push_back(0);
        reinterpret_cast<T&>(m_ptr->data[prev_size]) = t;
        return S_OK;
    }
    
    unsigned int GetCount () const {
        assert(m_ptr);
        assert(m_ptr->type == SAFEARRAY::TYPE_DATA);
        return static_cast<unsigned int>(m_ptr->data.size()/m_ptr->elm_size);
    }

    std::unique_ptr<SAFEARRAY> m_ptr;
};
// Template specializations. Implemented in cpp file.
template <> CComSafeArray<BSTR>::CComSafeArray (UINT size);
template <> CComSafeArray<IUnknown*>::CComSafeArray (UINT size);
template <> CComTypeWrapper<BSTR>::type& CComSafeArray<BSTR>::GetAt (int idx) const;
template <> CComTypeWrapper<IUnknown*>::type& CComSafeArray<IUnknown*>::GetAt (int idx) const;
template <> HRESULT                      CComSafeArray<BSTR>::Add (const typename CComTypeWrapper<BSTR>::type& t, BOOL copy);
template <> HRESULT                 CComSafeArray<IUnknown*>::Add (const typename CComTypeWrapper<IUnknown*>::type& t, BOOL copy);
template <> unsigned int CComSafeArray<BSTR>::GetCount () const;
template <> unsigned int CComSafeArray<IUnknown*>::GetCount () const;


#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr)    (((HRESULT)(hr)) < 0)

// COM calling convention (use default on non-Windows)
#define STDMETHODCALLTYPE

#define ATL_NO_VTABLE 

// QueryInterface support macros
#define BEGIN_COM_MAP(CLASS)         IUnknown* GetUnknown () { \
                                         return static_cast<IUnknown*>(this); \
                                     } \
                                     HRESULT QueryInterface (const GUID & iid, /*out*/void **obj) override { \
                                           *obj = nullptr;

#define COM_INTERFACE_ENTRY(INTERFACE) if (iid == __uuidof(INTERFACE)) \
                                           *obj = static_cast<INTERFACE*>(this); \
                                       else
#define END_COM_MAP()                  if (iid == __uuidof(IUnknown)) \
                                           *obj = static_cast<IUnknown*>(this); \
                                       else \
                                           return E_NOINTERFACE; \
                                       AddRef(); \
                                       return S_OK; \
                                     }

#define DECLARE_REGISTRY_RESOURCEID(dummy)


class CComMultiThreadModel {};

template <class ThreadModel>
class CComObjectRootEx {};

template <class T>
class CComObject {
public:
    static HRESULT CreateInstance (CComObject<T> ** arg) {
        assert(arg);
        assert(!*arg);
        
        *arg = new T;
        return S_OK;
    }
};

template <class T, const GUID* pclsid>
class CComCoClass : public CComObject<T> {
};
