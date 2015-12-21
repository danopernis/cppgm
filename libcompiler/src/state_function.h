// Copyright (c) 2016-2020 Dano Perniš
// See LICENSE for details
#pragma once

// This witchcraft enables type-safe functions returning pointers to themselves
template<class... Args>
class state_function {
public:
    using type = state_function(Args...);

private:
    type* m_ptr;

public:
    state_function(type* ptr) : m_ptr(ptr) { }

    operator type*() { return m_ptr; }
};
