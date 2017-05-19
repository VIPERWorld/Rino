#ifndef _PRE_TEXT_IMG_pre_tex_timg_H_
#define _PRE_TEXT_IMG_pre_tex_timg_H_


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/nonfree/gpu.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include "timemanager.h"

using namespace std;
using namespace cv;


void Preprocess( Mat& sourceImg );
void Preprocess_canny( Mat& sourceImg,Mat &edgeImg,int canny_1 = 100 );
void colorFilter(Mat& inputImage, Mat& outputImage);
void colorFilterGay(Mat& inputImage, Mat& outputImage,int grayLevel1 = 50,int grayLevel2 = 150);
void findTextRegion(Mat& srcMat,vector<Rect>& rect);
void constrastAndBright(Mat& srcImag,Mat& desImag,int constrast,int bright);


//heart stone  mat preopt  ---begin--

//���ڲ�����ɫ���˷�ʽ����ϵͳһ�ڣ�����,���ڲ��û���ѧϰ��ʽ����ϵͳ����ʶ����
//1-¯ʯ������ɫ����,ֻ�����������ƣ�����״̬�ȹؼ���ɫ��������ɫϵͳ���豣��!
void heartStone_friendList_color(Mat& srcImage,Mat& resultImage);

//2-ǰ����������ͼƬ��Ϊ���������,���������жϣ���ѯ�õ��ؼ�������Ϣ������
vector<Rect> get_heartStone_friendName_rect(Mat& _image);

//heart stone  mat preopt  ---end--


#endif // _PRE_TEXT_IMG_pre_tex_timg_H_
