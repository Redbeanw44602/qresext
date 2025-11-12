// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

#include "triple_finder/abstract_triple_finder.h"

namespace qresext {

enum KnownTripleFinder {
    triple_finder_lief,
};

triple_finder_ref
create(KnownTripleFinder type, const io::byte_container_t& filecontent);

} // namespace qresext
