#include "graph.h"
#include <cmath>
#include <stdexcept>
#include <ostream>
using namespace std;

namespace {
	static const double EPS = 1e-8;

	bool equal(double a, double b) {
		return abs(a - b) < EPS;
	}
}

double Point::distance(const Point& p) const {
	return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
}

double dot(const Point& v1, const Point& v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}

Point closest(const Point& a, const Point& b, const Point& p) {
    Point ap(p.x - a.x, p.y - a.y);
    Point ab(b.x - a.x, b.y - a.y);
    double d = dot(ap, ab) / dot(ab, ab);
    if (d < 0) {
       d = 0;
    }
    else if (d > 1) {
        d = 1;
    }
    return Point(a.x + (b.x - a.x) * d, a.y + (b.y - a.y) * d);
}

bool intersection_point_circle(const Circle& c, const Point&p) {
    Point a(p.x - c.c.x, p.y - c.c.y);
    return dot(a, a) <= c.r * c.r;
}

bool instersection_segment_circle(const Point&a, const Point&b, const Circle& c) {
    return intersection_point_circle(c, closest(a, b, c.c));
}

bool Point::operator==(const Point& p) const {
	return equal(x, p.x) && equal(y, p.y);
}

bool Point::operator!=(const Point& p) const {
	return !equal(x, p.x) || !equal(y, p.y);
}

bool Point::operator<(const Point& p) const {
	return (x < p.x) || (equal(x, p.x) && y < p.y);
}

bool Point::operator>(const Point& p) const {
	return (x > p.x) || (equal(x, p.x) && y > p.y);
}

bool Point::operator<=(const Point& p) const {
	return (x < p.x) || (equal(x, p.x) && ((y < p.y) || equal(y, p.y)));
}

bool Point::operator>=(const Point& p) const {
	return (x > p.x) || (equal(x, p.x) && ((y > p.y) || equal(y, p.y)));
}

ostream& operator<<(ostream& os, const Point& p) {
	os << "(" << p.x << ", " << p.y << ")";
	return os;
}



int Graph::add_vertex(const Point& p) {
    int vertex = points.size();

    if (!has_vertex(p)) {
        vertices[vertex] = std::map<int, double>();
        points.push_back(p);
        return vertex;
    }
    else {
        for (int i = 0; i < points.size(); ++i) {
            if (points[i] == p) {
                vertex = i;
            }
        }
        return vertex;
    }
}

void Graph::add_edge(const Point& p1, const Point& p2, double weight) {
    int start_vertex = add_vertex(p1);
    int end_vertex = add_vertex(p2);
    vertices[start_vertex][end_vertex] = weight;
    vertices[end_vertex][start_vertex] = weight;
}

std::vector<int> Graph::get_vertices() const {
    std::vector<int> result;
    for (const auto& p : vertices) {
        result.push_back(p.first);
    }
    return result;
}

std::vector<int> Graph::get_adjacent_vertices(int src_vertex) const {
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return std::vector<int> {};
    }
    vector<int> result;
    for (const auto& p : it->second) {
        result.push_back(p.first);
    }
    return result;
}

vector<pair<int, double>> Graph::get_adjacent_edges(int src_vertex) const {
    const auto it = vertices.find(src_vertex);
    if (it == vertices.end()) {
        return vector<pair<int, double>> {};
    }
    vector<pair<int, double>> result;
    for (const auto& p : it->second) {
        result.push_back(make_pair(p.first, p.second));
    }
    return result;
}

bool Graph::has_vertex(const Point& p) const {
    return (find(points.begin(), points.end(), p) != points.end());
}

bool Graph::has_edge(const Point& p1, const Point& p2) const {
    int start_vertex = points.size();
    int end_vertex = points.size();

    for (int i = 0; i < points.size(); ++i) {
        if (points[i] == p1) {
            start_vertex = i;
        }
        if (points[i] == p2) {
            end_vertex == i;
        }
    }

    const auto it = vertices.find(start_vertex);
    if (it == vertices.end()) {
        return false;
    }
    return(it->second.find(end_vertex) != it->second.end());
}

double Graph::edge_weight(int start_vertex, int end_vertex) const {
    const auto it_s = vertices.find(start_vertex);
    if (it_s == vertices.end()) {
        throw invalid_argument("Edge doesn't exist");
    }
    const auto it_e = it_s->second.find(end_vertex);
    if (it_e == it_s->second.end()) {
        throw invalid_argument("Edge doesn't exist");
    }
    return it_e->second;
}

void Graph::remove_vertex(const Point& p) {
    auto it_p = find(points.begin(), points.end(), p);
    if (it_p != points.end()) {
        int vertex = points.size();

        for (int i = 0; i < points.size(); ++i) {
            if (points[i] == p) {
                vertex = i;
            }
        }

        vector<int> adjacent_vertices = get_adjacent_vertices(vertex);
        for (const auto& adj_v : adjacent_vertices) {
            remove_edge(adj_v, vertex);
        }
        vertices.erase(vertex);
        points.erase(it_p);
    }
}

void Graph::remove_edge(int start_vertex, int end_vertex) {
    auto it_s = vertices.find(start_vertex);
    if (it_s != vertices.end()) {
        it_s->second.erase(end_vertex);
    }
    auto it_e = vertices.find(end_vertex);
    if (it_e != vertices.end()) {
        it_e->second.erase(start_vertex);
    }
}

void Graph::clear() {
    vertices.clear();
    points.clear();
}