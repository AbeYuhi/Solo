#pragma once
#include <windows.h>
#include <string>

/// <summary>
/// DebugLog.h
/// Logを表示したりstringを変換しているファイル
/// </summary>

void Log(const std::string& message);

std::wstring ConvertString(const std::string& str);

std::string ConvertString(const std::wstring& str);