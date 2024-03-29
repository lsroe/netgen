#include "mymeshclass.hpp"
#include <iostream>
#include <meshing.hpp>
#include <nginterface.h>
#include <nginterface_v2.hpp>

// Tests C++ mesh functinality and interface functionlity

int main()
{
    auto spm = make_shared<netgen::MyMesh>(3, 2, 2);
    netgen::MyMesh & m(*spm);
    std::cout << "Number of Nodes:" << m.getNumNodes() << "\n";
    for (auto i : m.getNodes())
    {
        std::cout << i << " at boundary " << i.boundary << "\n";
        std::cout << "Part of elements: " <<
                    i.partOfElement.size() << "\n";
        for(auto j : i.partOfElement)
            std::cout << j << " ";
        std::cout << "\nPart of bnd_elements: " <<
                    i.partOfBndElement.size() << "\n";
        for(auto j : i.partOfBndElement)
            std::cout << j << " ";
        std::cout << "\n\n";
    }

    std::cout << "\nNumber of bnd Nodes:" << m.getNumBndNodes() << "\n";
    for (auto i : m.getBndNodes())
    {
        std::cout << i << " at boundary " << i.boundary << "\n";
    }


    std::cout << "\n----------------\n\n";
    std::cout << "Number of Edges: " << m.getNumEdges() << "\n";
    for (auto i : m.getEdges())
    {
        std::cout << i << " at boundary " << i.boundary << "\n";
        for (auto j : i.nodeIdx)
            std::cout << j << " ";
        std::cout << "\n\n";
    }

    std::cout << "\nNumber of bnd Edges:" << m.getNumBndEdges() << "\n";
    for (auto i : m.getBndEdges())
    {
        std::cout << i << " at boundary " << i.boundary << "\n";
    }

    std::cout << "\n----------------\n\n";
    std::cout << "Number of Faces: " <<  m.getNumFaces() << "\n";
    for (auto i : m.getFaces())
    {
        std::cout << i << " at boundary " << i.boundary << "\n";
        std::cout << "\n\n";
    }

    std::cout << "\nNumber of bnd Faces:" << m.getNumBndFaces() << "\n";
    for (auto i : m.getBndFaces())
    {
        std::cout << i << " at boundary " << i.boundary << "\n";
    }

    std::cout << "\n----------------\n\n";
    std::cout << "Number of Volumes: " <<  m.getNumVolumes() << "\n";
    for (auto i : m.getVolumes())
    {
        std::cout << i << " at boundary " << i.boundary << "\n";
        for (auto j : i.t_faces)
            std::cout << j.nr << " ";
        std::cout << "\n\n";
    }



    std::cout << "\n\n\n---------Testing Interface-------\n\n";
    netgen::Ngx_MyMesh ngsmym(spm);
    std::cout << "Testing GetDimension(): " << ngsmym.GetDimension() << "\n";
    std::cout << "Testing GetNLevels(): " << ngsmym.GetNLevels() << "\n\n";

    std::cout << "Testing GetNElemts(0): " << ngsmym.GetNElements(0) << "\n";
    std::cout << "Testing GetNElemts(1): " << ngsmym.GetNElements(1) << "\n";
    std::cout << "Testing GetNElemts(2): " << ngsmym.GetNElements(2) << "\n";
    std::cout << "Testing GetNElemts(3): " << ngsmym.GetNElements(3) << "\n\n";

    std::cout << "Testing GetNNodes1(): " << ngsmym.GetNNodes1() << "\n";
    std::cout << "Testing GetNNodes2(): " << ngsmym.GetNNodes2() << "\n";

    std::cout << "Testing GetPoint(): MISSING!!" << "\n\n";

    std::cout << "Testing GetElement0(5): " << "\n";
    netgen::Ng_Element ngel0{ngsmym.GetElement0(5)};
    std::cout << "Return type: " << ngel0.type << " Return index: " << ngel0.index << " !!HARDCODED!!\n";
    std::cout << "Points number: " << ngel0.points.num << "\n";
    std::cout << "Points array: " << ngel0.points[0] << "\n";
    std::cout << "\n";

    std::cout << "Testing GetElement1(5): " << "\n";
    netgen::Ng_Element ngel1{ngsmym.GetElement1(5)};
    std::cout << "Return type: " << ngel1.type << " Return index: " << ngel0.index << " !!HARDCODED!!\n";
    std::cout << "Points number: " << ngel1.points.num << "\n";
    std::cout << "Points array: " << ngel1.points[0] << "\n";
    std::cout << "Points array: " << ngel1.points[1] << "\n";
    std::cout << "Points array: " << ngel1.points[2] << "\n";
    std::cout << "\n";

    std::cout << "Testing GetElement2(5): " << "\n";
    netgen::Ng_Element ngel2{ngsmym.GetElement2(5)};
    std::cout << "Return type: " << ngel2.type << " Return index: " << ngel0.index << " !!HARDCODED!!\n";
    std::cout << "Points number: " << ngel2.points.num << "\n";
    std::cout << "Points array: " << ngel2.points[0] << "\n";
    std::cout << "Edge number: " << ngel2.edges.num << "\n";
    std::cout << "Edge array: " << ngel2.edges[0] << "\n";
    std::cout << "\n";

    std::cout << "Testing GetMaterialCD0(0): " << ngsmym.GetMaterialCD0(0) << "\n";
    std::cout << "SAME FOR ALL OTHER\n\n";

    std::cout << "Testing ElementTransformation(): TEST NOT IMPLEMENTED" << "\n\n";
    std::cout << "Testing MultiElementTransformation(): TEST NOT IMPLEMENTED" << "\n\n";

    // Node<0>
    std::cout << "Testing GetNode0(4): " << "\n";
    netgen::Ng_Node<0> nd0{ngsmym.GetNode0(4)};
    std::cout << "nd0.elements.nd: " << nd0.elements.ne <<
        " nd0.bnd_elements.nd: " << nd0.bnd_elements.ne << "\n";

    std::cout << "nd0.elements[]: ";
    for (auto i = 0; i < nd0.elements.ne; ++i)
        std::cout << nd0.elements[i] << " ";

    std::cout << "\nnd0.bnd_elements[]: ";
    for (auto i = 0; i < nd0.bnd_elements.ne; ++i)
        std::cout << nd0.bnd_elements[i] << " ";

    std::cout << "\n\n";
    // Node<1>
    std::cout << "Testing GetNode1(4): " << "\n\n";

    // NNodes
    std::cout << "Testing GetNNodes1(): " << ngsmym.GetNNodes1() << "\n";
    std::cout << "Testing GetNNodes2(): " << ngsmym.GetNNodes2() << "\n";

}
