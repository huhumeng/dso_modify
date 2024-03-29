#include "minimal_image.h"

#include <unordered_set>
#include <opencv2/highgui/highgui.hpp>

namespace IOwrap{

std::unordered_set<std::string> openWindows;

void displayImage(const char* windowName, const cv::Mat& image, bool autoSize)
{

	if(!autoSize)
	{
		if(openWindows.find(windowName) == openWindows.end())
		{
			cv::namedWindow(windowName, cv::WINDOW_NORMAL);
			cv::resizeWindow(windowName, image.cols, image.rows);
			openWindows.insert(windowName);
		}
	}

	cv::imshow(windowName, image);
}

void displayImageStitch(const char* windowName, const std::vector<cv::Mat*>& images, int cc, int rc)
{
    if(images.size() == 0) 
        return;

    int w = images[0]->cols;
	int h = images[0]->rows;

    size_t num = images.size();

    int bestCC = 0;
    float bestLoss = 1e10;

    for(int cc=1; cc<10; ++cc){
        int ww = w * cc;
        int hh = h * ((num + cc - 1) / cc);

        float wLoss = ww/16.0f;
		float hLoss = hh/10.0f;

        float loss = std::max(wLoss, hLoss);

        if(loss < bestLoss)
		{
			bestLoss = loss;
			bestCC = cc;
		}
    }

    int bestRC = ((num + bestCC - 1) / bestCC);
	
    if(cc != 0)
	{
		bestCC = cc;
		bestRC= rc;
	}

    cv::Mat stitch = cv::Mat(bestRC * h, bestCC * w, images[0]->type());

    stitch.setTo(0);

    for(int i=0; i<(int)num && i < bestCC * bestRC; i++)
	{
		int c = i % bestCC;
		int r = i / bestCC;

		cv::Mat roi = stitch(cv::Rect(c * w, r * h, w, h));
		images[i]->copyTo(roi);
	}
}

void displayImage(const char* windowName, const MinimalImageB* img, bool autoSize)
{
	displayImage(windowName, cv::Mat(img->h, img->w, CV_8U, img->data), autoSize);
}

void displayImage(const char* windowName, const MinimalImageB3* img, bool autoSize)
{
	displayImage(windowName, cv::Mat(img->h, img->w, CV_8UC3, img->data), autoSize);
}

void displayImage(const char* windowName, const MinimalImageF* img, bool autoSize)
{
	displayImage(windowName, cv::Mat(img->h, img->w, CV_32F, img->data)*(1/254.0f), autoSize);
}

void displayImage(const char* windowName, const MinimalImageF3* img, bool autoSize)
{
	displayImage(windowName, cv::Mat(img->h, img->w, CV_32FC3, img->data)*(1/254.0f), autoSize);
}

void displayImage(const char* windowName, const MinimalImageB16* img, bool autoSize)
{
	displayImage(windowName, cv::Mat(img->h, img->w, CV_16U, img->data), autoSize);
}

void displayImageStitch(const char* windowName, const std::vector<MinimalImageB*> images, int cc, int rc)
{
	std::vector<cv::Mat*> imagesCV;
    
    for(size_t i=0; i < images.size(); i++)
		imagesCV.push_back(new cv::Mat(images[i]->h, images[i]->w, CV_8U, images[i]->data));
	
    displayImageStitch(windowName, imagesCV, cc, rc);
    
    for(size_t i=0; i < images.size();i++)
		delete imagesCV[i];
}

void displayImageStitch(const char* windowName, const std::vector<MinimalImageB3*> images, int cc, int rc)
{
	std::vector<cv::Mat*> imagesCV;

    for(size_t i=0; i < images.size();i++)
		imagesCV.push_back(new cv::Mat(images[i]->h, images[i]->w, CV_8UC3, images[i]->data));
	
    displayImageStitch(windowName, imagesCV, cc, rc);
    
    for(size_t i=0; i < images.size();i++)
		delete imagesCV[i];
}

void displayImageStitch(const char* windowName, const std::vector<MinimalImageF*> images, int cc, int rc)
{
	std::vector<cv::Mat*> imagesCV;
    
    for(size_t i=0; i < images.size();i++)
		imagesCV.push_back(new cv::Mat(images[i]->h, images[i]->w, CV_32F, images[i]->data));
	
    displayImageStitch(windowName, imagesCV, cc, rc);
    
    for(size_t i=0; i < images.size();i++)
		delete imagesCV[i];
}

void displayImageStitch(const char* windowName, const std::vector<MinimalImageF3*> images, int cc, int rc)
{
	std::vector<cv::Mat*> imagesCV;
    
    for(size_t i=0; i < images.size();i++)
		imagesCV.push_back(new cv::Mat(images[i]->h, images[i]->w, CV_32FC3, images[i]->data));
	
    displayImageStitch(windowName, imagesCV, cc, rc);
    
    for(size_t i=0; i < images.size();i++)
		delete imagesCV[i];
}

int waitKey(int milliseconds)
{
	return cv::waitKey(milliseconds);
}

void closeAllWindows()
{
	cv::destroyAllWindows();
	openWindows.clear();
}

}