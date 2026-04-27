#pragma once

enum class AppState {
    Idle,
    Loading,
    Animating,
    Paused,
    Finished,
    Error
};

enum class PlaybackMode {
    StepByStep,
    RunAtOnce
};
