/**
 * ParameterManager.cpp
 *
 * パラメーター管理
 * ステータスなどの数値を外部ファイルから読み込んで使用する
 */
#include "stdafx.h"
#include "ParameterManager.h"


ParameterManager* ParameterManager::instance_ = nullptr; //初期化


ParameterManager::ParameterManager()
{
	// リストの削除
	parameterMap_.clear();
}

ParameterManager::~ParameterManager()
{
	// リストの削除
	for (auto& it : parameterMap_) {
		auto& parameters = it.second;
		for (auto* parameter : parameters) {
			delete parameter;
		}
		parameters.clear();
	}
	parameterMap_.clear();
}