#include "stdafx.h"
#include "GameManager.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Sound/SoundManager.h"

GameManager* GameManager::instance_ = nullptr;


GameManager::~GameManager()
{
	SoundManager::DestroyInstance();
}


bool GameManager::Start()
{
	SoundManager::CreateInstance(); // SoundManagerのインスタンスを作成

	return true;
}


void GameManager::Update()
{
	if (m_currentScene_ != m_nextScene_) // 現在シーンと次シーンが異なる場合に遷移
	{
		ChangeScene(m_nextScene_, m_currentScene_);
	}

	SoundManager::Get().Update();
}


void GameManager::ChangeScene(SceneState createScene,SceneState deleteScene)
{
	//生成対象のシーンを作成
	switch (createScene) 
	{
	case SceneState::EnTitle:
		m_title_ = NewGO<Title>(0, "title");
		break;


	case SceneState::EnGame:
		m_game_ = NewGO<Game>(0, "game");
		break;


	case SceneState:: EnResult:
		m_result_ = NewGO<Result>(0, "result");
		break;


	default:
		break;
	}

	// 破棄対象のシーンを削除
	switch (deleteScene) 
	{
	case SceneState::EnTitle:
		DeleteGO(m_title_);
		m_title_ = nullptr;
		break;


	case SceneState::EnGame:
		DeleteGO(m_game_);
		m_game_ = nullptr;


	case SceneState::EnResult:
		DeleteGO(m_result_);
		m_result_ = nullptr;


	default:
		break;
	}
	m_currentScene_ = m_nextScene_; // 遷移が完了したので現在シーンを更新
}

void GameManager::Init() 
{
	SceneState m_currentScene = SceneState::EnNone; // 現在のシーン
	SceneState m_nextScene = SceneState::EnTitle;  // 次に遷移するシーン
}
