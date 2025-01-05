#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

/// <summary>
/// LoadCSV.h
/// CSVを読み込んでいるクラス
/// </summary>

class LoadCSV
{
public:

	/// <summary>
	/// CSVファイルを読み込む処理
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static std::stringstream Load(std::string fileName);


private:


};

