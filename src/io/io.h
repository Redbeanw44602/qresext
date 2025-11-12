// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include "error.h"

namespace qresext::io {

using byte_container_t        = std::vector<uint8_t>;
using shared_byte_container_t = std::shared_ptr<const byte_container_t>;

tl::expected<byte_container_t, ErrorCode> read(std::string_view path);

shared_byte_container_t read_shared(std::string_view path);

} // namespace qresext::io
