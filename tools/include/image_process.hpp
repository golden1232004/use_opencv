#ifndef __IMAGE_PROCESS_HPP__
#define __IMAGE_PROCESS_HPP__

namespace tk{

class ImageProcessor{

public:
    ImageProcessor();
    ImageProcessor(int width, int height);
    virtual ~ImageProcessor();
    
public:
    float getScale(const int src_width, const int src_height);

private:
    int m_width;
    int m_height;
    float m_scale;

};

}

#endif // __IMAGE_PROCESS_HPP__
