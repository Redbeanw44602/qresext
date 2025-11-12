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

shared_byte_container_t read_shared(std::string_view path) {
    // clang-format off
    return *io::read(path)
        .transform([](const io::byte_container_t& bytes) {
            return std::make_shared<const std::remove_cvref_t<decltype(bytes)>>(
                bytes
            );
        });
    // clang-format on
}

} // namespace qresext::io
