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

    auto symbol_struct = elf.get_symtab_symbol("_ZL18qt_resource_struct");
    auto symbol_name   = elf.get_symtab_symbol("_ZL16qt_resource_name");
    auto symbol_data   = elf.get_symtab_symbol("_ZL16qt_resource_data");

    if (symbol_struct && symbol_name && symbol_data) {
        logi("Itanium C++ ABI-compatible binary found.");
    } else {
        loge("Binary may have been stripped, try another triple finder.");
        return make_error_code(errc_symbol_not_found);
    }

    auto rawoff_struct = elf.virtual_address_to_offset(symbol_struct->value());
    auto rawoff_name   = elf.virtual_address_to_offset(symbol_name->value());
    auto rawoff_data   = elf.virtual_address_to_offset(symbol_data->value());

    if (!rawoff_struct || !rawoff_name || !rawoff_data) {
        loge("Can't translate virtual address, binary may be corrupted.");
        return make_error_code(errc_invalid_binary_format);
    }

    logi(
        "Qt resource triple found (struct={:#x}, name={:#x}, data={:#x}).",
        *rawoff_struct,
        *rawoff_name,
        *rawoff_data
    );

    auto result = ROTriple{
        .tree    = rawfile.data() + *rawoff_struct,
        .names   = rawfile.data() + *rawoff_name,
        .payload = rawfile.data() + *rawoff_data,
    };

    logd(
        "qt_resource_struct at {:#x}, first 64 bytes is: \n{}",
        symbol_struct->value(),
        LIEF::dump(result.tree, 64)
    );
    logd(
        "qt_resource_name at {:#x}, first 64 bytes is: \n{}",
        symbol_name->value(),
        LIEF::dump(result.names, 64)
    );
    logd(
        "qt_resource_data at {:#x}, first 64 bytes is: \n{}",
        symbol_data->value(),
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
