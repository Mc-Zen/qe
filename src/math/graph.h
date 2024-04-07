
#pragma once
#include "matrix.h"
#include "binary.h"
#include <cstdint>


namespace qe {

	class Graph {
	public:
		using AdjacencyMatrix = qe::Matrix<Binary, qe::dynamic, qe::dynamic>;

		AdjacencyMatrix adjacency_matrix;



		explicit constexpr Graph(size_t num_vertices) : adjacency_matrix(num_vertices, num_vertices) {}
		explicit constexpr Graph(int num_vertices) : adjacency_matrix(num_vertices, num_vertices) {}


		constexpr const AdjacencyMatrix& get_adjacency_matrix() const { return adjacency_matrix; }
		constexpr int num_vertices() const { return adjacency_matrix.rows(); }

		constexpr static auto fully_connected(int n) { return Graph{ n }.fully_connect(); }
		constexpr static auto star(int n, int center = 0) { return Graph{ n }.make_star(center); }
		constexpr static auto linear(int n) { return Graph{ n }.make_linear(); }
		constexpr static auto cycle(int n) { return Graph{ n }.make_cycle(); }
		constexpr static auto pusteblume(int n) { return Graph{ n }.make_pusteblume(); }


		constexpr bool has_edge(int vertex1, int vertex2) const {
			return adjacency_matrix(vertex1, vertex2) == 1;
		}

		constexpr int edge_count() const {
			return std::accumulate(adjacency_matrix.begin(), adjacency_matrix.end(), 0, [](int count, Binary b) { return count + b.to_int(); }) / 2;
		}

		constexpr void add_edge(int vertex1, int vertex2) {
			if (vertex1 == vertex2) return;
			adjacency_matrix(vertex1, vertex2) = 1;
			adjacency_matrix(vertex2, vertex1) = 1;
		}

		constexpr void add_path(const std::initializer_list<int>& vertices) {
			if (vertices.size() < 2) return;
			int previousVertex = *vertices.begin();
			for (auto it = vertices.begin() + 1; it != vertices.end(); ++it) {
				add_edge(*it, previousVertex);
				previousVertex = *it;
			}
		}

		constexpr void remove_edge(int vertex1, int vertex2) {
			adjacency_matrix(vertex1, vertex2) = 0;
			adjacency_matrix(vertex2, vertex1) = 0;
		}

		constexpr void remove_edges_to(int vertex) {
			for (int i = 0; i < num_vertices(); ++i) remove_edge(i, vertex);
		}

		constexpr void toggle_edge(int vertex1, int vertex2) {
			adjacency_matrix(vertex1, vertex2).negate();
			adjacency_matrix(vertex2, vertex1).negate();
		}

		constexpr void local_complementation(int vertex) {
			qe::Matrix<Binary, qe::dynamic, qe::dynamic> ithColumn = adjacency_matrix.col(vertex);
			adjacency_matrix += ithColumn * ithColumn.transpose();
			for (int i = 0; i < num_vertices(); ++i) {
				adjacency_matrix(i, i) = 0;
			}
		}

		constexpr void swap(int vertex1, int vertex2) {
			qe::Matrix<Binary, qe::dynamic, qe::dynamic> col = adjacency_matrix.col(vertex1);
			adjacency_matrix.col(vertex1) = adjacency_matrix.col(vertex2);
			adjacency_matrix.col(vertex2) = col;
			qe::Matrix<Binary, qe::dynamic, qe::dynamic> row = adjacency_matrix.row(vertex1);
			adjacency_matrix.row(vertex1) = adjacency_matrix.row(vertex2);
			adjacency_matrix.row(vertex2) = row;
		}

		/// @brief Perform a series of local complementations
		/// @param vertices Local complementations will be executed for vertices in the given order
		constexpr void local_complementation(const std::initializer_list<int>& vertices) {
			std::for_each(vertices.begin(), vertices.end(), [this](int vertex) { local_complementation(vertex); });
		}


		/// @brief Perform a vertex permutation from {0,1,2,3,...} to mapping. 
		/// @param mapping Each number from 0 to num_vertices-1 needs to occur exactly once. 
		/// @return permuted graph
		constexpr Graph graph_isomorphism(const std::vector<int>& mapping) const {
			Graph result(num_vertices());
			for (int i = 0; i < num_vertices() - 1; ++i) {
				for (int j = i + 1; j < num_vertices(); ++j) {
					result.adjacency_matrix(mapping[i], mapping[j]) = adjacency_matrix(i, j);
				}
			}
			return result;
		}

