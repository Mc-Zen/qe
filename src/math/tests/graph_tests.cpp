#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"

#include "graph.h"


using namespace qe;


TEST_CASE("Graph constructor") {
	Graph graph(5);
	REQUIRE(graph.num_vertices() == 5);
	REQUIRE(graph.edge_count() == 0);

	Graph graph2{ 8, { { 1, 4 }, { 0, 7 } } };
	REQUIRE(graph2.num_vertices() == 8);
	REQUIRE(graph2.edge_count() == 2);
	REQUIRE(graph2.has_edge(1, 4));
	REQUIRE(graph2.has_edge(0, 7));
}

TEST_CASE("Graph adjacencyMatrix()") {
	auto g = Graph::fully_connected(3);
	REQUIRE(g.get_adjacency_matrix() == Graph::AdjacencyMatrix{ 3, 3, { 0, 1, 1, 1, 0, 1, 1, 1, 0 } });
	REQUIRE(Graph::linear(3).adjacency_matrix == Graph::AdjacencyMatrix{ 3, 3, { 0, 1, 0, 1, 0, 1, 0, 1, 0 } });
	REQUIRE(Graph::star(3, 2).adjacency_matrix == Graph::AdjacencyMatrix{ 3, 3, { 0, 0, 1, 0, 0, 1, 1, 1, 0 } });
}

TEST_CASE("Graph manipulate edges") {
	Graph graph(5);
	REQUIRE(!graph.has_edge(2, 3));

	graph.add_edge(2, 3);
	REQUIRE(graph.has_edge(2, 3));
	REQUIRE(graph.edge_count() == 1);

	graph.add_path({ 1, 4, 0 });
	REQUIRE(graph.has_edge(4, 1));
	REQUIRE(graph.has_edge(4, 0));
	REQUIRE(graph.edge_count() == 3);

	graph.remove_edge(2, 3);
	REQUIRE(!graph.has_edge(2, 3));
	REQUIRE(graph.edge_count() == 2);

	graph.toggle_edge(2, 3);
	REQUIRE(graph.has_edge(2, 3));

	graph.toggle_edge(2, 3);
	REQUIRE(!graph.has_edge(2, 3));

	graph.remove_edges_to(4);
	REQUIRE(graph.edge_count() == 0);

	graph.add_path({ 1, 2, 3, 0, 3 });
	graph.clear();
	REQUIRE(graph.edge_count() == 0);
}

TEST_CASE("Graph swap vertices") {
	auto graph = Graph::star(4, 0);
	graph.swap(0, 1);
	REQUIRE(!graph.has_edge(0, 3));
	REQUIRE(graph.has_edge(0, 1));
	REQUIRE(graph.has_edge(1, 3));
	REQUIRE(graph.has_edge(1, 2));
}

TEST_CASE("Graph boolean operations") {
	using EdgeList = std::vector<std::pair<int, int>>;
	auto graph1 = Graph::star(4, 0);
	auto graph2 = Graph::linear(4);
	auto graph3 = Graph::add(graph1, graph2);
	REQUIRE(graph3.get_edges() == EdgeList{ { 0, 1 }, { 0, 2 }, { 0, 3 }, { 1, 2 }, { 2, 3 } });
}

TEST_CASE("Graph get_edges()") {
	using EdgeList = std::vector<std::pair<int, int>>;
	auto graph = Graph::star(4, 0);
	REQUIRE(graph.get_edges() == EdgeList{ { 0, 1 }, { 0, 2 }, { 0, 3 } });
}

TEST_CASE("Graph operator=") {
	REQUIRE(Graph::linear(3) == Graph::linear(3));
	REQUIRE(Graph::linear(3) != Graph::linear(4));
	REQUIRE(Graph::linear(3) != Graph::star(3));
}


TEST_CASE("Graph connected_components()") {
	using VertexGroups = std::vector<std::vector<int>>;
	REQUIRE(Graph::linear(3).connected_components() == VertexGroups{ { 0, 1, 2 } });

	Graph graph(6);
	graph.add_path({ 1, 2, 4, 3, 2, 4, 1 });
	REQUIRE(graph.connected_components() == VertexGroups{ { 0 }, { 1, 2, 4, 3 }, { 5 } });
	REQUIRE(graph.connected_components(true) == VertexGroups{ { 0 }, { 5 }, { 1, 2, 4, 3 } });
}


TEST_CASE("Compress/Decompress") {
	REQUIRE(Graph::decompress(3, Graph::compress(Graph::star(3))) == Graph::star(3));
	REQUIRE(Graph::decompress(4, Graph::compress(Graph::star(4))) == Graph::star(4));
	REQUIRE(Graph::decompress(6, Graph::compress(Graph::star(6))) == Graph::star(6));
	REQUIRE(Graph::decompress(5, Graph::compress(Graph::pusteblume(5))) == Graph::pusteblume(5));
	REQUIRE(Graph::decompress(7, Graph::compress(Graph::fully_connected(7))) == Graph::fully_connected(7));
	REQUIRE(Graph::decompress(9, Graph::compress(Graph::linear(9))) == Graph::linear(9));

	REQUIRE(Graph::decompress(9, Graph::compress(Graph::star(9))) == Graph::star(9));
	REQUIRE(Graph::decompress(9, Graph::compress(Graph::star(9, 4))) == Graph::star(9, 4));
	REQUIRE(Graph::decompress(9, Graph::compress(Graph::star(9, 6))) == Graph::star(9, 6));
	REQUIRE(Graph::decompress(9, Graph::compress(Graph::pusteblume(9))) == Graph::pusteblume(9));
	REQUIRE(Graph::decompress(9, Graph::compress(Graph::fully_connected(9))) == Graph::fully_connected(9));
	REQUIRE(Graph::decompress(9, Graph::compress(Graph::linear(9))) == Graph::linear(9));
}

TEST_CASE("Connected components") {
	Graph graph(8);
	graph.add_path({ 2, 3, 5 });
	graph.add_path({ 0, 7 });
	REQUIRE(graph.connected_components(true) == std::vector<std::vector<int>>{ { { 1 }, { 4 }, { 6 }, { 0, 7 }, { 2, 3, 5 } } });

	graph = Graph{ 8 };
	REQUIRE(graph.connected_components(true) == std::vector<std::vector<int>>{ { std::vector<int>{ 0 }, { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 } } });

	graph = Graph::star(8);
	REQUIRE(graph.connected_components(true) == std::vector<std::vector<int>>{ { { 0, 1, 2, 3, 4, 5, 6, 7 } } });
}

TEST_CASE("generate_subgraphs()") {
	auto graph = Graph::linear(3);
	auto subgraphs = generate_subgraphs(graph);
	REQUIRE(subgraphs.size() == 4);
	auto g1 = Graph(3);
	auto g2 = Graph(3, { { 0, 1 } });
	auto g3 = Graph(3, { { 1, 2 } });
	auto g4 = Graph(3, { { 0, 1 }, { 1, 2 } });

	auto is_in_list = [&](const Graph& graph) { return std::find(subgraphs.begin(), subgraphs.end(), graph) != subgraphs.end(); };

	REQUIRE(is_in_list(g1));
	REQUIRE(is_in_list(g2));
	REQUIRE(is_in_list(g3));
	REQUIRE(is_in_list(g4));
}