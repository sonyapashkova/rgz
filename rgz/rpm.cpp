#include "rpm.h"

void read_config(int& n, int& radius) {
    ifstream config;
    config.open("config.txt");

    config >> n;
    config >> radius;
   
    config.close();
}

int random_coordinate(double low, double high) {
    random_device random_device;
    default_random_engine engine{ random_device() };
    uniform_int_distribution<int> distribution(low, high);

    return distribution(engine);
}

Point random_sample() {
    return Point(random_coordinate(0, 570), random_coordinate(0, 470));
}

vector<Point> near(const Graph& graph, const Point& point, int& radius) {
    vector<Point> points = graph.points;
    vector<Point> near_points;

    for (int i = 0; i < points.size(); ++i) {
        if (points[i].distance(point) <= radius && points[i] != point) {
            near_points.push_back(points[i]);
        }
    }

    return near_points;
}

bool collision_free(const Point& point1, const Point& point2, const vector<Circle>& obstacles) {
    for (int i = 0; i < obstacles.size(); ++i) {
        if (instersection_segment_circle(point1, point2, obstacles[i]) ||
            intersection_point_circle(obstacles[i], point1) ||
            intersection_point_circle(obstacles[i], point2)) {
            return false;
        }
    }

    return true;
}

vector<Point> rpm(Graph& graph, const Point& q_init, const Point& q_goal, const vector<Circle>& obstacles) {
    int n;
    int radius;

    read_config(n, radius);

    graph.add_vertex(q_init);
    graph.add_vertex(q_goal);

    vector<Point> points = graph.points;

    while (points.size() < n) {
        Point q_rand = random_sample();
        graph.add_vertex(q_rand);
        points = graph.points;
    }

    for (int i = 0; i < points.size(); ++i) {
        vector<Point> q_near = near(graph, points[i], radius);

        for (int j = 0; j < q_near.size(); ++j) {
            if (collision_free(points[i], q_near[j], obstacles)) {
                graph.add_edge(points[i], q_near[j], points[i].distance(q_near[j]));
            }
        }
    }

    return dijkstra(graph, q_init, q_goal);
}

vector<Point> dijkstra(const Graph& graph, const Point& q_init, const Point& q_goal) {
    if (graph.get_vertices().size() < 2)
        return vector<Point> {};

    vector<int> result_vertices;
    vector<Point> result_points;
    map<int, double> distance;
    vector<int> vertices = graph.get_vertices();
    int start_vertex = 0;
    int end_vertex = 0;
    map<int, int> parent;

    for (int i = 0; i < graph.points.size(); ++i) {
        if (graph.points[i] == q_init) {
            start_vertex = i;
        }
        if (graph.points[i] == q_goal) {
            end_vertex = i;
        }
    }

    for (int i = 0; i < vertices.size(); ++i) {
        distance.emplace(vertices[i], DBL_MAX);
        parent.emplace(vertices[i], -1);
    }

    distance[start_vertex] = 0;

    while (!vertices.empty()) {
        double min_distance = distance[vertices[0]];
        int min_distance_vertex = vertices[0];

        for (int i = 0; i < vertices.size(); ++i) {
            if (distance[vertices[i]] < min_distance) {
                min_distance = distance[vertices[i]];
                min_distance_vertex = vertices[i];
            }
        }

        vertices.erase(find(vertices.begin(), vertices.end(), min_distance_vertex));

        if (min_distance_vertex == end_vertex) {
            int vertex = end_vertex;

            while (vertex != start_vertex) {
                result_vertices.insert(result_vertices.begin(), vertex);
                vertex = parent[vertex];
                if (vertex == -1) {
                    return vector<Point> {};
                }
            }
            result_vertices.insert(result_vertices.begin(), start_vertex);

            for (int i = 0; i < result_vertices.size(); ++i) {
                result_points.push_back(graph.points[result_vertices[i]]);
            }

            return result_points;
        }

        vector<int> adjacent_vertices = graph.get_adjacent_vertices(min_distance_vertex);
        for (int i = 0; i < adjacent_vertices.size(); ++i) {
            if (distance[adjacent_vertices[i]] > distance[min_distance_vertex] + graph.edge_weight(min_distance_vertex, adjacent_vertices[i])) {
                distance[adjacent_vertices[i]] = distance[min_distance_vertex] + graph.edge_weight(min_distance_vertex, adjacent_vertices[i]);
                parent[adjacent_vertices[i]] = min_distance_vertex;
            }
        }
    }
}