#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;


int main(int argc, char** argv)
{
    if (argc < 2){
        printf("./hog_testbed img");
    }
    
    char* fn = argv[1];
    Mat trainImg; //需要分析的图片
    
    trainImg=imread(fn);   //读取图片  
    HOGDescriptor *hog=new HOGDescriptor(cvSize(3,3),cvSize(3,3),cvSize(5,10),cvSize(3,3),9);  //具体意思见参考文章1,2  
    std::vector<float> descriptors;//结果数组  
    hog->compute(trainImg, descriptors,Size(1,1), Size(0,0)); //调用计算函数开始计算  
    printf("%d\n",descriptors.size()); //打屏一下结果数组的大小，看看是否符合文献2的预估， 发现完全一样，那篇文章很给力
    int descp = hog->getDescriptorSize();
    printf("descriptor size: %d\n", descp);
    

}
