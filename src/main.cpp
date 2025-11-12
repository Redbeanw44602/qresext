// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <argparse/argparse.hpp>

#include "io/io.h"
#include "qtutil/resource.h"
#include "triple_finder/triple_finder.h"

#include "config.h"
#include "logging.h"

using namespace qresext;

auto args_from_cmdline(int argc, char* argv[]) {
    struct {
        std::string filename;
        std::string savepath;
        std::string finder;
    } args;

    argparse::ArgumentParser program("qresext", VERSION);

    // clang-format off

    program.add_argument("filename")
        .help("Path to Qt binary executable.")
        .required();

    program.add_argument("-o", "--output")
        .help("Path to save directory.")
        .required();

    program.add_argument("-f", "--finder")
        .help("Specify the triple(tree, names, payload) finder.")
        .choices("lief")
        .default_value("lief")
        .implicit_value(true);

    // clang-format on

    program.parse_args(argc, argv);

    args.filename = program.get<std::string>("filename");
    args.savepath = program.get<std::string>("--output");
    args.finder   = program.get<std::string>("--finder");

    return args;
}

int main(int argc, char* argv[]) try {
    using namespace triple_finder;
    using namespace io;
    using namespace qtutil;

    auto args = args_from_cmdline(argc, argv);

    // data must be available during the main() lifecycle.
    auto data = read_shared(args.filename);

    // clang-format off
    auto result =
        data.and_then([&args](const shared_byte_container_t& data) {
                auto finder = triple_finder_lief;
                return create(finder, *data);
            })
            .and_then([](const triple_finder_ref& finder) {
                return finder->find();
            })
            .and_then([](const find_triple_result& triple) {
                return register_resource_data(*triple);
            })
            .map([&args]() {
                return dump_resources(args.savepath);
            });
    // clang-format off

    return result ? 0 : 1;
} catch (const std::exception& e) {
    loge("{}", e.what());
    return 1;
}
