#include "system.h"
#include "frame_shell.h"

#include <fstream>
#include <iomanip>

System::System(){

}

void System::addActiveFrame(MinimalImageF* image, unsigned long id){

}

void System::printResult(const std::string& file){
    
    std::unique_lock<std::mutex> lock(trackMutex);
    std::unique_lock<std::mutex> crlock(shellPoseMutex);
    
    std::ofstream myfile;
	myfile.open (file.c_str());
	myfile << std::setprecision(15);
    
    for(FrameShell* s : allFrameHistory)
	{
		if(!s->poseValid) 
            continue;

        myfile << s->timestamp <<
			" " << s->camToWorld.translation().transpose()<<
			" " << s->camToWorld.so3().unit_quaternion().x()<<
			" " << s->camToWorld.so3().unit_quaternion().y()<<
			" " << s->camToWorld.so3().unit_quaternion().z()<<
			" " << s->camToWorld.so3().unit_quaternion().w() << "\n";

    }

    myfile.close();
}