// __!!rengine_copyright!!__ //

#include "Project.h"

#include <fstream>
#include <cstring>
#include <iostream>

static std::string g_root_path = "";

namespace re
{
	void setFilesystemRootPath(std::string const &root)
	{
		g_root_path = root;
		if (!g_root_path.empty() && ((g_root_path.back() == '/') || (g_root_path.back() == '\\')))
		{
			g_root_path.pop_back();
		}
	}

	DirectoryContents getDirectoryContents(std::string const &directory_name)
	{
		//TODO review this, should not add the folder name
		DirectoryContents contents;

		for (const auto &entry : std::filesystem::directory_iterator(directory_name)) {
			contents.push_back(getSimpleFileName(entry.path()));
		}

		return contents;
	}

	std::string getCurrentDirectory()
	{
		return std::filesystem::current_path();
	}

	Bool makeDirectory(std::string const &path)
	{
		if (path.empty())
			return false;
		
		if (fileType(path) == FileDirectory)
			return false;
		
		std::string dir = path;
		std::vector<std::string> paths;
		while (true)
		{
			if (dir == g_root_path)
				break;
			
			if (dir.empty())
				break;
			

			// TODO: review this
			if (fileType(path) == FileNotFound)
			{
				paths.push_back(dir);
			}
			else
			{
				return false;
			}

			dir = getFilePath(std::string(dir));
		}

		while (!paths.empty())
		{
			std::string dir = paths.back();

			if (dir.size() == 2 && dir.c_str()[1] == ':')
			{
				paths.pop_back();
				continue;
			}

			if (!std::filesystem::create_directory(dir))
				return false;

			paths.pop_back();
		}

		return true;
	}

	FileType fileType(std::string const &filename)
	{
		auto status = std::filesystem::status(filename);

		if (std::filesystem::is_directory(status))
		{
			return FileDirectory;
		}
		else if (std::filesystem::is_regular_file(status))
		{
			return FileRegular;
		}

		return FileNotFound;
	}

	Bool makeDirectoryForFile(std::string const &path)
	{
		return makeDirectory(getFilePath(path));
	}

	Uint64 fileSize(std::string const &filename)
	{
		std::filesystem::path path(filename);
		Uint64 size = std::filesystem::file_size(path);
		return size;
	}

	Bool fileExists(std::string const &filename)
	{
		auto status = std::filesystem::status(filename);
		return std::filesystem::exists(status);
	}

	std::string getFilePath(std::string const &filename)
	{
		std::string::size_type slash1 = filename.find_last_of('/');
		std::string::size_type slash2 = filename.find_last_of('\\');

		if (slash1 == std::string::npos)
		{
			if (slash2 == std::string::npos)
			{
				return std::string();
			}
			return std::string(filename, 0, slash2);
		}

		if (slash2 == std::string::npos)
		{
			return std::string(filename, 0, slash1);
		}

		return std::string(filename, 0, slash1 > slash2 ? slash1 : slash2);
	}

	Uint removeFile(const std::string &path)
	{
		return std::filesystem::remove(path) ? 1 : 0;
	}

	Uint removeFolder(const std::string &path)
	{
		return std::filesystem::remove_all(path);
	}

	std::string getSimpleFileName(std::string const &filename)
	{
		std::string::size_type slash1 = filename.find_last_of('/');
		std::string::size_type slash2 = filename.find_last_of('\\');

		if (slash1 == std::string::npos)
		{
			if (slash2 == std::string::npos)
			{
				return filename;
			}

			return std::string(filename.begin() + slash2 + 1, filename.end());
		}

		if (slash2 == std::string::npos)
		{
			return std::string(filename.begin() + slash1 + 1, filename.end());
		}

		return std::string(filename.begin() + (slash1 > slash2 ? slash1 : slash2) + 1, filename.end());
	}

	std::string getFileExtension(std::string const &filename)
	{
		std::string::size_type dot = filename.find_last_of('.');

		if (dot == std::string::npos)
		{
			return std::string("");
		}

		return std::string(filename.begin() + dot + 1, filename.end());
	}

	std::string getLowerCaseFileExtension(std::string const &filename)
	{
		std::string ext = getFileExtension(filename);

		for (std::string::iterator itr = ext.begin(); itr != ext.end(); ++itr)
		{
			*itr = tolower(*itr);
		}

		return ext;
	}

	std::string getNameLessExtension(std::string const &filename)
	{
		std::string::size_type dot = filename.find_last_of('.');

		if (dot == std::string::npos)
		{
			return filename;
		}

		return std::string(filename.begin(), filename.begin() + dot);
	}

	std::string getStrippedName(std::string const &filename)
	{
		std::string simpleName = getSimpleFileName(filename);
		return getNameLessExtension(simpleName);
	}

