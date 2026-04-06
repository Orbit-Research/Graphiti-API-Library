/** ******************************************************************************
 * @file    internal_handle.hpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Small internal C++ handle wrapper for JNI bindings.
 *
 * @details
 * Defines a simple `GraphitiHandle` that contains a `GraphitiExtension` and
 * helper functions used by the JNI bridge to manage lifetime of the C++ API
 * object from Java.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#pragma once
#include <Graphiti/Extension.hpp>

struct GraphitiHandle {
    GraphitiExtension api;
};

inline GraphitiHandle* graphiti_create() {
    /**
     * @function: graphiti_create
     * @brief Allocate and return a new GraphitiHandle containing a GraphitiExtension.
     * @return: GraphitiHandle* - pointer to new handle
     **/
    return new GraphitiHandle();
}