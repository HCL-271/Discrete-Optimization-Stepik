#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <map>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <iostream>

#include <vector>
using namespace std;
class OOP_DISSAPOINTS_ME {
public:
   
    
    using Table = std::array<std::array<int, 4>, 4>;

    Table end{};

    OOP_DISSAPOINTS_ME() { //fiilling array  with 0 \dots 15
       /*for (int i = 0; i < 4; i = i+1) {
            for (int j = 0; j < 4; j++) {
               end[i][j]  = (i)*4+(j+1) ;
               end[3][3] = 15;
                cout << end[i ][j] << " ";
            }
        }*/
      
        for (int i = 0; i < 15; i = i+1) {//this works faster
            end[i / 4][i % 4] = i + 1;
            //cout << end[i][j] << " ";
        }
        /*for (int i = 0; i < 4; i = i + 1) {
            for (int j = 0; j < 4; j++) {
                
                cout << end[i][j] << " ";
            }
        }*/

    }

    int FINAL_SOLUTION(const Table& start) const {
        std::priority_queue<std::pair<int, Table>> ochered;

        ochered.push({ -able_movement(start), start });
        std::map<Table, int> distance;
        distance[start] = 0;

        while (!ochered.empty()) {
            auto table = ochered.top().second;
            ochered.pop();
            if (table == end) {
                break;
            }
            int newDistance = distance[table] + 1;
            for (auto to : sosedi(table)) {
                if (distance.count(to) == 0 || distance[to] > newDistance) {
                    distance[to] = newDistance;
                    int priority = newDistance + able_movement(to);
                    ochered.push({ -priority, to });
                }
            }
        }
        return distance[end];
    }


    static std::vector<Table> sosedi( Table& t1) {
        int h0;
        int w0;

        for (int i = 0; i < 4; i++) { //locating zero
            for (int j = 0; j < 4; j++) {
                if (t1[i][j] == 0) 
                {
                    h0 = i;
                    w0 = j;
                    break;
                }
            }
        }

        Table next = t1;
        std::vector<Table> nb_near;
        for (int index_of_height = -1; index_of_height < 2; index_of_height++) {
            for (int index_of_weight = -1; index_of_weight < 2; index_of_weight++) {
                if (std::abs(index_of_height) != std::abs(index_of_weight) && ((std::min(h0+ index_of_height, w0 + index_of_weight) >= 0 && (h0 + index_of_height < 4) && (w0 + index_of_weight < 4)))) {
                  
                    
                    
                    
                    std::swap(next[h0][w0], next[h0 + index_of_height][w0 + index_of_weight]);
                    nb_near.push_back(next);
                    std::swap(next[h0][w0], next[h0 + index_of_height][w0 + index_of_weight]);
                }
            }
        }
        return nb_near;
    }

    static int able_movement(const Table& t1) {
        int mv = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::pair<int, int> rp = { (t1[i][j] + 15) % 16 / 4,  (t1[i][j] + 15) % 16 % 4 };
                mv += std::abs(i - rp.first) + std::abs(j - rp.second);
            }
        }     
        return mv;
    }



};



int main() {
OOP_DISSAPOINTS_ME::Table t1;
    for (int i = 0; i < 4; i++) { // input
        for (int j = 0; j < 4; j++) {
            cin >> t1[i][j];
        }
    }
    cout << OOP_DISSAPOINTS_ME().FINAL_SOLUTION(t1) << '\n';
    return 0;
}
