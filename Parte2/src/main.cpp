#include <iostream>
#include <string.h>
#include "../include/image.hpp"
#include <chrono>
#include "../include/graph.hpp"
#include <random>

using namespace std;
void writeImage(Image *img, bool *visitados)
{
    FILE *saida = fopen("saida.ppm", "wb+");
    if (!saida)
    {
        std::cerr << "Erro ao abrir o arquivo de saída!" << std::endl;
        return;
    }
    // escreve o header
    fprintf(saida, "%s\n%d %d\n%d\n", img->header.format, img->header.width, img->header.height, img->header.color);

    for (int i = 0; i < img->header.height; i++)
    {
        for (int j = 0; j < img->header.width; j++)
        {
            if (visitados[i * img->header.width + j])
            {
                fprintf(saida, "%c%c%c", img->img[i][j].red, img->img[i][j].green, img->img[i][j].blue);
            }
            else
            {
                fprintf(saida, "%c%c%c", 0, 255, 0);
            }
        }
    }

    fclose(saida);
}

int main(int argc, char const *argv[])
{

    char imageName[40];
    char fileName[40] = "";
    char prefix[30] = "../images/";

    cout << "Qual o nome da imagem que voce deseja segmentar (sem o .ppm)" << endl;
    cin >> imageName;

    strcpy(fileName, imageName);
    strcat(fileName, ".ppm");
    strcat(prefix, fileName);
    Image *image = new Image(prefix);

    image->readImage();
    size_t graphSize = image->imgSize;

    float sigma = 10;

    cout << "Digite o valor do sigma (normal = 10)" << endl;

    cin >> sigma;

    cout << "Transformando a imagem para escala de cinza" << endl;

    // transforma em escala de cinza
    image->greyScale();

    chrono::steady_clock sc; // create an object of `steady_clock` class
    auto start = sc.now();
    // grafo implementado com lista de adjacencia
    Graph *g = new Graph(image, sigma);
    // le as seeds
    g->readSeed(imageName);
    // transforma a imagem em um grafo
    g->imageToGraph(image);
    cout << "Segmentando" << endl;
    // link o source e o skin com as seeds
    g->linkSourceAndSink(image);
    // segmenta o grafo
    bool *visitados = g->segmentation();
    // escreve a imagem no arquivo
    writeImage(image, visitados);
    g->freeImage(image);

    auto end = sc.now();                                                 // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    auto time_span = static_cast<chrono::duration<double>>(end - start); // measure time span between start & end
    cout << "A segmentacao durou: " << time_span.count() << " segundos !!!" << endl;
    cout << "Lembre-se de excluir a saida.ppm para evitar erros ao gerar outro arquivo!";
    // system("pause");
    return 0;
}
