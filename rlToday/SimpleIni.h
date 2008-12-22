// File:    SimpleIni.h
// Library: SimpleIni
// Author:  Brodie Thiesfield <brofield1@jellycan.com>
// Source:  http://code.jellycan.com/simpleini/
// Version: 2.4
//
// INTRODUCTION
// ============
//  This component allows an INI-style configuration file to be used on both
//  Windows and Linux/Unix. It is fast, simple and source code using this
//  component will compile unchanged on either OS.
//
// FEATURES
// ========
//  * public domain, free use in all software (including GPL and commercial)
//  * multi-platform (Windows 95/98/ME/NT/2K/XP/2003, Linux, Unix)
//  * loading and saving of INI-style configuration files
//  * liberal acceptance of file format
//      - key/values with no section
//      - removal of whitespace around sections, keys and values
//  * optional case-insensitive sections and keys (for ASCII characters only)
//  * supports both char or wchar_t programming interfaces
//  * supports both MBCS (system locale) and UTF-8 file encodings
//  * system locale does not need to be UTF-8 on Linux/Unix to load UTF-8 file
//  * support for non-ASCII characters in section, keys, values and comments
//  * support for non-standard character types or file encodings
//    via user-written converter classes
//  * support for adding/modifying values programmatically
//  * compiles cleanly at warning level 4 (Windows/VC.NET 2003), warning level
//    3 (Windows/VC6) and -Wall (Linux/gcc)
//
// USAGE SUMMARY
// =============
//  1. Declare an instance the appropriate class. Note that the following
//     definitions are just shortcuts for commonly used types. Other types
//     (e.g. PRUnichar, unsigned short, unsigned char) are also possible.
//
//      Interface   Case-sensitive  Load UTF-8  Load MBCS   Typedef
//      ---------   --------------  ----------  ---------   ---------------
//      char        No              No *1       Yes         CSimpleIniA
//      char        No              Yes         Yes *2      CSimpleIniGenA
//      char        Yes             Yes         Yes         CSimpleIniCaseA
//      wchar_t     No              Yes         Yes         CSimpleIniW
//      wchar_t     Yes             Yes         Yes         CSimpleIniCaseW
//
//      *1  Only affects Windows. On Windows this uses MBCS functions and
//          so may fold case incorrectly leading to uncertain results.
//      *2  Possible, but better to use CSimpleIniA on Windows.
//
//  2. Call LoadFile() to load and parse the INI configuration file
//
//  3. Access and modify the data of the file using the following functions
//
//      GetAllSections  Return all section names
//      GetAllKeys      Return all key names for a section
//      GetSection      Return all key names and values in a section
//      GetSectionSize  Return the number of keys in a section
//      GetValue        Return a value for a section & key
//      SetValue        Add or update a value for a section & key
//      Delete          Remove a section, or a key from a section
//
//  4. Call Save(), SaveFile() or SaveString() to save the INI configuration
//     data (as necessary)
//
// NOTES
// =====
//  * To compile using Microsoft Visual Studio 6, you need to modify this
//    header file and remove all instances of the "typename" keyword where
//    error C2899 occurs.
//  * To maintain compatibility with Windows 95 and UTF-8 files, you need to
//    either use Microsoft Layer for Unicode, or use the generic UTF-8
//    conversion library by defining SI_USE_GENERIC_CONVERSION.
//  * Linux/Unix systems and Windows compiles using SI_USE_GENERIC_CONVERSION
//    must also compile and link ConvertUTF.c
//  * To load a UTF-8 file on Windows AND expose it with SI_CHAR == char,
//    you must use CSimpleIniGenA instead of CSimpleIniA.
//  * The collation (sorting) order used for sections and keys returned from
//    iterators is NOT DEFINED. If collation order of the text is important
//    then it should be done yourself by either supplying a replacement
//    SI_STRCMP class, or by sorting the strings external to this library.
//  * Usage of the <errno.h> header can be disabled by defining SI_NO_ERRNO.
//    A non-zero value will returned instead of errno to signify a file error.
//  * Usage of the <mbstring.h> header on Windows can be disabled by defining
//    SI_NO_MBCS. This will force the generic comparison functions to be used.
//    i.e. CSimpleIniA == CSimpleIniGenA
//
// PUBLIC DOMAIN LICENCE
// =====================
// The author or authors of this code dedicate any and all copyright interest
// in this code to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and successors. We
// intend this dedication to be an overt act of relinquishment in perpetuity
// of all present and future rights this code under copyright law.
//
// In short, you can do with it whatever you like: use it, modify it,
// distribute it, sell it, delete it, or send it to your mother-in-law.  I
// make no promises or guarantees that this code will work correctly or at all.
// Use it completely at your own risk.

#ifndef INCLUDED_SimpleIni_h
#define INCLUDED_SimpleIni_h

// Windows CE doesn't have errno or MBCS libraries
#ifdef _WIN32_WCE
# ifndef SI_NO_MBCS
#  define SI_NO_MBCS
# endif
# ifndef SI_NO_ERRNO
#  define SI_NO_ERRNO
# endif
#endif

#ifdef _WIN32
# include <windows.h>
# ifndef SI_NO_MBCS
#  include <mbstring.h>
# endif
// Disable these warnings:
//  4127 "conditional expression is constant" as the conversion classes trigger
//  it with the statement if (sizeof(SI_CHAR) == sizeof(char)). This test will
//  be optimized away in a release build.
//  4702 "unreachable code" as the MS STL header causes it in release mode.
//  Again, the code causing the warning will be cleaned up by the compiler.
//  4786 "identifier truncated to 256 characters" as this is thrown hundreds
//  of times VC6 as soon as STL is used.
# pragma warning (disable: 4127 4702 4786)
#else // !_WIN32
# include <wchar.h>
# ifndef SI_USE_GENERIC_CONVERSION
#  define SI_USE_GENERIC_CONVERSION
# endif
#endif // _WIN32

#include <map>
#include <string>
#include <list>

#include <stdio.h>

#ifdef SI_NO_ERRNO
# define errno  1
#else // !SI_NO_ERRNO
# include <errno.h>
#endif // SI_NO_ERRNO

#ifdef SI_USE_GENERIC_CONVERSION
# include "ConvertUTF.h"
#endif // SI_USE_GENERIC_CONVERSION

// ---------------------------------------------------------------------------
//                                  HELPER CLASSES
// ---------------------------------------------------------------------------

#ifdef _WIN32
# define SI_EOL          '\n'
# define SI_NEWLINE_A    "\r\n"
# define SI_NEWLINE_W    L"\r\n"
#else // !_WIN32
# define SI_EOL          '\n'
# define SI_NEWLINE_A    "\n"
# define SI_NEWLINE_W    L"\n"
#endif // _WIN32

/**
 * Generic case-sensitive <less> comparison. This class returns numerically
 * ordered ASCII case-sensitive text for all possible sizes and types of
 * SI_CHAR.
 */
