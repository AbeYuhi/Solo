#pragma once
#include <windows.h>
#include <string>

/// <summary>
/// DebugLog.h
/// Logを表示したりstringを変換しているファイル
/// </summary>

/// <summary>
/// Logの表示処理
/// </summary>
/// <param name="message"></param>
void Log(const std::string& message);

/// <summary>
/// stringをwstringに変換する処理
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring ConvertString(const std::string& str);

/// <summary>
/// wstringをstringに変換する処理
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string ConvertString(const std::wstring& str);
