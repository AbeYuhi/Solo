#pragma once
#include <variant>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "External/nlohmann/json.hpp"

/// <summary>
/// GlobalVariables.h
/// jsonを読み込んだり出力したりするクラス
/// </summary>

using json = nlohmann::json;

class GlobalVariables
{
public:
	/// <summary>
	/// インスタンスの取得処理
	/// </summary>
	/// <returns></returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">作成したいグループの名前</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ImGuiの更新
	/// </summary>
	void Updates();

	/// <summary>
	/// 特定のファイルのセーブ
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 値のセット(int)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット(float)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, float value);
	
	/// <summary>
	/// 値のセット(Vec3)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 項目の追加(int)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddValue(const std::string& groupName, const std::string& key, int32_t value);
	
	/// <summary>
	/// 項目の追加(float)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddValue(const std::string& groupName, const std::string& key, float value);
	
	/// <summary>
	/// 項目の追加(Vec3)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// int値の取得
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// float値の取得
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	float GetFloatValue(const std::string& groupName, const std::string& key) const;

	/// <summary>
	/// Vector3値の取得
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;

	//項目
	struct Item {
		std::variant<int32_t, float, Vector3> value;
	};
	//グループ
	struct Group {
		std::map<std::string, Item> items;
	};
	//全データ
	std::map<std::string, Group> dates_;

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& globalVariables) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/ScriptFiles/GlobalVariables/";
};

