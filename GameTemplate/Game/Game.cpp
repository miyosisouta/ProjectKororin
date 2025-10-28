#include "stdafx.h"
#include "Game.h"
#include "GameManager.h"
#include "Camera/SphereCamera.h"
#include "Stage.h"
#include "Actor/Sphere/Player.h"
#include "Actor/Sphere/Sphere.h"
#include "InputSystem.h"
#include "Collision/CollisionManager.h"
#include "Core/LateStageObjectUpdateManager.h"
#include "Core/InGameManager.h"
#include "UI/Canvas.h"


Game::~Game()
{
	DeleteGO(sphere_);
	DeleteGO(m_player);
	DeleteGO(sphereCamera_);
	DeleteGO(stage_);

	CollisionHitManager::Delete();
	LateStageObjectUpdateManager::Get().UnRegisterSphere();
	LateStageObjectUpdateManager::Delete();
	InGameManager::DeleteInstance();
}


bool Game::Start()
{
	//m_player = NewGO<Player>(0,"player"); // �v���C���[
	sphere_ = NewGO<Sphere>(0,"sphere"); // ��
	sphereCamera_ = NewGO<SphereCamera>(0, "sphereCamera"); // ��̃J����
	stage_ = NewGO<Stage>(0,"stage"); // �X�e�[�W
	canvas_ = NewGO<Canvas>(0, "canvas"); // �L�����o�X


	// �A�b�v�f�[�g�̏������Ԃ�ݒ�
	// NewGO<�N���X��>(�����F���s���鏇�Ԃ�ݒ�ł���)
	NewGO<InGameUpdateObject>(GameObjectPriority::InGameManager);
	NewGO<InGameLateUpdateObject>(GameObjectPriority::InGameManagerLate);


	// ����p�N���X�̍쐬�Ƒ��삷�镨�̐ݒ�
	auto* inputSystem = NewGO<InputSystem>(0, "inputSystem");
	inputSystem->SetTarget(sphere_);


	/* �}�l�[�W���[�̃C���X�^���X���쐬 */
	CollisionHitManager::CreateInstance();
	LateStageObjectUpdateManager::CreateInstance();
	LateStageObjectUpdateManager::Get().RegisterSphere(sphere_);
	InGameManager::CreateInstance();
	

	return true;
}


void Game::Update()
{
	CollisionHitManager::Get().Update();
	LateStageObjectUpdateManager::Get().Update();
	InGameManager::Get().Update();

	CheckAndHandleGameState(); // �N���A�`�F�b�N
	
}

void Game::Render(RenderContext& rc)
{
}




void Game::CheckAndHandleGameState()
{

}

