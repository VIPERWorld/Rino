#ifndef CPUImageIdentification_H
#define CPUImageIdentification_H


#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/nonfree/features2d.hpp>

#include "imageidentification.h"
#include "opencv_gpu_cpu.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <map>

using namespace std;
using namespace cv;

struct ImageDescriptors
{
    vector<KeyPoint> keyPoints;     //特征关键点--
    Mat descriptors;                    //计算描述符（特征向量）--
    flann::Index* pFlannIndex;
};

typedef  std::map<string,vector<ImageDescriptors*>> ImageDescriptorsMap;



class CPUImageIdentification:public CImageIdentification
{
public:
    CPUImageIdentification();

    /**
     * @brief initResourceImage 初始化原始图片识别资源，系统必须进行调用初始化
     *
     * @return
     */
    virtual bool initResourceImage(IndentificationMap& imagesMap);

    /**
     * @brief shibie    函数识别 程序获取图片之后主动发起调用，该函数将进行识别循环处理
     * @param image 图片对象，使用OPENCV的Mat格式
     * @return  null表示没有找到，否则返回被识别对象名称
     */
	virtual char* shibie(void* pImage);
 

    /**
     * @brief match 根据遍历系统的 返回的图片特征对象
     * @param image 图片对象
     * @param pImageDescriptors 描述符
     * @return 返回匹配结果，超过30，则认为比较准确的识别
     */
    int match(Mat& image, ImageDescriptors* pImageDescriptors);
	int matchByDescriptors(Mat& captureDescriptionDes, ImageDescriptors* pImageDescriptors);

    /**
     * @brief makeImageDescriptors 创建图片特征值，用于创建初始化
     * @param fileName
     * @return
     */
    ImageDescriptors* makeImageDescriptors(string& fileName);
 protected:

    ImageDescriptorsMap m_imageDescriptorsMap;
    std::map<ImageDescriptors*,string> m_imageDescriptors2stringMap;

};



#endif // CPUImageIdentification_H
