/**
 * @file StageLoader.h
 * @brief ステージのオブジェクトをバイナリファイルから読み込むクラスの宣言。
 */
#pragma once
#include <unordered_map>

struct ObjectData;

class StageLoader
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	StageLoader() {}

	/**
	 * @brief デストラクタ。
	 */
	~StageLoader() {}

	/**
	 * @brief バイナリファイルからオブジェクトを読み込む。
	 * @param objectList  読み込んだデータを格納するマップ
	 */
	void LoadObject(std::unordered_map<int, ObjectData*>& objectList);
};
