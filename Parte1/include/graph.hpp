#ifndef UNGRAPH_HPP
#define UNGRAPH_HPP

#include <vertex.hpp>
#include <vector>
#include <iostream>
#include <image.hpp>
#include <algorithm>
#include <iterator>
#include <unionFind.hpp>
#include <math.h>
#include <numeric>

#define WIDTH 4
using namespace std;
class Graph
{
private:
    int height;
    int width;
    int min;
    int k;

public:
    Vertex **adj;
    std::vector<Edge> *edges;
    int vertices;
    void imageToGraph(Image *image)
    {
        int height = image->header.height;
        int width = image->header.width;
        int label = 0;
        // estebelece os pixeis para cada vertice
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                addPixel(i, j, image->img[i][j], label);
                label++;
            }
        }
        // liga os vertices pela vizinhaça dos 8 lados na imagem
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i + 1 < height && j + 1 < width)
                {
                    addEdge(i, j, i + 1, j + 1, 0, image->img[i + 1][j + 1]);
                }
                if (j + 1 < width)
                {
                    addEdge(i, j, i, j + 1, 0, image->img[i][j + 1]);
                }
                if (i + 1 < height)
                {
                    addEdge(i, j, i + 1, j, 0, image->img[i + 1][j]);
                }
                if (i + 1 < height && j - 1 >= 0)
                {
                    addEdge(i, j, i + 1, j - 1, 0, image->img[i + 1][j - 1]);
                }
                if (j - 1 >= 0)
                {
                    addEdge(i, j, i, j - 1, 0, image->img[i][j - 1]);
                }
                if (i - 1 >= 0)
                {
                    addEdge(i, j, i - 1, j, 0, image->img[i - 1][j]);
                }
                if (i - 1 >= 0 && j + 1 < width)
                {
                    addEdge(i, j, i - 1, j + 1, 0, image->img[i - 1][j + 1]);
                }
                if (i - 1 >= 0 && j - 1 >= 0)
                {
                    addEdge(i, j, i - 1, j - 1, 0, image->img[i - 1][j - 1]);
                }
                adj[i][j].check = true;
            }
        }

    }
    void freeImage(Image *image)
    {
        for (int i = 0; i < image->header.height; i++)
        {
            delete[] image->img[i]; // Libera cada array de img
        }
        delete[] image->img;
        delete image->pixels;
        for (int i = 0; i < image->header.height; i++)
        {
            delete[] adj[i]; // Libera cada array de Vertex
        }
        delete[] adj;
    }
    void addPixel(int i, int j, Image::Pixel pixel, int label)
    {
        adj[i][j].pixel = pixel;
        adj[i][j].label = label;
    }
    void addEdge(int i, int j, int destI, int destJ, int weight, Image::Pixel p)
    {
        if (!adj[destI][destJ].check)
        {
            int currLabel = adj[i][j].label;
            int destLabel = adj[destI][destJ].label;
            weight = (int)(intensity(p, adj[i][j].pixel));
            // adj[i][j].addVertex(p, weight, destLabel);
            // adj[destI][destJ].addVertex(adj[i][j].pixel, weight, currLabel);
            edges->push_back(Edge(new Vizinho(p, 0, currLabel), new Vizinho(adj[i][j].pixel, 0, destLabel), weight));
        }
    }
    // equilibrar o peso da aresta baseado nos pixeis que ela liga
    float intensity(Image::Pixel p, Image::Pixel p2)
    {
        float result = 0;
        result = std::sqrt(pow((p.red - p2.red), 2) + pow((p.green - p2.green), 2) + pow((p.blue - p2.blue), 2));
        return result;
    }
    Universe *segmentation()
    {

        Universe *u = new Universe(edges->size(), vertices);
        float c = k;
        float *threshold = (float *)malloc(vertices * sizeof(float));

        for (int i = 0; i < vertices; i++)
        {
            getThreshold(1, c);
        }
        // ordena as arestas
        std::sort(edges->begin(), edges->end());
        // ve se as arestas de duas arvores diferentes pertecem ao threshold(menor aresta da MST?), se sim, junte os componentes
        for (int i = 0; i < edges->size(); i++)
        {

            int v1 = u->find(edges->at(i).v1->label);
            int v2 = u->find(edges->at(i).v2->label);
            if (v1 != v2)
            {
                if (edges->at(i).value <= threshold[v1] && edges->at(i).value <= threshold[v2])
                {
                    u->join(v1, v2);
                    v1 = u->find(v1);
                    threshold[v1] = edges->at(i).value + getThreshold(u->size(v1), c);
                }
            }
        }
        // tratando os componentes pequenos
        for (int i = 0; i < edges->size(); i++)
        {
            int v1 = u->find(edges->at(i).v1->label);
            int v2 = u->find(edges->at(i).v2->label);
            if (v1 != v2 && u->size(v1) < min || u->size(v2) < min)
            {
                u->join(v1, v2);
            }
        }
        return u;
    }
    float getThreshold(float size, float c)
    {
        return c / (size);
    }

    Graph(size_t v, Image *img, int m, int t)
    {
        min = m;
        k = t;
        height = img->header.height;
        width = img->header.width;
        edges = new std::vector<Edge>();
        adj = new Vertex *[img->header.height];
        for (int i = 0; i < img->header.height; i++)
        {
            adj[i] = new Vertex[img->header.width];
        }

        vertices = v;
    };
    ~Graph()
    {
        free(adj);
    };
};
#endif