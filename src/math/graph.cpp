#include "graph.h"
#include <bit>
#include <iostream>

using namespace qe;

qe::Graph::Graph(int num_vertices, const std::initializer_list<std::pair<int, int>>& edges)
	: Graph(num_vertices) {
	std::for_each(edges.begin(), edges.end(), [this](const auto& edge) {
		add_edge(edge.first, edge.second);
	});
}

void qe::print_graph(const Graph& graph) {
	using std::cout;
	const auto& g = graph.get_adjacency_matrix();
	auto c = [&](int v1, int v2, char symb) { return g(v1, v2) ? symb : ' '; };
	switch (graph.num_vertices()) {
	case 3:
		cout << "o" << (g(0, 1) ? "--" : "  ") << "o\n";
		cout << ' ' << c(0, 2, '\\') << ' ' << c(1, 2, '|') << '\n';
		cout << ' ' << ' ' << c(0, 2, '\\') << c(1, 2, '|') << '\n';
		cout << "    o\n";
		break;
	case 4:
		cout << "o" << (g(0, 1) ? "--" : "  ") << "o\n";
		cout << c(0, 3, '|') << c(0, 2, '\\') << c(1, 3, '/') << c(1, 2, '|') << '\n';
		cout << c(0, 3, '|') << c(1, 3, '/') << c(0, 2, '\\') << c(1, 2, '|') << '\n';
		cout << "o" << (g(2, 3) ? "--" : "  ") << "o\n";
		break;
	case 5:
		cout << "   o" << (g(0, 1) ? "---" : "   ") << "o\n  ";
		cout << ' ' << (g(0, 4) ? '|' : g(0, 3) ? '\\' : ' ') << ' ';
		cout << (g(0, 2) ? '.' : ' ') << ' ' << (g(1, 3) ? '|' : c(1, 4, '/') ? '/' : ' ');
		cout << (g(1, 2) ? '\\' : ' ') << '\n' << ' ';
		cout << ' ' << ' ' << (g(0, 4) ? '|' : ' ') << (g(0, 3) ? '\\' : ' ') << ' ';
		cout << (g(1, 4) ? '/' : ' ') << (g(1, 3) ? '|' : ' ') << (g(0, 2) ? '.' : ' ');
		cout << (g(1, 2) ? '\\' : ' ') << '\n';
		cout << ' ' << ' ' << ' ' << (g(0, 4) ? '|' : ' ') << ' ';
		cout << (g(0, 3) && g(1, 4) ? 'X' : (g(0, 3) ? '\\' : (g(1, 4) ? '/' : ' ')));
		cout << ' ' << (g(1, 3) ? '|' : ' ') << ' ' << ' ' << "o\n ";
		cout << ' ' << ' ' << (g(0, 4) ? '|' : ' ') << (g(1, 4) ? '/' : ' ') << ' ';
		cout << (g(0, 3) ? '\\' : ' ') << (g(1, 3) ? '|' : ' ') << (g(2, 4) ? '.' : ' ');
		cout << (g(2, 3) ? '/' : ' ') << '\n' << ' ' << ' ';
		cout << ' ' << (g(0, 4) ? '|' : g(1, 4) ? '/' : ' ') << ' ' << (g(2, 4) ? '.' : ' ');
		cout << ' ' << (g(1, 3) ? '|' : g(0, 3) ? '\\' : ' ') << (g(2, 3) ? '/' : ' ') << '\n';
		cout << "   o" << (g(3, 4) ? "---" : "   ") << "o\n";
	case 6:
		cout << "   o" << (g(0, 1) ? "---" : "   ") << "o\n  ";
		cout << (g(0, 5) ? '/' : ' ') << (g(0, 4) ? '|' : g(0, 3) ? '\\' : ' ') << ' ';
		cout << (g(0, 2) || g(1, 5) ? '.' : ' ') << ' ' << (g(1, 3) ? '|' : g(1, 4) ? '/' : ' ');
		cout << (g(1, 2) ? '\\' : ' ') << '\n' << ' ';

		cout << (g(0, 5) ? '/' : ' ') << (g(1, 5) ? '.' : ' ') << (g(0, 4) ? '|' : ' ');
		cout << (g(0, 3) ? '\\' : ' ') << ' ' << (g(1, 4) ? '/' : ' ') << (g(1, 3) ? '|' : ' ');
		cout << (g(0, 2) ? '.' : ' ') << (g(1, 2) ? '\\' : ' ') << '\n';
		cout << 'o' << (g(2, 5) ? '-' : ' ') << (g(2, 5) ? '-' : ' ');
		cout << (g(0, 4) ? '|' : g(2, 5) ? '-' : ' ') << (g(2, 5) ? '-' : ' ');
		cout << (g(0, 3) && g(1, 4) ? 'X'
									: (g(0, 3) ? '\\' : (g(1, 4) ? '/' : (g(2, 5) ? '-' : ' '))));
		cout << (g(2, 5) ? '-' : ' ') << (g(1, 3) ? '|' : g(2, 5) ? '-' : ' ');
		cout << (g(2, 5) ? '-' : ' ') << (g(2, 5) ? '-' : ' ') << "o\n ";
		cout << (g(4, 5) ? '\\' : ' ') << (g(3, 5) ? '.' : ' ') << (g(0, 4) ? '|' : ' ');
		cout << (g(1, 4) ? '/' : ' ') << ' ' << (g(0, 3) ? '\\' : ' ') << (g(1, 3) ? '|' : ' ');
		cout << (g(2, 4) ? '.' : ' ') << (g(2, 3) ? '/' : ' ') << '\n' << ' ' << ' ';
		cout << (g(4, 5) ? '\\' : ' ') << (g(0, 4) ? '|' : g(1, 4) ? '/' : ' ') << ' ';
		cout << (g(2, 4) || g(3, 5) ? '.' : ' ') << ' ' << (g(1, 3) ? '|' : g(0, 3) ? '\\' : ' ');
		cout << (g(2, 3) ? '/' : ' ') << '\n';
		cout << "   o" << (g(3, 4) ? "---" : "   ") << "o\n";
		break;
	default: cout << "Cannot print graph of this size";
	}
}


