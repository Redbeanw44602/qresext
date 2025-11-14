// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2025-present, RedbeanW.
 * This file is part of the qresext software.
 */

#pragma once

#include <LIEF/LIEF.hpp>

namespace LIEF {

template <class T, class U>
T* dyn_cast(U* object) noexcept {
    return (object && T::classof(object)) ? static_cast<T*>(object) : nullptr;
}

template <class T, class U>
T* dyn_cast(const std::unique_ptr<U>& object) noexcept {
    return dyn_cast<T, U>(object.get());
}

} // namespace LIEF
