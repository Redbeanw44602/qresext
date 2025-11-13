// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

#include "triple_finder/abstract_triple_finder.h"

namespace qresext::triple_finder {

class AutoTripleFinder : public TripleFinder {
public:
    using TripleFinder::TripleFinder;

    find_triple_result find() override;
};

} // namespace qresext::triple_finder
