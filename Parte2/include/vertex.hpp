#ifndef VERTEX
#define VERTEX
#include <vector>
#include <list>
#include <image.hpp>

class Vizinho
{

public:
    Image::Pixel p;
    float edge;
    int label;

    // Construtor usando lista de inicialização
    Vizinho() : p(), edge(-300) {}
    Vizinho(Image::Pixel p, float e, int l) : p(p), edge(e), label(l) {}

};

class Vertex
{
public:
    Image::Pixel pixel;
    std::vector<Vizinho> vizinhos;
    int label;
    bool check = false;
    void addVertex(Image::Pixel pixel, float edgeWeight, int label)
    {
        vizinhos.push_back(Vizinho(pixel, edgeWeight, label));
    }
    Vertex() : vizinhos(std::vector<Vizinho>()) {};
    ~Vertex() = default;

    float getVizinhoWeight(int label)
    {
        for (int i = 0; i < vizinhos.size(); i++)
        {
            if (vizinhos.at(i).label == label)
            {
                return vizinhos.at(i).edge;
            }
        }

        return -1;
    }
    void alteraPesoVizinho(int label, float peso)
    {
        bool encontrado = false;
        for (int i = 0; i < vizinhos.size(); i++)
        {
            if (vizinhos.at(i).label == label)
            {
                vizinhos.at(i).edge += peso;
                encontrado = true;
                break;
            }
        }

    }
};
#endif