template<class SI_CHAR>
struct SI_Case {
	bool operator()(const SI_CHAR * pLeft, const SI_CHAR * pRight) const {
        long cmp;
        for ( ;*pLeft && *pRight; ++pLeft, ++pRight) {
            cmp = (long) *pLeft - (long) *pRight;
            if (cmp != 0) {
                return cmp < 0;
            }
        }
        return *pRight != 0;
    }
};

/**
 * Generic ASCII case-insensitive <less> comparison. This class returns
 * numerically ordered ASCII case-insensitive text for all possible sizes
 * and types of SI_CHAR. It is not safe for MBCS text comparison where
 * ASCII A-Z characters are used in the encoding of multi-byte characters.
 */
template<class SI_CHAR>
struct SI_NoCase {
    inline SI_CHAR locase(SI_CHAR ch) const {
        return (ch < 'A' || ch > 'Z') ? ch : (ch - 'A' + 'a');
    }
	bool operator()(const SI_CHAR * pLeft, const SI_CHAR * pRight) const {
        long cmp;
        for ( ;*pLeft && *pRight; ++pLeft, ++pRight) {
            cmp = (long) locase(*pLeft) - (long) locase(*pRight);
            if (cmp != 0) {
                return cmp < 0;
            }
        }
        return *pRight != 0;
    }
};

#if defined(_WIN32) && !defined(SI_NO_MBCS)
/**
 * Case-insensitive comparison class using Win32 MBCS functions. This class
 * returns a case-insensitive semi-collation order for MBCS text. It may not
 * be safe for UTF-8 text returned in char format as we don't know what
 * characters will be folded by the function! Therefore, if you are using
 * SI_CHAR == char and SetUnicode(true), then you need to use the generic
 * SI_NoCase class instead.
 */
template<class SI_CHAR>
struct SI_WindowsNoCase {
	bool operator()(const SI_CHAR * pLeft, const SI_CHAR * pRight) const {
        if (sizeof(SI_CHAR) == sizeof(char)) {
            return _mbsicmp((const unsigned char *)pLeft,
                (const unsigned char *)pRight) < 0;
        }
        if (sizeof(SI_CHAR) == sizeof(wchar_t)) {
            return wcsicmp((const wchar_t *)pLeft,
                (const wchar_t *)pRight) < 0;
        }
        return SI_NoCase<SI_CHAR>()(pLeft, pRight);
    }
};
#endif // _WIN32

/**
 * Null conversion class for MBCS/UTF-8 to char (or equivalent).
 */
template<class SI_CHAR, bool CAN_LOAD_UTF8 = true>
class SI_ConvertA {
    bool m_bStoreIsUtf8;
protected:
    SI_ConvertA() { }
public:
    SI_ConvertA(bool a_bStoreIsUtf8) : m_bStoreIsUtf8(a_bStoreIsUtf8) { }

    /* copy and assignment */
    SI_ConvertA(const SI_ConvertA & rhs) { operator=(rhs); }
    SI_ConvertA & operator=(const SI_ConvertA & rhs) {
        m_bStoreIsUtf8 = rhs.m_bStoreIsUtf8;
        return *this;
    }

    /** Calculate the number of SI_CHAR required for converting the input
     * from the storage format. The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  Data in storage format to be converted to SI_CHAR.
     * @param a_uInputDataLen Length of storage format data in bytes. This
     *                      must be the actual length of the data, including
     *                      NULL byte if NULL terminated string is required.
     * @return              Number of SI_CHAR required by the string when
     *                      converted. If there are embedded NULL bytes in the
     *                      input data, only the string up and not including
     *                      the NULL byte will be converted.
     * @return              -1 cast to size_t on a conversion error.
     */
    size_t SizeFromStore(
        const char *    /*a_pInputData*/,
        size_t          a_uInputDataLen)
    {
        // ASCII/MBCS/UTF-8 needs no conversion
        if (m_bStoreIsUtf8 && !CAN_LOAD_UTF8) {
            return (size_t)(-1);
        }
        return a_uInputDataLen;
    }

    /** Convert the input string from the storage format to SI_CHAR.
     * The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  Data in storage format to be converted to SI_CHAR.
     * @param a_uInputDataLen Length of storage format data in bytes. This
     *                      must be the actual length of the data, including
     *                      NULL byte if NULL terminated string is required.
     * @param a_pOutputData Pointer to the output buffer to received the
     *                      converted data.
     * @param a_uOutputDataSize Size of the output buffer in SI_CHAR.
     * @return              true if all of the input data was successfully
     *                      converted.
     */
    bool ConvertFromStore(
        const char *    a_pInputData,
        size_t          a_uInputDataLen,
        SI_CHAR *       a_pOutputData,
        size_t          a_uOutputDataSize)
    {
        // ASCII/MBCS/UTF-8 needs no conversion
        if (m_bStoreIsUtf8 && !CAN_LOAD_UTF8) {
            return false;
        }
        if (a_uInputDataLen > a_uOutputDataSize) {
            return false;
        }
        memcpy(a_pOutputData, a_pInputData, a_uInputDataLen);
        return true;
    }

    /** Calculate the number of char required by the storage format of this
     * data. The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  NULL terminated string to calculate the number of
     *                      bytes required to be converted to storage format.
     * @return              Number of bytes required by the string when
     *                      converted to storage format. This size always
     *                      includes space for the terminating NULL character.
     * @return              -1 cast to size_t on a conversion error.
     */
    size_t SizeToStore(
        const SI_CHAR * a_pInputData)
    {
        // ASCII/MBCS/UTF-8 needs no conversion
        if (m_bStoreIsUtf8 && !CAN_LOAD_UTF8) {
            return (size_t)(-1);
        }
        return strlen((const char *)a_pInputData) + 1;
    }

    /** Convert the input string to the storage format of this data.
     * The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  NULL terminated source string to convert. All of
     *                      the data will be converted including the
     *                      terminating NULL character.
     * @param a_pOutputData Pointer to the buffer to receive the converted
     *                      string.
     * @param a_pOutputDataSize Size of the output buffer in char.
     * @return              true if all of the input data, including the
     *                      terminating NULL character was successfully
     *                      converted.
     */
    bool ConvertToStore(
        const SI_CHAR * a_pInputData,
        char *          a_pOutputData,
        size_t          a_uOutputDataSize)
    {
        if (m_bStoreIsUtf8 && !CAN_LOAD_UTF8) {
            return false;
        }

        // calc input string length (SI_CHAR type and size independent)
        size_t uInputLen = strlen((const char *)a_pInputData) + 1;
        if (uInputLen > a_uOutputDataSize) {
            return false;
        }

        // ascii/UTF-8 needs no conversion
        memcpy(a_pOutputData, a_pInputData, uInputLen);
        return true;
    }
};

#ifdef SI_USE_GENERIC_CONVERSION
/**
 * Converts UTF-8 to a wchar_t (or equivalent) using the Unicode reference
 * library functions. This can be used on all platforms.
 */
