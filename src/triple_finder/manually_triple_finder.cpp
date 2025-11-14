// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <third_party/lief.h>

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

        LIEF::result<uint64_t> tree, names, payload;

        if (auto elf = LIEF::dyn_cast<LIEF::ELF::Binary>(binary)) {
            tree    = elf->virtual_address_to_offset(m_tree);
            names   = elf->virtual_address_to_offset(m_names);
            payload = elf->virtual_address_to_offset(m_payload);
        } else if (auto macho = LIEF::dyn_cast<LIEF::MachO::Binary>(binary)) {
            tree    = macho->virtual_address_to_offset(m_tree);
            names   = macho->virtual_address_to_offset(m_names);
            payload = macho->virtual_address_to_offset(m_payload);
        } else if (auto pe = LIEF::dyn_cast<LIEF::PE::Binary>(binary)) {
            return make_error_code(errc_not_implemented);
        } else {
            return make_error_code(errc_unreachable);
        }

        if (!tree || !names || !payload) {
            loge("Can't translate virtual address, binary may be corrupted.");
            return make_error_code(errc_invalid_binary_format);
        }

        m_tree    = *tree;
        m_names   = *names;
        m_payload = *payload;
    }

    logi(
        "Qt resource triple has been manually specified. (tree={:#x}, "
        "names={:#x}, "
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
