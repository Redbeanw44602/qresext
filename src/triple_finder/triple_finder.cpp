// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include "triple_finder/triple_finder.h"
#include "triple_finder/auto_triple_finder.h"
#include "triple_finder/lief_triple_finder.h"
#include "triple_finder/manually_triple_finder.h"

#include "logging.h"

namespace qresext::triple_finder {

tl::expected<triple_finder_ref, ErrorCode>
create(KnownTripleFinder type, const util::byte_container_t& filecontent) {
    switch (type) {
    case triple_finder_auto:
        logi("Selected triple finder: 'auto'");
        return std::make_unique<AutoTripleFinder>(filecontent);
    case triple_finder_manually:
        logi("Selected triple finder: 'manually'");
        return std::make_unique<ManuallyTripleFinder>(filecontent);
    case triple_finder_lief:
        logi("Selected triple finder: 'lief'");
        return std::make_unique<LiefTripleFinder>(filecontent);
    }
    return make_error_code(errc_invalid_finder_type);
}

} // namespace qresext::triple_finder
