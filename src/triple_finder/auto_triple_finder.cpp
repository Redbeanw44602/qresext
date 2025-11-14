// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <third_party/lief.h>

#include "triple_finder/auto_triple_finder.h"
#include "triple_finder/triple_finder.h"

#include "logging.h"

namespace qresext::triple_finder {

find_triple_result AutoTripleFinder::find() {
    auto automatically_finders = {triple_finder_var_symbol};

    for (auto which : automatically_finders) {
        auto result =
            create(which, m_data).and_then([](const triple_finder_ref& finder) {
                return finder->find();
            });
        if (result) return result;
    }

    loge("All automatic finders have been tried, but no triples can be found.");
    return make_error_code(errc_failed_to_find_triple);
}

} // namespace qresext::triple_finder
