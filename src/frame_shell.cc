#include "frame_shell.h"

FrameShell::FrameShell()
: id(0), 
  timestamp(0.0),
  trackingRef(nullptr),
  camToTrackingRef(),
  camToWorld(),
  poseValid(true),
  statistics_outlierResOnThis(0),
  statistics_goodResOnThis(0),
  marginalizedAt(-1),
  movedByOpt(0.0)
{
    
}