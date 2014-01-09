// Training application for Soft Cascades.

#include <sft/common.hpp>
#include <iostream>
#include <sft/dataset.hpp>
#include <sft/config.hpp>
#include "sft/ChannelFeatureBuilder_impl.hpp"

#include <opencv2/core/core_c.h>

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    using namespace sft;

    const string keys =
        "{help h usage ? |      | print this message              }"
        "{config c       |      | path to configuration xml       }"
    ;
#if 0
    Mat im = imread("/home/golden/Samples/image/0002.jpg");
    imshow("000.jpg", im);
    waitKey(0);
    int h = im.rows;
    int w = im.cols;
    int imCh = im.channels();
    int ch = 0;
    unsigned char *d = new unsigned char[h*w];
    int cnt=0;
    for (int i=ch; i < h * w * imCh; i += imCh){
        d[cnt++] = im.data[i];
    }
    Mat gray(h, w, CV_8UC1, d);
    imshow("gray.jpg", gray);
    waitKey(0);

#endif

    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("Soft cascade training application.");

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    if (!parser.check())
    {
        parser.printErrors();
        return 1;
    }

    string configPath = parser.get<string>("config");
    if (configPath.empty())
    {
        std::cout << "Configuration file is missing or empty. Could not start training." << std::endl;
        
        sft::Config cfg;
        cv::FileStorage fs("sft_config_template.xml", cv::FileStorage::WRITE);
        cfg.write(fs);
        std::cout << "Now a template configure file \"sft_config_template.xml\" is generated out." << std::endl;
        return 0;
    }

    std::cout << "Read configuration from file " << configPath << std::endl;
    cv::FileStorage fs(configPath, cv::FileStorage::READ);
    if(!fs.isOpened())
    {
        std::cout << "Configuration file " << configPath << " can't be opened." << std::endl;
        return 1;
    }

    // 1. load config
    sft::Config cfg;
    // fs["config"] >> cfg;
    fs["softcascade"] >> cfg;
    std::cout << std::endl << "Training will be executed for configuration:" << std::endl << cfg << std::endl;

    // 2. check and open output file
    cv::FileStorage fso(cfg.outXmlPath, cv::FileStorage::WRITE);
    if(!fso.isOpened())
    {
        std::cout << "Training stopped. Output classifier Xml file " << cfg.outXmlPath << " can't be opened." << std::endl;
        return 1;
    }

    fso << cfg.cascadeName
        << "{"
        << "stageType"   << "BOOST"
        << "featureType" << cfg.featureType
        << "octavesNum"  << (int)cfg.octaves.size()
        << "width"       << cfg.modelWinSize.width
        << "height"      << cfg.modelWinSize.height
        << "shrinkage"   << cfg.shrinkage
        << "octaves"     << "[";

    // 3. Train all octaves
    for (ivector::const_iterator it = cfg.octaves.begin(); it != cfg.octaves.end(); ++it)
    {
        // a. create random feature pool
        int nfeatures  = cfg.poolSize; 
        cv::Size model = cfg.model(it);
        std::cout << "Model " << model << std::endl;

        int nchannels = (cfg.featureType == "HOG6MagLuv") ? 10: 8;

        std::cout << "number of feature channels is " << nchannels << std::endl;
        
        cv::Ptr<cv::FeaturePool> pool = cv::FeaturePool::create(model, nfeatures, nchannels);
        nfeatures = pool->size();


        int npositives = cfg.positives;
        int nnegatives = cfg.negatives;
        int shrinkage  = cfg.shrinkage;
        cv::Rect boundingBox = cfg.bbox(it);
        std::cout << "Object bounding box" << boundingBox << std::endl;

        cv::Ptr<cv::ChannelFeatureBuilder> builder = cv::ChannelFeatureBuilder::create(cfg.featureType);
        cv::AlgorithmInfo *info = builder->info(); 
        std::cout << "Channel builder " << builder->info()->name() << std::endl;

        typedef cv::Octave Octave;
        cv::Ptr<Octave> boost = Octave::create(boundingBox, npositives, nnegatives, *it, shrinkage, builder);

        std::string path = cfg.trainPath;
        sft::ScaledDataset dataset(path, *it);
	
        if (boost->train(&dataset, pool, cfg.weaks, cfg.treeDepth))
        {
            CvFileStorage* fout = cvOpenFileStorage(cfg.resPath(it).c_str(), 0, CV_STORAGE_WRITE);
            boost->write(fout, cfg.cascadeName);

            cvReleaseFileStorage( &fout);

            cv::Mat thresholds;
            boost->setRejectThresholds(thresholds);

            boost->write(fso, pool, thresholds);

            cv::FileStorage tfs(("thresholds." + cfg.resPath(it)).c_str(), cv::FileStorage::WRITE);
            tfs << "thresholds" << thresholds;

            std::cout << "Octave " << *it << " was successfully trained..." << std::endl;
        }
    }

    fso << "]" << "}";
    fso.release();
    std::cout << "Training complete..." << std::endl;
    return 0;
}
