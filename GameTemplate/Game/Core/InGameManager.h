/**
 * �C���Q�[���Ǘ���
 */
#include "Actor/Object/AttachableObject.h"
#include "Actor/Sphere/Sphere.h"

// �ŏ��Ɏ��s����Update
class InGameUpdateObject : public IGameObject
{
public:
	InGameUpdateObject() {}
	~InGameUpdateObject() {}

	virtual bool Start() override { return true; }	// �g�p���Ȃ�
	virtual void Update() override;
	virtual void Render(RenderContext& rc) {}		// �g�p���Ȃ�
};

// ���Ɏ��s����Update
class InGameLateUpdateObject : public IGameObject
{
public:
	InGameLateUpdateObject() {}
	~InGameLateUpdateObject() {}

	virtual bool Start() override { return true; }	// �g�p���Ȃ�
	virtual void Update() override;
	virtual void Render(RenderContext& rc) {}		// �g�p���Ȃ�
};



/**************************/


// �\���̂ɂ��邱�Ƃ�Enum�̖��O�������ł����Ȃ���Ԃɂ���
struct NotifyType
{
	enum Enum
	{
		CollisionHit,
	};
};

struct INotify
{
	NotifyType::Enum type;
	// int�^�Ŏ󂯎��A����̌^�֕ύX����
	INotify(int type) : type(static_cast<NotifyType::Enum>(type)) {}
};


struct NotifyCollisionHit : INotify
{
	IGameObject* left;
	IGameObject* right;
	GameObjectType::Enum leftType;
	GameObjectType::Enum rightType;
	
	// CollisionHit�������Ƃ��ăR���X�g���N�^�ɓn��
	NotifyCollisionHit()
		: INotify(NotifyType::CollisionHit)
	{
	}
};



class InGameManager
{
private:
	std::vector<INotify*> notifyList_;



private:
	InGameManager();
	~InGameManager();

	bool CanAttach(AttachableObject& target, Sphere& sphere);

public:
	/**
	 * @brief �ʏ�X�V
	 */
	void Update();

	/**
	 * @brief �x��čX�V
	 */
	void LateUpdate();


public:
	/**
	 * @brief �ʒm�ǉ�
	 */
	void Notify(INotify* notify);




private:
	/**
	 * @brief ���X�g�̏������ׂč폜�Anullptr�ɂ���
	 */
	void ClearNotify()
	{
		for (auto* notify : notifyList_) {
			delete notify;
			notify = nullptr;
		}
		notifyList_.clear();
	}

	/**
	 * �w�肵���N���X���擾����
	 * NOTE: �w�肵���N���X�����݂��Ȃ��ꍇ��nullptr��Ԃ�
	 */
	template <typename T>
	T* GetTargetObject(NotifyCollisionHit& pair, const GameObjectType::Enum targetType)
	{
		if (pair.leftType == targetType)
		{
			return static_cast<T*>(pair.left);
		}
		else if (pair.rightType == targetType)
		{
			return static_cast<T*>(pair.right);
		}
		return nullptr;
	}



	/**
	 * �V���O���g���֘A
	 */
private:
	static InGameManager* instance_;


public:
	/* �C���X�^���X���쐬 */
	static void CreateInstance()
	{
		if (instance_ == nullptr) {
			instance_ = new InGameManager();
		}
	}

	/* ���p�\��Ԃ��m�F����֐� */
	static bool IsAvailable()
	{
		return instance_ != nullptr;
	}
	/**
	 * @brief �C���X�^���X�����
	 * @return 
	 */
	static InGameManager& Get()
	{
		return *instance_;
	}

	/**
	 * @brief �C���X�^���X���폜
	 */
	static void DeleteInstance()
	{
		if (instance_) {

		}
	}
};