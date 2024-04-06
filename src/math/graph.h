
#pragma once
#include "matrix.h"
#include "binary.h"


namespace qe {


	template<bool empty = true>
	struct GraphSize {
		constexpr friend bool operator==(const GraphSize& a, const GraphSize& b) = default;
	};


	template<>
	struct GraphSize<false> {
		int n{};
		constexpr friend bool operator==(const GraphSize& a, const GraphSize& b) = default;
	};



	template<size_t n = qe::dynamic>
	class Graph {
	public:
		using AdjacencyMatrix = qe::Matrix<Binary, n, n>;
		static constexpr bool is_dynamic = (n == qe::dynamic);
		GraphSize<!is_dynamic> graph_size;


		AdjacencyMatrix adjacency_matrix;


		constexpr Graph() requires !is_dynamic = default;

		explicit constexpr Graph(GraphSize<!is_dynamic> graph_size) : graph_size(graph_size) {
			if constexpr (is_dynamic) {
				adjacency_matrix = AdjacencyMatrix(num_vertices(), num_vertices());
			}
		}

		explicit constexpr Graph(int num_vertices) requires is_dynamic : graph_size{ num_vertices }, adjacency_matrix(num_vertices, num_vertices) {}


		constexpr int num_vertices() const {
			if constexpr (is_dynamic) return graph_size.n;
			else return n;
		}

		constexpr static auto fully_connected() requires (!is_dynamic) { return Graph{}.fully_connect(); }
		constexpr static auto fully_connected(int n) requires (is_dynamic) { return Graph{ n }.fully_connect(); }

		constexpr static auto star(int center = 0) requires (!is_dynamic) { return Graph{}.make_star(center); }
		constexpr static auto star(int n, int center = 0) requires (is_dynamic) { return Graph{ n }.make_star(center); }

		constexpr static auto linear() requires (!is_dynamic) { return Graph{}.make_linear(); }
		constexpr static auto linear(int n) requires (is_dynamic) { return Graph{ n }.make_linear(); }

		constexpr static auto cycle() requires (!is_dynamic) { return Graph{}.make_cycle(); }
		constexpr static auto cycle(int n) requires (is_dynamic) { return Graph{ n }.make_cycle(); }

		constexpr static auto pusteblume() requires (!is_dynamic) { return Graph{}.make_pusteblume(); }
		constexpr static auto pusteblume(int n) requires (is_dynamic) { return Graph{ n }.make_pusteblume(); }

		constexpr const AdjacencyMatrix& get_adjacency_matrix() const { return adjacency_matrix; }

		constexpr bool has_edge(int vertex1, int vertex2) const {
			return adjacency_matrix(vertex1, vertex2) == 1;
		}

		constexpr int edge_count() const {
			return std::accumulate(adjacency_matrix.begin(), adjacency_matrix.end(), 0, [](int count, Binary b) { return count + b.toInt(); }) / 2;
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
			qe::Vector<Binary, n> ithColumn = adjacency_matrix.col(vertex);
			adjacency_matrix += ithColumn * ithColumn.transpose();
			for (int i = 0; i < num_vertices(); ++i) {
				adjacency_matrix(i, i) = 0;
			}
		}

		constexpr void swap(int vertex1, int vertex2) {
			qe::Vector<Binary, n> col = adjacency_matrix.col(vertex1);
			adjacency_matrix.col(vertex1) = adjacency_matrix.col(vertex2);
			adjacency_matrix.col(vertex2) = col;
			qe::RowVector<Binary, n> row = adjacency_matrix.row(vertex1);
			adjacency_matrix.row(vertex1) = adjacency_matrix.row(vertex2);
			adjacency_matrix.row(vertex2) = row;
		}

		/// @brief Perform a series of local complementations
		/// @param vertices Local complementations will be executed for vertices in the given order
		constexpr void local_complementation(const std::initializer_list<int>& vertices) {
			std::ranges::for_each(vertices, [this](int vertex) { local_complementation(vertex); });
		}


		/// @brief Perform a vertex permutation from {0,1,2,3,...} to mapping. 
		/// @param mapping Each number from 0 to num_vertices-1 needs to occur exactly once. 
		/// @return permuted graph
		constexpr Graph graph_isomorphism(const std::vector<int>& mapping) const {
			Graph result;
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
			Graph result;
			std::transform(g1.adjacency_matrix.begin(), g1.adjacency_matrix.end(), g2.adjacency_matrix.begin(),
				result.adjacency_matrix.begin(), predicate);
			return result;
		}

