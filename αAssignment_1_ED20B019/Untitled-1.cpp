#include <iostream>
#include <vector>

int main() {
    int n = 5;  // Replace with your desired value for n

    std::vector<std::vector<int>> vec;
    std::vector<int> v1, v2, v3;

    v1.push_back(n);
    v1.push_back(n);
    vec.push_back(v1);

    v2.push_back(0);
    v2.push_back(0);
    vec.push_back(v2);

    v3.push_back(-n);
    v3.push_back(n);
    vec.push_back(v3);

    // Display the resulting matrix
    for (int i = 0; i < vec.size()+1; i++) { 
        for (int j = 0; j < vec[i].size(); j++) 
            std::cout << vec[i][j] << " "; 
        std::cout << std::endl; 
    } 

    return 0;
}



