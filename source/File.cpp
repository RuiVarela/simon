// __!!rengine_copyright!!__ //

#include "Project.h"
#include <ctype.h>

#if defined( _MSC_VER )
	#include <Io.h>
	#include <Windows.h>
	#include <Winbase.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <direct.h>

	#define mkdir(x,y) _mkdir((x))
    #define r_rmdir _rmdir
    #define r_unlink _unlink
	#define r_stat _stati64

	#ifndef F_OK
	#define F_OK 4
	#endif //F_OK

	#ifndef _IFMT
	#define _IFMT          0170000
	#endif //_IFMT

	#ifndef _IFDIR
	#define _IFDIR         0040000
	#endif //_IFDIR

	#ifndef S_ISDIR
	#define S_ISDIR(m)  (((m)&_IFMT) == _IFDIR)
	#endif //S_ISDIR
	
#else //NOT defined( _MSC_VER )

	#include <dirent.h>
	#include <unistd.h>
	#include <sys/stat.h>

	#define r_stat stat
    #define r_rmdir rmdir
    #define r_unlink unlink
#endif //NOT defined( _MSC_VER )

#include <errno.h>
#include <fstream>
#include <cstring>
#include <iostream>

static std::string g_root_path = "";

namespace re
{
    void setFilesystemRootPath(std::string const& root) {
        g_root_path = root;
        if (!g_root_path.empty() && ((g_root_path.back() == '/') || (g_root_path.back() == '\\'))) {
            g_root_path.pop_back();
        }
    }

    #if defined( _MSC_VER )
    DirectoryContents getDirectoryContents(std::string const& directory_name)
    {
        DirectoryContents contents;

        WIN32_FIND_DATA data;
        std::string path = directory_name + "\\*";
    #ifdef UNICODE
         HANDLE handle = FindFirstFile(stringToWstring(path).c_str(), &data);
    #else
        HANDLE handle = FindFirstFile(path.c_str(), &data);
    #endif // !UNICODE

        if (handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                contents.push_back(wstringToString(data.cFileName));
            }
            while (FindNextFile(handle, &data) != 0);

            FindClose(handle);
        }
        return contents;
    }

	std::string getCurrentDirectory()
	{
		Uint size = GetCurrentDirectory(0, 0);
        std::string current_directory;

#ifdef UNICODE
        wchar_t* conversion = new wchar_t[size + 1];
        GetCurrentDirectory(size, conversion);
        std::wstring ws(conversion);
        current_directory = wstringToString(ws);
        delete[](conversion);
#else
        Char* current_dir = new Char[size + 1];
        GetCurrentDirectory(size, current_dir);
        current_directory(current_dir);
        delete[](current_dir);
#endif // !UNICODE

		return current_directory;
	}
#else //NOT defined( _MSC_VER )

	DirectoryContents getDirectoryContents(std::string const& directory_name)
	{
		DirectoryContents contents;

		DIR *handle = opendir(directory_name.c_str());
		if (handle)
		{
			dirent *rc;
			while ((rc = readdir(handle)) != NULL)
			{
				contents.push_back(rc->d_name);
			}
			closedir(handle);
		}

		return contents;
	}

	std::string getCurrentDirectory()
	{
		std::string path_as_string;

		Char path[PATH_MAX];
		if(getcwd(&path[0], PATH_MAX))
		{
			path_as_string = std::string(path);
		}

		return path_as_string;
	}

