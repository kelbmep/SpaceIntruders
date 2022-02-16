#include <fileManager.hpp>
#include <SDL.h>
#include <algorithm>

namespace
{
    constexpr const char* default_file_paths[] =
    {
            "",
            "res/",
            "../res/"
    };
}

void FileManager::set_find_paths(std::vector<std::string> find_paths) const
{
    _find_paths = std::move(find_paths);
}

std::string FileManager::resource_location(std::string filename) const
{
    std::replace(filename.begin(), filename.end(), '\\', '/');

    std::string result;
    this->for_each_path([&](const std::string& path)
        {
            SDL_RWops* file = SDL_RWFromFile((path + filename).c_str(), "r");

            if (file != nullptr)
            {
                SDL_RWclose(file);

                result = path + filename;
                return;
            }

        });

    return result;
}

void FileManager::for_each_path(const std::function<void(std::string)>& callback) const
{
      for (const auto& path : _find_paths)
    {
        callback(path);
        callback(XSTRINGIFY(PROJECT_ROOT_FOLDER)"/" + path);
    }

    for (const auto& path : default_file_paths)
    {
        callback(path);
        callback(std::string(XSTRINGIFY(PROJECT_ROOT_FOLDER)"/") + path);
    }
}