/*
#include "opencv2/opencv.hpp"

namespace cv { namespace softcascade
{

CV_INIT_ALGORITHM(Detector, "SoftCascade.Detector",
                  obj.info()->addParam(obj, "minScale",    obj.minScale);
                  obj.info()->addParam(obj, "maxScale",    obj.maxScale);
                  obj.info()->addParam(obj, "scales",      obj.scales);
                  obj.info()->addParam(obj, "rejCriteria", obj.rejCriteria));
  
CV_INIT_ALGORITHM(SCascade, "CascadeDetector.SCascade",
                  obj.info()->addParam(obj, "minScale", obj.minScale);
                  obj.info()->addParam(obj, "maxScale", obj.maxScale);
                  obj.info()->addParam(obj, "scales",   obj.scales));

bool initModule_softcascadeDetector(void)
{
  //Ptr<Algorithm> sc = createSCascade_ptr_hidden();
    Ptr<Algorithm> sc1 = createDetector_ptr_hidden();
    return (sc1->info() != 0) && (sc->info() != 0);
}

} }
*/
