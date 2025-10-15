#include "stdafx.h"
#include "SceneLoader.h"


bool LoadScene(const std::string& filepath, const std::function<bool(const nlohmann::json&)>& callback)
{
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    nlohmann::json jsonRoot;
    file >> jsonRoot;

    if (!jsonRoot.contains("objects") || !jsonRoot["objects"].is_array())
        return false;

    // オブジェクトを一つ一つ呼び出す
    for (const auto& obj : jsonRoot["objects"]) {
        if (!callback(obj)) {
            // 何か処理があれば。現状はなし。
        }
    }

    return true;
}