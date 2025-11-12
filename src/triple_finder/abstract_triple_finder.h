// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

#include "error.h"
#include "io/io.h"

namespace qresext {

using find_triple_result = tl::expected<struct ROTriple, ErrorCode>;
using triple_finder_ref  = std::unique_ptr<class TripleFinder>;

struct ROTriple {
    const uint8_t* tree;    // qt_resource_struct
    const uint8_t* names;   // qt_resource_name
    const uint8_t* payload; // qt_resource_data
};

class TripleFinder {
public:
    explicit TripleFinder(const io::byte_container_t& filecontent)
    : m_data(filecontent) {}
    virtual ~TripleFinder() = default;

    virtual find_triple_result find() = 0;

protected:
    const io::byte_container_t& m_data;
};

} // namespace qresext
