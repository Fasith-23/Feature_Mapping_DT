#include "structures.h"
using namespace std;

class Delaunay
{
  public:
    Delaunay(const vector<Vertex_3D>& vertices_)
    {
      this->vertices = vertices_;
      this->Generate_PLY();
      this->Create_Hull();
      this->Generate_STL();
      //this->map = this->Map_Neigh();
    }
    unordered_map <Vertex_3D, vector<Vertex_3D>> Map_Neigh()
    {
      unordered_map <Vertex_3D, vector<Vertex_3D>> neigh_map;
      for (auto &vx : this->vertices)
      {
          for (auto &edg : this->edges)
              if (edg.end_vertices[0] == vx || edg.end_vertices[1] == vx)
                  neigh_map[vx].push_back(edg.end_vertices[0] == vx ? edg.end_vertices[1] : edg.end_vertices[0]);
      }
      return neigh_map;
    }
    void WriteVerticesToFile(const std::string& filename) {
        std::ofstream outfile(filename);

        if (!outfile.is_open()) {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
            return;
        }

        // Write vertices to the file
        for (const auto& vertex : this->vertices) {
            outfile << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")\n";
        }

        // Write edges to the file
        for (const auto& edge : this->edges) {
            outfile << "Edge: [" << edge.end_vertices[0].x << ", " << edge.end_vertices[0].y << ", " << edge.end_vertices[0].z << ", " << edge.end_vertices[1].x << ", " << edge.end_vertices[1].y << ", " << edge.end_vertices[1].z << "]\n";
        }

        outfile.close();
        std::cout << "Vertices and edges written to file: " << filename << std::endl;
    }
  private:
    vector<Vertex_3D> vertices = {};
    vector<Vertex_3D> Hull_vertices = {};
    list<Face> faces = {};
    list<Edge> edges = {};
    //unordered_map <Vertex_3D, vector<Vertex_3D>> map;
    unordered_map<size_t, Edge*> edge_map;


    void Generate_PLY(){
        vector<Vertex_3D> vxs=this->vertices;
        ofstream input_ply("inputdata.ply");
        input_ply << "ply\nformat ascii 1.0\nelement vertex " << vxs.size() << "\nproperty float x\nproperty float y\nproperty float z\nend_header\n";
        for (int j = 0; j < vxs.size(); j++) 
                input_ply << vxs[j].x << " " << vxs[j].y << " " << vxs[j].z << "\n";

    }
    void Create_Hull()
    {
      vector<Vertex_3D>& vertices = this->vertices;
      if(this->Start_Hull(vertices)){
        for(const auto& vx : vertices)
        {
          if(!vx.looped)
          {
          this->Increment_Hull(vx);
          this->Clean_Up();
          }
        }
      this->Hull_Vertices();
     }
      else return;
    }
    
