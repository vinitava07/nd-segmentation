#ifndef UNGRAPH_HPP
#define UNGRAPH_HPP

#include "vertex.hpp"
#include <vector>
#include <iostream>
#include <image.hpp>
#include <algorithm>
#include <iterator>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>

#define WIDTH 4
using namespace std;
class Graph
{
private:
    int height;
    int width;
    int threshold;
    typedef struct
    {
        int x, y, mk, la;
    } Seed;

public:
    Vertex *adj;
    Vertex **resultGraph;
    Seed *seeds;
    int nseeds;
    std::vector<Edge> *edges;
    int vertices;

    void readSeed(char *imageName)
    {
        char fileName[40];
        char prefix[40] = "seeds/seeds_";
        strcat(prefix, imageName);
        strcat(prefix, ".txt");
        strcpy(fileName, prefix);
        FILE *fp = fopen(fileName, "r");
        int ncols, nrows;

        if (fp == NULL)
        {
            fprintf(stderr, "Erro ao abrir o arquivo seeds.txt\n");
            exit(0);
            return;
        }
        if (fscanf(fp, "%d %d %d", &nseeds, &ncols, &nrows) != 3)
        {
            fprintf(stderr, "Formato inválido no arquivo seeds.txt\n");
            fclose(fp);
            return;
        }
        seeds = (Seed *)malloc(nseeds * sizeof(Seed));
        int t;
        for (int i = 0; i < nseeds; i++)
        {
            if (fscanf(fp, "%d %d %d %d", &seeds[i].x, &seeds[i].y, &seeds[i].mk, &t) != 4)
            {
                fprintf(stderr, "Formato inválido no arquivo 2 seeds.txt\n");
                free(seeds);
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }

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

        //ANTIGA PARTE DE COLOCAR AS SEEDS
        // cout << nseeds << endl;
        // for (int i = 0; i < nseeds; i++)
        // {
        //     int x = seeds[i].x;
        //     int y = seeds[i].y;
        //     int mk = seeds[i].mk;
        //     if (x >= 0 && x < width && y >= 0 && y < height)
        //     {
        //         adj[y * width + x].weight = 0;
        //         adj[y * width + x].seed = mk;
        //     }
        // }

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
                adj[i * width + j].check = true;
            }
        }

        // freeImage(image);
        
    }
    
    void freeImage(Image *image) {
        for (int i = 0; i < image->header.height; i++)
        {
            delete[] image->img[i]; // Libera cada array de img
        }
        delete[] image->img;
        delete image->pixels;
    }

    void addPixel(int i, int j, Image::Pixel pixel, int label)
    {
        adj[i * width + j].pixel = pixel;
        adj[i * width + j].label = label;
        adj[i * width + j].weight = INT32_MAX;
    }
    void addEdge(int i, int j, int destI, int destJ, int weight, Image::Pixel p)
    {
        //adiciona a aresta pro vertice e pro seu vizinho ida e volta 
        if (!adj[destI * width + destJ].check)
        {
            int currLabel = adj[i * width + j].label;
            int destLabel = adj[destI * width + destJ].label;
            int currW = adj[i * width + j].weight;
            int destW = adj[destI * width + destJ].weight;

            weight = (intensity(p, adj[i * width + j].pixel));
            adj[i * width + j].addVertex(p, weight, destLabel, destW);
            adj[destI * width + destJ].addVertex(adj[i * width + j].pixel, weight, currLabel, currW);
        }
    }
    // equilibrar o peso da aresta baseado nos pixeis que ela liga
    float intensity(Image::Pixel p, Image::Pixel p2)
    {
        float result = 0;
        int diffRed = p.red - p2.red;
        int diffGreen = p.green - p2.green;
        int diffBlue = p.blue - p2.blue;
        // result = std::abs(((p.red + p.green + p.blue) / 3) - ((p2.red + p2.green + p2.blue) / 3));
        result = std::sqrt(pow((p.red - p2.red), 2) + pow((p.green - p2.green), 2) + pow((p.blue - p2.blue), 2));
        // result = std::sqrt(diffRed * diffRed + diffGreen * diffGreen + diffBlue * diffBlue);

        return result;
    }

    void segmentation()
    {
        std::cout << "Nao implementado";
    }

    Graph(size_t v, Image *img, int t)
    {
        threshold = t;
        height = img->header.height;
        width = img->header.width;
        adj = new Vertex[img->header.height * img->header.width];
        vertices = v;
    };
    ~Graph()
    {
        free(adj);
    };
};
#endif