template<class SI_CHAR>
class SI_ConvertW {
    bool m_bStoreIsUtf8;
protected:
    SI_ConvertW() { }
public:
    SI_ConvertW(bool a_bStoreIsUtf8) : m_bStoreIsUtf8(a_bStoreIsUtf8) { }

    /* copy and assignment */
    SI_ConvertW(const SI_ConvertW & rhs) { operator=(rhs); }
    SI_ConvertW & operator=(const SI_ConvertW & rhs) {
        m_bStoreIsUtf8 = rhs.m_bStoreIsUtf8;
        return *this;
    }

    /** Calculate the number of SI_CHAR required for converting the input
     * from the storage format. The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  Data in storage format to be converted to SI_CHAR.
     * @param a_uInputDataLen Length of storage format data in bytes. This
     *                      must be the actual length of the data, including
     *                      NULL byte if NULL terminated string is required.
     * @return              Number of SI_CHAR required by the string when
     *                      converted. If there are embedded NULL bytes in the
     *                      input data, only the string up and not including
     *                      the NULL byte will be converted.
     * @return              -1 cast to size_t on a conversion error.
     */
    size_t SizeFromStore(
        const char *    a_pInputData,
        size_t          a_uInputDataLen)
    {
        if (m_bStoreIsUtf8) {
            // worst case scenario for UTF-8 to wchar_t is 1 char -> 1 wchar_t
            // so we just return the same number of characters required as for
            // the source text.
            return a_uInputDataLen;
        }
        else {
            return mbstowcs(NULL, a_pInputData, a_uInputDataLen);
        }
    }

    /** Convert the input string from the storage format to SI_CHAR.
     * The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  Data in storage format to be converted to SI_CHAR.
     * @param a_uInputDataLen Length of storage format data in bytes. This
     *                      must be the actual length of the data, including
     *                      NULL byte if NULL terminated string is required.
     * @param a_pOutputData Pointer to the output buffer to received the
     *                      converted data.
     * @param a_uOutputDataSize Size of the output buffer in SI_CHAR.
     * @return              true if all of the input data was successfully
     *                      converted.
     */
    bool ConvertFromStore(
        const char *    a_pInputData,
        size_t          a_uInputDataLen,
        SI_CHAR *       a_pOutputData,
        size_t          a_uOutputDataSize)
    {
        if (m_bStoreIsUtf8) {
            // This uses the Unicode reference implementation to do the
            // conversion from UTF-8 to wchar_t. The required files are
            // ConvertUTF.h and ConvertUTF.c which should be included in
            // the distribution but are publically available from unicode.org
            // at http://www.unicode.org/Public/PROGRAMS/CVTUTF/
            ConversionResult retval;
            const UTF8 * pUtf8 = (const UTF8 *) a_pInputData;
            if (sizeof(wchar_t) == sizeof(UTF32)) {
                UTF32 * pUtf32 = (UTF32 *) a_pOutputData;
                retval = ConvertUTF8toUTF32(
                    &pUtf8, pUtf8 + a_uInputDataLen,
                    &pUtf32, pUtf32 + a_uOutputDataSize,
                    lenientConversion);
            }
            else if (sizeof(wchar_t) == sizeof(UTF16)) {
                UTF16 * pUtf16 = (UTF16 *) a_pOutputData;
                retval = ConvertUTF8toUTF16(
                    &pUtf8, pUtf8 + a_uInputDataLen,
                    &pUtf16, pUtf16 + a_uOutputDataSize,
                    lenientConversion);
            }
            return retval == conversionOK;
        }
        else {
            size_t retval = mbstowcs(a_pOutputData,
                a_pInputData, a_uOutputDataSize);
            return retval != (size_t)(-1);
        }
    }

    /** Calculate the number of char required by the storage format of this
     * data. The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  NULL terminated string to calculate the number of
     *                      bytes required to be converted to storage format.
     * @return              Number of bytes required by the string when
     *                      converted to storage format. This size always
     *                      includes space for the terminating NULL character.
     * @return              -1 cast to size_t on a conversion error.
     */
    size_t SizeToStore(
        const SI_CHAR * a_pInputData)
    {
        if (m_bStoreIsUtf8) {
            // worst case scenario for wchar_t to UTF-8 is 1 wchar_t -> 6 char
            size_t uLen = 0;
            while (a_pInputData[uLen]) {
                ++uLen;
            }
            return (6 * uLen) + 1;
        }
        else {
            size_t uLen = wcstombs(NULL, a_pInputData, 0);
            if (uLen == (size_t)(-1)) {
                return uLen;
            }
            return uLen + 1; // include NULL terminator
        }
    }

    /** Convert the input string to the storage format of this data.
     * The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  NULL terminated source string to convert. All of
     *                      the data will be converted including the
     *                      terminating NULL character.
     * @param a_pOutputData Pointer to the buffer to receive the converted
     *                      string.
     * @param a_pOutputDataSize Size of the output buffer in char.
     * @return              true if all of the input data, including the
     *                      terminating NULL character was successfully
     *                      converted.
     */
    bool ConvertToStore(
        const SI_CHAR * a_pInputData,
        char *          a_pOutputData,
        size_t          a_uOutputDataSize)
    {
        if (m_bStoreIsUtf8) {
            // calc input string length (SI_CHAR type and size independent)
            size_t uInputLen = 0;
            while (a_pInputData[uInputLen]) {
                ++uInputLen;
            }
            ++uInputLen; // include the NULL char

            // This uses the Unicode reference implementation to do the
            // conversion from wchar_t to UTF-8. The required files are
            // ConvertUTF.h and ConvertUTF.c which should be included in
            // the distribution but are publically available from unicode.org
            // at http://www.unicode.org/Public/PROGRAMS/CVTUTF/
            ConversionResult retval;
            UTF8 * pUtf8 = (UTF8 *) a_pOutputData;
            if (sizeof(wchar_t) == sizeof(UTF32)) {
                const UTF32 * pUtf32 = (const UTF32 *) a_pInputData;
                retval = ConvertUTF32toUTF8(
                    &pUtf32, pUtf32 + uInputLen + 1,
                    &pUtf8, pUtf8 + a_uOutputDataSize,
                    lenientConversion);
            }
            else if (sizeof(wchar_t) == sizeof(UTF16)) {
                const UTF16 * pUtf16 = (const UTF16 *) a_pInputData;
                retval = ConvertUTF16toUTF8(
                    &pUtf16, pUtf16 + uInputLen + 1,
                    &pUtf8, pUtf8 + a_uOutputDataSize,
                    lenientConversion);
            }
            return retval == conversionOK;
        }
        else {
            size_t retval = wcstombs(a_pOutputData,
                a_pInputData, a_uOutputDataSize);
            return retval != (size_t) -1;
        }
    }
};

#else // !SI_USE_GENERIC_CONVERSION

