/**
 * CollisionManager.h
 * �����蔻��Ǘ�
 */
#pragma once


class Sphere; // ��
class StageObjectBase; // �I�u�W�F�N�g�Ǘ�
class SoundHitManager;

enum EnCollisionType
{
	enCollisionType_Sphere,
	enCollisionType_AttachableObject,
};


struct CollisionInfo
{
	EnCollisionType m_type;	// �����蔻��̎��(�v���C���[�ƐH�ނ��q�b�g�����݂����ȏ��������邽�߂ɕK�v�B���������ꂩ�̔��f)
	IGameObject* m_object;	// �����蔻������I�u�W�F�N�g�̃|�C���^
	CollisionObject* m_collision; // �����蔻��I�u�W�F�N�g�̃|�C���^
	//
	CollisionInfo() : m_type(enCollisionType_Sphere), m_object(nullptr), m_collision(nullptr) {}
	CollisionInfo(const EnCollisionType type, IGameObject* object, CollisionObject* collision) : m_type(type), m_object(object), m_collision(collision) {}
};




struct CollisionPair
{
	CollisionInfo* m_left;	// �����蔻��A
	CollisionInfo* m_right;	// �����蔻��B
	bool isPlayedSE = true;
	//
	CollisionPair() : m_left(nullptr), m_right(nullptr) {}
	CollisionPair(CollisionInfo* left, CollisionInfo* right) : m_left(left), m_right(right) {}
};




/**
 * �����蔻�菈�����ꊇ�ōs���N���X
 * NOTE:�V���O���g���ŃC���X�^���X����ɂ��ăA�N�Z�X�|�C���g��񋟂���
 */
class CollisionHitManager
{
private:
	/** �����蔻��I�u�W�F�N�g�̃��X�g */
	std::vector<CollisionInfo> m_collisionInfoList;
	/** �����蔻��̃y�A */
	std::vector<CollisionPair> m_collisionPairList;
	

private:
	CollisionHitManager();
	~CollisionHitManager();


public:
	void Update();


public:
	/** ���菈�����������I�u�W�F�N�g��o�^ */
	void RegisterCollisionObject(EnCollisionType type, IGameObject* object, CollisionObject* collision);
	void UnregisterCollisionObject(IGameObject* object);


private:
	/* Sphere�Ƌz���\�ȃI�u�W�F�N�g���ڐG�����Ƃ��̏��� */
	bool UpdateHitAttatchableObject(CollisionPair& pair);

private:
	/**
	 * �w�肵���N���X���擾����
	 * NOTE: �w�肵���N���X�����݂��Ȃ��ꍇ��nullptr��Ԃ�
	 */
	template <typename T>
	T* GetTargetObject(CollisionPair& pair, const EnCollisionType targetType)
	{
		if (pair.m_left->m_type == targetType)
		{
			return static_cast<T*>(pair.m_left->m_object);
		}
		else if (pair.m_right->m_type == targetType)
		{
			return static_cast<T*>(pair.m_right->m_object);
		}
		return nullptr;
	}


private:
	static CollisionHitManager* instance_;


public:
	static void Create()
	{
		if (instance_ == nullptr)
		{
			instance_ = new CollisionHitManager();
		}
	}
	static CollisionHitManager& Get()
	{
		return *instance_;
	}
	static void Delete()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}
};