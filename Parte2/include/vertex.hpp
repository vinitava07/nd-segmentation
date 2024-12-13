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
    float weight = INT32_MAX;

    // Construtor usando lista de inicialização
    Vizinho() : p(), edge(0) {}
    Vizinho(Image::Pixel p, float e, int l, float w) : p(p), edge(e), label(l), weight(w) {}

    bool operator>(const Vizinho &v) const
    {
        return weight > v.weight;
    }
};
class Edge
{
private:
    /* data */
public:
    int value;
    Vizinho *v1;
    Vizinho *v2;
    bool operator<(const Edge &e1) const
    {
        return value < e1.value;
    }
    Edge(/* args */) : v1(), v2(), value(0){};
    Edge(Vizinho *p1, Vizinho *p2, int weight) : v1(p1), v2(p2), value(weight) {}
    ~Edge(){};
};

class Vertex
{
private:
public:
    Image::Pixel pixel;
    std::vector<Vizinho> *adj;
    float weight = INT32_MAX;
    int label;
    bool check = false;
    int seed = -1;
    void addVertex(Image::Pixel pixel, float edgeWeight, int label, float weight)
    {
        adj->push_back(Vizinho(pixel, edgeWeight, label, weight));
    }
    Vertex(float w) : adj(new std::vector<Vizinho>()), weight(w){};
    Vertex() : adj(new std::vector<Vizinho>()), weight(0){};
    ~Vertex() = default;
    bool operator<(const Vertex &v) const
    {
        std::cout << weight << std::endl;
        return weight > v.weight;
    }
    //compara vertices pelo peso, era usado pelo djikstra
    struct CompareVertex
    {
        bool operator()(Vertex *const &v1, Vertex *const &v2)
        {
            return v1->weight >= v2->weight;
        }
    };
};
#endif