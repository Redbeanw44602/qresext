// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <LIEF/LIEF.hpp>

#include "triple_finder/manually_triple_finder.h"

#include "logging.h"

namespace qresext::triple_finder {

find_triple_result ManuallyTripleFinder::find() {
    if (m_is_virtual_address) {
        auto binary = LIEF::Parser::parse(m_data);
        if (!binary) {
            loge("Not a valid binary.");
            return make_error_code(errc_invalid_binary_format);
        }

        if (LIEF::ELF::Binary::classof(binary.get())) {
            auto& elf = static_cast<LIEF::ELF::Binary&>(*binary);

            auto rawoff_tree    = elf.virtual_address_to_offset(m_tree);
            auto rawoff_names   = elf.virtual_address_to_offset(m_names);
            auto rawoff_payload = elf.virtual_address_to_offset(m_payload);

            if (!rawoff_tree || !rawoff_names || !rawoff_payload) {
                loge("Can't translate virtual address, binary may be corrupted."
                );
                return make_error_code(errc_invalid_binary_format);
            }

            m_tree    = *rawoff_tree;
            m_names   = *rawoff_names;
            m_payload = *rawoff_payload;
        } else {
            return make_error_code(errc_unreachable);
        }
    }

    logi(
        "Qt resource triple found (tree={:#x}, names={:#x}, "
        "payload={:#x}).",
        m_tree,
        m_names,
        m_payload
    );

    auto result = ROTriple{
        .tree    = m_data.data() + m_tree,
        .names   = m_data.data() + m_names,
        .payload = m_data.data() + m_payload,
    };

    logdebug("qt_resource_struct", m_tree, result.tree);
    logdebug("qt_resource_name", m_names, result.names);
    logdebug("qt_resource_data", m_payload, result.payload);

    return result;
}

} // namespace qresext::triple_finder
