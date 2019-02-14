#include "mymeshclass.hpp"
#include <cmath>
#include <iostream>
#include <set>

namespace netgen
{
MyMesh ::MyMesh(std::size_t dimX,
                std::size_t dimY,
                std::size_t dimZ) : numNodes{0}, numEdges{0}, numFaces{0}, numVolumes{0}
{
    dim = dimX ? 1 : 0;
    dim += dimY ? 1 : 0;
    dim += dimZ ? 1 : 0;

    double ptStepX = dimX ? 1.0 / dimX : 0;
    double ptStepY = dimY ? 1.0 / dimY : 0;
    double ptStepZ = dimZ ? 1.0 / dimZ : 0;

    //Compute Nodes
    for (std::size_t k = 0; k <= dimZ; ++k)
    {
        for (std::size_t j = 0; j <= dimY; ++j)
        {
            for (std::size_t i = 0; i <= dimX; ++i)
            {
                Node p{i * ptStepX, j * ptStepY, k * ptStepZ, numNodes};

                if (dim == 3)
                {
                    if (k == 0 || k == dimZ || j == 0 || j == dimY || i == 0 || i == dimX)
                        p.boundary = true;
                }
                else if (dim == 2)
                {
                    if (j == 0 || j == dimY || i == 0 || i == dimX)
                        p.boundary = true;
                }
                else
                {
                    if (i == 0 || i == dimX)
                        p.boundary = true;
                }

                nodes.push_back(p);
                numNodes++;
            }
        }
    }

    //Compute Edges
    for (std::size_t k = 0; k <= dimZ; ++k)
    {
        for (std::size_t j = 0; j <= dimY; ++j)
        {
            for (std::size_t i = 0; i <= dimX; ++i)
            {
                std::size_t nodepos = k * (dimZ + 1) * (dimY + 1) + j * (dimX + 1) + i;

                if (i + 1 <= dimX)
                {
                    std::size_t right = k * (dimY + 1) * (dimX + 1) + j * (dimX + 1) + i + 1;
                    Edge e{nodes[nodepos], nodes[right], numEdges};
                    edges.push_back(e);
                    numEdges++;
                }

                if (j + 1 <= dimY)
                {
                    std::size_t below = k * (dimY + 1) * (dimX + 1) + (j + 1) * (dimX + 1) + i;
                    Edge e{nodes[nodepos], nodes[below], numEdges};
                    edges.push_back(e);
                    numEdges++;
                }

                if (k + 1 <= dimZ)
                {
                    std::size_t behind = (k + 1) * (dimY + 1) * (dimX + 1) + j * (dimX + 1) + i;
                    Edge e{nodes[nodepos], nodes[behind], numEdges};
                    edges.push_back(e);
                    numEdges++;
                }
            }
        }
    }

    //Compute Faces
    for (std::size_t k = 0; k <= dimZ; ++k)
    {
        for (std::size_t j = 0; j <= dimY; ++j)
        {
            for (std::size_t i = 0; i <= dimX; ++i)
            {
                std::size_t nodepos = k * (dimZ + 1) * (dimY + 1) + j * (dimX + 1) + i;

                if (i + 1 <= dimX && j + 1 <= dimY)
                {
                    std::vector<Node> faceNodesFront;
                    std::vector<Edge> faceEdgesFront;

                    std::size_t right = k * (dimY + 1) * (dimX + 1) + j * (dimX + 1) + i + 1;
                    std::size_t below = k * (dimY + 1) * (dimX + 1) + (j + 1) * (dimX + 1) + i;
                    std::size_t across = k * (dimY + 1) * (dimX + 1) + (j + 1) * (dimX + 1) + i + 1;

                    faceNodesFront.push_back(nodes[nodepos]);
                    faceNodesFront.push_back(nodes[right]);
                    faceNodesFront.push_back(nodes[below]);
                    faceNodesFront.push_back(nodes[across]);

                    for (auto edge : edges)
                    {
                        if (edge.a == nodes[nodepos] && (edge.b == nodes[right] || edge.b == nodes[below]))
                            faceEdgesFront.push_back(edge);
                        if (edge.a == nodes[right] && edge.b == nodes[across])
                            faceEdgesFront.push_back(edge);
                        if (edge.a == nodes[below] && edge.b == nodes[across])
                            faceEdgesFront.push_back(edge);
                    }

                    Face f{faceNodesFront, faceEdgesFront, numFaces};
                    faces.push_back(f);
                    numFaces++;
                }

                if (i + 1 <= dimX && k + 1 <= dimZ)
                {
                    std::vector<Node> faceNodesTop;
                    std::vector<Edge> faceEdgesTop;

                    std::size_t right = k * (dimY + 1) * (dimX + 1) + j * (dimX + 1) + i + 1;
                    std::size_t behind = (k + 1) * (dimY + 1) * (dimX + 1) + j * (dimX + 1) + i;
                    std::size_t across = (k + 1) * (dimY + 1) * (dimX + 1) + j * (dimX + 1) + i + 1;

                    faceNodesTop.push_back(nodes[nodepos]);
                    faceNodesTop.push_back(nodes[right]);
                    faceNodesTop.push_back(nodes[behind]);
                    faceNodesTop.push_back(nodes[across]);

                    for (auto edge : edges)
                    {
                        if (edge.a == nodes[nodepos] && (edge.b == nodes[right] || edge.b == nodes[behind]))
                            faceEdgesTop.push_back(edge);
                        if (edge.a == nodes[right] && edge.b == nodes[across])
                            faceEdgesTop.push_back(edge);
                        if (edge.a == nodes[behind] && edge.b == nodes[across])
                            faceEdgesTop.push_back(edge);
                    }

                    Face f{faceNodesTop, faceEdgesTop, numFaces};
                    faces.push_back(f);
                    numFaces++;
                }

                if (j + 1 <= dimY && k + 1 <= dimZ)
                {
                    std::vector<Node> faceNodesSide;
                    std::vector<Edge> faceEdgesSide;

                    std::size_t below = k * (dimY + 1) * (dimX + 1) + (j + 1) * (dimX + 1) + i;
                    std::size_t behind = (k + 1) * (dimY + 1) * (dimX + 1) + j * (dimX + 1) + i;
                    std::size_t across = (k + 1) * (dimY + 1) * (dimX + 1) + (j + 1) * (dimX + 1) + i;

                    faceNodesSide.push_back(nodes[nodepos]);
                    faceNodesSide.push_back(nodes[below]);
                    faceNodesSide.push_back(nodes[behind]);
                    faceNodesSide.push_back(nodes[across]);

                    for (auto edge : edges)
                    {
                        if (edge.a == nodes[nodepos] && (edge.b == nodes[below] || edge.b == nodes[behind]))
                            faceEdgesSide.push_back(edge);
                        if (edge.a == nodes[below] && edge.b == nodes[across])
                            faceEdgesSide.push_back(edge);
                        if (edge.a == nodes[behind] && edge.b == nodes[across])
                            faceEdgesSide.push_back(edge);
                    }

                    Face f{faceNodesSide, faceEdgesSide, numFaces};
                    faces.push_back(f);
                    numFaces++;
                }
            }
        }
    }
    //Compute Volumes
    std::size_t faceIdx{0};
    for (std::size_t k = 0; k < dimZ; ++k)
    {
        for (std::size_t j = 0; j < dimY; ++j)
        {
            for (std::size_t i = 0; i < dimX; ++i)
            {
/*                 std::set<Node> volNodes;
                std::vector<Edge> volEdges;
                std::vector<Face> volFaces;

                volFaces.push_back(faces[faceIdx]);
                //volNodes.insert(faces[faceIdx].nodes.begin(), faces[faceIdx].nodes.end());
                volFaces.push_back(faces[faceIdx + 1]);
                volFaces.push_back(faces[faceIdx + 2]);
                volFaces.push_back(faces[faceIdx + 5]);
                volFaces.push_back(faces[faceIdx + dimX * 3 + 2]);
                volFaces.push_back(faces[faceIdx + (dimY + dimX) * 3 + 2]);

                bool outerX{i == dimX - 1};
                bool outerY{j == dimY - 1};
                bool outerZ{k == dimZ - 1};

                if (outerX && outerY && outerZ)
                  faceIdx += 6;
                else if (outerX && outerY)
                  faceIdx += 6;
                else if (outerX && outerZ || outerY && outerZ)
                  faceIdx += 5;
                else if (outerX || outerY || outerZ)
                  faceIdx += 4;
                else
                  faceIdx += 3;
                //faceIdx += i == dimX - 1 || j == dimY - 1 || k == dimZ - 1 ? 4 : 3;
                Volume v(std::vector<Node>(volNodes.begin(), volNodes.end()), volEdges, volFaces, numVolumes);
                volumes.push_back(v);
                ++numVolumes;

              std::cout << numVolumes << " " << faceIdx << "\n"; */
            }
        }
    }

    for (MyMesh::Node &n : nodes)
        computeNeighborNodes(n);

    for (MyMesh::Edge &e : edges)
        computeNeighborEdges(e);

    for (MyMesh::Face &f : faces)
        computeNeighborFaces(f);
}

void MyMesh::computeNeighborNodes(MyMesh::Node &n)
{
    for (MyMesh::Edge e : edges)
    {
        if (e.a == n)
        {
            n.neighbors.push_back(static_cast<int>(e.b.idx));
            if (e.boundary)
                n.boundary_neighbors.push_back(static_cast<int>(e.b.idx));
        }

        if (e.b == n)
        {
            n.neighbors.push_back(static_cast<int>(e.a.idx));
            if (e.boundary)
                n.boundary_neighbors.push_back(static_cast<int>(e.b.idx));
        }
    }
}

void MyMesh::computeNeighborEdges(MyMesh::Edge &e)
{
    for (MyMesh::Edge posNeighEdge : edges)
    {
        if (e.a != posNeighEdge.a && e.b == posNeighEdge.b)
                e.neighbors.push_back(static_cast<int>(posNeighEdge.idx));

        if (e.b != posNeighEdge.b && e.a == posNeighEdge.a)
                e.neighbors.push_back(static_cast<int>(posNeighEdge.idx));
    }
}

void MyMesh::computeNeighborFaces(MyMesh::Face &f)
{
    for (MyMesh::Face posNeighFace : faces)
    {
        unsigned short numSharedNodes = 0;
        for (Node nodeFace : f.nodes)
        {
            for (Node nodeNeighFace : posNeighFace.nodes)
            {
                if(nodeFace == nodeNeighFace)
                    ++numSharedNodes;
            }
        }
        if (numSharedNodes == 2)
            f.neighbors.push_back(static_cast<int>(posNeighFace.idx));
    }

}

} // namespace netgen
