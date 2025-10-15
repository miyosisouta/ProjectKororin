#pragma once

/**
 * 3D�I�u�W�F�N�g�̈ʒu�A��]�A�X�P�[����\���\���̂ł��B
 * ���̃N���X�Ŏg���Ă���Transform�N���X�ƌ^����ʂ��܂�
 */
namespace json
{
    struct Transform
    {
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;
    };
}



static Vector3 ParseVector3(const nlohmann::json& j)
{
    return Vector3(j.at("x").get<float>(), j.at("y").get<float>(), j.at("z").get<float>());
}


static Quaternion ParseQuaternion(const nlohmann::json& j)
{
    return Quaternion(j.at("x").get<float>(), j.at("y").get<float>(), j.at("z").get<float>(), j.at("w").get<float>());
}


static json::Transform ParseTransformComponents(const nlohmann::json& transformJson)
{
    json::Transform trasnform;
    trasnform.position = ParseVector3(transformJson.at("position"));
    trasnform.rotation = ParseQuaternion(transformJson.at("rotation"));
    trasnform.scale = ParseVector3(transformJson.at("scale"));
    return trasnform;
}