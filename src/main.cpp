#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include "file_utils.h"

// ---------------- CLI ----------------

struct Config
{
    std::string input_dir;
    std::string output_file;
};

Config parse_args(int argc, char *argv[])
{
    Config cfg;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--input" && i + 1 < argc)
        {
            cfg.input_dir = argv[++i];
        }
        else if (arg == "--out" && i + 1 < argc)
        {
            cfg.output_file = argv[++i];
        }
    }

    if (cfg.input_dir.empty() || cfg.output_file.empty())
    {
        std::cerr << "Usage: program --input <dir> --out <file>\n";
        std::exit(1);
    }

    return cfg;
}

// ---------------- Filesystem ----------------

std::vector<std::string> get_txt_files(const std::string &dir)
{
    std::vector<std::string> files;

    for (const auto &entry : std::filesystem::directory_iterator(dir))
    {
        if (entry.is_regular_file() &&
            entry.path().extension() == ".txt")
        {
            files.push_back(entry.path().string());
        }
    }

    return files;
}

// ---------------- Main ----------------

int main(int argc, char *argv[])
{
    Config cfg = parse_args(argc, argv);

    auto files = get_txt_files(cfg.input_dir);

    std::ofstream out(cfg.output_file);
    if (!out.is_open())
    {
        std::cerr << "Cannot open output file\n";
        return 1;
    }

    std::size_t total = 0;

    for (const auto &f : files)
    {
        Result r = count_lines_in_file(f);
        out << f << ": " << r.line_count << "\n";
        total += r.line_count;
    }

    out << "TOTAL: " << total << "\n";
    out.close();

    std::cout << "Done. Results written to " << cfg.output_file << "\n";
    return 0;
}