#endif //NOT defined( _MSC_VER )

	Bool makeDirectory(std::string const& path)
	{
		if (path.empty())
		{
			return false;
		}

		struct r_stat stbuf;
		if (r_stat(path.c_str(), &stbuf) == 0)
		{
			return (S_ISDIR(stbuf.st_mode));
		}

		std::string dir = path;
		std::vector<std::string> paths;
		while (true)
		{
            if (dir == g_root_path) {
                break;
            }
            
			if (dir.empty())
			{
				break;
			}

			if (r_stat(dir.c_str(), &stbuf) < 0)
			{
				switch (errno)
				{
					case ENOENT:
					case ENOTDIR:
					paths.push_back(dir);
						break;

					default:
					return false;
				}
			}
			dir = getFilePath(std::string(dir));
		}

		while (!paths.empty())
		{
			std::string dir = paths.back();

#if defined( _MSC_VER )
			if (dir.size() == 2 && dir.c_str()[1] == ':')
			{
				paths.pop_back();
				continue;
			}
#endif //#defined( _MSC_VER )

			if (mkdir(dir.c_str(), 0755) < 0)
			{
				return false;
			}
			paths.pop_back();
		}
		return true;
	}

	FileType fileType(std::string const& filename)
	{
		struct r_stat file_stat;
		if (r_stat(filename.c_str(), &file_stat) == 0)
		{
			if (file_stat.st_mode & S_IFDIR)
			{
				return FileDirectory;
			}
			else if (file_stat.st_mode & S_IFREG)
			{
				return FileRegular;
			}
		}
		return FileNotFound;
	}

	Bool makeDirectoryForFile(std::string const& path)
	{
		return makeDirectory(getFilePath(path));
	}

	Uint64 fileSize(std::string const& filename)
	{
		Uint64 size = 0;

		struct r_stat file_stat;
		if (r_stat(filename.c_str(), &file_stat) == 0)
		{
			size = file_stat.st_size;
		}

		return size;
	}

	Bool fileExists(std::string const& filename)
	{

#if defined( _MSC_VER )
		return _access(filename.c_str(), F_OK) == 0;
#else //NOT defined( _MSC_VER )
		return access(filename.c_str(), F_OK) == 0;
#endif //NOT defined( _MSC_VER )
	}

	std::string getFilePath(std::string const& filename)
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
        if (r_unlink(path.c_str()) == 0)
            return 1;

        return 0;
    }

    Uint removeFolder(const std::string &path)
    {
        Uint count = 0;

        DirectoryContents contents = getDirectoryContents(path);
        for (auto current: contents) {
            if (current == "." || current == "..")
                continue;

            std::string current_path = convertFileNameToNativeStyle(path + "/" + current);

            FileType type = fileType(current_path);
            if (type == FileDirectory) {

                count += removeFolder(current_path);

            } else if (type == FileRegular) {

                count += removeFile(current_path);

            }
        }

        if (r_rmdir(path.c_str()) == 0) {
            count++;
        }

        return count;
    }

	std::string getSimpleFileName(std::string const& filename)
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

	std::string getFileExtension(std::string const& filename)
	{
		std::string::size_type dot = filename.find_last_of('.');

		if (dot == std::string::npos)
		{
			return std::string("");
		}

		return std::string(filename.begin() + dot + 1, filename.end());
	}

	std::string getLowerCaseFileExtension(std::string const& filename)
	{
		std::string ext = getFileExtension(filename);

		for (std::string::iterator itr = ext.begin(); itr != ext.end(); ++itr)
		{
			*itr = tolower(*itr);
		}

		return ext;
	}

	std::string getNameLessExtension(std::string const& filename)
	{
		std::string::size_type dot = filename.find_last_of('.');

		if (dot == std::string::npos)
		{
			return filename;
		}

		return std::string(filename.begin(), filename.begin() + dot);
	}

	std::string getStrippedName(std::string const& filename)
	{
		std::string simpleName = getSimpleFileName(filename);
		return getNameLessExtension(simpleName);
	}

	Bool readRawText(std::string const& filename, std::string& text)
	{
		Bool is_good = true;

		std::ifstream in(filename.c_str(), std::ios::in | std::ios::ate | std::ios::binary);
		is_good = in.good();

		if (is_good)
		{
			Ulong end_position = Ulong(in.tellg());

			Char* script_c_str = new Char[end_position + 1UL];

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

	Bool readTextFile(std::string const& filename, TextLines& text_lines)
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

	EqualFilename::EqualFilename(std::string const& match_filename, CaseSensitivity match_case_sensitivity) :
		filename(match_filename), case_sensitivity(match_case_sensitivity)
	{
	}

	Bool EqualFilename::operator()(std::string const& current_directory, std::string const& current_filename, std::string const& directory_filename)
	{
        RENGINE_NOT_USED(current_directory);
        RENGINE_NOT_USED(directory_filename);

		return ( (case_sensitivity == CaseInsensitive &&
				  equalCaseInsensitive(filename, current_filename)) ||
				  (filename == current_filename) );
	}

	EqualExtension::EqualExtension(std::string const& match_extension)
	:extension(match_extension)
	{
	}

	Bool EqualExtension::operator()(std::string const& current_directory, std::string const& current_filename, std::string const& directory_filename)
	{
        RENGINE_NOT_USED(current_directory);
        RENGINE_NOT_USED(directory_filename);

		return equalCaseInsensitive(getFileExtension(current_filename), extension );
	}

	std::string convertFileNameToWindowsStyle(std::string const& filename)
	{
		std::string new_filename(filename);
		std::string::size_type slash = 0;

		while( (slash = new_filename.find_first_of('/', slash)) != std::string::npos)
		{
			new_filename[slash]= '\\';
		}
		return new_filename;
	}

	std::string convertFileNameToUnixStyle(std::string const& filename)
	{
		std::string new_filename(filename);
		std::string::size_type slash = 0;

		while( (slash = new_filename.find_first_of('\\', slash)) != std::string::npos)
		{
			new_filename[slash]='/';
		}

		return new_filename;
	}

	Bool isFileNameNativeStyle(std::string const& filename)
	{
#if defined( _MSC_VER )
		return filename.find('/') == std::string::npos;
#else //NOT defined( _MSC_VER )
		return filename.find('\\') == std::string::npos;
#endif //NOT defined( _MSC_VER )
	}

	std::string convertFileNameToNativeStyle(std::string const& filename)
	{
#if defined( _MSC_VER )
		return convertFileNameToWindowsStyle(filename);
#else //NOT defined( _MSC_VER )
		return convertFileNameToUnixStyle(filename);
#endif //NOT defined( _MSC_VER )
	}

	Bool isTextWindowsStyle(std::string const& text)
	{
		return text.find("\r\n") != std::string::npos;
	}

	Bool isTextLinuxStyle(std::string const& text)
	{
		Bool r_found = text.find("\r") != std::string::npos;
		Bool n_found = text.find("\n") != std::string::npos;

		return !r_found && n_found;
	}

	Bool isTextMacStyle(std::string const& text)
	{
		Bool r_found = text.find("\r") != std::string::npos;
		Bool n_found = text.find("\n") != std::string::npos;

		return r_found && !n_found;
	}

	void convertTextToNativeStyle(std::string& text)
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

    std::string sanitizeDevicePathText(std::string input) {
        //if(input.contains("{") && input.contains("}")) {
        //    //remove device path sub id which may lead to false matches
        //    inputString = inputString.left(inputString.lastIndexOf('{')) + inputString.right(inputString.size() - inputString.lastIndexOf('}') - 1);
        //}

        lowercase(input);
        std::string out;
        for(size_t i = 0; i < input.size(); ++i) {
            if((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= '0' && input[i] <= '9')) {
                out += input[i];
            }
        }
        return out;
    }


} // namespace re


