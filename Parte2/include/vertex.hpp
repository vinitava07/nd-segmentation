#ifndef VERTEX
#define VERTEX
#include <vector>
#include <list>
#include <image.hpp>

class Vizinho
{
private:
    /* data */
public:
    Image::Pixel p;
    float edge;
    int label;

    // Construtor usando lista de inicialização
    Vizinho() : p(), edge(-300) {}
    Vizinho(Image::Pixel p, float e, int l) : p(p), edge(e), label(l) {}

    // bool operator>(const Vizinho &v) const
    // {
    //     return weight > v.weight;
    // }
};

class Vertex
{
private:
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
    // bool operator<(const Vertex &v) const
    // {
    //     std::cout << weight << std::endl;
    //     return weight > v.weight;
    // }
    float getVizinho(int label)
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
    void aumentaPesoVizinho(int label, float peso)
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
        // if (!encontrado && peso > 0)
        // {

        //     vizinhos.push_back(Vizinho(pixel, peso, label));
        // }
    }
    void diminuiPesoVizinho(int label, float peso)
    {
        for (int i = 0; i < vizinhos.size(); i++)
        {
            if (vizinhos.at(i).label == label)
            {
                vizinhos.at(i).edge -= peso;
                // Opcional: Se o peso for zero, remova o vizinho
                // if (vizinhos.at(i).edge <= 0)
                // {
                //     vizinhos.erase(vizinhos.begin() + i);
                // }
                break;
            }
        }
    }

    // compara vertices pelo peso, era usado pelo djikstra
    // struct CompareVertex
    // {
    //     bool operator()(Vertex const &v1, Vertex const &v2)
    //     {
    //         return v1.weight >= v2.weight;
    //     }
    // };
};
#endif