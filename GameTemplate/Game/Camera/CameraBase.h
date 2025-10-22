#pragma once


class CameraBase : public IGameObject
{
protected:
	Transform transform_;


public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CameraBase() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CameraBase() {};


	/**
	 * @brief ����������
	 * @return 
	 */
	virtual bool Start() { return true; };
	/**
	 * @brief �X�V����
	 */
	virtual void Update()override {};
	/**
	 * @brief �`�揈��
	 * @param rc 
	 */
	virtual void Render(RenderContext& rc)override {};



private:

};