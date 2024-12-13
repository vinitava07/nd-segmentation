#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <string.h>
#include <string>
#include <math.h>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <vector>

#define WIDTH 4

class Image
{
public:
    typedef struct
    {
        char format[3];
        int height;
        int width;
        int color;

    } ImageHeader;
    typedef struct
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;

    } Pixel;
    ImageHeader header;
    Pixel *pixels;
    Pixel **img;
    size_t imgSize;
    double sigma;
    Image(const char *filename);
    ~Image();
    void readImage();
    void smooth(double sig);
    std::vector<double> *make_fgauss();
    std::vector<double> *normalize(const std::vector<double> *mask);
    Image::Pixel **convolve_even(Image::Pixel **src, const std::vector<double> *mask);
    Pixel **arrToMatrix();
    void greyScale();

private:
    FILE *f;
    void readFileUntil(char buffer[], char condition);
    void readImageHeader();
};

#endif