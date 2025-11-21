#pragma once
#include <Graphiti/Extension.hpp>

struct GraphitiHandle {
    GraphitiExtension api;
};

GraphitiHandle* graphiti_create() {
    return new GraphitiHandle();
}