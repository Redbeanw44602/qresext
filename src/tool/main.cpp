// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <argparse/argparse.hpp>

#include "triple_finder/manually_triple_finder.h"
#include "triple_finder/triple_finder.h"
#include "util/fileio.h"
#include "util/qrc.h"

#include "config.h"
#include "logging.h"

using namespace qresext;

auto args_from_cmdline(int argc, char* argv[]) {
    struct {
        std::string qt_bin;
        std::string save_path;
        std::string finder;

        // used by '-f manually'
        uint64_t tree;
        uint64_t names;
        uint64_t payload;
        bool     is_virtual_address;
        bool     is_file_offset;
    } args;

    argparse::ArgumentParser program("qresext", VERSION);

    // clang-format off

    program.add_argument("filename")
        .help("Path to Qt binary executable.")
        .store_into(args.qt_bin)
        .required();

    program.add_argument("-o", "--output")
        .help("Path to save directory.")
        .store_into(args.save_path)
        .required();

    program.add_argument("-f", "--finder")
        .help("Specify the triple(tree, names, payload) finder."
              "[auto, manually, var_symbol]")
        .choices("auto", "manually", "var_symbol")
        .default_value("auto")
        .store_into(args.finder);
    
    program.add_argument("--tree")
        .help("Specify the offset of the tree. (Used by '-f manually')")
        .scan<'x', uint64_t>();

    program.add_argument("--names")
        .help("Specify the offset of the names. (Used by '-f manually')")
        .scan<'x', uint64_t>();

    program.add_argument("--payload")
        .help("Specify the offset of the payload. (Used by '-f manually')")
        .scan<'x', uint64_t>();
    
    auto& offset_type_group = program.add_mutually_exclusive_group();

    offset_type_group.add_argument("--virtual-address")
        .help("Offset is a virtual address. (Used by '-f manually')")
        .store_into(args.is_virtual_address);

    offset_type_group.add_argument("--file-offset")
        .help("Offset is a file offset. (Used by '-f manually')")
        .store_into(args.is_file_offset);

    // clang-format on

    program.parse_args(argc, argv);

    // validate the arguments

    if (args.finder == "manually") {
        if (!program.is_used("--tree") || !program.is_used("--names")
            || !program.is_used("--payload")) {
            throw std::invalid_argument(
                "When using the manual triple finder, "
                "you must specify --tree, --names, --payload."
            );
        }
        if (!program.is_used("--virtual-address")
            && !program.is_used("--file-offset")) {
            throw std::invalid_argument(
                "When using the manual triple finder, you must specify one of "
                "--virtual-address or --file-offset."
            );
        }

        // there's an issue with argparse's `store_into`, so we store it
        // manually now.
        // see https://github.com/p-ranav/argparse/issues/415

        args.tree    = program.get<uint64_t>("--tree");
        args.names   = program.get<uint64_t>("--names");
        args.payload = program.get<uint64_t>("--payload");
    }

    return args;
}

int main(int argc, char* argv[]) try {
    using namespace triple_finder;
    using namespace util;

    auto args = args_from_cmdline(argc, argv);

    // data must be available during the main() lifecycle.
    auto data = read(args.qt_bin);

    // clang-format off
    auto result =
        data.and_then([&args](const byte_container_t& data) {
                if (args.finder == "manually") {
                    auto ret = create(triple_finder_manually, data);
                    auto finder = (ManuallyTripleFinder*)ret->get();
                    finder->set_triple(args.tree, args.names, args.payload);
                    finder->is_virtual_address(args.is_virtual_address);
                    return ret;
                } else if (args.finder == "var_symbol") {
                    return create(triple_finder_var_symbol, data);
                } else {
                    return create(triple_finder_auto, data);
                }
            })
            .and_then([](const triple_finder_ref& finder) {
                return finder->find();
            })
            .and_then([](const ROTriple& triple) {
                return register_resource_data(triple);
            })
            .map([&args]() {
                return dump_resources(args.save_path);
            });
    // clang-format off

    return result ? 0 : 1;
} catch (const std::exception& e) {
    loge("{}", e.what());
    return 1;
}
