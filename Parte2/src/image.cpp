#include "image.hpp"
#include <iostream>
#include <string.h>

Image::Image(const char *filename)
{
    Image::header = {
        .height = 0,
        .width = 0,
        .color = 0};
    f = fopen(filename, "rb+");
    if (f == NULL)
    {
        printf("The Image is not found. The program will now exit.");
        exit(0);
    }
    readImageHeader();
    imgSize = header.height * header.width;
    pixels = (Pixel *)malloc(sizeof(Pixel) * imgSize);
};

Image::~Image()
{
    fclose(f);
    free(pixels);
};

void Image::readImage()
{
    size_t triples = (imgSize * 3);
    unsigned char *buffer = (unsigned char *)malloc(sizeof(char) * triples + 1);
    int t = fread(buffer, sizeof(char), triples, f);
    size_t bufferOffset = 0;
    for (size_t i = 0; i < imgSize; i++)
    {
        pixels[i].red = (int)buffer[bufferOffset];
        pixels[i].green = (int)buffer[bufferOffset + 1];
        pixels[i].blue = (int)buffer[bufferOffset + 2];
        bufferOffset += 3;
    }
    free(buffer);
    arrToMatrix();
}

Image::Pixel **Image::arrToMatrix()
{
    img = new Pixel *[header.height];
    for (int i = 0; i < header.height; i++)
    {
        img[i] = new Pixel[header.width];
    }
    // Transforma o vetor de pixels na matriz de pixels
    for (int i = 0; i < header.height; i++)
    {
        for (int j = 0; j < header.width; j++)
        {
            img[i][j] = pixels[i * header.width + j];
        }
    }
    // free(pixels);

    return img;
}

void Image::greyScale()
{
    for (int i = 0; i < header.height; i++)
    {
        for (int j = 0; j < header.width; j++)
        {
            int grey = (img[i][j].red + img[i][j].green + img[i][j].blue) / 3;
            img[i][j].red = grey;
            img[i][j].green = grey;
            img[i][j].blue = grey;
        }
    }
}

void Image::readFileUntil(char buffer[], char condition)
{
    char c;
    int cont = 0;
    do
    {
        c = fgetc(f);
        buffer[cont++] = c;
    } while (c != condition);
    buffer[cont - 1] = '\0';
}

void Image::readImageHeader()
{
    char format[3];
    char fHeight[5];
    char fWidth[5];
    char colorScheme[4];
    readFileUntil(format, '\n');
    readFileUntil(fWidth, ' ');
    readFileUntil(fHeight, '\n');
    readFileUntil(colorScheme, '\n');
    header.color = atoi(colorScheme);
    strcpy(header.format, format);
    header.height = atoi(fHeight);
    header.width = atoi(fWidth);
}

void Image::smooth(double sig)
{
    sigma = sig;
    std::vector<double> *mask = make_fgauss();
    mask = normalize(mask);
    Image::Pixel **tmp = convolve_even(img, mask);
    Image::Pixel **dst = convolve_even(tmp, mask);
    for (int i = 0; i < header.height; i++)
    {
        delete[] img[i];
        img[i] = nullptr;
    }
    delete[] img;
    img = dst;
}

// Função para criar máscara gaussiana
std::vector<double> *Image::make_fgauss()
{
    sigma = std::max(sigma, 0.01);
    int length = std::ceil(sigma * WIDTH) + 1;
    std::vector<double> *mask = new std::vector<double>(length, 0.0);

    for (int i = 0; i < length; ++i)
    {
        mask->at(i) = std::exp(-0.5 * std::pow(i / sigma, i / sigma));
    }

    return mask;
}

// Função para normalizar a máscara
std::vector<double> *Image::normalize(const std::vector<double> *mask)
{
    double sum = 2 * std::accumulate(mask->begin(), mask->end(), 0.0, [](double acc, double val)
                                     { return acc + std::abs(val); }) +
                 std::abs(mask->at(0));

    std::vector<double> *normalized_mask = new std::vector<double>(mask->size());
    std::transform(mask->begin(), mask->end(), normalized_mask->begin(), [sum](double val)
                   { return val / sum; });

    return normalized_mask;
}

// Função para convolver src com a máscara
Image::Pixel **Image::convolve_even(Image::Pixel **src, const std::vector<double> *mask)
{
    int length = mask->size();
    Image::Pixel **output = new Image::Pixel *[header.height];
    for (int i = 0; i < header.height; ++i)
    {
        output[i] = new Image::Pixel[header.width];
    }

    for (int y = 0; y < header.height; ++y)
    {
        for (int x = 0; x < header.width; ++x)
        {
            double sum_red = mask->at(0) * src[y][x].red;
            double sum_green = mask->at(0) * src[y][x].green;
            double sum_blue = mask->at(0) * src[y][x].blue;

            for (int i = 1; i < length; ++i)
            {
                sum_red += mask->at(i) * (src[y][std::max(x - i, 0)].red + src[y][std::min(x + i, header.width - 1)].red);
                sum_green += mask->at(i) * (src[y][std::max(x - i, 0)].green + src[y][std::min(x + i, header.width - 1)].green);
                sum_blue += mask->at(i) * (src[y][std::max(x - i, 0)].blue + src[y][std::min(x + i, header.width - 1)].blue);
            }
            output[y][x].red = static_cast<unsigned char>(std::min(255.0, std::max(0.0, sum_red)));
            output[y][x].green = static_cast<unsigned char>(std::min(255.0, std::max(0.0, sum_green)));
            output[y][x].blue = static_cast<unsigned char>(std::min(255.0, std::max(0.0, sum_blue)));
        }
    }

    return output;
}
