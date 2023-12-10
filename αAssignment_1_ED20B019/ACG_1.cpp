#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
#include <fstream>
#include <chrono>
using namespace std::chrono;


struct point{

    int x, y;
    point ():x(0), y(0){}
    point(int x, int y): x(x), y(y){}
};

int clamp_idx(int x, int a, int b){

    x = x<a?b:x;
    x = x>b?a:x;

    return x;
}

bool XOR(bool x, bool y){

    return !x^!y;
}

double Area(point a, point b, point c){

    return (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y);
}

bool isLeft(point a, point b, point c){

    return Area(a, b, c) >0;
}

bool isLeftOn(point a, point b, point c){

    return Area(a, b, c) >=0;
}

bool collinear(point a, point b, point c){

    return Area(a, b, c) == 0;
}

bool IntersectProp(point a, point b, point c, point d){

    if( collinear(a, b, c) ||
        collinear(a, b, d) ||
        collinear(c, d, a) ||
        collinear(c, d, b)
      )

      return false;

    return XOR(isLeft(a, b, c), isLeft(a, b, d)) && XOR(isLeft(c, d, a), isLeft(c, d, b));
}

bool between(point a, point b, point c){

    if(!collinear(a, b, c))return false;

    if(a.x !=b.x){

        return ((a.x <= c.x) && (c.x <= b.x)) || ((a.x >=c.x) && (c.x >=b.x));
    }

    return ((a.y <= c.y) && (c.y <= b.y)) || ((a.y >= c.y) && (c.y >= b.y));
}


bool Intersect(point a, point b, point c, point d){

    if(IntersectProp(a, b, c, d))return true;

    if(between(a, b, c) ||
       between(a, b, d) ||
       between(c, d, a) ||
       between(c, d, b)
       )

       return true;

    return false;
}

bool inCone(int i, int j, vector<point> points){

    point a0, a1;
    int n = points.size();
    point a = points[i];
    point b = points[j];
    a1 = points[clamp_idx(i-1, 0, n-1)];
    a0 = points[clamp_idx(i+1, 0, n-1)];

    if(isLeftOn(a, a1, a0)){

        return isLeft(a, b, a0) && isLeft(b, a, a1);
    }

    return !( isLeftOn(a, b, a1) && isLeftOn(b, a, a0));
}


bool Diagonalie(int i, int j, vector<point> points){

    int i0 = 0;
    int i1 = 0;
    int n = points.size();
    point a = points[i];
    point b = points[j];

    while(i<n){
        i1 = clamp_idx(i0+1, 0, n-1);
        point c = points[i0];
        point c1 = points[i1];

        if(((c.x==a.x) && (c.y==a.y)) || ((c.x==b.x) && (c.y==b.y)) ||
           ((c1.x==a.x) && (c1.y==a.y)) || ((c1.x==b.x) && (c1.y==b.y))){i++;continue;}
        if(Intersect(a, b, c, c1))return false;
        i++;
    }
    return true;
}

bool isDiagonal(int i, int j, vector<point> points){

    return inCone(i, j, points) && inCone(j, i, points) && Diagonalie(i, j, points);
}

void EarinIt(vector<point> points, vector<bool>& earStatus){

    int i0, i1, i2;
    i0 = 1, i1 = 1, i2 = 1;

    int n = points.size();
    while(i1<n){

        i2 = clamp_idx(i1+1, 0, n-1);
        i0 = i1-1;

        earStatus[i1] = isDiagonal(i0, i2, points);
        i1++;
    }

    return ;
}

vector<vector<point>> triangulate(vector<point> points, vector<bool>& earStatus){

    int n = points.size();
    //int m = n;
    vector<vector<point>> triangles;
    EarinIt(points, earStatus);
    int i0, i1, i2, i3, i4;
    while(n>3){

        i2 = 0;
        while(i2<n){

            if(earStatus[i2]){

                i3 = clamp_idx(i2+1, 0, n-1);
                i4 = clamp_idx(i3+1, 0, n-1);
                i1 = clamp_idx(i2-1, 0, n-1);
                i0 = clamp_idx(i1-1, 0, n-1);

                triangles.push_back({points[i1], points[i2], points[i3]});

                earStatus[i1] = isDiagonal(i0, i3, points);
                earStatus[i3] = isDiagonal(i1, i4, points);

                points.erase(points.begin()+i2);
                earStatus.erase(earStatus.begin()+i2);
                n--;
            }
            i2++;
        }
    }

    return triangles;
}
void to_plot(vector<vector<int>> pts){
    std::ofstream triangleplot("triangledata.csv");
    int n;
    vector<point> points;
    n = pts.size();
    for(int i = 0;i<n;i++){

        double x = pts[i][0];
        double y = pts[i][1];

        point a(x, y);
        points.push_back(a);
    }
    vector<bool> earStatus(n, false);
    vector<vector<point>> triangles = triangulate(points, earStatus);
    for (int i = 0; i < triangles.size(); i++) { 
        for (int j = 0; j < triangles[i].size(); j++) 
            triangleplot << triangles[i][j].x << "," << triangles[i][j].y << ",";
        triangleplot << "\n"; 
    }
 
    //return 0;
}

auto to_timeplot(vector<vector<int>> pts){
    int n;
    vector<point> points;
    n = pts.size();
    for(int i = 0;i<n;i++){

        double x = pts[i][0];
        double y = pts[i][1];

        point a(x, y);
        points.push_back(a);
    }
    vector<bool> earStatus(n, false);

    auto start = high_resolution_clock::now();
    vector<vector<point>> triangles = triangulate(points, earStatus);
    auto stop = high_resolution_clock::now();


    auto duration = duration_cast<microseconds>(stop - start);
    //cout << duration.count() << endl;
    return duration;
}

int main(){
    std::ofstream timeplot("timedata.csv");
    //std::ofstream pointplot("pointdata.csv");
    for(int n=2;n<10;n++){
        //int n=10;
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
        int h_prev=0;
        int h_prev_prev=0;
        for (int i=1;i<n;i++){
            int h = rand() % (n) + 1;
            if (h==h_prev || (h-h_prev) == (h_prev-h_prev_prev) ){
                h=h+1;
            }
            h_prev_prev=h_prev;
            h_prev=h;
            
            vector<int> addn={i,h};
            pts.push_back(addn);
        }
        
        //for (int i = 0; i < pts.size(); i++) { 
        //    pointplot << pts[i][0] << ","<< pts[i][1] << "\n"; 
        //} 
        auto time = to_timeplot(pts);
        timeplot << time.count() << "\n";
        //to_plot(pts);

    }
    
    return 0;
}



