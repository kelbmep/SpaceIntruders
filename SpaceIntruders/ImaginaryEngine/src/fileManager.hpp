#pragma once

#include <string>
#include <vector>
#include <functional>

#define XSTRINGIFY(A) STRINGIFY(A)
#define STRINGIFY(A) #A

class FileManager
{
public:
    std::string resource_location(std::string) const;

    void set_find_paths(std::vector<std::string>) const;
    void for_each_path(const std::function<void(std::string)>&) const;
private:
    mutable std::vector<std::string> _find_paths;
};