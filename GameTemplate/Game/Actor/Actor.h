/**
 * Actor.h
 * �����ڂ����݂���I�u�W�F�N�g�Ɍp��������
 */
#pragma once


class Actor : public IGameObject
{
protected:
	ModelRender modelRender_;
	Transform transform_;


public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Actor() {}
	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~Actor() {}

	virtual bool Start() override { return true; }
	virtual void Update() override {}
	virtual void Render(RenderContext& rc) override {}


public:
	Transform* GetTransform() { return &transform_; }
};