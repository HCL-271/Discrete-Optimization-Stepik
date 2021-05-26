#include <algorithm>
#include <cmath>
#include <fstream>
#include <queue>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <numeric>



class Point2D {
public:
    using CoordType = long double;

    Point2D(CoordType x, CoordType y) : coord_x(x), coord_y(y) 
    {}
    Point2D() : coord_x(0), coord_y(0)
    {}

    CoordType Distance(const Point2D& other) const 
    {
        return std::sqrt((other.coord_x - coord_x) * (other.coord_x - coord_x) +
            (other.coord_y - coord_y) * (other.coord_y - coord_y));
    }

    friend std::istream& operator>>(std::istream& in, Point2D& pt) 
    {
        in >> pt.coord_x >> pt.coord_y;
        return in;
    }

private:
    CoordType coord_x, coord_y;
};

std::vector<std::pair<size_t, size_t>>

FindMST(const std::vector<Point2D>& pts) 
{
    size_t Partcle_count = pts.size();

    std::vector<bool> used(Partcle_count, false);

    used[0] = true;

    std::priority_queue<std::pair<Point2D::CoordType, std::pair<size_t, size_t>>>que;
    
    for (size_t i = 1; i < Partcle_count; ++i)
    {
        que.push({ -pts[0].Distance(pts[i]), {0, i} });
    }

    std::vector<std::pair<size_t, size_t>> mst;

    while (!que.empty()) 
    {
        auto edge = que.top().second;
        que.pop();

        if (used[edge.second]) 
        {
            continue;
        }

        used[edge.second] = true;

        mst.push_back(edge);

        for (size_t i = 0; i < Partcle_count; ++i)
        {
            if (!used[i]) 
            {
                que.push({ -pts[edge.second].Distance(pts[i]), {edge.second, i} });
            }
        }
    }
    return mst;
}

using Graph = std::vector<std::vector<size_t>>;

void Dfs(const Graph& g, size_t vertex, std::vector<bool>& used,
    std::vector<size_t>& path) 
{
    path.push_back(vertex);
    used[vertex] = true;

    for (auto to : g[vertex]) 
    {
        if (!used[to]) 
        {

            Dfs(g, to, used, path);
       
        }

    }
}

Point2D::CoordType ComputeTourDistance(const std::vector<Point2D>& pts,
    const std::vector<size_t>& tour) 
{
    Point2D::CoordType distance = 0;

    for (size_t i = 0; i < tour.size(); ++i) 
    {
        distance += pts[tour[i]].Distance(pts[tour[(i + 1) % tour.size()]]);
    }

    return distance;
}

std::vector<size_t> GetCycle(const Graph& g, const std::vector<Point2D>& pts) 
{
    size_t Counter_of_nodes = g.size();
    std::vector<size_t> path;
    std::vector<bool> used(Counter_of_nodes, false);
    std::vector<size_t> bestCycle;

    Point2D::CoordType bestDistance = 0;

    for (size_t i = 0; i < Counter_of_nodes; ++i)
    {
        path.clear();
        std::fill(used.begin(), used.end(), false);

        Dfs(g, i, used, path);

        auto currentDistance = ComputeTourDistance(pts, path);

        if (!i || bestDistance > currentDistance) 
        {
            bestCycle = path;
            bestDistance = currentDistance;
        }
    }
    return bestCycle;
}

using Solution = std::pair<Point2D::CoordType, std::vector<size_t>>;

Solution MSTSolution(const std::vector<Point2D>& pts) 
{
    size_t ptCount = pts.size();
    auto mst = FindMST(pts);
    Graph tree(ptCount);

    for (const auto& e : mst) 
    {
        tree[e.first].push_back(e.second);
        tree[e.second].push_back(e.first);
    }

    auto cycle = GetCycle(tree, pts);
    return { ComputeTourDistance(pts, cycle), cycle };
}

Solution LocalSearchSolution(const std::vector<Point2D>& pts) 
{
    auto Mst_Sol = MSTSolution(pts);
    auto bestDistance = Mst_Sol.first;

    std::cerr << "Init distance: " << bestDistance << std::endl;

    auto bestSolution = Mst_Sol.second;
    auto currentDistance = Mst_Sol.first;
    auto currentSolution = Mst_Sol.second;

    std::uniform_real_distribution<long double> unif(0, 1);
    std::uniform_int_distribution<size_t> ind(0, pts.size() - 1);
    std::random_device rand_dev;
    std::mt19937 rand_engine(rand_dev());

    long double temp = 5000;

    for (size_t it = 0; it < 6000000; ++it) 
    {
        size_t lft = ind(rand_engine);
        size_t right = ind(rand_engine);

        if (lft > right)
        {
            std::swap(lft, right);
        }
        auto next = currentSolution;
        std::reverse(next.begin() + lft, next.begin() + right + 1);

        Point2D::CoordType nextDistance = ComputeTourDistance(pts, next);

        auto delta = (nextDistance - currentDistance) / nextDistance;
        auto prob = std::exp(-delta / temp);

        if (nextDistance < currentDistance || unif(rand_engine) < prob)
        {
            currentDistance = nextDistance;
            currentSolution = next;
            if (currentDistance < bestDistance) 
            {
                std::cerr << "New distance found: " << currentDistance << '\r';
                bestSolution = currentSolution;
                bestDistance = currentDistance;
            }
            else 
            {
                temp *= 0.99996;
            }
        }
    }
    return { bestDistance, bestSolution };
}

Solution BruteForceSolution(const std::vector<Point2D>& pts) 
{
    size_t Partcle_count = pts.size();

    std::vector<size_t> perm(Partcle_count);
    std::iota(perm.begin(), perm.end(), 0);
    std::vector<size_t> bestSolution = perm;

    Point2D::CoordType bestDistance = ComputeTourDistance(pts, perm);

    while (std::next_permutation(perm.begin(), perm.end())) 
    {
        auto currentDistance = ComputeTourDistance(pts, perm);
        if (currentDistance < bestDistance)
        {
            bestSolution = perm;
            bestDistance = currentDistance;
        }
    }
    return { bestDistance, bestSolution };
}

void solve(std::istream& in, std::ostream& out) 
{
    size_t Partcle_count;
    in >> Partcle_count;
    std::vector<Point2D> Points(Partcle_count);
    std::unordered_map<size_t, int64_t> idByInd;

    int number;

    for (size_t i = 0; i < Partcle_count; ++i)
    {
        int64_t id = i;
        in >> number;
        in >> Points[i];
        //out << "read \n";
        idByInd[i] = id;
    }

    Solution solution;


        solution = LocalSearchSolution(Points);
        //solution = MSTSolution(pts);
   
    for (auto v : solution.second) 
    {
        out << idByInd[v] + 1 << ' ';
    }
  
    out << idByInd[solution.second[0]] + 1;
}

int main() 
{
    solve(std::cin, std::cout);

    return EXIT_SUCCESS;
}
