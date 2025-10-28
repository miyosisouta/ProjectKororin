#include "stdafx.h"
#include "InGameManager.h"
#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Sound/SoundManager.h"


void InGameUpdateObject::Update()
{
	InGameManager::Get().Update();
}


void InGameLateUpdateObject::Update()
{
	InGameManager::Get().LateUpdate();
}



/*********************************/


InGameManager* InGameManager::instance_ = nullptr;


InGameManager::InGameManager()
{
	notifyList_.clear();

}


InGameManager::~InGameManager()
{
	ClearNotify();
}


void InGameManager::Update()
{

}


void ::InGameManager::LateUpdate()
{
	// �ʒm������������s�����
	for (auto& notify : notifyList_) {
		switch (notify->type)
		{
			case NotifyType::CollisionHit:
			{
				NotifyCollisionHit* hitNotify = static_cast<NotifyCollisionHit*>(notify);

				Sphere* sphere = GetTargetObject<Sphere>(*hitNotify, GameObjectType::Sphere);
				AttachableObject* attachableObject = GetTargetObject<AttachableObject>(*hitNotify, GameObjectType::AttachableObject);


				// �R���W�������m�������������̏���
				// Sphere�̃��x�����z���\���x���ɒB���Ă��邩����
				if (!CanAttach(*attachableObject, *sphere))
				{
					continue;
				}

				if (attachableObject->IsPlayedSE()) {
					continue;
				}


				// pair�������t�����Ƃ���x����SE�𗬂�
				if (!attachableObject->IsPlayedSE())  
				{
					SoundManager::Get().PlaySE(enSoundKind_AttachSuccess);
					attachableObject->SetPlayedSE(true);
				}

				//�I�u�W�F�N�g����ɂ��A�ꏏ�ɓ����悤�ɂ���
				sphere->SetParent(attachableObject); // transform�̐e�q�֌W��ݒ�
				attachableObject->GetTransform()->ResetLocalPosition(); // ���[�J���|�W�V�����̏�����
				attachableObject->GetTransform()->ResetLocalRotation(); // ���[�J�����[�e�[�V�����̏�����
				attachableObject->DeletePhysicsStatics();

				// ��̔��a���L����
				sphere->GrowByRadius(attachableObject->GetGrowAmount());

				break;
			}
			default:
			{
				K2_ASSERT(false, "�ʒm�̎�ނ��ǉ�����Ă��܂���B\n");
				break;
			}
		}
	}

	ClearNotify();
}


void InGameManager::Notify(INotify* notify)
{
	// �ʒm������
	notifyList_.push_back(notify);
}

bool InGameManager::CanAttach(AttachableObject& target, Sphere& sphere)
{
	// �z���\�I�u�W�F�N�g���ǂ����̔���
	if (target.GetObjectSize() == 0)
	{
		return true;
	}

	// ��̃��x�����I�u�W�F�N�g�̃T�C�Y�ȏ�̏ꍇ
	if (sphere.GetSphereSizeLevel() < target.GetObjectSize())
	{
		return true;
	}

	return false;
}