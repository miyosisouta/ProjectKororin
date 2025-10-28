/**
 * CollisionManager.h
 * �����蔻��Ǘ�
 */
#pragma once


class Sphere; // ��
class StageObjectBase; // �I�u�W�F�N�g�Ǘ�


struct CollisionInfo
{
	int type;	// �I�u�W�F�N�g�̎��(�v���C���[�ƕǂ��q�b�g�����݂����ȏ��������邽�߂ɕK�v�B���������ꂩ�̔��f)
	IGameObject* object;	// �����蔻������I�u�W�F�N�g�̃|�C���^
	CollisionObject* collision; // �����蔻��I�u�W�F�N�g�̃|�C���^
	//
	CollisionInfo() : type(GameObjectType::Sphere), object(nullptr), collision(nullptr) {}
	CollisionInfo(const GameObjectType::Enum type, IGameObject* object, CollisionObject* collision) : type(type), object(object), collision(collision) {}
};




struct CollisionPair
{
	CollisionInfo* left;	// �����蔻��A
	CollisionInfo* right;	// �����蔻��B
	//
	CollisionPair() : left(nullptr), right(nullptr) {}
	CollisionPair(CollisionInfo* left, CollisionInfo* right) : left(left), right(right) {}
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
	void RegisterCollisionObject(GameObjectType::Enum type, IGameObject* object, CollisionObject* collision);
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
	T* GetTargetObject(CollisionPair& pair, const GameObjectType::Enum targetType)
	{
		if (pair.left->type == targetType)
		{
			return static_cast<T*>(pair.left->object);
		}
		else if (pair.right->type == targetType)
		{
			return static_cast<T*>(pair.right->object);
		}
		return nullptr;
	}


private:
	static CollisionHitManager* instance_;


public:
	static void CreateInstance()
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