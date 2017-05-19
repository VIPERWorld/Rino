#ifndef _CaffeClassifier_H_
#define _CaffeClassifier_H_

#include "caffe_header.h"

/**
* \brief:	ʶ����Ԥ����Ϣ
*/
struct PredictionRes
{
	std::string name;	/** ������� */
	int classId;		/** ���ID */
	float percentage;	/** ���ƶ�(�ٷֱ�) */

	PredictionRes() : name(""), classId(0), percentage(0.0f)
	{}

	PredictionRes(std::string& _name, int _id, float _percentage) 
		: name(_name), classId(_id), percentage(_percentage)
	{}
};


/**
* \brief:	Caffe���ʶ�������	
*/
class CaffeClassifier 
{
public:
	/**
	* \brief:	CaffeClassifier���캯��
	* \param [model_file]:	ģ�������ļ�(deploy.prototxt)
	* \param [trained_file]:	ѵ�������caffemodel�ļ�
	* \param [mean_file]:	��ֵ�ļ�(mean.binaryproto)
	* \param [trained_file]:	���˵���ļ�(label.txt)
	*/
	CaffeClassifier(const string& model_file, const string& trained_file, const string& mean_file, const string& label_file);

	std::vector<PredictionRes> classify(const cv::Mat& img, int N = 5);

private:
	void setMean(const string& mean_file);

	/** Ԥ�� */
	std::vector<float> predict(const cv::Mat& img);

	void wrapInputLayer(std::vector<cv::Mat>* input_channels);

	void preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels);

private:
	boost::shared_ptr<Net<float> > m_net;
	cv::Size m_inputGeometry;
	int num_channels_;
	cv::Mat m_mean;
	std::vector<string> m_labels;
};

#endif