		constexpr void clear() {
			adjacency_matrix.fill(0);
		}


		template<class Predicate>
		static constexpr Graph transform(const Graph& g1, const Graph& g2, Predicate predicate) {
			Graph result(g1.num_vertices());
			std::transform(g1.adjacency_matrix.begin(), g1.adjacency_matrix.end(), g2.adjacency_matrix.begin(),
				result.adjacency_matrix.begin(), predicate);
			return result;
		}

		template<class Predicate>
		constexpr void transform(const Graph& g, Predicate predicate) {
			Graph result(num_vertices());
			std::transform(adjacency_matrix.begin(), adjacency_matrix.end(), g.adjacency_matrix.begin(), adjacency_matrix.begin(), predicate);
		}

		static constexpr Graph add(const Graph& g1, const Graph& g2) {
			return Graph::transform(g1, g2, [](Binary b1, Binary b2) { return b1 | b2; });
		}

		static constexpr Graph intersect(const Graph& g1, const Graph& g2) {
			return Graph::transform(g1, g2, [](Binary b1, Binary b2) { return b1 & b2; });
		}

		/// @brief Subtract edges of g2 from g1
		static constexpr Graph subtract(const Graph& g1, const Graph& g2) {
			return Graph::transform(g1, g2, [](Binary b1, Binary b2) { return b1 * b2.negate(); });
		}

		/// @brief Add edges from other graph to this graph
		constexpr void add(const Graph& g) {
			transform(g, [](Binary b1, Binary b2) { return b1 | b2; });
		}

		/// @brief Form intersection of this graphs and the other graphs edges
		constexpr void intersect(const Graph& g) {
			transform(g, [](Binary b1, Binary b2) { return b1 & b2; });
		}

		/// @brief Remove all edges of this graph that occur in the other graph
		constexpr void subtract(const Graph& g) {
			transform(g, [](Binary b1, Binary b2) { return b1 * b2.negate(); });
		}

		/// @brief Get all edges in form of integer pairs
		constexpr auto get_edges() const {
			std::vector<std::pair<int, int>> edges;
			for (int i = 0; i < num_vertices() - 1; ++i) {
				for (int j = i + 1; j < num_vertices(); ++j) {
					if (has_edge(i, j)) edges.emplace_back(i, j);
				}
			}
			return edges;
		}

		constexpr friend bool operator==(const Graph& g1, const Graph& g2) = default;

		/// @brief Compress the graph into a single 64-bit integer. 
		static uint64_t compress(const Graph& graph);


		/// @brief Restore a graph from its compressed form. 
		static Graph decompress(int num_vertices, uint64_t code);

		/// @brief Get connected components of the graph in form of a vector of vector of vertex indices. 
		/// @param sort_by_size If true, the components are sorted by size (smallest to largest). 
		/// @return Connected components of the graph
		std::vector<std::vector<int>> connected_components(bool sort_by_size = false) const;



	private:

		static void decompress_impl(Graph& graph, int64_t code);

		constexpr Graph& fully_connect() {
			adjacency_matrix.fill(1);
			for (int i = 0; i < num_vertices(); ++i) adjacency_matrix(i, i) = 0;
			return *this;
		}

		constexpr Graph& make_star(int center) {
			std::for_each(adjacency_matrix.col_begin(center), adjacency_matrix.col_end(center), [](Binary& el) { el.negate(); });
			std::for_each(adjacency_matrix.row_begin(center), adjacency_matrix.row_end(center), [](Binary& el) { el.negate(); });
			return *this;
		}

		constexpr Graph& make_linear() {
			for (int i = 0; i < num_vertices() - 1; ++i) {
				add_edge(i, i + 1);
			}
			return *this;
		}

		constexpr Graph& make_cycle() {
			make_linear();
			add_edge(0, num_vertices() - 1);
			return *this;
		}

		constexpr Graph& make_pusteblume() {
			assert(num_vertices() >= 5 && "The Pusteblume graph is only possible for at least 5 vertices");
			for (int i = 1; i < 4; ++i) {
				add_edge(0, i);
			}
			for (int i = 4; i < num_vertices(); ++i) {
				add_edge(3, i);
			}
			return *this;
		}
	};

	void print_graph(const Graph& graph);


	/// @brief Generate all subgraphs of given graph that have at least [min_edges] edges and at most [max_edges] edges
	std::vector<Graph> generate_subgraphs(const Graph& graph, int min_edges, int max_edges);

	auto generate_subgraphs(const Graph& graph, int max_edges = std::numeric_limits<int>::max());


}
