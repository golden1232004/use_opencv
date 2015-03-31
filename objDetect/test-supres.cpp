#include <signal.h>
#include <iostream>
#include <iomanip>
#include <string>
#include<opencv2/opencv.hpp>
//#include "opencv2/core.hpp"
//#include "opencv2/core/utility.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/contrib.hpp"
//#include "opencv2/superres.hpp"
#include "opencv2/superres.hpp"
#include "opencv2/superres/optical_flow.hpp"
#include "opencv2/opencv_modules.hpp"

#define HAVE_OPENCV_OCL
#if defined(HAVE_OPENCV_OCL)
#include "opencv2/ocl/ocl.hpp"
#endif

using namespace std;
using namespace cv;
using namespace cv::superres;
bool useOclChanged;
// #define MEASURE_TIME(op) \
//     { \
//         TickMeter tm; \
//         tm.start(); \
//         op; \
//         tm.stop(); \
//         cout << tm.getTimeSec()*1000 << " ms" << endl; \
//     }
#define MEASURE_TIME(op) op;

#if 0
//选择光流算法
static Ptr<DenseOpticalFlowExt> createOptFlow(const string& name, bool useGpu)
{
    if (name == "farneback")
    {
	if (useGpu)
	    return NULL;// createOptFlow_Farneback_CUDA();
        else
            return createOptFlow_Farneback();
    }
    else if (name == "simple")
        return createOptFlow_Simple();
    else if (name == "tvl1")
    {
        if (useGpu)
            return createOptFlow_DualTVL1_GPU();
        else
            return cv::createOptFlow_DualTVL1();
    }
    else if (name == "brox")
        return createOptFlow_Brox_GPU();
    else if (name == "pyrlk")
        return createOptFlow_PyrLK_GPU();
    else
    {
        cerr << "Incorrect Optical Flow algorithm - " << name << endl;
    }
    return Ptr<DenseOpticalFlowExt>();
}
#endif

#if defined(HAVE_OPENCV_OCL)
#if 0
static Ptr<DenseOpticalFlowExt> createOptFlow(const string& name)
{
    if (name == "farneback")
    {
        return createOptFlow_Farneback_OCL();
    }
    else if (name == "simple")
    {
        useOclChanged = true;
        std::cout<<"simple on OpenCL has not been implemented. Use CPU instead!\n";
        return createOptFlow_Simple();
    }
    else if (name == "tvl1")
        return createOptFlow_DualTVL1_OCL();
    else if (name == "brox")
    {
        std::cout<<"brox has not been implemented!\n";
        return Ptr<DenseOpticalFlowExt>();
    }
    else if (name == "pyrlk")
        return createOptFlow_PyrLK_OCL();
    else
    {
        cerr << "Incorrect Optical Flow algorithm - " << name << endl;
    }
    return Ptr<DenseOpticalFlowExt>();
}
#endif
#endif

bool g_shouldTerm = false;

void my_signal_handler(int signal)
{
    switch (signal) {
      case SIGTERM:
          cout << "SIGTERM's handler has been triggered, waiting for a moment..." << endl;
          g_shouldTerm = true;
          break;
      case SIGINT:
          cout << "SIGINT's handler has been triggered, waiting for a moment..." << endl;
          g_shouldTerm = true;
          break;
    }
}

