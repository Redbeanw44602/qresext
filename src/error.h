// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

namespace qresext {

enum ErrorCode {
    errc_unknown,
    errc_invalid_binary_format,
    errc_symbol_not_found,
    errc_failed_to_open_file,
    errc_failed_to_read_file,
    errc_unreachable,
};

inline tl::unexpected<ErrorCode> make_error_code(ErrorCode errc) {
    return tl::unexpected(errc);
}

} // namespace qresext
