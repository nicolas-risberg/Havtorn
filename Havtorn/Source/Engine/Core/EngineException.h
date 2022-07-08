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

		static void EngineErrorMessage(HRESULT result, const int line, const char* function, const char* file, const char* formattedMessage, ...)
		{
			_com_error err(result);
			LPCTSTR error_msg = err.ErrorMessage();
			std::wstring wstr;
			std::wstring fileString = StringToWstring(file);
			std::wstring functionString = StringToWstring(function);
			va_list argptr;
			va_start(argptr, formattedMessage);
			std::string str{ StringVSprintf(formattedMessage, argptr) };
			std::wstring wideFormattedMessage = StringToWstring(str);

			wstr.append(L"\nERROR: ");
			wstr.append(error_msg);
			wstr.append(L"\n_____________________");
			wstr.append(L"\n\nFILE:\n");
			wstr.append(fileString);
			wstr.append(L"\n\nFUNCTION:\n");
			wstr.append(functionString);
			wstr.append(L"\n\nLINE:\n");
			wstr.append(std::to_wstring(line));
			wstr.append(L"\n\nFURTHER INFO:\n");
			wstr.append(wideFormattedMessage);
			MessageBox(0, wstr.c_str(), L"ENGINE EXCEPTION", MB_ICONERROR);
		}

		static void EngineErrorMessage(const int line, const char* function, const char* file, const char* formattedMessage, ...)
		{
			std::wstring wstr;
			std::wstring fileString = StringToWstring(file);
			std::wstring functionString = StringToWstring(function);
			va_list argptr;
			va_start(argptr, formattedMessage);
			std::string str{ StringVSprintf(formattedMessage, argptr) };
			std::wstring wideFormattedMessage = StringToWstring(str);

			wstr.append(L"\nERROR: ");
			wstr.append(L"Error occurred in engine.");
			wstr.append(L"\n_____________________");
			wstr.append(L"\n\nFILE:\n");
			wstr.append(fileString);
			wstr.append(L"\n\nFUNCTION:\n");
			wstr.append(functionString);
			wstr.append(L"\n\nLINE:\n");
			wstr.append(std::to_wstring(line));
			wstr.append(L"\n\nFURTHER INFO:\n");
			wstr.append(wideFormattedMessage);
			MessageBox(0, wstr.c_str(), L"ENGINE EXCEPTION", MB_ICONERROR);
		}

		static std::wstring StringToWstring(const std::string& str)
		{
#pragma warning(suppress : 4244)
			return std::wstring(str.begin(), str.end());
		}

		static std::string StringVSprintf(const char* format, std::va_list args)
		{
			// Unfortunately you cannot consume a va_list twice, so we have to copy it
			va_list tempArgs; 
			va_copy(tempArgs, args);
			const int requiredLength = _vscprintf(format, tempArgs) + 1;
			va_end(tempArgs);

			char buff[4096];
			memset(buff, 0, requiredLength);
			if (vsnprintf_s(buff, requiredLength, format, args) < 0)
			{
				return "StringVSprintf encoding error";
			}
			return std::string(buff);
		}
	};
}
