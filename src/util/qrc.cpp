// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <QDirIterator>

#include "util/qrc.h"

#include "logging.h"

// Link to qt corelib.
// (qtbase) src/corelib/io/qresource.cpp

bool qRegisterResourceData(int, const unsigned char*, const unsigned char*, const unsigned char*);
bool qUnregisterResourceData(int, const unsigned char*, const unsigned char*, const unsigned char*);

// ...

namespace qresext::util {

constexpr int RCC_VERSION = 0x3;

tl::expected<void, ErrorCode>
register_resource_data(const triple_finder::ROTriple& triple) {
    if (!qRegisterResourceData(
            RCC_VERSION,
            triple.tree,
            triple.names,
            triple.payload
        )) {
        loge("Failed to register to Qt resource system.");
        return make_error_code(errc_unknown);
    }
    logi("Registered to Qt resource system.");
    return {};
}

tl::expected<void, ErrorCode>
unregister_resource_data(const triple_finder::ROTriple& triple) {
    if (!qUnregisterResourceData(
            RCC_VERSION,
            triple.tree,
            triple.names,
            triple.payload
        )) {
        loge("Failed to unregister from Qt resource system.");
        return make_error_code(errc_unknown);
    }
    logi("Unregistered from Qt resource system.");
    return {};
}

tl::expected<void, ErrorCode> dump_resources(std::string_view savepath) {
    std::string savepath_zterm(savepath);

    if (!fs::exists(savepath_zterm)) {
        fs::create_directories(savepath_zterm);
    }

    QDirIterator iterator(":", QDirIterator::Subdirectories);
    auto         savepath_q = QString::fromStdString(savepath_zterm);

    auto number_count = 0;
    auto size_count   = 0;

    while (iterator.hasNext()) {
        auto qrc_path = iterator.next();              // ":/qml/..."
        auto path     = savepath_q + qrc_path.mid(2); // "/path/qml/..."

        auto info = iterator.fileInfo();

        number_count += 1;
        size_count   += info.size();

        if (info.isDir()) {
            if (!QDir().mkpath(path)) {
                loge("Interrupted, can't mkpath: {}", path.toStdString());
                return make_error_code(errc_failed_to_make_directory);
            }
        } else {
            QFile(path).remove();
            if (!QFile(qrc_path).copy(path)) {
                loge("Interrupted, can't copy file: {}", path.toStdString());
                return make_error_code(errc_failed_to_copy_file);
            }
        }
    }

    logi(
        "{} file(s) dumped, approximately {:.2f} MiB.",
        number_count,
        size_count / 1024.0 / 1024.0
    );

    return {};
}

} // namespace qresext::util