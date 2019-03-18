#include "dataset_reader.h"
#include "global_calib.h"

#include <dirent.h>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

inline size_t getdir(std::string dir, std::vector<std::string>& files)
{
    DIR* dp;
    struct dirent* dirp;

    if((dp  = opendir(dir.c_str())) == nullptr)
    {
        return -1;
    }

    while((dirp = readdir(dp)) != nullptr){
    	
        std::string name = std::string(dirp->d_name);

    	if(name != "." && name != "..")
    		files.push_back(name);
    }
    closedir(dp);


    std::sort(files.begin(), files.end());

    if(dir.at(dir.length() - 1) != '/') 
        dir = dir+"/";
    
	for(unsigned int i=0;i<files.size();i++)
	{
		if(files[i].at(0) != '/')
			files[i] = dir + files[i];
	}

    return files.size();
}


ImageFolderReader::ImageFolderReader(std::string path, std::string calibFile)
    : isUndistorted(true)
{
    num_file = getdir(path, files);

    std::cout << "load " << num_file << " files...\n";

    std::string l1, l2, l3, l4;

    std::ifstream infile(calibFile);

    std::getline(infile, l1);
	std::getline(infile, l2);
    std::getline(infile, l3);
    std::getline(infile, l4);

    float parsOrg[8];
    int sizeOrg[2];

    if(std::sscanf(l1.c_str(), "%f %f %f %f %f %f %f %f", 
        &parsOrg[0], 
        &parsOrg[1], 
        &parsOrg[2], 
        &parsOrg[3], 
        &parsOrg[4],
        &parsOrg[5], 
        &parsOrg[6], 
        &parsOrg[7]) == 8 &&
       std::sscanf(l2.c_str(), "%d %d", 
        &sizeOrg[0], 
        &sizeOrg[1]) == 2
    )
    {
        printf("Input resolution: %d %d\n", sizeOrg[0], sizeOrg[1]);
        printf("In: %f %f %f %f %f %f %f %f\n",
                parsOrg[0], parsOrg[1], parsOrg[2], parsOrg[3], 
                parsOrg[4], parsOrg[5], parsOrg[6], parsOrg[7]);
    }else{
        printf("Fatal Error...\n");
        return;
    }

    if(parsOrg[4] == 0){
        printf("none rectify\n");
        isUndistorted = false;
        return;
    }

    cv::Mat cameraMatrixK = cv::Mat::eye(3, 3, CV_32F);
    cameraMatrixK.at<float>(0, 0) = parsOrg[0];
    cameraMatrixK.at<float>(0, 2) = parsOrg[2];
    cameraMatrixK.at<float>(1, 1) = parsOrg[1];
    cameraMatrixK.at<float>(1, 2) = parsOrg[3];

    cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_32F);
    distCoeffs.at<float>(0, 0) = parsOrg[4];
    distCoeffs.at<float>(1, 0) = parsOrg[5];
    distCoeffs.at<float>(2, 0) = parsOrg[6];
    distCoeffs.at<float>(3, 0) = parsOrg[7];

    cv::Size imageSize(sizeOrg[0], sizeOrg[1]);

    cv::Mat cameraMatrixKNew = cv::Mat::eye(3, 3, CV_32F);
    
    // cameraMatrixKNew.at<float>(0, 0) = 674.668;
    // cameraMatrixKNew.at<float>(0, 2) = 645.47;
    // cameraMatrixKNew.at<float>(1, 1) = 674.668;
    // cameraMatrixKNew.at<float>(1, 2) = 346.152;

    cv::Mat newCameraMatrixK = cv::getOptimalNewCameraMatrix(cameraMatrixK, distCoeffs, imageSize, 0);

    cv::initUndistortRectifyMap(cameraMatrixK, distCoeffs, cv::Mat(),
        // cv::getOptimalNewCameraMatrix(cameraMatrixK, distCoeffs, imageSize, 0),
        // cameraMatrixKNew,
        newCameraMatrixK,
        imageSize, CV_32FC2, remap1 , remap2
    );

    printf("new camera matrix:\n");
    std::cout << newCameraMatrixK << std::endl;

    Eigen::Matrix3f K;

    K << newCameraMatrixK.at<float>(0, 0), 0, newCameraMatrixK.at<float>(0, 2),
         0, newCameraMatrixK.at<float>(1, 1), newCameraMatrixK.at<float>(1, 2),
         0, 0, 1;

    global::setGlobalCalib(sizeOrg[0], sizeOrg[1], K);

}


cv::Mat ImageFolderReader::getImage(size_t i) const {

    cv::Mat image = cv::imread(files[i], 0);

    if(!isUndistorted)
        return image;

    cv::remap(image, image, remap1, remap2, cv::INTER_LINEAR);

    return image;
}

MinimalImageF* ImageFolderReader::getImageF(size_t i) const {

    MinimalImageF* ret = new MinimalImageF(global::wG[0], global::hG[0]);

    float* data_raw = ret->data;

    cv::Mat image = getImage(i);

    assert(image.isContinuous());

    unsigned char* ptr = image.ptr<unsigned char>();

    for(int i=0; i<global::wG[0]*global::hG[0]; ++i){
        data_raw[i] = ptr[i];
    }


    return ret;
}