#include "file_utils.h"
#include <fstream>

Result count_lines_in_file(const std::string &filepath)
{
    std::ifstream file(filepath);
    std::size_t lines = 0;
    std::string tmp;

    if (!file.is_open())
    {
        return {filepath, 0};
    }

    while (std::getline(file, tmp))
    {
        ++lines;
    }

    return {filepath, lines};
}
