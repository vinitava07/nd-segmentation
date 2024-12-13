#ifndef UNIVERSE
#define UNIVERSE

#include <vector>
#include <iostream>
#include <stdexcept>

class Universe
{
private:
    struct Element
    {
        int rank;
        int size;
        int parent;
    };

    int num;
    std::vector<Element> elts;

public:
    Universe(int n_elements, int e) : num(e), elts(n_elements)
    {
        if (n_elements <= 0)
        {
            throw std::invalid_argument("Number of elements must be positive");
        }

        for (int i = 0; i < n_elements; ++i)
        {
            elts[i].rank = 0;
            elts[i].size = 1;
            elts[i].parent = i;
        }
    }

    int size(int x) const
    {
        if (x < 0 || x >= elts.size())
        {
            throw std::out_of_range("Index out of range in size()");
        }
        return elts[x].size;
    }

    int num_sets() const
    {
        return num;
    }

    int find(int x)
    {
        if (x < 0 || x >= elts.size())
        {
            throw std::out_of_range("Index out of range in find()");
        }
        if (x != elts[x].parent)
        {
            elts[x].parent = find(elts[x].parent); // Path compression
        }
        return elts[x].parent;
    }

    void join(int x, int y)
    {
        if (x < 0 || x >= elts.size() || y < 0 || y >= elts.size())
        {
            throw std::out_of_range("Index out of range in join()");
        }

        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY)
        {
            // Union by rank
            if (elts[rootX].rank > elts[rootY].rank)
            {
                elts[rootY].parent = rootX;
                elts[rootX].size += elts[rootY].size;
            }
            else
            {
                elts[rootX].parent = rootY;
                elts[rootY].size += elts[rootX].size;
                if (elts[rootX].rank == elts[rootY].rank)
                {
                    elts[rootY].rank++;
                }
            }
            num--;
        }
    }
};

#endif
