// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

#include "triple_finder/abstract_triple_finder.h"

namespace qresext::triple_finder {

class ManuallyTripleFinder : public TripleFinder {
public:
    using TripleFinder::TripleFinder;

    find_triple_result find() override;

    void set_triple(uint64_t tree, uint64_t names, uint64_t payload) {
        m_tree    = tree;
        m_names   = names;
        m_payload = payload;
    }

    void is_virtual_address(bool value) { m_is_virtual_address = value; }

private:
    uint64_t m_tree, m_names, m_payload;
    bool     m_is_virtual_address;
};

} // namespace qresext::triple_finder
