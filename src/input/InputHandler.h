#pragma once

#include "../config/Common.h"

enum class OperationType {
    Initialize,
    Add,
    Delete,
    Update,
    Search
};

struct OperationRequest {
    OperationType type = OperationType::Initialize;
    std::string payload;
};

class InputHandler {
public:
    static bool validate(OperationType type, const std::string& payload, std::string& error);
};
