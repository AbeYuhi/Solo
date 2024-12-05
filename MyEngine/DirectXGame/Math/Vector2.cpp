#include "Vector2.h"

/// <summary>
/// Vectro2.h
/// Vector2構造体に関する処理の実装ファイル
/// </summary>

Vector2Int operator+(Vector2Int num1, Vector2Int num2) {
	return { num1.x + num2.x, num1.y + num2.y };
}

Vector2Int operator-(Vector2Int num1, Vector2Int num2) {
	 return { num1.x - num2.x, num1.y - num2.y };
}

Vector2 operator+(Vector2 num1, Vector2 num2) {
	return { num1.x + num2.x, num1.y + num2.y };
}

Vector2 operator-(Vector2 num1, Vector2 num2) {
	return { num1.x - num2.x, num1.y - num2.y };
}