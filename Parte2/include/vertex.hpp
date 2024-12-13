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
    int edge;
    int label;

    // Construtor usando lista de inicialização
    Vizinho() : p(), edge(0) {}

    Vizinho(Image::Pixel p, int e, int l) : p(p), edge(e), label(l) {}
};
class Edge
{

public:
    int value;
    Vizinho *v1;
    Vizinho *v2;
    bool operator<(const Edge &e1) const
    {
        return value < e1.value;
    }
    Edge(/* args */) : v1(), v2(), value(0) {};
    Edge(Vizinho *p1, Vizinho *p2, int weight) : v1(p1), v2(p2), value(weight) {}
    ~Edge() {};
};

class Vertex
{
public:
    Image::Pixel pixel;
    std::vector<Vizinho> *adj;
    int label;
    bool check = false;
    void addVertex(Image::Pixel p, int w, int l)
    {
        adj->push_back(Vizinho(p, w, l));
    }
    Vertex() : adj(new std::vector<Vizinho>()) {};

    ~Vertex() = default;
};
#endif