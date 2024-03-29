#ifndef NGINTERFACE_V2
#define NGINTERFACE_V2


/**************************************************************************/
/* File:   nginterface_v2.hpp                                             */
/* Author: Joachim Schoeberl                                              */
/* Date:   May  09                                                        */
/**************************************************************************/

/*
  C++ interface to Netgen
*/

namespace netgen
{

  static constexpr int POINTINDEX_BASE = 1;
  
  struct T_EDGE2
  {
    // int orient:1;
    // int nr:31;    // 0-based
    int nr;    // 0-based
  };
  struct T_FACE2
  {
    // int orient:3;
    // int nr:29;    // 0-based
    int nr;    // 0-based
  };

  template <typename T>
  class Ng_Buffer
  {
    size_t s;
    T * data;
  public:
    Ng_Buffer (size_t as, T * adata)
      : s(as), data(adata) { ; }
    Ng_Buffer (Ng_Buffer && buffer)
      : s(buffer.Size()), data(buffer.Release()) { ; }
    ~Ng_Buffer () { delete [] data; }
    size_t Size() const { return s; }
    T * Release() { T * hd = data; data = nullptr; return hd; }
  };
  
  class Ng_Element
  {

    class Ng_Points
    {
    public:
      size_t num;
      const int * ptr;
  
      size_t Size() const { return num; }
      int operator[] (size_t i) const { return ptr[i];}//-POINTINDEX_BASE; }
    };


    class Ng_Vertices
    {
    public:
      size_t num;
      const int * ptr;
  
      size_t Size() const { return num; }
      int operator[] (size_t i) const { return ptr[i];}//-POINTINDEX_BASE; }
    };

    class Ng_Edges
    {
    public:
      size_t num;
      const T_EDGE2 * ptr;
  
      size_t Size() const { return num; }
      int operator[] (size_t i) const { return ptr[i].nr; }
    };

    class Ng_Faces
    {
    public:
      size_t num;
      const T_FACE2 * ptr;
  
      size_t Size() const { return num; }
      int operator[] (size_t i) const { return ptr[i].nr; }
    };

    class Ng_Facets
    {
    public:
      size_t num;
      int base;
      const int * ptr;
      
      size_t Size() const { return num; }
      int operator[] (size_t i) const { return ptr[i];}//-base; }
    };

    
  public:
    NG_ELEMENT_TYPE type;
    int index;           // material / boundary condition 
    const string * mat;   // material / boundary label
    NG_ELEMENT_TYPE GetType() const { return type; }
    int GetIndex() const { return index-1; }
    Ng_Points points;      // all points
    Ng_Vertices vertices;
    Ng_Edges edges;
    Ng_Faces faces;
    Ng_Facets facets;
    bool is_curved;
  };

  
  class Ng_Point
  {
    double * pt;
  public:
    Ng_Point (double * apt) : pt(apt) { ; }
    double operator[] (size_t i)
    { return pt[i]; }
    operator const double * () { return pt; }
  };




  template <int DIM> class Ng_Node;

  template <>
  class Ng_Node<0>
  {
    class Ng_Elements
    {
    public:
      size_t ne;
      const int * ptr;
  
      size_t Size() const { return ne; }
      int operator[] (size_t i) const { return ptr[i]; }
    };


  public:
    Ng_Elements elements;
    Ng_Elements bnd_elements;
  };



  
  template <>
  class Ng_Node<1>
  {
    class Ng_Vertices
    {
    public:
      size_t ne = 2;
      const int * ptr;
  
      size_t Size() const { return ne; }
      int operator[] (size_t i) const { return ptr[i];}//-POINTINDEX_BASE; }
    };


  public:
    Ng_Vertices vertices;
  };



  template <>
  class Ng_Node<2>
  {
    class Ng_Vertices
    {
    public:
      size_t nv;
      const int * ptr;
  
      size_t Size() const { return nv; }
      int operator[] (size_t i) const { return ptr[i];}//-POINTINDEX_BASE; }
    };

    class Ng_Edges
    {
    public:
      size_t ned;
      const int * ptr;
  
      size_t Size() const { return ned; }
      int operator[] (size_t i) const { return ptr[i];}//-1; }
    };


  public:
    Ng_Vertices vertices;
    Ng_Edges edges;
    int surface_el;  // -1 if face not on surface
  };



    




  class Mesh;


  inline void DummyTaskManager2 (function<void(int,int)> func)
  { func(0,1); }
  inline void DummyTracer2 (string, bool) { ; } 
  
  class DLL_HEADER Ngx_Mesh
  {
  public:
    // Ngx_Mesh () { ; }
    // Ngx_Mesh(class Mesh * amesh) : mesh(amesh) { ; }
    virtual void LoadMesh (const string & filename) = 0;

    virtual void LoadMesh (istream & str) = 0;
    virtual void SaveMesh (ostream & str) const = 0;
    virtual void UpdateTopology () = 0;
    virtual void DoArchive (ngstd::Archive & archive) = 0;

    virtual ~Ngx_Mesh() = 0;

