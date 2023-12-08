#ifndef STRUCTURES
#define STRUCTURES


#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <fstream>
#include <set>
#include <math.h>
#include <algorithm>
using namespace std;

struct Vertex_3D {
  float x,y,z;
  bool looped;
  Vertex_3D() = default;
  Vertex_3D(float _x, float _y, float _z):\
      x(_x), y(_y), z(_z), looped(false) {}

  bool operator !=(const Vertex_3D& vx) const
  {
      return x != vx.x || y != vx.y || z != vx.z;
  }
  bool operator ==(const Vertex_3D& vx) const
  {
      return x == vx.x && y == vx.y && z == vx.z;
  }
  bool operator <(const Vertex_3D& vx) const
  {
      return x < vx.x && y < vx.y && z < vx.z;
  }
  double dist(Vertex_3D& vx){

            return sqrt(((vx.x-x)*(vx.x-x)) + ((vx.y-y)*(vx.y-y)));
  }
      size_t operator()(const Vertex_3D& v) const {
        // Create a hash combining the individual hashes of x, y, and z
        size_t seed = 0;
        hash_combine(seed, std::hash<float>{}(v.x));
        hash_combine(seed, std::hash<float>{}(v.y));
        hash_combine(seed, std::hash<float>{}(v.z));
        return seed;
    }

private:
    // Helper function for combining hashes
    void hash_combine(size_t& seed, size_t hash) const {
        seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

// Specialization of std::hash for Vertex_3D
namespace std {
    template <>
    struct hash<Vertex_3D> {
        size_t operator()(const Vertex_3D& v) const {
            return v(v); // Call the operator() defined in Vertex_3D
        }
    };
};

struct Vertex_Hash
{
  size_t operator() (const Vertex_3D& v) const
  {
      return hash<string>{}(to_string(v.x)+to_string(v.y)+to_string(v.z));
  }
};

struct Face
{
  bool visible;
  Vertex_3D vertices[3];
  Face(const Vertex_3D& v1, const Vertex_3D& v2, const Vertex_3D& v3): visible(false)
      { vertices[0] = v1; vertices[1] = v2; vertices[2] = v3;};

  void Reverse(){swap(vertices[0], vertices[2]); };
};

struct Edge
{
  bool remove; 
  Face *face_linked1, *face_linked2;
  Vertex_3D end_vertices[2];
  Edge(const Vertex_3D& v1, const Vertex_3D& v2): 
      face_linked1(nullptr), face_linked2(nullptr), remove(false) 
      { end_vertices[0] = v1; end_vertices[1] = v2; };
  
  void Face_linked(Face* face) 
  {
    if( face_linked1 == NULL || face_linked2 == NULL )
      (face_linked1 == NULL ? face_linked1 : face_linked2)= face;
  };

  void Erase(Face* face) 
  {
    if(face_linked1 == face || face_linked2 == face) 
      (face_linked1 == face ? face_linked1 : face_linked2) = nullptr;
    else return;
  };
};
#endif