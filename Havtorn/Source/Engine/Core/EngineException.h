// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <codecvt>
#include <cstdarg>
#include <string>
#include <comdef.h>

#define ENGINE_HR_ERROR_MESSAGE(hr, ...)	{ Havtorn::EngineException::EngineErrorMessage(hr, __LINE__, __FUNCTION__, __FILE__, ##__VA_ARGS__);  }
#define ENGINE_HR(hr)						{ HRESULT hresult = hr; if (FAILED(hresult))	{ ENGINE_HR_ERROR_MESSAGE(hresult, ""); }}
#define ENGINE_HR_BOOL(hr)					{ HRESULT hresult = hr; if (FAILED(hresult))	{ ENGINE_HR_ERROR_MESSAGE(hresult, ""); return 0;}}
#define ENGINE_HR_MESSAGE(hr, ...)			{ HRESULT hresult = hr; if (FAILED(hresult))	{ ENGINE_HR_ERROR_MESSAGE(hresult, ##__VA_ARGS__); }}
#define ENGINE_HR_BOOL_MESSAGE(hr, ...)		{ HRESULT hresult = hr; if (FAILED(hresult))	{ ENGINE_HR_ERROR_MESSAGE(hresult, ##__VA_ARGS__); return 0;}}

#define ENGINE_ERROR_MESSAGE(...)				{ Havtorn::EngineException::EngineErrorMessage(__LINE__, __FUNCTION__, __FILE__, ##__VA_ARGS__);  }
#define ENGINE_ERROR_BOOL(result)				{ if (!result)	{ ENGINE_ERROR_MESSAGE(""); return 0;}}
#define ENGINE_ERROR_BOOL_MESSAGE(result, ...)	{ if (!result)	{ ENGINE_ERROR_MESSAGE(##__VA_ARGS__); return 0;}}
#define ENGINE_BOOL_POPUP(result, ...)			{ if (!result)  { ENGINE_ERROR_MESSAGE(##__VA_ARGS__); }}

namespace Havtorn
{
	class EngineException
	{
	public:

		static void EngineErrorMessage(HRESULT result, const int line, const char* function, const char* file, const char* aFormattedMessage, ...)
		{
			_com_error err(result);
			LPCTSTR error_msg = err.ErrorMessage();
			std::wstring wstr;
			std::wstring aFileString = stringToWstring(file);
			std::wstring aFunctionString = stringToWstring(function);
			va_list argptr;
			va_start(argptr, aFormattedMessage);
			std::string str{ string_vsprintf(aFormattedMessage, argptr) };
			std::wstring aWideFormattedMessage = stringToWstring(str);

			wstr.append(L"\nERROR: ");
			wstr.append(error_msg);
			wstr.append(L"\n_____________________");
			wstr.append(L"\n\nFILE:\n");
			wstr.append(aFileString);
			wstr.append(L"\n\nFUNCTION:\n");
			wstr.append(aFunctionString);
			wstr.append(L"\n\nLINE:\n");
			wstr.append(std::to_wstring(line));
			wstr.append(L"\n\nFURTHER INFO:\n");
			wstr.append(aWideFormattedMessage);
			MessageBox(0, wstr.c_str(), L"ENGINE EXCEPTION", MB_ICONERROR);
		}

		static void EngineErrorMessage(const int line, const char* function, const char* file, const char* aFormattedMessage, ...)
		{
			std::wstring wstr;
			std::wstring aFileString = stringToWstring(file);
			std::wstring aFunctionString = stringToWstring(function);
			va_list argptr;
			va_start(argptr, aFormattedMessage);
			std::string str{ string_vsprintf(aFormattedMessage, argptr) };
			std::wstring aWideFormattedMessage = stringToWstring(str);

			wstr.append(L"\nERROR: ");
			wstr.append(L"Error occurred in engine.");
			wstr.append(L"\n_____________________");
			wstr.append(L"\n\nFILE:\n");
			wstr.append(aFileString);
			wstr.append(L"\n\nFUNCTION:\n");
			wstr.append(aFunctionString);
			wstr.append(L"\n\nLINE:\n");
			wstr.append(std::to_wstring(line));
			wstr.append(L"\n\nFURTHER INFO:\n");
			wstr.append(aWideFormattedMessage);
			MessageBox(0, wstr.c_str(), L"ENGINE EXCEPTION", MB_ICONERROR);
		}

		static std::wstring stringToWstring(const std::string& t_str)
		{
#pragma warning(suppress : 4244)
			return std::wstring(t_str.begin(), t_str.end());
		}

		static std::string string_vsprintf(const char* format, std::va_list args)
		{
			va_list tmp_args; //unfortunately you cannot consume a va_list twice
			va_copy(tmp_args, args); //so we have to copy it
			const int required_len = _vscprintf(format, tmp_args) + 1;
			va_end(tmp_args);

			char buff[4096];
			memset(buff, 0, required_len);
			if (vsnprintf_s(buff, required_len, format, args) < 0)
			{
				return "string_vsprintf encoding error";
			}
			return std::string(buff);
		}
	};
}
//CustomMessageBox(0, wstr.c_str(), L"ENGINE EXCEPTION", MB_OK, IDI_ICON1);

//static int CustomMessageBox(HWND hWnd,
//	LPCTSTR lpText,
//	LPCTSTR lpCaption,
//	UINT uType,
//	UINT uIconResID)
//{
//	MSGBOXPARAMS mbp;
//	mbp.cbSize = sizeof(MSGBOXPARAMS);
//	mbp.hwndOwner = hWnd;
//	mbp.hInstance = GetModuleHandle(NULL);
//	mbp.lpszText = lpText;
//	mbp.lpszCaption = lpCaption;
//	mbp.dwStyle = uType | MB_USERICON;
//	mbp.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
//	mbp.lpfnMsgBoxCallback = NULL;
//	mbp.dwContextHelpId = 0;
//	mbp.lpszIcon = MAKEINTRESOURCE(uIconResID);

//	return MessageBoxIndirect(&mbp);
//}
