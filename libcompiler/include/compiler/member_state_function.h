// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

namespace compiler {

// type-safe member function returning pointer to itself
template<class T, class... Args>
class member_state_function {
public:
    using type = member_state_function (T::*)(Args...);

private:
    type m_ptr;

public:
    member_state_function(type ptr) : m_ptr(ptr) { }

    operator type() { return m_ptr; }
};

} // namespace compiler {
