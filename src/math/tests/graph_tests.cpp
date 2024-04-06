#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"

#include "graph.h"


using namespace qe;


TEST_CASE("Graph") {
	auto g = Graph::fully_connected(3);
	REQUIRE(g.get_adjacency_matrix() == Graph::AdjacencyMatrix{ 3, 3, {0,1,1,1,0,1,1,1,0} });
	REQUIRE(Graph::linear(3).adjacency_matrix == Graph::AdjacencyMatrix{ 3, 3, { 0,1,0,1,0,1,0,1,0 } });
	REQUIRE(Graph::star(3, 2).adjacency_matrix == Graph::AdjacencyMatrix{ 3, 3, { 0,0,1,0,0,1,1,1,0 } });
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
	graph.add_path({ 2,3,5 });
	graph.add_path({ 0,7 });
	REQUIRE(graph.connected_components(true) == std::vector<std::vector<int>>{ { {1}, { 4 }, { 6 }, { 0,7 }, { 2,3,5 }}});

	graph = Graph{ 8 };
	REQUIRE(graph.connected_components(true) == std::vector<std::vector<int>>{ { std::vector<int>{0}, { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }}});

	graph = Graph::star(8);
	REQUIRE(graph.connected_components(true) == std::vector<std::vector<int>>{ { {0, 1, 2, 3, 4, 5, 6, 7}}});
}