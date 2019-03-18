#include "dataset_reader.h"

#include <opencv2/opencv.hpp>

int main(int argc, char** argv){

    // ------------- config -------------
    size_t start = 176;
    size_t end   = 10000;
    bool reserve = false;
    bool step    = 1;
    // -------------  end   -------------

    ImageFolderReader reader(argv[1], argv[2]);

    end = std::min(end, reader.getImageNum());

    if(reserve){
        std::swap(start, end);
        step = -step;
    }
        
    for(size_t i=start; i<=end; i+=step){

        std::cout << std::setw(6) << std::setfill('0') << "------------- frame " << i << " -------------" << std::endl;

        MinimalImageF* image = reader.getImageF(i);

        IOwrap::displayImage("image", image);
        IOwrap::waitKey(0);
    }


    IOwrap::closeAllWindows();

    return 0;
}