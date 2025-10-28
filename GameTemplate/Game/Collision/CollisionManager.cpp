#include "stdafx.h"
#include "CollisionManager.h"

#include "Actor/Sphere/Sphere.h"
#include "Actor/Object/AttachableObject.h"
#include "Core/LateStageObjectUpdateManager.h"
#include "Core/InGameManager.h"


CollisionHitManager* CollisionHitManager::instance_ = nullptr;


CollisionHitManager::CollisionHitManager()
{
	m_collisionInfoList.clear();
	m_collisionPairList.clear();
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

			if (infoA->collision->IsHit(infoB->collision) || infoB->collision->IsHit(infoA->collision))
			{
				// CollisionPair�̒��ɓ����g�ݍ��킹���Ȃ����`�F�b�N
				bool exists = false;
				for (const auto& pair : m_collisionPairList) {
					if ((pair.left == infoA && pair.right == infoB) || (pair.left == infoB && pair.right == infoA)) {
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

	m_collisionPairList.clear();
}


void CollisionHitManager::RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, CollisionObject* collision)
{
	CollisionInfo info(type, object, collision);
	m_collisionInfoList.push_back(std::move(info));
}


void CollisionHitManager::UnregisterCollisionObject(IGameObject* object)
{
	for (auto it = m_collisionInfoList.begin(); it != m_collisionInfoList.end(); ++it)
	{
		if (it->object == object)
		{
			m_collisionInfoList.erase(it);
			break;
		}
	}
}


bool CollisionHitManager::UpdateHitAttatchableObject(CollisionPair& pair)
{
	// pair�Ƃ��č��ꂽ�z���\�ȃI�u�W�F�N�g�Ɖ�̏���Get����
	AttachableObject* attachableObject = GetTargetObject<AttachableObject>(pair, GameObjectType::AttachableObject);
	Sphere* sphere = GetTargetObject<Sphere>(pair, GameObjectType::Sphere);

	if (attachableObject == nullptr) {
		return false;
	}
	if (sphere == nullptr) {
		return false;
	}


	/** �z���\�ȃI�u�W�F�N�g�ƃX�t�B�A�̓����蔻�菈�����A��������ł��� */


	NotifyCollisionHit* notify = new NotifyCollisionHit();
	notify->left = attachableObject;
	notify->leftType = GameObjectType::AttachableObject;
	notify->right = sphere;
	notify->rightType = GameObjectType::Sphere;

	InGameManager::Get().Notify(notify);
	return true;

	



	//// �z���\�I�u�W�F�N�g�̓����蔻����擾
	//auto* attachableObjectCollider = attachableObject->GetPhysicsStaticObject()->GetCollider()->GetBody();

	//bool isFind = false; // �����蔻��������Ă��邩�̃t���O
	//auto* shpereBody = sphere->GetCollider()->GetBody();
	//const int size = shpereBody->getNumChildShapes(); // ��̓����蔻��̎q�̐����擾
	//if (size > 0) {
	//	auto* childList = shpereBody->getChildList(); // ��̓����蔻��̎q�ǂ��ɂ���I�u�W�F�N�g���i�[
	//	// ��ɋz�����Ă���I�u�W�F�N�g�̐������J��Ԃ�
	//	for (int i = 0; i < size; ++i)
	//	{
	//		auto* child = &childList[i];
	//		if (child->m_childShape == attachableObjectCollider) // �z�����Ă���I�u�W�F�N�g�����������ꍇ
	//		{
	//			isFind = true; // �T���Ȃ�
	//			break;
	//		}
	//	}
	//}
	//if (!isFind)
	//{ // �������Ă��Ȃ��ꍇ�A��̓����蔻��̎q�ǂ��ɉ�����
	//	btTransform transform;
	//	transform.setIdentity();
	//	shpereBody->addChildShape(transform, attachableObjectCollider);

	//	// @todo for test
	//	LateStageObjectUpdateManager::Get().RegisterObject(attachableObject);
	//}

	return true;
}
