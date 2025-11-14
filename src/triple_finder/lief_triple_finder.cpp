// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#include <third_party/lief.h>

#include "triple_finder/lief_triple_finder.h"

#include "logging.h"

namespace qresext::triple_finder {

find_triple_result _find_in_elf(
    const util::byte_container_t& rawfile,
    const LIEF::ELF::Binary&      elf
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

    logdebug("qt_resource_struct", symbol_tree->value(), result.tree);
    logdebug("qt_resource_name", symbol_names->value(), result.names);
    logdebug("qt_resource_data", symbol_payload->value(), result.payload);

    return result;
}

find_triple_result _find_in_macho(
    const util::byte_container_t& rawfile,
    const LIEF::MachO::Binary&    macho
) {
    return make_error_code(errc_not_implemented);
}

find_triple_result
_find_in_pe(const util::byte_container_t& rawfile, const LIEF::PE::Binary& pe) {
    return make_error_code(errc_not_implemented);
}

find_triple_result LiefTripleFinder::find() {
    auto binary = LIEF::Parser::parse(m_data);
    if (!binary) {
        loge("Not a valid binary.");
        return make_error_code(errc_invalid_binary_format);
    }

    if (auto elf = LIEF::dyn_cast<LIEF::ELF::Binary>(binary)) {
        return _find_in_elf(m_data, *elf);
    } else if (auto macho = LIEF::dyn_cast<LIEF::MachO::Binary>(binary)) {
        return _find_in_macho(m_data, *macho);
    } else if (auto pe = LIEF::dyn_cast<LIEF::PE::Binary>(binary)) {
        return _find_in_pe(m_data, *pe);
    } else {
        return make_error_code(errc_unreachable);
    }
}

} // namespace qresext::triple_finder
