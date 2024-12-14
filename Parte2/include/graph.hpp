#ifndef UNGRAPH_HPP
#define UNGRAPH_HPP

#include <vector>
#include <iostream>
#include "image.hpp"
#include <algorithm>
#include <iterator>
#include <math.h>
#include <numeric>
#include <queue>
#include <stack>
#include <set>
#include "../include/vertex.hpp"

#define WIDTH 4
#define SIGMA 30.0

using namespace std;
class Graph
{
private:
    int height;
    int width;
    int threshold;
    enum SeedType
    {
        OBJECT,
        BACKGROUND
    };
    typedef struct
    {
        int x, y;
        SeedType seedType;
    } Seed;

public:
    Vertex *adj;
    Vertex **resultGraph;
    Seed *seeds;
    int nseeds;
    int vertices;
    int source;
    int sink;
    int graphSize;
    float Kmax = INT32_MIN;

    void readSeed(char *imageName)
    {
        char fileName[40];
        char prefix[40] = "seeds/seeds_";
        strcat(prefix, imageName);
        strcat(prefix, ".txt");
        strcpy(fileName, prefix);
        FILE *fp = fopen(fileName, "r");

        if (fp == NULL)
        {
            fprintf(stderr, "Erro ao abrir o arquivo %s\n", fileName);
            exit(0);
            return;
        }

        // Contagem de seeds (não existe mais a linha com nseeds, ncols, nrows)
        nseeds = 0;
        int x, y, seedType;

        // Lê todas as linhas e conta as sementes
        while (fscanf(fp, "%d %d %d", &x, &y, &seedType) == 3)
        {
            nseeds++;
        }

        // Voltar ao início do arquivo para ler os dados
        rewind(fp);

        Seed *seeds = (Seed *)malloc(nseeds * sizeof(Seed));
        if (seeds == NULL)
        {
            fprintf(stderr, "Falha ao alocar memória para seeds\n");
            fclose(fp);
            return;
        }

        // Lendo os dados novamente para preencher a estrutura de seeds
        int i = 0;
        while (fscanf(fp, "%d %d %d", &seeds[i].x, &seeds[i].y, &seeds[i].seedType) == 3)
        {
            // Atribuindo o valor de SeedType com base no valor de 'seedType'
            seeds[i].seedType = (seeds[i].seedType == 0) ? OBJECT : BACKGROUND;
            i++;
        }

        fclose(fp);

        // Exemplo de impressão dos dados lidos
        // for (int i = 0; i < nseeds; i++)
        // {
        //     printf("Seed %d: x = %d, y = %d, seedType = %s\n",
        //            i, seeds[i].x, seeds[i].y,
        //            (seeds[i].seedType == OBJECT) ? "OBJECT" : "BACKGROUND");
        // }

        free(seeds);
    }