/**
 * Converts MBCS and UTF-8 to a wchar_t (or equivalent) on Windows. This uses
 * only the Win32 functions and doesn't require the external Unicode UTF-8
 * conversion library. It will not work on Windows 95 without using Microsoft
 * Layer for Unicode in your application.
 */
template<class SI_CHAR>
class SI_ConvertW {
    UINT m_uCodePage;
protected:
    SI_ConvertW() { }
public:
    SI_ConvertW(bool a_bStoreIsUtf8) {
        m_uCodePage = a_bStoreIsUtf8 ? CP_UTF8 : CP_ACP;
    }

    /* copy and assignment */
    SI_ConvertW(const SI_ConvertW & rhs) { operator=(rhs); }
    SI_ConvertW & operator=(const SI_ConvertW & rhs) {
        m_uCodePage = rhs.m_uCodePage;
        return *this;
    }

    /** Calculate the number of SI_CHAR required for converting the input
     * from the storage format. The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  Data in storage format to be converted to SI_CHAR.
     * @param a_uInputDataLen Length of storage format data in bytes. This
     *                      must be the actual length of the data, including
     *                      NULL byte if NULL terminated string is required.
     * @return              Number of SI_CHAR required by the string when
     *                      converted. If there are embedded NULL bytes in the
     *                      input data, only the string up and not including
     *                      the NULL byte will be converted.
     * @return              -1 cast to size_t on a conversion error.
     */
    size_t SizeFromStore(
        const char *    a_pInputData,
        size_t          a_uInputDataLen)
    {
        int retval = MultiByteToWideChar(
            m_uCodePage, 0,
            a_pInputData, (int) a_uInputDataLen,
            0, 0);
        return (size_t)(retval > 0 ? retval : -1);
    }

    /** Convert the input string from the storage format to SI_CHAR.
     * The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  Data in storage format to be converted to SI_CHAR.
     * @param a_uInputDataLen Length of storage format data in bytes. This
     *                      must be the actual length of the data, including
     *                      NULL byte if NULL terminated string is required.
     * @param a_pOutputData Pointer to the output buffer to received the
     *                      converted data.
     * @param a_uOutputDataSize Size of the output buffer in SI_CHAR.
     * @return              true if all of the input data was successfully
     *                      converted.
     */
    bool ConvertFromStore(
        const char *    a_pInputData,
        size_t          a_uInputDataLen,
        SI_CHAR *       a_pOutputData,
        size_t          a_uOutputDataSize)
    {
        int nSize = MultiByteToWideChar(
            m_uCodePage, 0,
            a_pInputData, (int) a_uInputDataLen,
            (wchar_t *) a_pOutputData, (int) a_uOutputDataSize);
        return (nSize > 0);
    }

    /** Calculate the number of char required by the storage format of this
     * data. The storage format is always UTF-8.
     *
     * @param a_pInputData  NULL terminated string to calculate the number of
     *                      bytes required to be converted to storage format.
     * @return              Number of bytes required by the string when
     *                      converted to storage format. This size always
     *                      includes space for the terminating NULL character.
     * @return              -1 cast to size_t on a conversion error.
     */
    size_t SizeToStore(
        const SI_CHAR * a_pInputData)
    {
        int retval = WideCharToMultiByte(
            m_uCodePage, 0,
            (const wchar_t *) a_pInputData, -1,
            0, 0, 0, 0);
        return (size_t) (retval > 0 ? retval : -1);
    }

    /** Convert the input string to the storage format of this data.
     * The storage format is always UTF-8 or MBCS.
     *
     * @param a_pInputData  NULL terminated source string to convert. All of
     *                      the data will be converted including the
     *                      terminating NULL character.
     * @param a_pOutputData Pointer to the buffer to receive the converted
     *                      string.
     * @param a_pOutputDataSize Size of the output buffer in char.
     * @return              true if all of the input data, including the
     *                      terminating NULL character was successfully
     *                      converted.
     */
    bool ConvertToStore(
        const SI_CHAR * a_pInputData,
        char *          a_pOutputData,
        size_t          a_uOutputDataSize)
    {
        int retval = WideCharToMultiByte(
            m_uCodePage, 0,
            (const wchar_t *) a_pInputData, -1,
            a_pOutputData, (int) a_uOutputDataSize, 0, 0);
        return retval > 0;
    }
};
#endif // SI_USE_GENERIC_CONVERSION

// ---------------------------------------------------------------------------
//                              MAIN TEMPLATE CLASS
// ---------------------------------------------------------------------------

/**
 * Simple INI file reader. This can be instantiated with the choice of unicode
 * or native characterset, and case sensitive or insensitive comparisons of
 * section and key names. The supported combinations are pre-defined with the
 * following typedefs:
 *
 *  Interface   Case-sensitive      Typedef
 *  ---------   --------------      ---------------
 *  char        No                  CSimpleIniA
 *  char        Yes                 CSimpleIniCaseA
 *  wchar_t     No                  CSimpleIniW
 *  wchar_t     Yes                 CSimpleIniCaseW
 *
 * Note that using other types for the SI_CHAR is supported. For instance,
 * unsigned char, unsigned short, etc. Note that where the alternative type
 * is a different size to char/wchar_t you may need to supply new helper
 * classes for SI_STRCMP and SI_CONVERTER.
 */
template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
class CSimpleIniTempl
{
public:
    /** map keys to values */
    typedef std::map<const SI_CHAR *,const SI_CHAR *,SI_STRCMP> TKeyVal;

    /** map sections to key/value map */
    typedef std::map<const SI_CHAR *,TKeyVal,SI_STRCMP>         TSection;

    /** vector of dependent string pointers. Note that these pointers are
     * dependent on memory owned by CSimpleIni. */
    typedef std::list<const SI_CHAR *>                          TNamesDepend;

    /** interface definition for the OutputWriter object to pass to Save()
     * in order to output the INI file data. */
    class OutputWriter {
    public:
        OutputWriter() { }
        virtual ~OutputWriter() { }
        virtual void Write(const char * a_pBuf) = 0;
    };

    /** OutputWriter class to write the INI data to a file */
    class FileWriter : public OutputWriter {
        FILE * m_file;
    public:
        FileWriter(FILE * a_file) : m_file(a_file) { }
        void Write(const char * a_pBuf) {
            fputs(a_pBuf, m_file);
        }
    };

    /** OutputWriter class to write the INI data to a string */
    class StringWriter : public OutputWriter {
        std::string & m_string;
    public:
        StringWriter(std::string & a_string) : m_string(a_string) { }
        void Write(const char * a_pBuf) {
            m_string.append(a_pBuf);
        }
    };

