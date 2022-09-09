#include <mosse/Mosse_Export.h>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

#define LARGE_INTEGER unsigned long long;

#if 1
# include <iostream>
# define debug(a) std::cout << (#a) << " : " << (a) << std::endl
# define debugstr(a) std::cout << (a) << std::endl;
#else
# define debug(...)
# define debugstr(...)
#endif

void run(std::string pWorkDir, std::string pFileExt, int pZeros, int pStartFrame, int pX, int pY, int pW, int pH)
{
	int curFrame = pStartFrame;
	int trackerId = 0;
	std::string curFrameName = "";
	cv::namedWindow("output");
	cv::Mat imRgb(320, 240, CV_8UC3);
	cv::Mat imGray(320, 240, CV_8UC1);

	int rectX = pX;
	int rectY = pY;
	int rectW = pW;
	int rectH = pH;

	float learnRate = 0.08f;	// The Mosse Tracker learning rate. Read the paper for more information

	// To output
//	cv::Mat imH = cv::Mat(pW, pH, cv::DataType<unsigned char>::type);
//	cv::Mat imR = cv::Mat(pW, pH, cv::DataType<unsigned char>::type);

	while (true)
	{
		// Generate new file name
		debugstr("markerWhileTrue");
		curFrameName.clear();
		curFrameName = std::to_string(curFrame);
		curFrameName.insert(0, pZeros - curFrameName.length(), '0');
		curFrameName = std::string(pWorkDir).append(curFrameName).append(pFileExt);
		debug(curFrameName);

		// Read file
		imRgb = cv::imread(curFrameName.c_str());
		debugstr("marker imread");

		if (imRgb.empty())
			break;					// No more files

		cv::cvtColor(imRgb, imGray, CV_RGB2GRAY);	// Convert to graysacle
		debugstr("marker cvtColor");
		debug(imRgb.isContinuous());

		// Call tracker
		if (curFrame == pStartFrame) {
			debug(imGray.size());
			trackerId = Mosse_Init(imGray.ptr(), (int)imRgb.step, rectX, rectY, rectW, rectH, learnRate);
			Mosse_OnFrame(trackerId, imGray.ptr(), (int)imRgb.step, rectX, rectY, rectW, rectH);
			debugstr("marker Mosse_Init");
		}
		else {
			debugstr("marker before Mosse_OnFrame");
			Mosse_OnFrame(trackerId, imGray.ptr(), (int)imRgb.step, rectX, rectY, rectW, rectH);
			debug(rectX);
			debug(rectY);
		}

		const auto size = imGray.size();

		if (rectX < 0 || rectX >= size.width) {
			rectX = 0;
		}

		if (rectY < 0 || rectY >= size.height) {
			rectY = 0;
		}

		// Draw result
		cv::rectangle(imGray, cv::Rect(rectX, rectY, rectW, rectH), cv::Scalar(255, 255, 255), 1);
		debugstr("markerRectangle");

		// Debug output
//		float *ptrH = Mosse_GetFilter(trackerId);
//		float minH = *ptrH;
//		float maxH = *ptrH;
//		for (int i = 1; i < rectW * rectH; ++i)
//		{
//			if (ptrH[i] > maxH)
//				maxH = ptrH[i];
//			if (ptrH[i] < minH)
//				minH = ptrH[i];
//		}
//		for (int i = 1; i < rectW * rectH; ++i)
//			imH.data[i] = (unsigned char)((ptrH[i] - minH) / (maxH - minH) * 255);
//		cv::imshow("filter", imH);
		// Debug
//		float *ptrR = Mosse_GetResponse(trackerId);
//		float minR = *ptrR;
//		float maxR = *ptrR;
//		for (int i = 1; i < rectW * rectH; ++i)
//		{
//			if (ptrR[i] > maxR)
//				maxR = ptrR[i];
//			if (ptrR[i] < minR)
//				minR = ptrR[i];
//		}
//		for (int i = 1; i < rectW * rectH; ++i)
//			imR.data[i] = (unsigned char)((ptrR[i] - minR) / (maxR - minR) * 255);
//		cv::imshow("response", imR);

		// Show output
		debug(imGray.size());
		cv::imshow("output", imGray);
		debugstr("markerimshow");
		cv::waitKey(50);
		debugstr("markerWaitKey");
		++curFrame;
	}
}

int main()
{
	constexpr const char *kWorkingDir = "/home/dmurashov/Documents/CODE-MosseTracker/MosseRef/video/dog/";
	constexpr const char *kFileExtension = ".jpg";
	constexpr int kZeros = 8;
	constexpr int kStartFrame = 0;
	constexpr int kX = 150;
	constexpr int kY = 110;
	constexpr int kW = 40;
	constexpr int kH = 40;
	run(kWorkingDir, kFileExtension, kZeros, kStartFrame, kX, kY, kW, kH);
	return 0;
}

