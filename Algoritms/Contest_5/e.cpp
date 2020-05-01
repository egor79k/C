#include <iostream>
#include <vector>
#include <climits>

#define DEBUG

#ifdef DEBUG
#define flag printf ("flag in file %s, line %d, function %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
#else
#define flag ;
#endif

#ifdef DEBUG
#define SHOW(expr) printf ("showing \"%s\" = %11d | in file %s, line %d, function %s\n", #expr, expr, __FILE__, __LINE__, __PRETTY_FUNCTION__);
#else
#define SHOW(expr) ;
#endif

int count_ones (int mask) {
    unsigned int count = 0;
    for (; mask; count++)
        mask &= (mask - 1);
    
    return count;
}

int main () {
    std::ios::sync_with_stdio (0);
    std::cin.tie (0);
    std::cout.tie (0);
    
    const int max_val = 2000000000;
    
    int n = 0;
    std::cin >> n;
    
    std::vector<std::vector<int>> table ((1 << n), std::vector<int> (n, max_val));
    std::vector<std::vector<int>> paths (n, std::vector<int> (n, 0));
    
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            std::cin >> paths[i][j];
    
    table[0][0] = 0;
    int end = 1 << n;
    
    for (int i = 0; i < n; ++i)
        table[0][i] = 0;
    
    for (int mask = 0; mask < end; ++mask) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mask & (1 << i)) {
                    if (count_ones (mask) == 1)
                        table[mask][i] = 0;
                    else
                        table[mask][i] = std::min (table[mask][i],
                                                   table[mask - (1 << i)][j] + paths[j][i]);
                }
            }
        }
    }
    
    int optimal_path = INT_MAX;
    
    int index = 0;
    for (int i = 0; i < n; ++i)
        if (optimal_path > table[end - 1][i]) {
            index = i;
            optimal_path = table[end - 1][i];
        }
    
    std::cout << optimal_path << "\n";
    
    std::vector<int> path;
    path.push_back (index);
    int n_ones = n - 1;
    
    for (int mask = (1 << n) - 1; n_ones != 0; ) {
        for (int j = 0; j < n; ++j) {
            if (mask & (1 << j) and table[mask][index] == table[mask - (1 << index)][j] + paths[j][index]) {
                //std::cout << index + 1 << "-(" << paths[index][j] << ")->" << j + 1 << '\n';
                path.push_back (j);
                
                mask -= (1 << index);
                --n_ones;
                index = j;
                //std::cout << "mask: " << mask << '\n';
            }
        }
    }
    
    for (int i = n - 1; i >= 0; --i)
        std::cout << path[i] + 1 << ' ';
}