    /** Characterset conversion utility class to convert strings to the
     * same format as is used for the storage. */
    class Converter : private SI_CONVERTER {
    public:
        Converter(bool a_bStoreIsUtf8) : SI_CONVERTER(a_bStoreIsUtf8) {
            m_scratch.resize(1024);
        }
        Converter(const Converter & rhs) { operator=(rhs); }
        Converter & operator=(const Converter & rhs) {
            m_scratch = rhs.m_scratch;
            return *this;
        }
        bool ConvertToStore(const SI_CHAR * a_pszString) {
            size_t uLen = SizeToStore(a_pszString);
            if (uLen == (size_t)(-1)) {
                return false;
            }
            while (uLen > m_scratch.size()) {
                m_scratch.resize(m_scratch.size() * 2);
            }
            return SI_CONVERTER::ConvertToStore(
                a_pszString,
                const_cast<char*>(m_scratch.data()),
                m_scratch.size());
        }
        const char * Data() { return m_scratch.data(); }
    private:
        std::string m_scratch;
    };

public:
    /**
     * Default constructor. 
     * 
     * @param a_bIsUtf8       See the method SetUnicode() for details.
     */
    CSimpleIniTempl(bool a_bIsUtf8 = false);

    /**
     * Destructor
     */
    ~CSimpleIniTempl();

    /**
     * Deallocate all memory stored by this object
     */
    void Reset();

    /**
     * Set the storage format of the INI data. This affects both the loading
     * and saving of the INI data using all of the Load/Save API functions.
     * This value cannot be changed after any INI data has been loaded.
     *
     * If the file is not set to Unicode (UTF-8), then the data encoding is
     * assumed to be the OS native encoding. This encoding is the system
     * locale on Linux/Unix and the legacy MBCS encoding on Windows NT/2K/XP.
     * If the storage format is set to Unicode then the file will be loaded
     * as UTF-8 encoded data regardless of the native file encoding. If
     * SI_CHAR == char then all of the char* parameters take and return UTF-8
     * encoded data regardless of the system locale.
     */
    void SetUnicode(bool a_bIsUtf8 = true) {
        if (!m_pData) m_bStoreIsUtf8 = a_bIsUtf8;
    }

    /**
     * Get the storage format of the INI data.
     */
    bool IsUnicode() const { return m_bStoreIsUtf8; }

    /**
     * Load an INI file from disk into memory
     *
     * @param a_pszFile     Path of the file to be loaded. This will be passed
     *                      to fopen() and so must be a valid path for the
     *                      current platform.
     *
     * @return 0            Success, no error
     * @return errno        Error codes from errno
     */
    int LoadFile(const char * a_pszFile);

#ifdef _WIN32
    /**
     * Load an INI file from disk into memory
     *
     * @param a_pwszFile    Path of the file to be loaded in UTF-16. This will
     *                      be passed to _wfopen() on Windows. There is no
     *                      wchar_t fopen function on Linux/Unix so this
     *                      function is not supported there.
     *
     * @return 0            Success, no error
     * @return errno        Error codes from errno
     */
    int LoadFile(const wchar_t * a_pwszFile);
#endif // _WIN32

    /**
     * Load INI file data direct from memory
     *
     * @param a_pData       Data to be loaded
     * @param a_uDataLen    Length of the data in bytes
     *
     * @return 0            Success, no error
     * @return errno        Error codes from errno
     */
    int LoadFile(
        const char *    a_pData,
        size_t          a_uDataLen);

    /**
     * Save the INI data. The data will be written to the output device
     * in a format appropriate to the current data, selected by:
     *
     *      SI_CHAR     FORMAT
     *      char        same format as when loaded (MBCS or UTF-8)
     *      wchar_t     UTF-8 (no BOM)
     *      other       UTF-8 (no BOM)
     *
     * Note that comments, etc from the original data are not saved. Only the
     * valid data contents stored in the file are written out. Any data
     * prepended or appended to the output device should use the same format
     * (MBCS or UTF-8) as this data, use the GetConverter() method to convert
     * text to the correct format.
     */
    int Save(OutputWriter & a_oOutput) const;

    /**
     * Save the INI data to a file. See Save() for details.
     *
     * @param a_pFile       Handle to a file. File should be opened for
     *                      binary output.
     */
    int SaveFile(FILE * a_pFile) const {
        FileWriter writer(a_pFile);
        return Save(writer);
    }

    /**
     * Save the INI data to a file. See Save() for details.
     *
     * @param a_sBuffer     String to have the INI data appended to.
     */
    int SaveString(std::string & a_sBuffer) const {
        StringWriter writer(a_sBuffer);
        return Save(writer);
    }

    /**
     * Retrieve the value for a specific key.
     *
     * NOTE! The returned value is a pointer to string data stored in memory
     * owned by CSimpleIni. Ensure that the CSimpleIni object is not destroyed
     * or Reset while you are using this pointer!
     *
     * @param a_pSection        Section to search
     * @param a_pKey            Key to search for
     * @param a_pDefault        Value to return if the key is not found
     *
     * @return a_pDefault       Key was not found in the section
     * @return other            Value of the key
     */
    const SI_CHAR * GetValue(
        const SI_CHAR * a_pSection,
        const SI_CHAR * a_pKey,
        const SI_CHAR * a_pDefault = 0 ) const;

    /**
     * Return values from SetValue().
     */
    enum {
        UPDATED = 1,     /*!< An existing value was updated */
        INSERTED = 2     /*!< A new value as inserted */
    };

    /**
     * Add or update a section or value.
     *
     * @param a_pSection        Section to add or update
     * @param a_pKey            Key to add or update. Set to NULL to
     *                          create an empty section.
     * @param a_pValue          Value to set. Set to NULL to create an
     *                          empty section.
     *
     * @return 0                Failed to add/update the value
     * @return UPDATED          Value was updated
     * @return INSERTED         Value was inserted
     */
    int SetValue(
        const SI_CHAR * a_pSection,
        const SI_CHAR * a_pKey,
        const SI_CHAR * a_pValue)
    {
        return AddEntry(a_pSection, a_pKey, a_pValue, true);
    }

    /**
     * Delete an entire section, or a key from a section. Note that the
     * data returned by GetSection is invalid and must not be used after
     * anything has been deleted from that section using this method.
     *
     * @param a_pSection        Section to delete key from, or if
     *                          a_pKey is NULL, the section to remove.
     * @param a_pKey            Key to remove from the section. Set to
     *                          NULL to remove the entire section.
     * @param a_bRemoveEmpty    If the section is empty after this key has
     *                          been deleted, should the empty section be
     *                          removed?
     *
     * @return true             Key or section was deleted.
     * @return false            Key or section was not found.
     */
    bool Delete(
        const SI_CHAR * a_pSection,
        const SI_CHAR * a_pKey,
        bool            a_bRemoveEmpty = false);

    /**
     * Query the number of entries in a specific section
     *
     * @param a_pSection        Section to request data for
     *
     * @return -1               Section does not exist in the file
     * @return >=0              Number of keys in the section
     */
    int GetSectionSize(
        const SI_CHAR * a_pSection ) const;

