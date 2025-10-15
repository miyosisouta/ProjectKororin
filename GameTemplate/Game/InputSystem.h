/**
 * InputSystem.h
 * ���͑���ɂ��Ă܂Ƃ߂�N���X�̐錾
 */
#pragma once


class Sphere;
class InputSystem : public IGameObject
{
private:
	Sphere* target_ = nullptr;
	int inputNo_ = 0;


public:
	/**
	 * @brief �R���X�g���N�^
	 */
	InputSystem() {}
	/**
	 * @brief �f�X�g���N�^
	 */
	~InputSystem() {}

	/**
	 * @brief �I�u�W�F�N�g�̊J�n���������s���܂��B
	 * @return �J�n���������������ꍇ�� true�A���s�����ꍇ�� false ��Ԃ��܂��B
	 */
	bool Start() override;
	/**
	 * @brief �I�u�W�F�N�g�̏�Ԃ��X�V���܂��B���N���X��Update���\�b�h���I�[�o�[���C�h���Ă��܂��B
	 */
	void Update() override;
	/**
	 * @brief �`�揈�������s���܂�
	 * @param rc 
	 */
	void Render(RenderContext& rc) override;


public:
	/**
	 * @brief �^�[�Q�b�g�ƂȂ� Sphere �I�u�W�F�N�g��ݒ肵�܂��B
	 * @param target �ݒ肷�� Sphere �I�u�W�F�N�g�ւ̃|�C���^�B
	 */
	void SetTarget(Sphere* target) { target_ = target; }


	/**
	 * @brief �R���g���[���[�ԍ���ݒ肵�܂��B
	 * @param no �ݒ肷��R���g���[���[�ԍ��B
	 */
	void SetControllerNo(const int no) { inputNo_ = no; }


private:
	/**
	 * @brief �w�肳�ꂽ���͔ԍ��ɑΉ�����p�b�h���擾���܂��B
	 * @return ���͔ԍ��ɑΉ�����p�b�h�ւ̃|�C���^�B
	 */
	auto* GetPad() { return g_pad[inputNo_]; }


	/**
	 * @brief ���X�e�B�b�N�̌��݂̈ʒu���擾���܂��B
	 * @return ���X�e�B�b�N�̈ړ����x��\���B
	 */
	Vector3 GetStickL();


private:
	
};