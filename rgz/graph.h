#pragma once
#ifndef H_GRAPH
#define H_GRAPH
#include <map>
#include <vector>
#include <random>

using namespace std;

class Point {
public:
	Point() {}
	Point(int xx, int yy) : x(xx), y(yy) {}

	double distance(const Point& p) const;

	bool operator==(const Point& p) const;
	bool operator!=(const Point& p) const;
	bool operator<(const Point& p) const;
	bool operator>(const Point& p) const;
	bool operator<=(const Point& p) const;
	bool operator>=(const Point& p) const;

public:
	int x{ 0 };
	int y{ 0 };
};

std::ostream& operator<<(std::ostream& os, const Point& p);
double dot(const Point& v1, const Point& v2);
Point closest(const Point& a, const Point& b, const Point& p);


class Graph {
public:
	Graph() {}

	int add_vertex(const Point& p);
	void add_edge(const Point& p1, const Point& p2, double weight = 1.0);
	std::vector<int> get_vertices() const;
	std::vector<int> get_adjacent_vertices(int src_vertex) const;
	std::vector<std::pair<int, double>> get_adjacent_edges(int src_vertex) const;
	bool has_vertex(const Point& p) const;
	bool has_edge(const Point& p1, const Point& p2) const;
	double edge_weight(int start_vertex, int end_vertex) const;
	void remove_vertex(const Point& p);
	void remove_edge(int start_vertex, int end_vertex);
	void clear();
	std::vector<Point> points;

private:
	std::map<int, std::map<int, double>> vertices;
};

class Circle {
public:
	Circle() {}
	Circle(Point pp, int rr) : c(pp), r(rr) {};
	Point c = { Point(0, 0) };
	int r{ 0 };
};

bool intersection_point_circle(const Circle& c, const Point& p);
bool instersection_segment_circle(const Point& a, const Point& b, const Circle& c);

#endif 