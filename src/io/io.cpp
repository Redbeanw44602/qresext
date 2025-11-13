// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include "io/io.h"

#include "logging.h"

namespace qresext::io {

tl::expected<byte_container_t, ErrorCode> read(std::string_view path) {
    std::ifstream ifs((std::string(path)), std::ios::binary | std::ios::ate);
    if (!ifs) {
        loge("Can't open file: {}", path);
        return make_error_code(errc_failed_to_open_file);
    }

    auto size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    byte_container_t buffer(size);

    if (!ifs.read(reinterpret_cast<char*>(buffer.data()), size)) {
        loge("Can't read file: {}", path);
        return make_error_code(errc_failed_to_read_file);
    }

    logd("Read {} bytes from {}", buffer.size(), path);
    return buffer;
}

} // namespace qresext::io
