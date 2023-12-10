#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <cstdlib>  // for srand
#include <ctime>  
using namespace std;

#include <chrono>
using namespace std::chrono;
int main(){
    for(int n=2;n<10000;n++){
        std::srand(std::time(0));
        std::vector<std::vector<int>> pts;
        std::vector<int> v1 = {n, n};
        std::vector<int> v2 = {n, 0};
        std::vector<int> v3 = {0, 0};
        std::vector<int> v4 = {0, n};

        pts.push_back(v1);
        pts.push_back(v2);
        pts.push_back(v3);
        pts.push_back(v4);

        //// Display the resulting matrix
        //for (int i = 0; i < pts.size(); i++) { 
        //    for (int j = 0; j < pts[i].size(); j++) 
        //        std::cout << pts[i][j] << " "; 
        //    std::cout << std::endl; 
        //}    

        for (int i=1;i<n;i++){
            int h = rand() % (n) + 1;
            vector<int> addn={i,h};
            pts.push_back(addn);
        }
        //for (int i = 0; i < pts.size(); i++) { 
        //for (int j = 0; j < pts[i].size(); j++) 
        //    std::cout << pts[i][j] << " "; 
        //std::cout << std::endl; 
        //} 
    }
    
    return 0;
    //int n;
    //vector<point> points;
//
    //vector<vector<double>> pts = {{343, 392}, {475, 103}, {245, 151}, {193, 323}, {91, 279}, {51, 301},
    //                                {25, 381}, {80, 334}, {142, 418}, {325, 480}, {340, 564}, {468, 597}};
//
    //n = pts.size();
    //for(int i = 0;i<n;i++){
//
    //    double x = pts[i][0];
    //    double y = pts[i][1];
//
    //    point a(x, y);
    //    points.push_back(a);
    //}
//
    ////cout<<"X = ";
    ////cout<<'[';
    ////for(auto pt: points){
////
    ////    cout<<pt.x<<", ";
    ////}
    ////cout<<points[0].x;
    ////cout<<']';
    ////cout<<'\n';
////
    ////cout<<"Y = ";
    ////cout<<'[';
    ////for(auto pt: points){
////
    ////    cout<<pt.y<<", ";
    ////}
    ////cout<<points[0].y;
    ////cout<<']';
    ////cout<<"\n\n";
    //vector<bool> earStatus(n, false);
//
    //auto start = high_resolution_clock::now();
    //vector<vector<point>> triangles = triangulate(points, earStatus);
    //auto stop = high_resolution_clock::now();
//
    //cout<<"triangles = ";
    //cout<<'[';
    //for(auto triangle: triangles){
    //    cout<<'[';
    //    cout<<'[';
    //    for(auto point: triangle){
//
    //        cout<<point.x<<", ";
    //    }
//
    //    cout<<triangle[0].x;
    //    cout<<"],";
//
    //    cout<<'\n';
//
    //    cout<<'[';
    //    for(auto point: triangle){
//
    //        cout<<point.y<<", ";
    //    }
    //    cout<<triangle[0].y;
    //    cout<<']';
    //    cout<<"],";
    //    cout<<"\n\n";
    //}
    //cout<<']';
//
    //cout<<'\n';
    //auto duration = duration_cast<microseconds>(stop - start);
    //cout << duration.count() << endl;
    //return 0;
}
