// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <LIEF/LIEF.hpp>

#include "triple_finder/lief_triple_finder.h"

#include "logging.h"

namespace qresext::triple_finder {

find_triple_result _find_in_elf(
    const io::byte_container_t& rawfile,
    const LIEF::ELF::Binary&    elf
) {
    logi("Executable Linkable Format (ELF) binary found.");

    auto symbol_tree    = elf.get_symtab_symbol("_ZL18qt_resource_struct");
    auto symbol_names   = elf.get_symtab_symbol("_ZL16qt_resource_name");
    auto symbol_payload = elf.get_symtab_symbol("_ZL16qt_resource_data");

    if (symbol_tree && symbol_names && symbol_payload) {
        logi("Itanium C++ ABI-compatible binary found.");
    } else {
        loge("Binary may have been stripped, try another triple finder.");
        return make_error_code(errc_symbol_not_found);
    }

    auto rawoff_tree  = elf.virtual_address_to_offset(symbol_tree->value());
    auto rawoff_names = elf.virtual_address_to_offset(symbol_names->value());
    auto rawoff_payload =
        elf.virtual_address_to_offset(symbol_payload->value());

    if (!rawoff_tree || !rawoff_names || !rawoff_payload) {
        loge("Can't translate virtual address, binary may be corrupted.");
        return make_error_code(errc_invalid_binary_format);
    }

    logi(
        "Qt resource triple found (tree={:#x}, names={:#x}, payload={:#x}).",
        *rawoff_tree,
        *rawoff_names,
        *rawoff_payload
    );

    auto result = ROTriple{
        .tree    = rawfile.data() + *rawoff_tree,
        .names   = rawfile.data() + *rawoff_names,
        .payload = rawfile.data() + *rawoff_payload,
    };

    logd(
        "qt_resource_struct at {:#x}, first 64 bytes is: \n{}",
        symbol_tree->value(),
        LIEF::dump(result.tree, 64)
    );
    logd(
        "qt_resource_name at {:#x}, first 64 bytes is: \n{}",
        symbol_names->value(),
        LIEF::dump(result.names, 64)
    );
    logd(
        "qt_resource_data at {:#x}, first 64 bytes is: \n{}",
        symbol_payload->value(),
        LIEF::dump(result.payload, 64)
    );

    return result;
}

find_triple_result LiefTripleFinder::find() {
    auto binary = LIEF::Parser::parse(m_data);
    if (!binary) {
        loge("Not a valid binary.");
        return make_error_code(errc_invalid_binary_format);
    }

    if (LIEF::ELF::Binary::classof(binary.get())) {
        return _find_in_elf(m_data, static_cast<LIEF::ELF::Binary&>(*binary));
    }

    return make_error_code(errc_unreachable);
}

} // namespace qresext::triple_finder