    void imageToGraph(Image *image)
    {
        int height = image->header.height;
        int width = image->header.width;
        int label = 0;

        float bp;
        // estebelece os pixeis para cada vertice
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                addPixel(i, j, image->img[i][j], label);
                label++;
            }
        }

        // ANTIGA PARTE DE COLOCAR AS SEEDS
        //  cout << nseeds << endl;
        //  for (int i = 0; i < nseeds; i++)
        //  {
        //      int x = seeds[i].x;
        //      int y = seeds[i].y;
        //      int mk = seeds[i].mk;
        //      if (x >= 0 && x < width && y >= 0 && y < height)
        //      {
        //          adj[y * width + x].weight = 0;
        //          adj[y * width + x].seed = mk;
        //      }
        //  }

        // liga os vertices pela vizinhaça dos 8 lados na imagem
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i + 1 < height && j + 1 < width)
                {
                    bp = addEdge(i, j, i + 1, j + 1, image->img[i + 1][j + 1]);
                    Kmax = std::max(Kmax, bp);
                }
                if (j + 1 < width)
                {
                    bp = addEdge(i, j, i, j + 1, image->img[i][j + 1]);
                    Kmax = std::max(Kmax, bp);
                }
                if (i + 1 < height)
                {
                    bp = addEdge(i, j, i + 1, j, image->img[i + 1][j]);
                    Kmax = std::max(Kmax, bp);
                }
                if (i + 1 < height && j - 1 >= 0)
                {
                    bp = addEdge(i, j, i + 1, j - 1, image->img[i + 1][j - 1]);
                    Kmax = std::max(Kmax, bp);
                }
                if (j - 1 >= 0)
                {
                    bp = addEdge(i, j, i, j - 1, image->img[i][j - 1]);
                    Kmax = std::max(Kmax, bp);
                }
                if (i - 1 >= 0)
                {
                    bp = addEdge(i, j, i - 1, j, image->img[i - 1][j]);
                    Kmax = std::max(Kmax, bp);
                }
                if (i - 1 >= 0 && j + 1 < width)
                {
                    bp = addEdge(i, j, i - 1, j + 1, image->img[i - 1][j + 1]);
                    Kmax = std::max(Kmax, bp);
                }
                if (i - 1 >= 0 && j - 1 >= 0)
                {
                    bp = addEdge(i, j, i - 1, j - 1, image->img[i - 1][j - 1]);
                    Kmax = std::max(Kmax, bp);
                }
                adj[i * width + j].check = true;
            }
        }

        // freeImage(image);
    }

    void freeImage(Image *image)
    {
        for (int i = 0; i < image->header.height; i++)
        {
            delete[] image->img[i]; // Libera cada array de img
        }
        delete[] image->img;
        delete image->pixels;
    }

    // void linkSourceAndSink(Image *image)
    // {
    //     for (int i = 0; i < nseeds; i++)
    //     {

    //         int destLabel = adj[destI * width + destJ].label;
    //         if (seeds[i].seedType == OBJECT)
    //         {
    //             adj[source].addVertex(image->img[seeds->x][seeds->y], Kmax, destLabel, 0);
    //         }
    //         else
    //         {
    //             adj[sink].addVertex(image->img[seeds->x][seeds->y], Kmax, destLabel, 0);
    //         }
    //     }
    // }

    void addPixel(int i, int j, Image::Pixel pixel, int label)
    {
        adj[i * width + j].pixel = pixel;
        adj[i * width + j].label = label;
        adj[i * width + j].weight = INT32_MAX;
    }

    float addEdge(int i, int j, int destI, int destJ, Image::Pixel p)
    {
        // adiciona a aresta pro vertice e pro seu vizinho ida e volta
        float weight = (boundaryPenalty(p.blue, adj[i * width + j].pixel.blue));
        if (!adj[destI * width + destJ].check)
        {
            int currLabel = adj[i * width + j].label;
            int destLabel = adj[destI * width + destJ].label;

            int currW = adj[i * width + j].weight;
            int destW = adj[destI * width + destJ].weight;

            adj[i * width + j].addVertex(p, weight, destLabel, destW);
            adj[destI * width + destJ].addVertex(adj[i * width + j].pixel, weight, currLabel, currW);
        }
        return weight;
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

    // Função que calcula a penalidade de fronteira entre dois pixels
    float boundaryPenalty(unsigned char ip, unsigned char iq)
    {
        // Calculando a diferença entre os dois valores de intensidade
        float diff = static_cast<float>(ip) - static_cast<float>(iq);

        // Calculando a penalidade usando a fórmula Gaussiana
        float bp = 100 * exp(-pow(diff, 2) / (2 * pow(SIGMA, 2)));

        return bp;
    }

    bool bfs(Vertex *residualGraph, int *parent)
    {
        std::queue<int> q;
        bool *visited = new bool[graphSize];
        for (int i = 0; i < graphSize; i++)
        {
            visited[i] = false;
        }

        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int i = 0; i < graphSize; i++)
            {
                if (!visited[i] && residualGraph[u].getVizinho(i) > 0)
                {
                    q.push(i);
                    parent[i] = u;
                    visited[i] = true;
                }
            }
        }
        return visited[sink];
    }

    void dfs(Vertex *residualGraph, bool *visited)
    {
        std::stack<int> stack;
        stack.push(source);
        while (!stack.empty())
        {
            int v = stack.top();
            stack.pop();
            if (!visited[v])
            {
                visited[v] = true;
                for (int i = 0; i < graphSize; i++)
                {
                    if (residualGraph[v].getVizinho(i) > 0 && !visited[u])
                    {
                        stack.push(u);
                    }
                    
                }
                
            }
            
        }
    }

    void segmentation()
    {
        std::cout << "Clonando grafo";

        Vertex *residualGraph = new Vertex[graphSize];
        int *parent = new int[graphSize];

        for (int i = 0; i < graphSize; i++)
        {
            // Copia os valores do vértice
            residualGraph[i].weight = adj[i].weight;
            residualGraph[i].label = adj[i].label;
            residualGraph[i].check = adj[i].check;
            residualGraph[i].seed = adj[i].seed;

            // Copia os vizinhos (adjacentes)
            for (const auto vizinho : *adj[i].adj)
            {
                residualGraph[i].addVertex(vizinho.p, vizinho.edge, vizinho.label, vizinho.weight);
            }
        }
        float pathFlow = INT32_MAX;

        while (bfs(residualGraph, parent))
        {
            int v = sink;
            while (v != source)
            {
                int u = parent[v];
                pathFlow = std::min(pathFlow, residualGraph[u].getVizinho(v));
                v = parent[v];
            }
            v = sink;
            while (v != source)
            {
                int u = parent[v];
                residualGraph[u].aumentaPesoVizinho(v, pathFlow);
                residualGraph[v].diminuiPesoVizinho(u, pathFlow);
                v = parent[v];
            }
        }

        bool *visited = new bool[graphSize];

        dfs(residualGraph, source, visited);
    }

    Graph(size_t v, Image *img, int t)
    {
        threshold = t;
        height = img->header.height;
        width = img->header.width;
        graphSize = (img->header.height * img->header.width) + 2;
        source = graphSize - 2;
        sink = graphSize - 1;
        std::cout << source << " " << sink << std::endl;
        adj = new Vertex[graphSize];
        vertices = v;
    };

    ~Graph()
    {
        free(adj);
    };
};
#endif