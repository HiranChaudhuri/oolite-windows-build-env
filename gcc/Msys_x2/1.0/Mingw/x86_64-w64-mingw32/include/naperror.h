/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _INC_NAPERROR
#define _INC_NAPERROR

#if (_WIN32_WINNT >= 0x0600)

#define NAP_E_INVALID_PACKET            _HRESULT_TYPEDEF_(0x80270001)
#define NAP_E_MISSING_SOH               _HRESULT_TYPEDEF_(0x80270002)
#define NAP_E_CONFLICTING_ID            _HRESULT_TYPEDEF_(0x80270003)
#define NAP_E_NO_CACHED_SOH             _HRESULT_TYPEDEF_(0x80270004)
#define NAP_E_STILL_BOUND               _HRESULT_TYPEDEF_(0x80270005)
#define NAP_E_NOT_REGISTERED            _HRESULT_TYPEDEF_(0x80270006)
#define NAP_E_NOT_INITIALIZED           _HRESULT_TYPEDEF_(0x80270007)
#define NAP_E_MISMATCHED_ID             _HRESULT_TYPEDEF_(0x80270008)
#define NAP_E_NOT_PENDING               _HRESULT_TYPEDEF_(0x80270009)
#define NAP_E_ID_NOT_FOUND              _HRESULT_TYPEDEF_(0x8027000A)
#define NAP_E_MAXSIZE_TOO_SMALL         _HRESULT_TYPEDEF_(0x8027000B)
#define NAP_E_SERVICE_NOT_RUNNING       _HRESULT_TYPEDEF_(0x8027000C)
#define NAP_S_CERT_ALREADY_PRESENT      _HRESULT_TYPEDEF_(0x0027000D)
#define NAP_E_ENTITY_DISABLED           _HRESULT_TYPEDEF_(0x8027000E)
#define NAP_E_NETSH_GROUPPOLICY_ERROR   _HRESULT_TYPEDEF_(0x8027000F)
#define NAP_E_TOO_MANY_CALLS            _HRESULT_TYPEDEF_(0x80270010)

#if (_WIN32_WINNT >= 0x0601)

#define NAP_E_SHV_CONFIG_EXISTED        _HRESULT_TYPEDEF_(0x80270011)
#define NAP_E_SHV_CONFIG_NOT_FOUND      _HRESULT_TYPEDEF_(0x80270012)
#define NAP_E_SHV_TIMEOUT               _HRESULT_TYPEDEF_(0x80270013)

#endif /*(_WIN32_WINNT >= 0x0601)*/

#endif /*(_WIN32_WINNT >= 0x0600)*/

#endif /* _INC_NAPERROR */

