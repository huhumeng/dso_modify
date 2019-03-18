#pragma once

#include <sophus/se3.h>

class FrameShell{

public:

    FrameShell();

    ~FrameShell() = default;


private:
    
    unsigned long   id; 			// INTERNAL ID, starting at zero.
	unsigned long   incoming_id;	// ID passed into DSO
    double          timestamp;      // timestamp passed into DSO.

    // set once after tracking
    Sophus::SE3 camToTrackingRef;
	FrameShell* trackingRef;

    // @{
    //  constantly adapted.

    // Write: TRACKING, while frame is still fresh;
    // MAPPING: only when locked [shellPoseMutex].
    Sophus::SE3 camToWorld;			 
	bool poseValid;

    // }

    // statisitcs
    int statistics_outlierResOnThis;
	int statistics_goodResOnThis;
	int marginalizedAt;
    double movedByOpt;

    friend class System;
    friend class FrameHession;
};