    bool Collinear(const Vertex_3D& a, const Vertex_3D& b, const Vertex_3D& c) const
    {
      return ((b.z - a.z) * (c.x - a.x) - (b.x - a.x) * (c.z - a.z)) == 0 &&\
             ((c.z - a.z) * (b.y - a.y) - (b.z - a.z) * (c.y - a.y)) == 0 &&\
             ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) == 0;
    }
    int Volume_Sign(const Face& f, const Vertex_3D& v) const
    {
      float vol;
      float ax, ay, az, bx, by, bz, cx, cy, cz;
      ax = f.vertices[0].x - v.x;  
      ay = f.vertices[0].y - v.y;  
      az = f.vertices[0].z - v.z;
      bx = f.vertices[1].x - v.x;  
      by = f.vertices[1].y - v.y;  
      bz = f.vertices[1].z - v.z;
      cx = f.vertices[2].x - v.x;  
      cy = f.vertices[2].y - v.y;  
      cz = f.vertices[2].z - v.z;
      vol = ax * (by * cz - bz * cy) + ay * (bz * cx - bx * cz) + az * (bx * cy - by * cx);
      if(vol == 0) return 0;
      return vol < 0 ? -1 : 1;
    }

    void Create_Face(const Vertex_3D& a, const Vertex_3D& b, const Vertex_3D& c, const Vertex_3D& inner_vx)
    {
      this->faces.push_back(Face(a, b, c));
      auto& new_face = this->faces.back();
      if(this->Volume_Sign(this->faces.back(), inner_vx) < 0) new_face.Reverse();
      Create_Edge(a, b,new_face);
      Create_Edge(a, c,new_face);
      Create_Edge(b, c,new_face);
    }
    void Create_Edge(const Vertex_3D& v1, const Vertex_3D& v2, Face& new_face) {
        size_t key = this->Edge_Key(v1, v2);
        if (!this->edge_map.count(key)) { 
            this->edges.push_back(Edge(v1, v2));
            this->edge_map.insert({key, &this->edges.back()});
        }
        this->edge_map[key]->Face_linked(&new_face);
    }

    bool Start_Hull(vector<Vertex_3D>& vertices)
    {
      const int n = vertices.size();
      if(n < 4)
      {
        cout<<"no. of vertices < 4\n";
        return false;    
      }

      int i = 2;
      for (; i < n - 1 && this->Collinear(vertices[i], vertices[i - 1], vertices[i - 2]); i++) {
        if (i == n - 1) {
            cout << "vertices are collinear\n";
            return false;
        }
      }

      int k = Get_Off_Vertex(vertices,i);
      
      if (!k){
        cout<<"vertices are coplanar!\n";
        return false;
      } 

      Vertex_3D& v1 = vertices[i];  
      Vertex_3D& v2 = vertices[i-1];
      Vertex_3D& p3 = vertices[i-2];  
      Vertex_3D& p4 = vertices[k];

      v1.looped = v2.looped = p3.looped = p4.looped = true;
      
      this->Create_Face(v1, v2, p3, p4);
      this->Create_Face(v1, v2, p4, p3);
      this->Create_Face(v1, p3, p4, v2);
      this->Create_Face(v2, p3, p4, v1);
      return true;

    }

    int Get_Off_Vertex(vector<Vertex_3D>& vertices, int i){
        const int n = vertices.size();
        Face face(vertices[i], vertices[i-1], vertices[i-2]);
            int k = i;
            while(!this->Volume_Sign(face, vertices[k])) if(k++ == n-1) return 0;
            return k;
    }

    Vertex_3D Get_Inner_Vertex(const Face* f, const Edge& e)
    {
      for(int i = 0; i <= 2; i++)
      {
        if(f->vertices[i] != e.end_vertices[0] && f->vertices[i] != e.end_vertices[1]) return f->vertices[i];
      } 
    }
    bool Visibility_Check(const Vertex_3D vx){
      bool any_vis = false;
      for(auto& face : this->faces)
      {
        if(Volume_Sign(face, vx) < 0) 
        {      
          face.visible = true;
          any_vis = true;
        }
      }
      return any_vis;
    }

    void Increment_Hull(const Vertex_3D& vx)
    {
      if(!Visibility_Check(vx)) return;
      for(auto it = this->edges.begin(); it != this->edges.end(); it++)
      {
        auto& edge = *it;
        auto& face1 = edge.face_linked1;
        auto& face2 = edge.face_linked2;
        if(face1 == NULL || face2 == NULL)
        {
          continue;
        }
        else if(face1->visible && face2->visible) 
        {
          edge.remove = true;
        }
        else if(face1->visible|| face2->visible) 
        {
          if(face1->visible) std::swap(face1, face2);
          auto inner_vx = this->Get_Inner_Vertex(face2, edge);
          edge.Erase(face2);
          this->Create_Face(edge.end_vertices[0], edge.end_vertices[1], vx, inner_vx);
        }
      }
    }

    

    size_t Edge_Key(const Vertex_3D& a, const Vertex_3D& b) const
    {
      Vertex_Hash vh;
      return vh(a) ^ vh(b);
    }

    void Clean_Up()
    {
      Edge_Clean_Up();
      Face_Clean_Up();

    }
    void Edge_Clean_Up(){
      auto edge_ = this->edges.begin();
      while(edge_ != this->edges.end())
      {
        if(edge_->remove)
        {
          auto vx1 = edge_->end_vertices[0];
          auto vx2 = edge_->end_vertices[1];
          auto key_remove = this->Edge_Key(vx1, vx2);
          this->edge_map.erase(key_remove);
          this->edges.erase(edge_++);
        }
        else edge_++;
      };
    }
    void Face_Clean_Up(){
      auto face_ = this->faces.begin();
      while(face_ != this->faces.end())
      {
        if(face_->visible) this->faces.erase(face_++);
        else face_++;
      }
    }
    void Hull_Vertices()
    {
      set<Vertex_3D> Unique_vertices;
      for(const auto& f : this->faces)
      {
        for(int i = 0; i <= 2; i++){
          const auto& vertex_ = f.vertices[i];
          if(Unique_vertices.find(vertex_)==Unique_vertices.end()){
              Unique_vertices.insert(f.vertices[i]);
          }
          
        } 
      }
      this->Hull_vertices = vector<Vertex_3D>(Unique_vertices.begin(), Unique_vertices.end());
    }

    void Generate_STL() {
        ofstream stl_file("Delaunay.stl", ios::out | ios::binary);

        if (!stl_file.is_open()) {
            cerr << "Error opening the file!" << endl;
            return;
        }


        string header = "Generate_d by Convex Hull Algorithm";
        stl_file.write(header.c_str(), 80);


        unsigned int num_triangles = static_cast<unsigned int>(this->faces.size());
        stl_file.write(reinterpret_cast<char *>(&num_triangles), sizeof(num_triangles));

        for (const auto& face : this->faces) {
            float normal[3] = {0.0f, 0.0f, 0.0f};
            stl_file.write(reinterpret_cast<char *>(normal), 3 * sizeof(float));
            for (int i = 0; i < 3; ++i) {
                float vertex[3] = {face.vertices[i].x, face.vertices[i].y, face.vertices[i].z};
                stl_file.write(reinterpret_cast<char *>(vertex), 3 * sizeof(float));
            }
            unsigned short attribute_byte_count = 0;
            stl_file.write(reinterpret_cast<char *>(&attribute_byte_count), sizeof(attribute_byte_count));
        }

        stl_file.close();
    }

   

        

};