    virtual bool Valid () = 0;
    
    virtual int GetDimension() const = 0;
    virtual int GetNLevels() const = 0;

    virtual int GetNElements (int dim) const = 0;
    virtual int GetNNodes (int nt) const = 0;

    virtual Ng_Point GetPoint (int nr) const = 0;

    //template <int DIM> 
    virtual Ng_Element GetElement0 (size_t nr) const = 0;
    virtual Ng_Element GetElement1 (size_t nr) const = 0;
    virtual Ng_Element GetElement2 (size_t nr) const = 0;
    virtual Ng_Element GetElement3 (size_t nr) const = 0;

    //template <int DIM> 
    virtual int GetElementIndex0 (size_t nr) const = 0;
    virtual int GetElementIndex1 (size_t nr) const = 0;
    virtual int GetElementIndex2 (size_t nr) const = 0;
    virtual int GetElementIndex3 (size_t nr) const = 0;

    /// material/boundary label of region, template argument is co-dimension
    //template <int DIM> 
    virtual const string & GetMaterialCD0 (int region_nr) const = 0;
    virtual const string & GetMaterialCD1 (int region_nr) const = 0;
    virtual const string & GetMaterialCD2 (int region_nr) const = 0;
    virtual const string & GetMaterialCD3 (int region_nr) const = 0;

    /// Curved Elements:
    /// elnr .. element nr
    /// xi..... DIM_EL local coordinates
    /// x ..... DIM_SPACE global coordinates 
    /// dxdxi...DIM_SPACE x DIM_EL Jacobian matrix (row major storage)
    virtual void ElementTransformation3x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation2x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation1x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation0x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation2x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation1x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation1x1 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation0x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    virtual void ElementTransformation0x1 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const = 0;

    /// Curved Elements:
    /// elnr .. element nr
    /// npts .. number of points
    /// xi..... DIM_EL local coordinates
    /// sxi ... step xi
    /// x ..... DIM_SPACE global coordinates
    /// dxdxi...DIM_SPACE x DIM_EL Jacobian matrix (row major storage)

    virtual void MultiElementTransformation3x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation2x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation2x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation1x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation0x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation1x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation1x1 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation0x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation0x1 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation3x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation2x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation2x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation1x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation0x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation1x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation1x1 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation0x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual void MultiElementTransformation0x1 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const = 0;

    virtual const Ng_Node<0> GetNode0 (int nr) const = 0;
    virtual const Ng_Node<1> GetNode1 (int nr) const = 0;
    virtual const Ng_Node<2> GetNode2 (int nr) const = 0;
    
    
    virtual int GetNNodes1 () = 0;
    virtual int GetNNodes2 () = 0;

    // returns domain numbers of domains next to boundary bnr -> (domin, domout)
    // 3D only
    // std::pair<int,int> GetBoundaryNeighbouringDomains (int bnr);

    void Curve (int order);
    void Refine (NG_REFINEMENT_TYPE reftype,
                 void (*taskmanager)(function<void(int,int)>) = &DummyTaskManager2,
                 void (*tracer)(string, bool) = &DummyTracer2);

    virtual void GetParentNodes (int ni, int * parents) const = 0;
    virtual int GetParentElement (int ei) const = 0;
    virtual int GetParentSElement (int ei) const = 0;

    virtual int GetNIdentifications() const = 0;
    virtual int GetIdentificationType(int idnr) const = 0;
    virtual Ng_Buffer<int[2]> GetPeriodicVertices(int idnr) const = 0;

    // Find element of point, returns local coordinates
    template <int DIM>
    int FindElementOfPoint 
    (double * p, double * lami,
     bool build_searchtrees = false, 
     int * const indices = NULL, int numind = 0) const;
    

#ifdef PARALLEL
    std::tuple<int,int*> GetDistantProcs (int nodetype, int locnum) const;
#endif

    virtual shared_ptr<Mesh> GetMesh () const = 0; 
    virtual shared_ptr<Mesh> SelectMesh () const = 0; 
    virtual inline int GetTimeStamp() const = 0;
  };

  class MyMesh;

  class DLL_HEADER Ngx_MyMesh : public Ngx_Mesh
  {
  private:
    shared_ptr<MyMesh> mesh;
  public: 
    Ngx_MyMesh(shared_ptr<MyMesh> angxmesh = NULL);

    void LoadMesh (const string & filename) override;

    void LoadMesh (istream & str) override;
    void SaveMesh (ostream & str) const override;
    void UpdateTopology () override;
    void DoArchive (ngstd::Archive & archive) override;

    virtual ~Ngx_MyMesh ();

    int GetDimension() const override;
    int GetNLevels() const override;

    int GetNElements (int dim) const override;
    int GetNNodes (int nt) const override;

    Ng_Point GetPoint (int nr) const override;

    int GetElementIndex0 (size_t nr) const override;
    int GetElementIndex1 (size_t nr) const override;
    int GetElementIndex2 (size_t nr) const override;
    int GetElementIndex3 (size_t nr) const override;

