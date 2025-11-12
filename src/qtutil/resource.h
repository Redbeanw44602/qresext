// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

#include "triple_finder/abstract_triple_finder.h"

namespace qresext::qtutil {

tl::expected<void, ErrorCode>
register_resource_data(const triple_finder::ROTriple& triple);

tl::expected<void, ErrorCode>
unregister_resource_data(const triple_finder::ROTriple& triple);

tl::expected<void, ErrorCode> dump_resources(std::string_view savepath);

} // namespace qresext::qtutil