    /**
     * Retrieve all key and value pairs for a section. The data is returned
     * as a pointer to an STL map and can be iterated or searched as
     * desired.
     *
     * NOTE! This structure contains only pointers to strings. The actual
     * string data is stored in memory owned by CSimpleIni. Ensure that the
     * CSimpleIni object is not destroyed or Reset() while these strings
     * are in use!
     *
     * @param a_pSection        Name of the section to return
     * @param a_pData           Pointer to the section data.
     * @return boolean          Was a section matching the supplied
     *                          name found.
     */
    const TKeyVal * GetSection(
        const SI_CHAR * a_pSection) const;

    /**
     * Retrieve all section names. The list is returned as an STL vector of
     * names and can be iterated or searched as necessary. Note that the
     * collation order of the returned strings is NOT DEFINED.
     *
     * NOTE! This structure contains only pointers to strings. The actual
     * string data is stored in memory owned by CSimpleIni. Ensure that the
     * CSimpleIni object is not destroyed or Reset() while these strings
     * are in use!
     *
     * @param a_names           Vector that will receive all of the section
     *                          names. See note above!
     */
    void GetAllSections(
        TNamesDepend & a_names ) const;

    /**
     * Retrieve all key names in a section. The list is returned as an STL
     * vector of names and can be iterated or searched as necessary. Note
     * that the collation order of the returned strings is NOT DEFINED.
     *
     * NOTE! This structure contains only pointers to strings. The actual
     * string data is stored in memory owned by CSimpleIni. Ensure that the
     * CSimpleIni object is not destroyed or Reset() while these strings
     * are in use!
     *
     * @param a_pSection        Section to request data for
     * @param a_names           Vector that will receive all of the key
     *                          names. See note above!
     */
    void GetAllKeys(
        const SI_CHAR * a_pSection,
        TNamesDepend &  a_names ) const;

    /**
     * Return a conversion object to convert text to the same encoding
     * as is used by the Save(), SaveFile() and SaveString() functions.
     * Use this to prepare the strings that you wish to append or prepend
     * to the output INI data.
     */
    Converter GetConverter() const {
        return Converter(m_bStoreIsUtf8);
    }

private:
    /** Load the file from a file pointer. */
    int LoadFile(FILE * a_fpFile);

    /** Parse the data looking for the next valid entry. The memory pointed to
     * by a_pData is modified by inserting NULL characters. The pointer is
     * updated to the current location in the block of text. */
    bool FindEntry(
        SI_CHAR *&  a_pData,
        const SI_CHAR *&  a_pSection,
        const SI_CHAR *&  a_pKey,
        const SI_CHAR *&  a_pVal ) const;

    /** Add the section/key/value to our data. Strings will be copied only
     * if a_bCopyStrings is set to true, otherwise the pointers will be
     * used as is. */
    int AddEntry(
        const SI_CHAR * a_pSection,
        const SI_CHAR * a_pKey,
        const SI_CHAR * a_pValue,
        bool            a_bCopyStrings);

    /** Is the supplied character a whitespace character? */
    inline bool IsSpace(SI_CHAR ch) const {
        return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n');
    }

    /** Does the supplied character start a comment line? */
    inline bool IsComment(SI_CHAR ch) const {
        return (ch == ';' || ch == '#');
    }

    /** Make a copy of the supplied string, replacing the original pointer. */
    bool CopyString(const SI_CHAR *& a_pString);

    /** Delete a string from the copied strings buffer if necessary. */
    void DeleteString(const SI_CHAR * a_pString);

private:
    /** Copy of the INI file data in our character format. This will be
     * modified when parsed to have NULL characters added after all
     * interesting string entries. All of the string pointers to sections,
     * keys and values point into this block of memory.
     */
    SI_CHAR * m_pData;

    /** Length of the data that we have stored. Used when deleting strings
     * to determine if the string is stored here or in the allocated string
     * buffer.
     */
    size_t m_uDataLen;

    /** Parsed INI data. Section -> (Key -> Value). */
    TSection m_data;

    /** This vector stores allocated memory for copies of strings that have
     * been supplied after the file load. It will be empty unless SetValue()
     * has been called.
     */
    TNamesDepend m_strings;

    /** Is the format of our datafile UTF-8 or MBCS? */
    bool m_bStoreIsUtf8;
};

