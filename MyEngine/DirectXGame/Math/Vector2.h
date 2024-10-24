#pragma once

/// <summary>
/// Vector2.h
/// Vector2構造体のデータファイル
/// </summary>

struct Vector2 final {
	float x;
	float y;
};

struct Vector2Int final {
	int x;
	int y;
};

Vector2Int operator+(Vector2Int num1, Vector2Int num2);

Vector2Int operator-(Vector2Int num1, Vector2Int num2);

Vector2 operator+(Vector2 num1, Vector2 num2);

Vector2 operator-(Vector2 num1, Vector2 num2);