uint64_t qe::Graph::compress(const Graph& graph) {
	assert(graph.num_vertices() * (graph.num_vertices() - 1) / 2 <= 64 &&
		   "Compression is not supported for graphs of this size");
	uint64_t code{};
	int index{};
	for (int i = 0; i < graph.num_vertices() - 1; ++i) {
		for (int j = i + 1; j < graph.num_vertices(); ++j) {
			if (graph.has_edge(i, j)) code |= (1ULL << index);
			++index;
		}
	}
	return code;
}


Graph qe::Graph::decompress(int num_vertices, uint64_t code) {
	assert(num_vertices * (num_vertices - 1) / 2 <= 64 &&
		   "Decompression is not supported for graphs of this size");
	Graph graph(num_vertices);
	decompress_impl(graph, code);
	return graph;
}

std::vector<std::vector<int>> qe::Graph::connected_components(bool sort_by_size) const {
	std::vector<std::vector<int>> components;

	auto copy = adjacency_matrix;
	auto edges = get_edges();
	std::vector<int> remaining_vertices(num_vertices());
	std::vector<int> visited(num_vertices());
	std::iota(remaining_vertices.begin(), remaining_vertices.end(), 0);

	for (int i = 0; i < num_vertices(); ++i) {
		if (visited[i]) continue;
		visited[i] = 1;
		std::vector<int> component{ i };
		std::vector<int> queue{ i };
		while (!queue.empty()) {
			auto vertex = queue.back();
			queue.pop_back();
			for (int k = 0; k < num_vertices(); ++k) {
				if (has_edge(vertex, k) && visited[k] == 0) {
					queue.push_back(k);
					component.push_back(k);
					visited[k] = 1;
				}
			}
		}
		components.emplace_back(std::move(component));
	}
	if (sort_by_size) {
		// std::ranges::sort(components, std::less{}, &std::vector<int>::size);
		std::sort(components.begin(), components.end(), [](const auto& a, const auto& b) {
			return a.size() < b.size();
		});
	}
	return components;
}

void qe::Graph::decompress_impl(Graph& graph, int64_t code) {
	int index{};
	for (int i = 0; i < graph.num_vertices() - 1; ++i) {
		for (int j = i + 1; j < graph.num_vertices(); ++j) {
			if (code & (1ULL << index)) graph.add_edge(i, j);
			++index;
		}
	}
}


std::vector<Graph> qe::generate_subgraphs(const Graph& graph, int min_edges, int max_edges) {
	std::vector<std::pair<size_t, size_t>> edges;
	std::vector<Graph> subgraphs;
	for (int i = 0; i < graph.num_vertices(); ++i) {
		for (int j = i + 1; j < graph.num_vertices(); ++j) {
			if (graph.adjacency_matrix(i, j) == 1) edges.push_back({ i, j });
		}
	}
	assert(edges.size() < 64 && "this algorithm only works with less than 64 edges");
	const auto end = 1 << edges.size();
	for (size_t i = 0; i < end; ++i) {
		if (const auto edge_count = std::popcount(i);
			edge_count < min_edges || edge_count > max_edges)
			continue;

		Graph subgraph(graph.num_vertices());
		for (size_t j = 0; j < edges.size(); ++j) {
			if (i & (1ULL << j)) {
				subgraph.adjacency_matrix(edges[j].first, edges[j].second) = 1;
				subgraph.adjacency_matrix(edges[j].second, edges[j].first) = 1;
			}
		}
		subgraphs.push_back(subgraph);
	}
	return subgraphs;
}

std::vector<Graph> qe::generate_subgraphs(const Graph& graph, int max_edges) {
	return generate_subgraphs(graph, 0, max_edges);
}
