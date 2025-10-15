#include "TransformHelper.h"
#include <iostream>
#include <fstream>
#include <functional>


bool LoadScene(const std::string& filepath, const std::function<bool(const nlohmann::json&)>& callback);