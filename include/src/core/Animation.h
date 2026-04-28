#pragma once

#include "Step.h"
#include "../config/Common.h"

class Animation {
public:
    void clear();
    void setSteps(const std::vector<Step>& steps);
    void addStep(const Step& step);

    bool empty() const;
    int totalSteps() const;
    int currentIndex() const;

    const Step* currentStep() const;
    const Step* stepAt(int index) const;

    bool moveNext();
    bool movePrev();
    void moveToStart();
    void moveToEnd();
    void setCursor(int index);

private:
    std::vector<Step> steps_;
    int cursor_ = 0;
};