class Feature_Match{
  public:
    Feature_Match(unordered_map<Vertex_3D, vector<Vertex_3D>> a, unordered_map<Vertex_3D, vector<Vertex_3D>> b)
    {
      this->map_a = a;
      this->map_b = b;
      //this->check();
    }
    unordered_map<Vertex_3D, vector<Vertex_3D>> check(){
            unordered_map<Vertex_3D, vector<Vertex_3D>> fp = this->map_a;
            for(auto pt: this->map_a){
              bool ans;
              for(auto ptb: this->map_b){

              
                vector<float>ap, bp;
                Vertex_3D point = pt.first;
                Vertex_3D pointb = ptb.first;
                ap = this->dist_pt(1, point);
                bp = this->dist_pt(2, pointb);

                ans = this->is_equal(ap, bp);
                
              }
              if (ans == 0){
                    fp.erase(pt.first);
                }
            }

            return fp;
        }
    

        unordered_map<Vertex_3D, vector<Vertex_3D>> map_a;
        unordered_map<Vertex_3D, vector<Vertex_3D>> map_b;
        
        bool is_equal(vector<float> ap,vector<float> bp){

            sort(ap.begin(), ap.end());
            sort(bp.begin(), bp.end());
            
            if (ap.size() != bp.size()){
              return false;
            }
            int sz = bp.size();
            int threshold = 1;
            int max_thres = 0.8 * sz;

            int ans = 0;
            for(int i = 0;i<sz;i++){

                ans+=(abs(ap[i]-bp[i])<threshold);
            }


            return ans>=max_thres ? 1 : 0;
        }

        vector<float> dist_pt(const int num, Vertex_3D& pt){
            vector<float> ans;
            if(num==1){
                for(auto p: this->map_a[pt]){

                    ans.push_back(p.dist(pt));
                }
            }
            else{

                    for(auto p: this->map_b[pt]){

                    ans.push_back(p.dist(pt));
                }
            }
            return ans;
        }  
};

vector<Vertex_3D> readKeypointsFromFile(const string& filename) {
    ifstream file(filename);
    vector<Vertex_3D> vertices;

    if (file.is_open()) {
        float x, y;
        while (file >> x && file.ignore() && file >> y && file.ignore()) {
            float z = x * x + y * y;  // Calculate z coordinate
            Vertex_3D vertex(x, y, z);
            vertices.push_back(vertex);
        }
        file.close();
    } else {
        cerr << "Unable to open file!" << endl;
    }

    return vertices;
}
void WriteVerticesToFile(const std::string& filename, vector<Vertex_3D> vertices) {
        std::ofstream outfile(filename);

        if (!outfile.is_open()) {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
            return;
        }

        // Write vertices to the file
        for (const auto& vertex : vertices) {
            outfile << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")\n";
        }

        outfile.close();
        std::cout << "Vertices and edges written to file: " << filename << std::endl;
    }
int main()
{
  vector<Vertex_3D> tests_a = readKeypointsFromFile("keypoints1.txt");
  vector<Vertex_3D> tests_b = readKeypointsFromFile("keypoints2.txt");
  Delaunay C_a(tests_a);  
  unordered_map<Vertex_3D, vector<Vertex_3D>> neighbor_map_a = C_a.Map_Neigh();
  C_a.WriteVerticesToFile("output_vertices_edges_a.txt");
  Delaunay C_b(tests_b);  
  unordered_map<Vertex_3D, vector<Vertex_3D>> neighbor_map_b = C_b.Map_Neigh();
  C_b.WriteVerticesToFile("output_vertices_edges_b.txt");
  Feature_Match Fm(neighbor_map_a, neighbor_map_b);
  unordered_map<Vertex_3D, vector<Vertex_3D>> feature_map = Fm.check();

  vector<Vertex_3D> features;
  for (const auto& pt : feature_map){
    features.push_back(pt.first);
  }
  WriteVerticesToFile("final_features.txt",features);
 return 0;
}