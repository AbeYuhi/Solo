#pragma once

/// <summary>
/// SafeDelete.h
/// ポインタを安全に処理するためのファイル
/// </summary>

template<class T>
inline void SafeDelete(T p) {
	delete(p);
	p = nullptr;
}