// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <argparse/argparse.hpp>

#include "io/io.h"
#include "triple_finder/triple_finder.h"

#include "config.h"
#include "logging.h"

using namespace qresext;

auto args_from_cmdline(int argc, char* argv[]) {
    struct {
        std::string filename;
    } args;

    argparse::ArgumentParser program("qresext", VERSION);

    // clang-format off

    program.add_argument("filename")
        .help("Path to Qt binary executable.")
        .required();

    // clang-format on

    program.parse_args(argc, argv);

    args.filename = program.get<std::string>("filename");

    return args;
}

int main(int argc, char* argv[]) try {
    auto args = args_from_cmdline(argc, argv);

    auto data = io::read_shared(args.filename);
    if (!data) {
        return 1;
    }

    if (auto triple_finder = create(triple_finder_lief, *data)) {
        triple_finder->find();
    }

    return 0;
} catch (const std::exception& e) {
    loge("{}", e.what());
    return 1;
}
