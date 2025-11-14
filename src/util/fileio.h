// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include "error.h"

namespace qresext::util {

using byte_container_t = std::vector<uint8_t>;

tl::expected<byte_container_t, ErrorCode> read(std::string_view path);

} // namespace qresext::util
