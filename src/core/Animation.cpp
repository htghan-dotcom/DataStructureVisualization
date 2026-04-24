#include "Animation.h"

void Animation::clear() {
    steps_.clear();
    cursor_ = 0;
}

void Animation::setSteps(const std::vector<Step>& steps) {
    steps_ = steps;
    cursor_ = 0;
}

void Animation::addStep(const Step& step) {
    steps_.push_back(step);
}

bool Animation::empty() const {
    return steps_.empty();
}

int Animation::totalSteps() const {
    return static_cast<int>(steps_.size());
}

int Animation::currentIndex() const {
    return cursor_;
}

const Step* Animation::currentStep() const {
    if (steps_.empty()) {
        return nullptr;
    }
    if (cursor_ < 0 || cursor_ >= totalSteps()) {
        return nullptr;
    }
    return &steps_[cursor_];
}

const Step* Animation::stepAt(int index) const {
    if (index < 0 || index >= totalSteps()) {
        return nullptr;
    }
    return &steps_[index];
}

bool Animation::moveNext() {
    if (steps_.empty() || cursor_ + 1 >= totalSteps()) {
        return false;
    }
    ++cursor_;
    return true;
}

bool Animation::movePrev() {
    if (steps_.empty() || cursor_ - 1 < 0) {
        return false;
    }
    --cursor_;
    return true;
}

void Animation::moveToStart() {
    cursor_ = 0;
}

void Animation::moveToEnd() {
    if (steps_.empty()) {
        cursor_ = 0;
        return;
    }
    cursor_ = totalSteps() - 1;
}

void Animation::setCursor(int index) {
    if (steps_.empty()) {
        cursor_ = 0;
        return;
    }
    cursor_ = std::clamp(index, 0, totalSteps() - 1);
}
