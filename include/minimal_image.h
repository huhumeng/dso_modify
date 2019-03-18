#pragma once

#include <cstring>

#include <Eigen/Core>
#include <opencv2/core/core.hpp>

template<typename T>
class MinimalImage
{
private:

    bool ownData;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	int w;
	int h;
	T* data;


    inline MinimalImage(int w_, int h_) 
    : ownData(true), w(w_), h(h_)
	{
		data = new T[w*h];
	}

    inline MinimalImage(int w_, int h_, T* data_) 
    : ownData(false), w(w_), h(h_), data(data_)
	{
    }

    inline ~MinimalImage()
	{
		if(ownData) 
            delete [] data;
	}

    inline MinimalImage* getClone()
	{
		MinimalImage* clone = new MinimalImage(w, h);
		memcpy(clone->data, data, sizeof(T)*w*h);
		return clone;
	}

    inline T& at(int x, int y){return data[x+y*w];}
	inline T& at(int i){return data[i];}

    inline T at(int x, int y) const {return data[x+y*w];}
	inline T at(int i) const {return data[i];}

    inline void setBlack()
	{
		memset(data, 0, sizeof(T)*w*h);
	}

    inline void setConst(T val)
	{
		for(int i=0;i<w*h;i++) 
            data[i] = val;
	}

    inline void setPixel1(float u, float v, T val)
	{
		at(static_cast<int>(u+0.5f), static_cast<int>(v+0.5f)) = val;
	}

    inline void setPixel4(const float &u, const float &v, T val)
	{

        int uu = static_cast<int>(u);
        int vv = static_cast<int>(v);

		at(uu + 1, vv + 1) = val;
		at(uu + 1, vv) = val;
		at(uu, vv + 1) = val;
		at(uu, vv) = val;
	}

    inline void setPixel9(const int &u, const int &v, T val)
	{
		at(u+1,v-1) = val;
		at(u+1,v) = val;
		at(u+1,v+1) = val;
		at(u,v-1) = val;
		at(u,v) = val;
		at(u,v+1) = val;
		at(u-1,v-1) = val;
		at(u-1,v) = val;
		at(u-1,v+1) = val;
	}

    inline void setPixelCirc(const int &u, const int &v, T val)
	{
        #pragma omp for
		for(int i=-3;i<=3;i++)
		{
			at(u+3, v+i) = val;
			at(u-3, v+i) = val;
			at(u+2, v+i) = val;
			at(u-2, v+i) = val;

			at(u+i, v-3) = val;
			at(u+i, v+3) = val;
			at(u+i, v-2) = val;
			at(u+i, v+2) = val;
		}
	}
};

typedef Eigen::Matrix<unsigned char,3,1> Vec3b;
typedef Eigen::Matrix<float, 3, 1> Vec3f;
typedef MinimalImage<float> MinimalImageF;
typedef MinimalImage<Vec3f> MinimalImageF3;
typedef MinimalImage<unsigned char> MinimalImageB;
typedef MinimalImage<Vec3b> MinimalImageB3;
typedef MinimalImage<unsigned short> MinimalImageB16;

namespace IOwrap{

void displayImage(const char* windowName, const cv::Mat& image, bool autoSize = false);
void displayImageStitch(const char* windowName, const std::vector<cv::Mat*>& images, int cc=0, int rc=0);


void displayImage(const char* windowName, const MinimalImageB* img, bool autoSize = false);
void displayImage(const char* windowName, const MinimalImageB3* img, bool autoSize = false);
void displayImage(const char* windowName, const MinimalImageF* img, bool autoSize = false);
void displayImage(const char* windowName, const MinimalImageF3* img, bool autoSize = false);
void displayImage(const char* windowName, const MinimalImageB16* img, bool autoSize = false);

void displayImageStitch(const char* windowName, const std::vector<MinimalImageB*> images, int cc=0, int rc=0);
void displayImageStitch(const char* windowName, const std::vector<MinimalImageB3*> images, int cc=0, int rc=0);
void displayImageStitch(const char* windowName, const std::vector<MinimalImageF*> images, int cc=0, int rc=0);
void displayImageStitch(const char* windowName, const std::vector<MinimalImageF3*> images, int cc=0, int rc=0);




int waitKey(int milliseconds);
void closeAllWindows();

}