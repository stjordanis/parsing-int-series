#pragma once

#include <cstddef>
#include <random>
#include <map>
#include <string>

#include "command_line.h"

class Application {

public:
    class Exit {};

private:
    CommandLine cmdline;

    size_t size;
    size_t debug_size;
    std::discrete_distribution<> numbers;
    std::discrete_distribution<> separators;

    std::random_device rd;
    std::mt19937 random;

protected:
    Application(int argc, char* argv[]);

    std::string generate();

private:
    void print_help() const;
};

