#include "stdafx.h"
#include "CollisionManager.h"

#include "Actor/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Sound/SoundManager.h"


CollisionHitManager* CollisionHitManager::instance_ = nullptr;


CollisionHitManager::CollisionHitManager()
{
	m_collisionInfoList.clear();
	m_collisionPairList.clear();
	SoundManager::CreateInstance(); // SoundManager�̃C���X�^���X���쐬
}


CollisionHitManager::~CollisionHitManager()
{
	m_collisionInfoList.clear();
	m_collisionPairList.clear();
}


void CollisionHitManager::Update()
{
	// �q�b�g����I�u�W�F�N�g�̃y�A�����
	const uint32_t colSize = static_cast<uint32_t>(m_collisionInfoList.size());
	for (uint32_t i = 0; i < colSize - 1; ++i) {
		for (uint32_t j = i + 1; j < colSize; ++j) {
			CollisionInfo* infoA = &m_collisionInfoList[i];
			CollisionInfo* infoB = &m_collisionInfoList[j];

			if (infoA->m_collision->IsHit(infoB->m_collision) || infoB->m_collision->IsHit(infoA->m_collision))
			{
				// CollisionPair�̒��ɓ����g�ݍ��킹���Ȃ����`�F�b�N
				bool exists = false;
				for (const auto& pair : m_collisionPairList) {
					if ((pair.m_left == infoA && pair.m_right == infoB) || (pair.m_left == infoB && pair.m_right == infoA)) {
						exists = true;
						break;
					}
				}
				// ���łɓo�^�ς݂ł͂Ȃ��Ȃ�ǉ�����
				if (!exists) {
					m_collisionPairList.push_back(CollisionPair(infoA, infoB));
					

				}
			}
		}
	}

	// �q�b�g�����y�A�ŏՓ˂������̏���������
	for (auto& pair : m_collisionPairList)
	{
		// �z���\�ȏꍇ�����𑱍s
		if (UpdateHitAttatchableObject(pair)) 
		{
			continue;
		}

	}
}


void CollisionHitManager::RegisterCollisionObject(EnCollisionType type, IGameObject* object, CollisionObject* collision)
{
	CollisionInfo info(type, object, collision);
	m_collisionInfoList.push_back(std::move(info));
}


void CollisionHitManager::UnregisterCollisionObject(IGameObject* object)
{
	for (auto it = m_collisionInfoList.begin(); it != m_collisionInfoList.end(); ++it)
	{
		if (it->m_object == object)
		{
			m_collisionInfoList.erase(it);
			break;
		}
	}
}


bool CollisionHitManager::UpdateHitAttatchableObject(CollisionPair& pair)
{
	// pair�Ƃ��č��ꂽ�z���\�ȃI�u�W�F�N�g�Ɖ�̏���Get����
	AttachableObject* attachableObject = GetTargetObject<AttachableObject>(pair, enCollisionType_AttachableObject);
	Sphere* sphere = GetTargetObject<Sphere>(pair, enCollisionType_Sphere);

	if (attachableObject == nullptr) {
		return false;
	}
	if (sphere == nullptr) {
		return false;
	}


	/** �z���\�ȃI�u�W�F�N�g�ƃX�t�B�A�̓����蔻�菈�����A��������ł��� */

	// pair�������t�����Ƃ���x����SE�𗬂�
	if (pair.isPlayedSE)  
	{
		SoundManager& soundManagerInstance = SoundManager::Get(); // SoundMangaer�̃C���X�^���X���擾
		soundManagerInstance.PlaySE(enSoundKind_Attach);
		pair.isPlayedSE = false;
	}

	//�I�u�W�F�N�g����ɂ��A�ꏏ�ɓ����悤�ɂ���
	sphere->SetParent(attachableObject); // transform�̐e�q�֌W��ݒ�
	attachableObject->GetTransform()->ResetLocalPosition(); // ���[�J���|�W�V�����̏�����
	attachableObject->GetTransform()->ResetLocalRotation(); // ���[�J�����[�e�[�V�����̏�����
	attachableObject->DeletePhysicsStatics();


	return true;
}
