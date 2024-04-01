#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

/*

Input

8
1 9
1 6
1 10
1 4
3 6
3 14
2 6
3 6

Output

Yes
No
No

*/


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */

    int n;
    cin >> n;
    
    std::set<int> ret;

    for(int i=1 ; i <= n ; i++)
    {
        int x,y;
        cin >> x >>y;
        
        // std::cout << x  << "," << y << std::endl ;
        
        if(x == 1)
        {
            ret.insert(y);
        }
        else if(x == 2)
        {
            ret.erase(y);
        }
        else if(x == 3)
        {
            auto iter = ret.find(y);
            if(iter == ret.end())
            {
                std::cout << "No" << std::endl;
            }
            else{
                std::cout << "Yes" << std::endl;
            }
        }
        
    }
    return 0;
}