	Bool readRawText(std::string const &filename, std::string &text)
	{
		Bool is_good = true;

		std::ifstream in(filename.c_str(), std::ios::in | std::ios::ate | std::ios::binary);
		is_good = in.good();

		if (is_good)
		{
			Ulong end_position = Ulong(in.tellg());

			Char *script_c_str = new Char[end_position + 1UL];

			in.seekg(0, std::ios::beg);
			in.read(script_c_str, end_position);
			script_c_str[end_position] = '\0';
			is_good = in.good();
			in.close();

			text = script_c_str;
			delete[](script_c_str);
		}

		convertTextToNativeStyle(text);

		return is_good;
	}

	Bool readTextFile(std::string const &filename, TextLines &text_lines)
	{
		std::ifstream in(filename.c_str());

		if (in.good())
		{
			while (in.good() && !in.eof())
			{
				std::string current_line;
				std::getline(in, current_line);
				text_lines.push_back(current_line);
			}
			in.close();
		}
		else
		{
			return false;
		}

		return true;
	}

	EqualFilename::EqualFilename(std::string const &match_filename, CaseSensitivity match_case_sensitivity) : filename(match_filename), case_sensitivity(match_case_sensitivity)
	{
	}

	Bool EqualFilename::operator()(std::string const &current_directory, std::string const &current_filename, std::string const &directory_filename)
	{
		RENGINE_NOT_USED(current_directory);
		RENGINE_NOT_USED(directory_filename);

		return ((case_sensitivity == CaseInsensitive &&
				 equalCaseInsensitive(filename, current_filename)) ||
				(filename == current_filename));
	}

	EqualExtension::EqualExtension(std::string const &match_extension)
		: extension(match_extension)
	{
	}

	Bool EqualExtension::operator()(std::string const &current_directory, std::string const &current_filename, std::string const &directory_filename)
	{
		RENGINE_NOT_USED(current_directory);
		RENGINE_NOT_USED(directory_filename);

		return equalCaseInsensitive(getFileExtension(current_filename), extension);
	}

	std::string convertFileNameToWindowsStyle(std::string const &filename)
	{
		std::string new_filename(filename);
		std::string::size_type slash = 0;

		while ((slash = new_filename.find_first_of('/', slash)) != std::string::npos)
		{
			new_filename[slash] = '\\';
		}
		return new_filename;
	}

	std::string convertFileNameToUnixStyle(std::string const &filename)
	{
		std::string new_filename(filename);
		std::string::size_type slash = 0;

		while ((slash = new_filename.find_first_of('\\', slash)) != std::string::npos)
		{
			new_filename[slash] = '/';
		}

		return new_filename;
	}

	Bool isFileNameNativeStyle(std::string const &filename)
	{
#if defined(_MSC_VER)
		return filename.find('/') == std::string::npos;
#else  //NOT defined( _MSC_VER )
		return filename.find('\\') == std::string::npos;
#endif //NOT defined( _MSC_VER )
	}

	std::string convertFileNameToNativeStyle(std::string const &filename)
	{
#if defined(_MSC_VER)
		return convertFileNameToWindowsStyle(filename);
#else  //NOT defined( _MSC_VER )
		return convertFileNameToUnixStyle(filename);
#endif //NOT defined( _MSC_VER )
	}

	Bool isTextWindowsStyle(std::string const &text)
	{
		return text.find("\r\n") != std::string::npos;
	}

	Bool isTextLinuxStyle(std::string const &text)
	{
		Bool r_found = text.find("\r") != std::string::npos;
		Bool n_found = text.find("\n") != std::string::npos;

		return !r_found && n_found;
	}

	Bool isTextMacStyle(std::string const &text)
	{
		Bool r_found = text.find("\r") != std::string::npos;
		Bool n_found = text.find("\n") != std::string::npos;

		return r_found && !n_found;
	}

	void convertTextToNativeStyle(std::string &text)
	{
		if (isTextWindowsStyle(text))
		{
			replace(text, "\r\n", "\n");
		}
		else if (isTextMacStyle(text))
		{
			replace(text, "\r", "\n");
		}
	}

	std::string sanitizeDevicePathText(std::string input)
	{
		//if(input.contains("{") && input.contains("}")) {
		//    //remove device path sub id which may lead to false matches
		//    inputString = inputString.left(inputString.lastIndexOf('{')) + inputString.right(inputString.size() - inputString.lastIndexOf('}') - 1);
		//}

		lowercase(input);
		std::string out;
		for (size_t i = 0; i < input.size(); ++i)
		{
			if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= '0' && input[i] <= '9'))
			{
				out += input[i];
			}
		}
		return out;
	}

} // namespace re
