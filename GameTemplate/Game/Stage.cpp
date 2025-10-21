#include "stdafx.h"
#include "Stage.h"
#include "SceneLoader/SceneLoader.h"
#include <vector>

// ���̃N���X�ł����g���Ȃ��悤�ɂ��邽�߂�namespace
// �܂��A���̃t�@�C������A�N�Z�X�ł��Ȃ����ߖ��O�̋������h��
namespace
{

	// ���肵�����I�u�W�F�N�g���̃��X�g
	// (1)��(2)���ꂸ�A�I�u�W�F�N�g�̖��O����������
	// array<�^,�f�[�^�̐�>
	const std::array<std::string, 31> objectNamePatterns = {
		"BHouse_01_color01",
		"BHouse_02_color01",
		"BHouse_02_color02",
		"BHouse_02_color03",
		"BHouse_03_color02",
		"BHouse_04_color02",
		"BHouse_04_color03",
		"NBush01",
		"NCubeTree",
		"NFirTree",
		"NGrassFence",
		"NGrassTile",
		"NHouseFloor",
		"NRock_Big",
		"PBench_01",
		"PBusStop",
		"PHydrant",
		"PStreetLight",
		"PTrafficSignal_Big",
		"PLaneBusStop",
		"RIntersection",
		"RLane01",
		"RLane04",
		"RLaneBusStop",
		"VBus_color01",
		"VContainer_color03",
		"VPickUpTruck_color01",
		"VPickUpTruck_color03",
		"VSUV_color01",
		"VSUV_color02",
		"VTaxi",
	};


	// �V�[������ǂݍ��񂾃I�u�W�F�N�g�̖��O�����S��v��
	bool IsMatchObjectName(const char* jsonNameA, const char* nameB)
	{
		if (strcmp(jsonNameA, nameB) == 0) {
			// ���S��v
			return true;
		}
		// ��v���Ȃ�
		return false;
	}

	// �擪���� len ����������v���Ă��邩���肷��֐�
	// �I�u�W�F�N�g���m1�F1�Ŗ��O����v���Ă��邩�𒲂ׂ�
	bool IsForwardMatchObjectName(const char* jsonNameA, const char* nameB)
	{
		auto len = strlen(nameB);
		auto namelen = strlen(jsonNameA);
		if (len > namelen) {
			//���O�������B�s��v�B
			return false;
		}
		if (strncmp(jsonNameA, nameB, len) == 0) {
			// ���S��v
			return true;
		}
		// ��v���Ȃ�
		return false;
	}

	// �I�u�W�F�N�g���m���F1�Ŗ��O����v���Ă��邩�𒲂ׂ�
	bool IsForwardMatchObjectsName(const char* jsonObjectName)
	{
		for (auto targetName : objectNamePatterns) {
			if (IsForwardMatchObjectName(jsonObjectName, targetName.c_str())) {
				return true;
			}
		}
		return false;
	}


	// AttributeValue�f�[�^���擾��int�^�ŕԂ����߂̊֐�
	auto ParseAttachableValue(const nlohmann::json& attributeJson)
	{
		return attributeJson["attachableValue"].get<int>();
	}

	// attachableValue�f�[�^���擾��int�^�ŕԂ����߂̊֐�
	auto ParseRequiredSphereSize(const nlohmann::json& attributeJson)
	{
		return attributeJson["requiredSphereSize"].get<int>();
	}

	// attachableValue�f�[�^���擾��int�^�ŕԂ����߂̊֐�
	auto ParseGrowthAmount(const nlohmann::json& attributeJson)
	{
		return attributeJson["objectRadius"].get<int>();
	}

	// objectAssetName�f�[�^���擾��string�^�ŕԂ����߂̊֐�
	auto ParseObjectAssetName(const nlohmann::json& attributeJson) {
		return attributeJson["objectAssetName"].get<std::string>();
	}
}


bool Stage::Start() 
{

	// for���ŌJ��Ԃ����
	LoadScene("Assets/Scene/SceneExport.json", [&](const nlohmann::json& j)
		{
			// JSON�f�[�^���� "name" �t�B�[���h���擾���Aname �ϐ��Ɋi�[
			const std::string name = j["name"];


			// �I�u�W�F�N�g���� "  " �Ŏn�܂邩�ǂ����𔻒肷��B
			// �Ⴆ�� "block01" �� "blockA" �Ȃǂ���v����B
			if (IsForwardMatchObjectsName(name.c_str())) { // �m�F�������I�u�W�F�N�g�̖��O�����X�g�̒��ɂ��邩�𒲂ׂ�

				// JSON�f�[�^�� "Transform" �t�B�[���h�i�ʒu�E��]�E�X�P�[�����j��
				// ParseTransformComponents �֐��Ńp�[�X���Atransform �ϐ��Ɋi�[���܂��B
				// �p�[�X:�f�[�^�i�������t�@�C���Ȃǁj���v���O�����Łg�����E��́h���āA�g���₷���`�i�\���̂�N���X�Ȃǁj�ɕϊ����邱��
				const auto transform = ParseTransformComponents(j["Transform"]);

				// Attribute�̃A�h���X���擾
				const auto& attributeJson = j["Attribute"];

				// �A�h���X�̒��̃f�[�^���i�[
				const auto attributeValue = ParseAttachableValue(attributeJson); // �z���\���ǂ����̃f�[�^���i�[
				const auto requiredSphereSize = ParseRequiredSphereSize(attributeJson); // �򂪋z���ł���T�C�Y���i�[
				const auto objectAssetName = ParseObjectAssetName(attributeJson); // �A�Z�b�g�̖��O���i�[
				const auto growthAmount = ParseGrowthAmount(attributeJson);
				const auto colliderPivot = ParseVector3(attributeJson.at("colliderCenter")); // �R���C�_�[�̋N�_�̃|�W�V�������i�[
				const auto colliderSize = ParseVector3(attributeJson.at("colliderSize")); // �R���C�_�[�̑傫�����i�[


			// �����ʂ�NewGO
			switch (attributeValue)
			{
				case 0:
				{
					auto* obj = NewGO<AttachableObject>(0, "AttachableObject");
					const auto colliderPivot = ParseVector3(attributeJson.at("colliderCenter")); // �R���C�_�[�̋N�_�̃|�W�V�������i�[
					obj->Initialize(attributeValue, transform.position, transform.scale, transform.rotation, requiredSphereSize ,objectAssetName, growthAmount , colliderPivot, colliderSize);
					attachableObjectList_.push_back(obj);
					break;
				}
				case 1:
				{
					auto* obj = NewGO<StaticObject>(0, "StaticObject");
					obj->Initialize(attributeValue, transform.position, transform.scale, transform.rotation, requiredSphereSize, objectAssetName, growthAmount, colliderPivot, colliderSize);
					staticObjectList_.push_back(obj);
					break;
				}
				case 2:
				{
					auto* obj = NewGO<MovableObject>(0, "MovableObject");
					obj->Initialize(attributeValue, transform.position, transform.scale, transform.rotation, requiredSphereSize, objectAssetName, growthAmount, colliderPivot, colliderSize);
					movableObjectList_.push_back(obj);
					break;
				}
				default:
				{
					K2_ASSERT(false, "���Ή��ł��B�ǉ����Ă��������B");
					break;
				}
			}
		}
		

		return true;
	});



	return true;
}
