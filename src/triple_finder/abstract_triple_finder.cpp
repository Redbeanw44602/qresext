// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <third_party/lief.h>

#include "logging.h"

#include "triple_finder/abstract_triple_finder.h"

namespace qresext::triple_finder {

void logdebug(
    std::string_view name,
    uint64_t         vaddr,
    const uint8_t*   ptr,
    size_t           len
) {
    logd(
        "{} at {:#x}, first {} bytes is: \n{}",
        name,
        vaddr,
        len,
        LIEF::dump(ptr, len)
    );
}

} // namespace qresext::triple_finder