		template<class Predicate>
		constexpr void transform(const Graph& g, Predicate predicate) {
			Graph result;
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
		static constexpr uint64_t compress(const Graph& graph) {
			static_assert(n * (n - 1) / 2 <= 64 || n != qe::dynamic, "Compression is not supported for graphs of this size");
			assert(graph.num_vertices() * (graph.num_vertices() - 1) / 2 <= 64 && "Compression is not supported for graphs of this size");
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

		/// @brief Restore a graph from its compressed form. 
		static constexpr Graph decompress(uint64_t code) requires(!is_dynamic) {
			static_assert(n * (n - 1) / 2 <= 64, "Deompression is not supported for graphs of this size");
			Graph graph;
			decompress_impl(graph, code);
			return graph;
		}

		/// @brief Restore a graph from its compressed form. 
		static constexpr Graph decompress(int num_vertices, uint64_t code) requires(is_dynamic) {
			assert(num_vertices * (num_vertices - 1) / 2 <= 64 && "Decompression is not supported for graphs of this size");
			Graph graph(num_vertices);
			decompress_impl(graph, code);
			return graph;
		}

		/// @brief Get connected components of the graph in form of a vector of vector of vertex indices. 
		/// @param sort_by_size If true, the components are sorted by size (smallest to largest). 
		/// @return Connected components of the graph
		std::vector<std::vector<int>> connectedComponents(bool sort_by_size = false) const {
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
				std::ranges::sort(components, std::less{}, &std::vector<int>::size);
			}
			return components;
		}



	private:

		static constexpr void decompress_impl(Graph& graph, int64_t code) {
			int index{};
			for (int i = 0; i < graph.num_vertices() - 1; ++i) {
				for (int j = i + 1; j < graph.num_vertices(); ++j) {
					if (code & (1ULL << index)) graph.add_edge(i, j);
					++index;
				}
			}
		}

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
			static_assert(n >= 5 || n == qe::dynamic, "The Pusteblume graph is only possible for at least 5 vertices");
			assert(num_vertices() >= 5 && "The Pusteblume graph is only possible for at least 5 vertices");
			for (size_t i = 1; i < 4; ++i) {
				add_edge(0, i);
			}
			for (size_t i = 4; i < num_vertices(); ++i) {
				add_edge(3, i);
			}
			return *this;
		}
	};


	//     0 o--o 1
	//        \ |
	//         \|
	//          o 2
	void print_graph(const Graph<3>& graph);


	//     0 o--o 1
	//       |\/|
	//       |/\|
	//     3 o--o 2
	void print_graph(const Graph<4>& graph);

	//          0     1
	//           o---o     
	//           |   |\
	//           |\ /|.\
	//           | X |  o 2
	//           |/ \|./
	//           | . |/	
	//           o---o	
	//          4     3
	void print_graph(const Graph<5>& graph);
	//          0     1
	//           o---o             o---o            o---o
	//          /| . |\		      /| .		        |   /\
	//         /.|\ /|.\	     / |\   .	        |  /  \
	//      5 o--|-X-|--o 2     o  | \    o	     o  | /    o
	//         \.|/ \|./	       |  \		      \ |/    /
	//          \| . |/		       |   \	       \|    /
	//           o---o		       o   o	        o---o
	//          4     3
	void print_graph(const Graph<6>& graph);

	void print_graph(const Graph<qe::dynamic>& graph);


	/// @brief Generate all subgraphs of given graph that have at least [minEdges] edges and at most [maxEdges] edges
	template<int n>
	std::vector<Graph<n>> generate_subgraphs(const Graph<n>& graph, int minEdges, int maxEdges) {
		std::vector<std::pair<size_t, size_t>> edges;
		std::vector<Graph<n>> subgraphs;
		for (int i = 0; i < graph.num_vertices(); ++i) {
			for (int j = i + 1; j < graph.num_vertices(); ++j) {
				if (graph.adjacency_matrix(i, j) == 1) edges.push_back({ i,j });
			}
		}
		assert(edges.size() < 64 && "this algorithm only works with less than 64 edges");
		const auto end = 1 << edges.size();
		for (size_t i = 0; i < end; ++i) {
			if (const auto edge_count = std::popcount(i); edge_count < minEdges || edge_count > maxEdges) continue;

			Graph<n> subgraph(graph.graph_size);
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

	template<int n>
	auto generate_subgraphs(const Graph<n>& graph, int maxEdges = std::numeric_limits<int>::max()) {
		return generate_subgraphs(graph, 0, maxEdges);
	}


}
