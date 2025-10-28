#pragma once
/**
 * @\brief ���ʂ̃C���^�[�t�F�[�X���`�������
 */


class BlackOverRay : public IGameObject
{
public:
	/* �R���X�g���N�^ */
	BlackOverRay() {};
	/* �f�X�g���N�^ */
	virtual ~BlackOverRay() {}

	//*< IGameObject�ɂ�Start�AUpdate�ARender�����z�֐��ɂ��Ă��邽��
	//*< �p�����͐�΂�Start�AUpdate�ARender�̓w�b�_�[�ɂď���override������
	//*< �܂����̃N���X���p������Ă���̂�virtual������
	virtual bool Start() override { return true; }
	virtual void Update() override {}
	virtual void Render(RenderContext& rc) override {}



protected:
	FontRender fontRender_;

	/**
	 * @brief �R���p�C������UI�̃t�@�C���p�X��萔��
	 */
	static constexpr const char* UIAseetsFilePath = "Assets/Sprite/UI/";


protected:

	/**
	 * @brief ���F�̃f�B�X�v���C��\��
	 * @param spriteRender_ 
	 */
	void RenderDarkOverlay(SpriteRender* spriteRender_, Vector3 pos);


	/**
	 * @brief UI�̉摜�̏������EUpdate
	 * @param spriteRender_ �X�v���C�g�����_�[
	 * @param failPath �t�@�C���p�X
	 * @param pos ���W
	 * @param scale �傫��
	 * @param mulColor ��Z�J���[
	 */
	void Initialize(SpriteRender* spriteRender, const std::string failPath, const int pixelSize ,const Vector3 pos, const Vector3 scale, const Vector4 mulColor);


	/**
	 * @brief UI�̃A�Z�b�g�p�t�@�C���p�X��������֐�
	 * @param spriteName �g������dds�t�@�C���̖��O
	 * @return 
	 */
	inline std::string FindFailPath_Sprite(const std::string& spriteName)
	{
		return std::string(UIAseetsFilePath) + spriteName;
	}


};