    Ng_Element GetElement0 (size_t nr) const override;
    Ng_Element GetElement1 (size_t nr) const override;
    Ng_Element GetElement2 (size_t nr) const override;
    Ng_Element GetElement3 (size_t nr) const override;

    const string & GetMaterialCD0 (int region_nr) const override;
    const string & GetMaterialCD1 (int region_nr) const override;
    const string & GetMaterialCD2 (int region_nr) const override;
    const string & GetMaterialCD3 (int region_nr) const override;

    void ElementTransformation3x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation2x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation1x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation0x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation2x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation1x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation1x1 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation0x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation0x1 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void MultiElementTransformation3x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x1 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x1 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation3x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x1 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x1 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    const Ng_Node<0> GetNode0 (int nr) const override;
    const Ng_Node<1> GetNode1 (int nr) const override;
    const Ng_Node<2> GetNode2 (int nr) const override;

    int GetNNodes1 () override;
    int GetNNodes2 () override;

    void GetParentNodes (int ni, int * parents) const override;
    int GetParentElement (int ei) const override;
    int GetParentSElement (int ei) const override;

    int GetNIdentifications() const override;
    int GetIdentificationType(int idnr) const override;
    Ng_Buffer<int[2]> GetPeriodicVertices(int idnr) const override;

    inline int GetTimeStamp() const override;


    virtual shared_ptr<Mesh> GetMesh () const override { return nullptr; } 
    virtual shared_ptr<Mesh> SelectMesh () const override{ return nullptr; }
    bool Valid () override { return true; }
    
  };

  class DLL_HEADER Ngx_netgen_Mesh : public Ngx_Mesh
  {
  private:
    shared_ptr<Mesh> mesh;
  public: 
    Ngx_netgen_Mesh(shared_ptr<Mesh> angxmesh = NULL);

    void LoadMesh (const string & filename) override;

    void LoadMesh (istream & str) override;
    void SaveMesh (ostream & str) const override;
    void UpdateTopology () override;
    void DoArchive (ngstd::Archive & archive) override;

    virtual ~Ngx_netgen_Mesh();

    bool Valid () override { return mesh != NULL; }

    int GetDimension() const override;
    int GetNLevels() const override;

    int GetNElements (int dim) const override;
    int GetNNodes (int nt) const override;

    Ng_Point GetPoint (int nr) const override;

    int GetElementIndex0 (size_t nr) const override;
    int GetElementIndex1 (size_t nr) const override;
    int GetElementIndex2 (size_t nr) const override;
    int GetElementIndex3 (size_t nr) const override;

    Ng_Element GetElement0 (size_t nr) const override;
    Ng_Element GetElement1 (size_t nr) const override;
    Ng_Element GetElement2 (size_t nr) const override;
    Ng_Element GetElement3 (size_t nr) const override;

    const string & GetMaterialCD0 (int region_nr) const override;
    const string & GetMaterialCD1 (int region_nr) const override;
    const string & GetMaterialCD2 (int region_nr) const override;
    const string & GetMaterialCD3 (int region_nr) const override;

    void ElementTransformation3x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation2x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation1x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation0x3 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation2x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation1x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation1x1 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation0x2 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void ElementTransformation0x1 (int elnr,
                                const double * xi, 
                                double * x, 
                                double * dxdxi) const override;

    void MultiElementTransformation3x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x3 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x1 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x2 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x1 (int elnr, int npts,
                                     const double * xi, size_t sxi,
                                     double * x, size_t sx,
                                     double * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation3x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation2x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x3 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation1x1 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x2 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    void MultiElementTransformation0x1 (int elnr, int npts,
                                     const tAVXd * xi, size_t sxi,
                                     tAVXd * x, size_t sx,
                                     tAVXd * dxdxi, size_t sdxdxi) const override;

    const Ng_Node<0> GetNode0 (int nr) const override;
    const Ng_Node<1> GetNode1 (int nr) const override;
    const Ng_Node<2> GetNode2 (int nr) const override;

    int GetNNodes1 () override;
    int GetNNodes2 () override;

    void GetParentNodes (int ni, int * parents) const override;
    int GetParentElement (int ei) const override;
    int GetParentSElement (int ei) const override;

    int GetNIdentifications() const override;
    int GetIdentificationType(int idnr) const override;
    Ng_Buffer<int[2]> GetPeriodicVertices(int idnr) const override;

    shared_ptr<Mesh> GetMesh () const override; 
    shared_ptr<Mesh> SelectMesh () const override; 

    inline int GetTimeStamp() const override;
  };

DLL_HEADER Ngx_netgen_Mesh * LoadMesh (const string & filename);
}



#ifdef HAVE_NETGEN_SOURCES
#include <meshing.hpp>

namespace netgen
{
#ifdef __GNUC__
#define NGX_INLINE  __attribute__ ((__always_inline__)) inline
#else
#define NGX_INLINE inline
#endif  
#include <nginterface_v2_impl.hpp>
}

#endif


#endif