// ---------------------------------------------------------------------------
//                                  IMPLEMENTATION
// ---------------------------------------------------------------------------

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::CSimpleIniTempl(bool a_bIsUtf8)
    : m_pData(0), m_uDataLen(0), m_bStoreIsUtf8(a_bIsUtf8)
{ }

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::~CSimpleIniTempl()
{
    Reset();
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
void
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::Reset()
{
    // remove all data
    delete[] m_pData;
    m_pData = 0;
    m_uDataLen = 0;
    if (!m_data.empty()) {
        m_data.erase(m_data.begin(), m_data.end());
    }

    // remove all strings
    if (!m_strings.empty()) {
        typename TNamesDepend::iterator i = m_strings.begin();
        for (; i != m_strings.end(); ++i) {
            delete[] const_cast<SI_CHAR*>(*i);
        }
        m_strings.erase(m_strings.begin(), m_strings.end());
    }
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
int
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::LoadFile(
    const char *    a_pszFile)
{
    FILE * fp = fopen(a_pszFile, "rb");
    if (!fp) {
        return errno;
    }
    int retval = LoadFile(fp);
    fclose(fp);
    return retval;
}

#ifdef _WIN32
template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
int
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::LoadFile(
    const wchar_t * a_pwszFile)
{
    FILE * fp = _wfopen(a_pwszFile, L"rb");
    if (!fp) {
        return errno;
    }
    int retval = LoadFile(fp);
    fclose(fp);
    return retval;
}
#endif // _WIN32

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
int
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::LoadFile(
    FILE *  a_fpFile)
{
    // load the raw file data
    int retval = fseek(a_fpFile, 0, SEEK_END);
    if (retval != 0) {
        return errno;
    }
    long lSize = ftell(a_fpFile);
    if (lSize < 0) {
        return errno;
    }
    char * pData = new char[lSize];
    fseek(a_fpFile, 0, SEEK_SET);
    size_t uRead = fread(pData, sizeof(char), lSize, a_fpFile);
    if (uRead != (size_t) lSize) {
        delete[] pData;
        return -1; //TODO: real error here
    }

    // convert the raw data to unicode
    retval = LoadFile(pData, uRead);
    delete[] pData;

    return retval;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
int
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::LoadFile(
    const char *    a_pData,
    size_t          a_uDataLen)
{
    SI_CONVERTER converter(m_bStoreIsUtf8);

    // determine the length of the converted data
    size_t uLen = converter.SizeFromStore(a_pData, a_uDataLen);
    if (uLen == (size_t)(-1)) {
        return 12;
    }

    // allocate memory for the data, ensure that there is a NULL
    // terminator wherever the converted data ends
    SI_CHAR * pData = new SI_CHAR[uLen+1];
    if (!pData) {
        return 22;
    }
    memset(pData, 0, sizeof(SI_CHAR)*(uLen+1));

    // convert the data
    if (!converter.ConvertFromStore(a_pData, a_uDataLen, pData, uLen)) {
        delete[] pData;
        return 12;
    }

    // parse it
    const static SI_CHAR empty = 0;
    SI_CHAR * pWork = pData;
    const SI_CHAR * pSection = &empty;
    const SI_CHAR * pKey = 0;
    const SI_CHAR * pVal = 0;

    // add every entry in the file to the data table. We copy the strings if
    // we are loading data into this class when we already have stored some
    // because we only store a single block.
    bool bCopyStrings = (m_pData != 0);
    while (FindEntry(pWork, pSection, pKey, pVal)) {
        AddEntry(pSection, pKey, pVal, bCopyStrings);
    }

    // store these strings if we didn't copy them
    if (bCopyStrings) {
        delete[] pData;
    }
    else {
        m_pData = pData;
        m_uDataLen = uLen+1;
    }

    return 0;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
bool
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::FindEntry(
    SI_CHAR *&        a_pData,
    const SI_CHAR *&  a_pSection,
    const SI_CHAR *&  a_pKey,
    const SI_CHAR *&  a_pVal ) const
{
    SI_CHAR * pTrail;
    while (*a_pData) {
        // skip spaces and empty lines
        while (*a_pData && IsSpace(*a_pData)) {
            ++a_pData;
        }
        if (!*a_pData) {
            break;
        }

        // skip comment lines
        if (IsComment(*a_pData)) {
            while (*a_pData && *a_pData != SI_EOL) {
                ++a_pData;
            }
            continue;
        }

        // process section names
        if (*a_pData == '[') {
            // skip leading spaces
            ++a_pData;
            while (*a_pData && IsSpace(*a_pData)) {
                ++a_pData;
            }

            // find the end of the section name (it may contain spaces)
            // and convert it to lowercase as necessary
            a_pSection = a_pData;
            while (*a_pData && *a_pData != ']' && *a_pData != SI_EOL) {
                ++a_pData;
            }

            // if it's an invalid line, just skip it
            if (*a_pData != ']') {
                continue;
            }

            // remove trailing spaces from the section
            pTrail = a_pData - 1;
            while (pTrail >= a_pSection && IsSpace(*pTrail)) {
                --pTrail;
            }
            ++pTrail;
            *pTrail = 0;

            // skip to the end of the line
            ++a_pData;  // safe as checked that it == ']' above
            while (*a_pData && *a_pData != SI_EOL) {
                ++a_pData;
            }

            a_pKey = 0;
            a_pVal = 0;
            return true;
        }

        // find the end of the key name (it may contain spaces)
        // and convert it to lowercase as necessary
        a_pKey = a_pData;
        while (*a_pData && *a_pData != '=' && *a_pData != SI_EOL) {
            ++a_pData;
        }

        // if it's an invalid line, just skip it
        if (*a_pData != '=') {
            continue;
        }

        // empty keys are invalid
        if (a_pKey == a_pData) {
            while (*a_pData && *a_pData != SI_EOL) {
                ++a_pData;
            }
            continue;
        }

        // remove trailing spaces from the key
        pTrail = a_pData - 1;
        while (pTrail >= a_pKey && IsSpace(*pTrail)) {
            --pTrail;
        }
        ++pTrail;
        *pTrail = 0;

        // skip leading whitespace on the value
        ++a_pData;  // safe as checked that it == '=' above
        while (*a_pData && *a_pData != SI_EOL && IsSpace(*a_pData)) {
            ++a_pData;
        }

        // find the end of the value which is the end of this line
        a_pVal = a_pData;
        while (*a_pData && *a_pData != SI_EOL) {
            ++a_pData;
        }

        // remove trailing spaces from the value
        pTrail = a_pData - 1;
        if (*a_pData) { // prepare for the next round
            ++a_pData;
        }
        while (pTrail >= a_pVal && IsSpace(*pTrail)) {
            --pTrail;
        }
        ++pTrail;
        *pTrail = 0;

        // return the entry
        return true;
    }

    return false;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
bool
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::CopyString(
    const SI_CHAR *& a_pString)
{
    size_t uLen = 0;
    if (sizeof(SI_CHAR) == sizeof(char)) {
        uLen = strlen((const char *)a_pString);
    }
    else if (sizeof(SI_CHAR) == sizeof(wchar_t)) {
        uLen = wcslen((const wchar_t *)a_pString);
    }
    else {
        for ( ; a_pString[uLen]; ++uLen) /*loop*/ ;
    }
    ++uLen; // NULL character
    SI_CHAR * pCopy = new SI_CHAR[uLen];
    if (!pCopy) {
        return false;
    }
    memcpy(pCopy, a_pString, sizeof(SI_CHAR)*uLen);
    m_strings.push_back(pCopy);
    a_pString = pCopy;
    return true;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
int
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::AddEntry(
    const SI_CHAR * a_pSection,
    const SI_CHAR * a_pKey,
    const SI_CHAR * a_pValue,
    bool            a_bCopyStrings)
{
    bool bInserted = false;

    // check for existence of the section first if we need string copies
    typename TSection::iterator iSection = m_data.end();
    if (a_bCopyStrings) {
        iSection = m_data.find(a_pSection);
        if (iSection == m_data.end()) {
            // if the section doesn't exist then we need a copy as the
            // string needs to last beyond the end of this function
            // because we will be inserting the section next
            if (!CopyString(a_pSection)) {
                return 0;
            }
        }
    }

    // create the section entry
    if (iSection == m_data.end()) {
        std::pair< TSection::iterator,bool> i =
            m_data.insert(
                 TSection::value_type( a_pSection, TKeyVal() ) );
        iSection = i.first;
        bInserted = true;
    }
    if (!a_pKey || !a_pValue) {
        // section only entries are specified with pKey and pVal as NULL
        return bInserted ? INSERTED : UPDATED;
    }

    // check for existence of the key first if we need string copies
    TKeyVal & keyval = iSection->second;
    typename TKeyVal::iterator iKey = keyval.end();
    if (a_bCopyStrings) {
        iKey = keyval.find(a_pKey);
        if (iKey == keyval.end()) {
            // if the key doesn't exist then we need a copy as the
            // string needs to last beyond the end of this function
            // because we will be inserting the key next
            if (!CopyString(a_pKey)) {
                return 0;
            }
        }

        // we always need a copy of the value
        if (!CopyString(a_pValue)) {
            return 0;
        }
    }

    // create the key entry
    if (iKey == keyval.end()) {
        std::pair<TKeyVal::iterator,bool> i =
            keyval.insert( TKeyVal::value_type( a_pKey, 0 ) );
        iKey = i.first;
        bInserted = true;
    }
    iKey->second = a_pValue;
    return bInserted ? INSERTED : UPDATED;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
const SI_CHAR *
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::GetValue(
    const SI_CHAR * a_pSection,
    const SI_CHAR * a_pKey,
    const SI_CHAR * a_pDefault ) const
{
    if (!a_pSection || !a_pKey) {
        return a_pDefault;
    }
    typename TSection::const_iterator i = m_data.find(a_pSection);
    if (i == m_data.end()) {
        return a_pDefault;
    }
    typename TKeyVal::const_iterator j = i->second.find(a_pKey);
    if (j == i->second.end()) {
        return a_pDefault;
    }
    return j->second;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
int
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::GetSectionSize(
    const SI_CHAR * a_pSection ) const
{
    if (a_pSection) {
        typename TSection::const_iterator i = m_data.find(a_pSection);
        if (i != m_data.end()) {
            return (int) i->second.size();
        }
    }
    return -1;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
const typename CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::TKeyVal *
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::GetSection(
    const SI_CHAR * a_pSection) const
{
    if (a_pSection) {
        typename TSection::const_iterator i = m_data.find(a_pSection);
        if (i != m_data.end()) {
            return &(i->second);
        }
    }
    return 0;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
void
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::GetAllSections(
    TNamesDepend & a_names ) const
{
    typename TSection::const_iterator i = m_data.begin();
    for (int n = 0; i != m_data.end(); ++i, ++n ) {
        a_names.push_back(i->first);
    }
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
void
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::GetAllKeys(
    const SI_CHAR * a_pSection,
    TNamesDepend &  a_names ) const
{
    if (a_pSection) {
        typename TSection::const_iterator i = m_data.find(a_pSection);
        if (i != m_data.end()) {
            const TKeyVal & section = i->second;
            typename TKeyVal::const_iterator i = section.begin();
            for (int n = 0; i != section.end(); ++i, ++n ) {
                a_names.push_back(i->first);
            }
        }
    }
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
int
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::Save(
    OutputWriter & a_oOutput) const
{
    Converter convert(m_bStoreIsUtf8);

    // iterate through our sections and output the data
    bool bFirstLine = true;
    typename TSection::const_iterator iSection = m_data.begin();
    for ( ; iSection != m_data.end(); ++iSection ) {
        // write the section (unless there is no section name)
        if (iSection->first[0]) {
            if (!convert.ConvertToStore(iSection->first)) {
                return 155;//ERANGE;
            }
            if (!bFirstLine) {
                a_oOutput.Write(SI_NEWLINE_A);
            }
            a_oOutput.Write("[");
            a_oOutput.Write(convert.Data());
            a_oOutput.Write("]");
            a_oOutput.Write(SI_NEWLINE_A);
        }

        // write all keys and values
        typename TKeyVal::const_iterator iKeyVal = iSection->second.begin();
        for ( ; iKeyVal != iSection->second.end(); ++iKeyVal) {
            // write the key
            if (!convert.ConvertToStore(iKeyVal->first)) {
                return 155;//ERANGE;
            }
            a_oOutput.Write(convert.Data());

            // write the value
            if (!convert.ConvertToStore(iKeyVal->second)) {
                return 155;//ERANGE;
            }
            a_oOutput.Write("=");
            a_oOutput.Write(convert.Data());
            a_oOutput.Write(SI_NEWLINE_A);
        }

        bFirstLine = false;
    }

    return 0;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
bool
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::Delete(
    const SI_CHAR * a_pSection,
    const SI_CHAR * a_pKey,
    bool            a_bRemoveEmpty)
{
    if (!a_pSection) {
        return false;
    }

    typename TSection::iterator iSection = m_data.find(a_pSection);
    if (iSection == m_data.end()) {
        return false;
    }

    // remove a single key if we have a keyname
    if (a_pKey) {
        typename TKeyVal::iterator iKeyVal = iSection->second.find(a_pKey);
        if (iKeyVal == iSection->second.end()) {
            return false;
        }

        // remove any copied strings and then the key
        DeleteString(iKeyVal->first);
        DeleteString(iKeyVal->second);
        iSection->second.erase(iKeyVal);

        // done now if the section is not empty or we are not pruning away
        // the empty sections. Otherwise let it fall through into the section
        // deletion code
        if (!a_bRemoveEmpty || !iSection->second.empty()) {
            return true;
        }
    }
    else {
        // delete all copied strings from this section. The actual
        // entries will be removed when the section is removed.
        typename TKeyVal::iterator iKeyVal = iSection->second.begin();
        for ( ; iKeyVal != iSection->second.end(); ++iKeyVal) {
            DeleteString(iKeyVal->first);
            DeleteString(iKeyVal->second);
        }
    }

    // delete the section itself
    DeleteString(iSection->first);
    m_data.erase(iSection);

    return true;
}

template<class SI_CHAR, class SI_STRCMP, class SI_CONVERTER>
void
CSimpleIniTempl<SI_CHAR,SI_STRCMP,SI_CONVERTER>::DeleteString(
    const SI_CHAR * a_pString )
{
    // strings may exist either inside the data block, or they will be
    // individually allocated and stored in m_strings. We only physically
    // delete those stored in m_strings.
    if (a_pString < m_pData || a_pString >= m_pData + m_uDataLen) {
        typename TNamesDepend::iterator i = m_strings.begin();
        for (;i != m_strings.end(); ++i) {
            if (a_pString == *i) {
                delete[] const_cast<SI_CHAR*>(*i);
                m_strings.erase(i);
                break;
            }
        }
    }
}

// ---------------------------------------------------------------------------
//                                  TYPE DEFINITIONS
// ---------------------------------------------------------------------------

typedef CSimpleIniTempl<char,
    SI_NoCase<char>,SI_ConvertA<char> >                 CSimpleIniGenA;
typedef CSimpleIniTempl<wchar_t,
    SI_NoCase<wchar_t>,SI_ConvertW<wchar_t> >           CSimpleIniGenW;

#if defined(_WIN32) && !defined(SI_NO_MBCS)
typedef CSimpleIniTempl<char,
    SI_WindowsNoCase<char>,SI_ConvertA<char,false> >    CSimpleIniA;
typedef CSimpleIniTempl<wchar_t,
    SI_WindowsNoCase<wchar_t>,SI_ConvertW<wchar_t> >    CSimpleIniW;
#else // !_WIN32
typedef CSimpleIniGenA                                  CSimpleIniA;
typedef CSimpleIniGenW                                  CSimpleIniW;
#endif // _WIN32

typedef CSimpleIniTempl<char,
    SI_Case<char>,SI_ConvertA<char> >                   CSimpleIniCaseA;
typedef CSimpleIniTempl<wchar_t,
    SI_Case<wchar_t>,SI_ConvertW<wchar_t> >             CSimpleIniCaseW;

#ifdef _UNICODE
# define CSimpleIni      CSimpleIniW
# define CSimpleIniCase  CSimpleIniCaseW
# define SI_NEWLINE      SI_NEWLINE_W
#else // !_UNICODE
# define CSimpleIni      CSimpleIniA
# define CSimpleIniCase  CSimpleIniCaseA
# define SI_NEWLINE      SI_NEWLINE_A
#endif // _UNICODE

#ifdef SI_NO_ERRNO
# undef errno
#endif

#endif // INCLUDED_SimpleIni_h

