// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

#include "triple_finder/abstract_triple_finder.h"

namespace qresext::triple_finder {

enum KnownTripleFinder {
    triple_finder_auto,
    triple_finder_manually,
    triple_finder_lief,
};

tl::expected<triple_finder_ref, ErrorCode>
create(KnownTripleFinder type, const util::byte_container_t& filecontent);

} // namespace qresext::triple_finder
