#include "stdafx.h"
#include "Title.h"
#include "GameManager.h"

bool Title::Start() 
{
	m_titleRender_.Init("Assets/sprite/title/titleKari.dds",1920,1080);
	return true;
}

void Title::Update() 
{
	if (m_gameManager_ == nullptr) // ゲームマネージャーが見つからない場合
	{
		m_gameManager_ = FindGO<GameManager>("gameManager");
	}

	if (g_pad[0]->IsTrigger(enButtonA))  // Aボタンが押された場合
	{
		LoadGameScene();
	}

	m_titleRender_.Update(); // タイトル画像の更新
}

void Title::Render(RenderContext& rc) 
{
	m_titleRender_.Draw(rc);
}

void Title::LoadGameScene()
{
	GameManager::Get().SetNextScene(GameManager::SceneState::EnGame);
}


