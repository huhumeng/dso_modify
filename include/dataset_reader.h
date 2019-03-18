#pragma once

#include <string>
#include <vector>

#include <opencv2/core/core.hpp>

#include "minimal_image.h"

class ImageFolderReader{

public:

    ImageFolderReader(std::string path, std::string calibFile);

    inline size_t getImageNum() const {return num_file;}

    cv::Mat getImage(size_t i) const;

    MinimalImageF* getImageF(size_t i) const;

private:
    
    std::vector<std::string>    files;
    size_t                      num_file;
    bool                        isUndistorted;
    cv::Mat                     remap1, remap2;


};