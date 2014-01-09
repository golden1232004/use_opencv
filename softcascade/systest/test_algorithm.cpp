#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;


class MyOpencvAlgorithm : public cv::Algorithm {  
 public:  
  virtual AlgorithmInfo* info() const;  
  
  void Do() {  
    printf("=== Do MyOpencv Algorithm! ===");  
  }  
};  
  
Algorithm* CreateMyOpencvAlgorithm() {  
  return new MyOpencvAlgorithm;  
}  
  
AlgorithmInfo& MyOpencvAlgorithmInfo() {  
  static AlgorithmInfo info("MyOpencvAlgorithm", CreateMyOpencvAlgorithm);  
  return info;  
}  
  
AlgorithmInfo* MyOpencvAlgorithm::info() const {  
  return &(MyOpencvAlgorithmInfo());  
}  
  
bool initModule_MyOpencv() {  
  //  Ptr<Algorithm> p = CreateMyOpencvAlgorithm();  
  Algorithm *p = CreateMyOpencvAlgorithm();
  return p->info() != NULL;  
}  
  
int main( int argc, char** argv )  
{  
  // initModule_MyOpencv();  
  Ptr<MyOpencvAlgorithm> pm = Algorithm::create<MyOpencvAlgorithm>("MyOpencvAlgorithm");  
  pm->Do();  
  
  cout << endl;  
  vector<string> algorithms;  
  //  Algorithm::getList(algorithms);  
  //  copy(algorithms.begin(), algorithms.end(), ostream_iterator<string>(cout, "\n"));  
  cout << algorithms.size();  
  
  return 0;  
}  