int main(int argc, const char* argv[])
{
    useOclChanged = false;
#if 0
	CommandLineParser cmd(argc, argv, 
        "{ v video      |           | Input video }"
        "{ o output     |           | Output video }"
        "{ s scale      | 4         | Scale factor }"
        "{ i iterations | 180       | Iteration count }"
        "{ t temporal   | 4         | Radius of the temporal search area }"
        "{ f flow       | farneback | Optical flow algorithm (farneback, simple, tvl1, brox, pyrlk) }"
        "{ g gpu        | CPU       | CPU as default device, cuda for CUDA and ocl for OpenCL }"
        "{ n count      | -1        | The maximum number of frame to be processed. }"
        "{ h help       | false     | Print help message }"
    );
#else
	const char* keys = {
		"{ @video       |           | Input video }"
		"{ o output     |           | Output video }"
		"{ s scale      | 4         | Scale factor }"
		"{ i iterations | 180       | Iteration count }"
		"{ t temporal   | 4         | Radius of the temporal search area }"
		"{ f flow       | farneback | Optical flow algorithm (farneback, simple, tvl1, brox, pyrlk) }"
		"{ g gpu        | CPU       | CPU as default device, cuda for CUDA and ocl for OpenCL }"
		"{ n count      | -1        | The maximum number of frame to be processed. }"
		"{ h help       | false     | Print help message }"
	};
	CommandLineParser cmd(argc, argv, keys);

#endif
    if (cmd.get<bool>("help"))
    {
        cout << "This sample demonstrates Super Resolution algorithms for video sequence" << endl;
	//	cmd.printParams();
        return 0;
    }

    string inputVideoName = cmd.get<string>("@video");
    const string outputVideoName = cmd.get<string>("output");
    const double scale = cmd.get<double>("scale");
    const int iterations = cmd.get<int>("iterations");
    const int temporalAreaRadius = cmd.get<int>("temporal");
    const string optFlow = cmd.get<string>("flow");
    string gpuOption = cmd.get<string>("gpu");
    const long N = cmd.get<int>("count");
#if 0
    std::transform(gpuOption.begin(), gpuOption.end(), gpuOption.begin(), ::tolower);

    bool useCuda = false;
    bool useOcl = false;

    if(gpuOption.compare("ocl") == 0)
        useOcl = true;
    else if(gpuOption.compare("cuda") == 0)
        useCuda = true;

#ifndef HAVE_OPENCV_OCL
    if(useOcl)
    {
        {
            cout<<"OPENCL is not compiled\n";
            return 0;
        }
    }
#endif
#if defined(HAVE_OPENCV_OCL)
    if(useCuda)
    {
        CV_Assert(!useOcl);
    }
#endif
    Ptr<SuperResolution> superRes;


#if defined(HAVE_OPENCV_OCL)
    if(useOcl)
    {
        Ptr<DenseOpticalFlowExt> of = createOptFlow(optFlow);
        if (of.empty())
            exit(-1);
        if(useOclChanged)
        {
            superRes = createSuperResolution_BTVL1();
            useOcl = !useOcl;
        }else
            superRes = createSuperResolution_BTVL1_OCL();
        superRes->set("opticalFlow", of);
    }
    else
#endif
    {
#if 0
        if (useCuda)
	  //            superRes = createSuperResolution_BTVL1_GPU();
        else
	  //            superRes = createSuperResolution_BTVL1();
#endif
	  //        Ptr<DenseOpticalFlowExt> of = createOptFlow(optFlow, useCuda);

        if (of.empty())
            exit(-1);
        superRes->set("opticalFlow", of);
    }

    superRes->set("scale", scale);
    superRes->set("iterations", iterations);
    superRes->set("temporalAreaRadius", temporalAreaRadius);

    Ptr<FrameSource> frameSource;
    if (useCuda)
    {
        // Try to use gpu Video Decoding
        try
        {
            frameSource = createFrameSource_Video_GPU(inputVideoName);
            Mat frame;
            frameSource->nextFrame(frame);
        }
        catch (const cv::Exception&)
        {
            frameSource.release();
        }
    }
    if (!frameSource)
        frameSource = createFrameSource_Video(inputVideoName);

    if (SIG_ERR == signal(SIGINT, my_signal_handler)) {
        cerr << "Set SIGINT signal failed." << endl;
        exit(1);
    }


    // skip first frame, it is usually corrupted 跳过第一帧
    {
        Mat frame;
        frameSource->nextFrame(frame);
        cout << "Input           : " << inputVideoName << " " << frame.size() << endl;
        cout << "Scale factor    : " << scale << endl;
        cout << "Iterations      : " << iterations << endl;
        cout << "Temporal radius : " << temporalAreaRadius << endl;
        cout << "Optical Flow    : " << optFlow << endl;
#if defined(HAVE_OPENCV_OCL)
        cout << "Mode            : " << (useCuda ? "CUDA" : useOcl? "OpenCL" : "CPU") << endl;
#else
        cout << "Mode            : " << (useCuda ? "CUDA" : "CPU") << endl;
#endif
    }

    superRes->setInput(frameSource); //设置超分辨率算法的第一帧

    int ex;
    double fps;
    Size S;
    {
        VideoCapture reader(inputVideoName);
        ex = static_cast<int>(reader.get(CV_CAP_PROP_FOURCC));
        fps = reader.get(CV_CAP_PROP_FPS);//获得视频的宽和高
    }
    VideoWriter writer;

    unsigned long i = 0;
    while (1) {
        Mat result;

        double start_time = (double)getTickCount();

#if defined(HAVE_OPENCV_OCL)
        cv::ocl::oclMat result_;

        if(useOcl)
        {
            MEASURE_TIME(
            {
                superRes->nextFrame(result_);
                ocl::finish();
            });
        }
        else
#endif
        {
            MEASURE_TIME(superRes->nextFrame(result));
        }

#ifdef HAVE_OPENCV_OCL
        if(useOcl)
        {
            if(!result_.empty())
            {
                result_.download(result);
            }
        }
#endif

        if (result.empty()) {
            cerr << "result is empty." << endl;
            break;
        }

        if (i == 0) {
            Size S(result.cols, result.rows);
            cout << "Output          : " << outputVideoName << " " 
                 << result.cols << "x" << result.rows << endl;
            cout << "FPS             : " << fps << endl;
            writer.open(outputVideoName, CV_FOURCC('x','v','i','d'), fps, S, true);
            if (!writer.isOpened()) {
                cerr << outputVideoName << " open failed." << endl;
                exit(1);
            }
        }


/*
        imshow("Super Resolution", result);

        if (waitKey(1000) > 0)
            break;
*/

        writer << result;
        cout << '[' << setw(3) << i << "] : " << setw(3)
             << ((double)getTickCount() - start_time)/getTickFrequency() 
             << " sec" << endl;

        ++i;
        if ((N > 0 && i > N) || g_shouldTerm)
            break;
    }
#endif
    return 0;
}
