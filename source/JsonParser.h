#pragma once

#include "Project.h"

namespace re {

void setJsonBundleTemplate(std::string const& template_name);
void setJsonBundleRoot(std::string const& root);
void setJsonBaseFolder(std::string const& root);

// simple json functions to save an object to disk
// they don't support includes
// they don't support resources
nlohmann::json loadObjectFromFile(std::string basePath, std::string baseName, std::string name);
bool saveObjectToFile(std::string basePath, std::string baseName, std::string name, nlohmann::json obj);


//
// main json configuration loading
//
bool saveJsonObject(std::string const& TAG, nlohmann::json obj, std::string filename);
nlohmann::json loadJsonObject(std::string const& TAG, std::string filename, std::string* error, bool allowResources);

}

