#pragma once

#include "minimal_image.h"

#include <string>
#include <mutex>

class FrameShell;

class System{

public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    System();
    ~System();

    void addActiveFrame(MinimalImageF* image, unsigned long id);

    void printResult(const std::string& file);

private:

    // === changed by tracker-thread. protected by trackMutex ===
    std::mutex trackMutex;
    std::vector<FrameShell*> allFrameHistory;

    // === changed by mapper-thread. protected by mapMutex ===
    std::mutex mapMutex;
    std::vector<FrameShell*> allKeyFramesHistory;

    // mutex for camToWorl's in shells (these are always in a good configuration).
	std::mutex shellPoseMutex;
};