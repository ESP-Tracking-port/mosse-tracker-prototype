#include <mosse/Mosse_Export.h>
#include <string>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define LARGE_INTEGER unsigned long long;

void run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pX, int pY, int pW, int pH)
{
	int curFrame = pStartFrame;
	int trackerId;
	std::string curFrameName = "";
	cv::namedWindow("output");
	cv::namedWindow("filter");
	cv::namedWindow("response");
	cv::moveWindow("output", 600, 300);
	cv::moveWindow("filter", 600, 200);
	cv::moveWindow("response", 700, 200);
	cv::Mat imRgb, imGray;

	int rectX = pX;
	int rectY = pY;
	int rectW = pW;
	int rectH = pH;

	float learnRate = 0.08f;	// The Mosse Tracker learning rate. Read the paper for more information

	// For FPS counting
	double dt = 0;
	bool firstFps = true;
	double alpha = 0.05;

	// To output
	cv::Mat imH = cv::Mat(pW, pH, cv::DataType<unsigned char>::type);
	cv::Mat imR = cv::Mat(pW, pH, cv::DataType<unsigned char>::type);

	while (true)
	{
		// Generate new file name
		curFrameName = std::to_string(curFrame);
		curFrameName.insert(0, pZeros - curFrameName.length(), '0');
		curFrameName = std::string(pWorkDir).append(curFrameName).append(pFileExt);

		// Read file
		imRgb = cv::imread(curFrameName);
		if (imRgb.empty()) break;					// No more files
		cv::cvtColor(imRgb, imGray, CV_RGB2GRAY);	// Convert to graysacle

		// Call tracker
		if (curFrame == pStartFrame)
			trackerId = Mosse_Init(imGray.ptr(), (int)imGray.step, rectX, rectY, rectW, rectH, learnRate);

		// Draw result
		cv::rectangle(imGray, cv::Rect(rectX, rectY, rectW, rectH), cv::Scalar(255, 255, 255), 1);

		// Debug output
		float *ptrH = Mosse_GetFilter(trackerId);
		float minH = *ptrH;
		float maxH = *ptrH;
		for (int i = 1; i < rectW * rectH; ++i)
		{
			if (ptrH[i] > maxH)
				maxH = ptrH[i];
			if (ptrH[i] < minH)
				minH = ptrH[i];
		}
		for (int i = 1; i < rectW * rectH; ++i)
			imH.data[i] = (unsigned char)((ptrH[i] - minH) / (maxH - minH) * 255);
		cv::imshow("filter", imH);
		// Debug
		float *ptrR = Mosse_GetResponse(trackerId);
		float minR = *ptrR;
		float maxR = *ptrR;
		for (int i = 1; i < rectW * rectH; ++i)
		{
			if (ptrR[i] > maxR)
				maxR = ptrR[i];
			if (ptrR[i] < minR)
				minR = ptrR[i];
		}
		for (int i = 1; i < rectW * rectH; ++i)
			imR.data[i] = (unsigned char)((ptrR[i] - minR) / (maxR - minR) * 255);
		cv::imshow("response", imR);

		// Show output
		cv::imshow("output", imGray);
		if (curFrame == 100)
			cv::waitKey(3000);
		else
			cv::waitKey(1);
		++curFrame;
	}

	cv::waitKey(10000);
}

int main()
{
	constexpr const char *kWorkingDir = "/home/dmurashov/Documents/CODE-MosseTracker/MosseRef/video/dog";
	constexpr const char *kFileExtension = ".jpg";
	constexpr int kZeros = 8;
	constexpr int kStartFrame = 0;
	constexpr int kX = 190;
	constexpr int kY = 110;
	constexpr int kW = 30;
	constexpr int kH = 30;
	run(kWorkingDir, kFileExtension, kZeros, kStartFrame, kX, kY, kW, kH);
	return 0;
}

