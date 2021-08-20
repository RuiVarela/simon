#include "Project.h"

using namespace re;

static bool jsonFileExist(std::string path)
{
    return fileExists(path);
}

static std::string loadJsonString(std::string path)
{
    std::string content;
    if (!readRawText(path, content))
    {
        content = "";
    }
    return content;
}

static std::string g_bundle_template = "";
static std::string g_bundle_root = ":/data/templates/";
static std::string g_base_root = "";

namespace re
{

    void setJsonBundleTemplate(const std::string &template_name)
    {
        g_bundle_template = template_name;
    }

    void setJsonBundleRoot(std::string const &root)
    {
        g_bundle_root = root;
    }

    void setJsonBaseFolder(std::string const &root)
    {
        g_base_root = root;
    }

    static std::string reduceName(std::string filename)
    {
        replace(filename, g_bundle_root, "[bundle_root]/");
        replace(filename, g_base_root, "[base_root]");
        return filename;
    }

    static nlohmann::json loadJsonObject(const std::string &TAG, std::string filename, std::string *error, bool allowResources, bool allowFile, std::string indent);

    static bool getTemplateInfo(std::string &loading_file, std::string &template_name)
    {

        std::string name = getSimpleFileName(loading_file);

        // check if a resource version exists
        std::string template_filename = g_bundle_root + g_bundle_template + "/" + name;
        std::string default_template_filename = g_bundle_root + name;

        if (!g_bundle_template.empty() && jsonFileExist(template_filename))
        {
            loading_file = template_filename;
            template_name = g_bundle_template;
            return true;
        }
        else if (jsonFileExist(default_template_filename))
        {
            loading_file = default_template_filename; //there isn't, lets try to load the default one
            template_name = "[base_template]";
            return true;
        }

        return false;
    }

    static nlohmann::json processJsonObjectIncludes(std::string const &TAG, const std::string &originalFilename, nlohmann::json obj, std::string *error, bool allowResources, std::string indent)
    {
        std::vector<std::string> include_files;

        for (auto it = obj.begin(); it != obj.end(); ++it)
        {
            if (it.key() == "pragma include")
            {
                if (it->is_string())
                {
                    std::string sv = it->get<std::string>();
                    if (!sv.empty())
                    {
                        include_files.push_back(sv);
                    }
                }
                else if (it->is_array())
                {
                    for (auto v : *it)
                    {
                        if (v.is_string())
                        {
                            std::string sv = v.get<std::string>();
                            if (!sv.empty())
                            {
                                include_files.push_back(sv);
                            }
                        }
                    }
                }
            }
            else if (it->is_object())
            {
                obj[it.key()] = processJsonObjectIncludes(TAG, originalFilename, *it, error, allowResources, indent);
            }
        }

        if (!include_files.empty())
        {
            obj.erase("pragma include"); //remove the "pragma include" object

            for (auto include_filename : include_files)
            {
                if (!endsWith(include_filename, ".json"))
                {
                    include_filename += ".json";
                }

                include_filename = convertFileNameToNativeStyle(getFilePath(originalFilename) + "/" + include_filename);
                logInf(TAG, sfmt("%sInclude %s", indent, reduceName(include_filename)));

                nlohmann::json o = loadJsonObject(TAG, include_filename, error, allowResources, true, indent + "   ");

                //move all inner values to the same level as the "pragma include" object
                for (auto oit = o.begin(); oit != o.end(); ++oit)
                {
                    obj[oit.key()] = oit.value();
                }
            }
        }

        return obj;
    }

    static nlohmann::json mergeJson(nlohmann::json source, nlohmann::json target)
    {
        for (auto source_element = source.begin(); source_element != source.end(); ++source_element)
        {
            std::string name = source_element.key();

            if (source_element->is_object())
            {
                nlohmann::json target_current;
                if (target.contains(name))
                    target_current = target[name];

                target[name] = mergeJson(*source_element, target_current);
            }
            else
            {
                target[name] = source_element.value();
            }
        }
        return target;
    }

