// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include "triple_finder/triple_finder.h"
#include "triple_finder/lief_triple_finder.h"

namespace qresext {

triple_finder_ref
create(KnownTripleFinder type, const io::byte_container_t& filecontent) {
    switch (type) {
    case triple_finder_lief:
        return std::make_unique<LiefTripleFinder>(filecontent);
    }
    return nullptr;
}

} // namespace qresext
