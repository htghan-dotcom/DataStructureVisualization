#include "InputHandler.h"

bool InputHandler::validate(OperationType type, const std::string& payload, std::string& error) {
    if (type == OperationType::Initialize) {
        return true;
    }

    if (payload.empty()) {
        error = "Input payload is required for this operation.";
        return false;
    }

    return true;
}