    static nlohmann::json processOverrides(std::string const &TAG, const std::string &originalFilename, std::string *error, nlohmann::json obj, std::string indent)
    {
        std::string path = getFilePath(originalFilename);
        std::string suffix = getFileExtension(originalFilename);
        std::string name = getStrippedName(originalFilename);

        std::string override = convertFileNameToNativeStyle(path + "/" + name + ".overrides." + suffix);

        //
        // check if a template exists
        //
        {
            std::string template_override = override;
            std::string template_name;
            if (getTemplateInfo(template_override, template_name))
            {
                logInf(TAG, sfmt("%sOverriding %s from template %s", indent, name, template_name));
                nlohmann::json override_obj = loadJsonObject(TAG, override, error, true, false, indent);
                obj = mergeJson(override_obj, obj);
            }
        }

        //
        // check if a file exists
        //
        if (jsonFileExist(override))
        {

            logInf(TAG, sfmt("%sOverriding %s from file", indent, name));
            nlohmann::json override_obj = loadJsonObject(TAG, override, error, false, true, indent);
            obj = mergeJson(override_obj, obj);
        }

        return obj;
    }

    static nlohmann::json loadJsonObject(const std::string &TAG, std::string filename, std::string *error,
                                         bool allowResources,
                                         bool allowFile,
                                         std::string indent)
    {

        std::string loading_file = filename;

        bool exists_file = allowFile && jsonFileExist(loading_file);

        if (allowResources && !exists_file)
        {
            std::string template_name;
            if (getTemplateInfo(loading_file, template_name))
            {
                logInf(TAG, sfmt("%sUsing %s template", indent, template_name));
            }
        }

        std::string content = loadJsonString(loading_file);
        if (content.empty())
        {
            std::string message = sfmt("%sEmpty json file %s", indent, reduceName(loading_file));
            if (error)
                *error = message;
            logErr(TAG, message);
            return nlohmann::json();
        }

        try
        {
            nlohmann::json resource = nlohmann::json::parse(content);

            if (!resource.empty())
            { //open sucess
                resource = processJsonObjectIncludes(TAG, filename, resource, error, allowResources, indent);
                resource = processOverrides(TAG, filename, error, resource, indent);
            }
            else
            { //failed to open!!!
                std::string message = sfmt("%sFailed to open json file %s", indent, loading_file);
                if (error)
                    *error = message;
                logErr(TAG, message);
            }
            return resource;
        }
        catch (std::exception exception)
        {
            std::string message = sfmt("%sFailed to parse json file %s (%s)", indent, loading_file, exception.what());
            if (error)
                *error = message;
            logErr(TAG, message);
            return nlohmann::json();
        }
        catch (...)
        {
            std::string message = sfmt("%sFailed to parse json file %s", indent, loading_file);
            if (error)
                *error = message;
            logErr(TAG, message);
            return nlohmann::json();
        }
    }

    nlohmann::json loadJsonObject(const std::string &TAG, std::string filename, std::string *error, bool allowResources)
    {
        return loadJsonObject(TAG, filename, error, allowResources, true, "");
    }

    //
    // basic json functions
    //

    nlohmann::json loadObjectFromFile(std::string basePath, std::string baseName, std::string name)
    {
        std::string filename;

        if (baseName.empty() && name.empty())
        {
            filename = basePath;
        }
        else
        {
            filename = sfmt("/%s_%s.json", baseName, name);
            lowercase(filename);
            filename = convertFileNameToNativeStyle(basePath + filename);
        }

        if (!jsonFileExist(filename))
        {
            return nlohmann::json();
        }
        try
        {
            std::string content = loadJsonString(filename);
            if (content.empty())
            {
                logErr("JsonParser", sfmt("empty json file %s", filename));
                return nlohmann::json();
            }

            return nlohmann::json::parse(content);
        }
        catch (std::exception exception)
        {
            logErr("JsonParser", sfmt("Failed to parse json file %s (%s)", filename, exception.what()));
            return nlohmann::json();
        }
        catch (...)
        {
            logErr("JsonParser", sfmt("Failed to parse json file %s", filename));
            return nlohmann::json();
        }
    }

    bool saveObjectToFile(std::string basePath, std::string baseName, std::string name, nlohmann::json obj)
    {
        std::string filename;

        if (baseName.empty() && name.empty())
        {
            filename = basePath;
        }
        else
        {
            filename = sfmt("/%s_%s.json", baseName, name);
            lowercase(filename);
            filename = convertFileNameToNativeStyle(basePath + filename);
        }

        makeDirectoryForFile(filename);

        return saveJsonObject("JsonParser", obj, filename);
    }

    bool saveJsonObject(const std::string &TAG, nlohmann::json obj, std::string filename)
    {
        std::ofstream output;
        output.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc);

        if (output.good())
        {
            output << obj.dump(4);
            output.close();
            return true;
        }
        else
        {
            logErr(TAG, sfmt("Unable to saveJsonObject %s", filename));
            return false;
        }
    }

}
