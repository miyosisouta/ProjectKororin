/**
 * SoundManager.h
 * �T�E���h�Ǘ�
 * �K�v�ȃT�E���h�t�@�C����ǂݍ��񂾂�Đ�������ȂǊǗ�����
 */
#pragma once
#include <memory>
#include "Sound/SoundSource.h"
#include "sound/Types.h"


 /** �T�E���h�p�̃n���h���� */
using SoundHandle = uint32_t;
/** �n���h�������l */
static constexpr SoundHandle INVALID_SOUND_HANDLE = 0xffffffff;


/**
 * �T�E���h�Ǘ��N���X
 */
class SoundManager
{
private:
    /** BMG�p�̃T�E���h�\�[�X�C���X�^���X��ێ� */
    SoundSource* bgm_ = nullptr;
    /** SE�p�̃T�E���h�\�[�X�C���X�^���X��ێ� */
    std::map<SoundHandle, SoundSource*> seList_;
    /**
     * �}�b�v�ŎQ�Ƃ���悤�Ƀn���h������ێ�
     * NOTE: �e�T�E���h�\�[�X���Q�Ƃ��邽�߂̐��l�̓��j�[�N�Ȑ��l�ɂȂ�
     */
    SoundHandle soundHandleCount_ = 0;


private:
    SoundManager();
    ~SoundManager();


public:
    /**
     * �X�V����
     * NOTE: �s�v�ɂȂ����C���X�^���X�̍폜�Ȃ�
     */
    void Update();


public:
    /** BGM�Đ� */
    void PlayBGM(const int kind);
    /** BGM��~ */
    void StopBGM();

    /** SE�Đ� */
    SoundHandle PlaySE(const int kind, const bool isLood = false, const bool is3D = false);
    /** SE��~ */
    void StopSE(const SoundHandle handle);


    SoundSource* FindSE(const SoundHandle handle)
    {
        auto it = seList_.find(handle);
        if (it != seList_.end()) {
            return it->second;
        }
        K2_ASSERT(false, "�폜�ς݂��ǉ�����Ă��Ȃ�SE�ɃA�N�Z�X���悤�Ƃ��Ă��܂��B\n");
        return nullptr;
    }




    /**
     * �V���O���g���p
     */
public:
    /**
     * �C���X�^���X�����
     */
    static void CreateInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new SoundManager();
        }
    }


    /**
     * �C���X�^���X���擾
     */
    static SoundManager& Get()
    {
        return *instance_;
    }


    /**
     * �C���X�^���X��j��
     */
    static void DestroyInstance()
    {
        if (instance_ != nullptr)
        {
            delete instance_;
            instance_ = nullptr;
        }
    }

private:
    /** �V���O���g���C���X�^���X */
    static SoundManager* instance_;
};