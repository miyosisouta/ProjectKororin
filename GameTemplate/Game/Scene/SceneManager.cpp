#include "stdafx.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Scene/ResultScene.h"
#include "Sound/SoundManager.h"


SceneManager* SceneManager::instance_ = nullptr;


SceneManager::~SceneManager()
{
	SoundManager::DestroyInstance();
}


bool SceneManager::Start()
{
	SoundManager::CreateInstance(); // SoundManagerのインスタンスを作成

	// 初期シーンを設定
	currentScene_ = NextScene(TitleScene::ID());		// @todo for test
	currentScene_->Start();

	return true;
}


void SceneManager::Update()
{
	if (currentScene_ != nullptr) {
		currentScene_->Update(); // 現在のシーンを更新する
		uint32_t nextId = -1;

		if (currentScene_->RequestID(nextId)) { // 引数で次に遷移するシーンのIDをもらう
			delete currentScene_; // 現在のシーンを削除
			currentScene_ = NextScene(nextId); // 次のシーンを作成
			currentScene_->Start(); // 初期化処理を行う
		}
	}

	SoundManager::Get().Update();
}


void SceneManager::Render(RenderContext& rc) 
{
	if (currentScene_) {
		currentScene_->Render(rc);
	}
}


IScene* SceneManager::NextScene(const uint32_t nextId)
{
	// 文字列じゃなくてIDという数値にしているのは、文字列の比較が遅いから
	// 1文字1文字比較したくない
	if (nextId == TitleScene::ID()) 
	{
		return new TitleScene();
	}

	if (nextId == GameScene::ID()) 
	{
		return new GameScene();
	}

	if (nextId == ResultScene::ID()) {
		return new ResultScene();
	}
	return nullptr;
}


/****************************************/


SceneManagerObject::SceneManagerObject()
{
}

SceneManagerObject::~SceneManagerObject()
{
	SceneManager::DeleteInstance();
}

bool SceneManagerObject::Start()
{
	SceneManager::CreateInstance();
	SceneManager::Get().Start();
	return true;
}

void SceneManagerObject::Update()
{
	SceneManager::Get().Update();
}

void SceneManagerObject::Render(RenderContext& rc)
{
	SceneManager::Get().Render(rc);
}
