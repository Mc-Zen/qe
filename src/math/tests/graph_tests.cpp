#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"

#include "graph.h"


using namespace qe;


TEST_CASE("Graph") {
	auto g = Graph<3>::fully_connected();
	REQUIRE(g.adjacency_matrix == Graph<3>::AdjacencyMatrix{ 0,1,1,1,0,1,1,1,0 });
	REQUIRE(Graph<3>::star(1).adjacency_matrix == Graph<3>::AdjacencyMatrix{ 0,1,0,1,0,1,0,1,0 });
	REQUIRE(Graph<3>::star(2).adjacency_matrix == Graph<3>::AdjacencyMatrix{ 0,0,1,0,0,1,1,1,0 });
}



TEST_CASE("Compress/Decompress") {
	REQUIRE(Graph<9>::decompress(Graph<9>::compress(Graph<9>::star())) == Graph<9>::star());
	REQUIRE(Graph<9>::decompress(Graph<9>::compress(Graph<9>::star(4))) == Graph<9>::star(4));
	REQUIRE(Graph<9>::decompress(Graph<9>::compress(Graph<9>::star(6))) == Graph<9>::star(6));
	REQUIRE(Graph<9>::decompress(Graph<9>::compress(Graph<9>::pusteblume())) == Graph<9>::pusteblume());
	REQUIRE(Graph<9>::decompress(Graph<9>::compress(Graph<9>::fully_connected())) == Graph<9>::fully_connected());
	REQUIRE(Graph<9>::decompress(Graph<9>::compress(Graph<9>::linear())) == Graph<9>::linear());

	REQUIRE(Graph<>::decompress(9, Graph<>::compress(Graph<>::star(9))) == Graph<>::star(9));
	REQUIRE(Graph<>::decompress(9, Graph<>::compress(Graph<>::star(9, 4))) == Graph<>::star(9, 4));
	REQUIRE(Graph<>::decompress(9, Graph<>::compress(Graph<>::star(9, 6))) == Graph<>::star(9, 6));
	REQUIRE(Graph<>::decompress(9, Graph<>::compress(Graph<>::pusteblume(9))) == Graph<>::pusteblume(9));
	REQUIRE(Graph<>::decompress(9, Graph<>::compress(Graph<>::fully_connected(9))) == Graph<>::fully_connected(9));
	REQUIRE(Graph<>::decompress(9, Graph<>::compress(Graph<>::linear(9))) == Graph<>::linear(9));
}

TEST_CASE("Connected components") {
	Graph<> graph(8);
	graph.add_path({ 2,3,5 });
	graph.add_path({ 0,7 });
	REQUIRE(graph.connectedComponents(true) == std::vector<std::vector<int>>{ { {1}, { 4 }, { 6 }, { 0,7 }, { 2,3,5 }}});

	graph = Graph<>{ 8 };
	REQUIRE(graph.connectedComponents(true) == std::vector<std::vector<int>>{ { std::vector<int>{0}, { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }}});

	graph = Graph<>::star(8);
	REQUIRE(graph.connectedComponents(true) == std::vector<std::vector<int>>{ { {0, 1, 2, 3, 4, 5, 6, 7}}});
}