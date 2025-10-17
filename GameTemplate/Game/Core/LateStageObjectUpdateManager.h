/**
 * LateStageObjectUpdateManager.h
 * �x��ăX�e�[�W�I�u�W�F�N�g���X�V���邽�߂̏���
 * NOTE: 
 */
#pragma once


class Sphere; // ��
class StageObjectBase; // �I�u�W�F�N�g�Ǘ�


/**
 * �x��ăX�e�[�W�I�u�W�F�N�g���X�V����Ǘ���
 */
class LateStageObjectUpdateManager
{
private:
	/**
	 * �X�t�B�A
	 * NOTE:�I�u�W�F�N�g���X�V���邽�߂̑Ώ�
	 */
	Sphere* sphere_;

	/** �I�u�W�F�N�g�Q */
	std::vector<StageObjectBase*> objectList_;


private:
	LateStageObjectUpdateManager();
	~LateStageObjectUpdateManager();


public:
	void Update();


public:
	/** �I�u�W�F�N�g��o�^ */
	void RegisterObject(StageObjectBase* object);
	void UnregisterObject(StageObjectBase* object);

	/** �X�t�B�A��o�^ */
	void RegisterSphere(Sphere* sphere)
	{
		sphere_ = sphere;
	}
	void UnRegisterSphere()
	{
		sphere_ = nullptr;
	}

private:
	StageObjectBase* FindTargetObject(btCollisionShape* target);




private:
	static LateStageObjectUpdateManager* instance_;


public:
	static void Create()
	{
		if (instance_ == nullptr)
		{
			instance_ = new LateStageObjectUpdateManager();
		}
	}
	static LateStageObjectUpdateManager& Get()
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