#pragma once

/// <summary>
/// SafeDelete.h
/// ポインタを安全に処理するためのファイル
/// </summary>

/// <summary>
/// メモリを開放する処理
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="p"></param>
template<class T>
inline void SafeDelete(T p) {
	delete(p);
	p = nullptr;
}