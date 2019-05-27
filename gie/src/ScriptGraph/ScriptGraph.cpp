//
// Created by alex on 2/16/19.
//

#include <gie/ScriptGraph/ScriptGraph.h>

#include <optional>
#include <algorithm>
#include <functional>
#include <type_traits>

static long lookup(const ScriptGraph& graph, NodeId id)
{
    auto it = std::lower_bound(graph.nodes.begin(), graph.nodes.end(), id, [](auto node, auto id)
    {
        return node.second < id;
    });

    return it == graph.nodes.end() ? -1 : std::distance(graph.nodes.begin(), it);
}

NodeCachePair getNode(ScriptGraph& graph, NodeId id)
{
    auto r = lookup(graph, id);
    return {graph.nodes[r].first, graph.cache[r].first};
}

ConstNodeCachePair getNode(const ScriptGraph& graph, NodeId id)
{
    auto r = lookup(graph, id);
    return {graph.nodes[r].first, graph.cache[r].first};
}

NodeId addNode(ScriptGraph& graph, const Node& node)
{
    NodeId id = graph.nodes.empty() ? 0 : graph.nodes.back().second + 1;

    graph.nodes.emplace_back(node, id);
    graph.cache.emplace_back(std::nullopt, id);

    return id;
}

void editNode([[maybe_unused]]ScriptGraph& graph, [[maybe_unused]]NodeId id, [[maybe_unused]]const Node& newNode)
{
    auto r = lookup(graph, id);

    if(r != -1)
    {
        Node& node = graph.nodes[r].first;
        node = newNode;
    }
}

void removeNode([[maybe_unused]]ScriptGraph& graph, [[maybe_unused]]NodeId id)//BUG: boost doesn't delete the vertices
{
    auto r = lookup(graph, id);
    graph.nodes.erase(graph.nodes.begin() + r);
    graph.cache.erase(graph.cache.begin() + r);
}

void addResult(ScriptGraph& graph, std::string tag, NodeId id)
{
    graph.results.emplace_back(tag, id);
}
void editResult(ScriptGraph& graph, std::string tag, NodeId id)
{
    auto it = std::find_if(graph.results.begin(), graph.results.end(), [&tag](auto x) { return x.first == tag; });
    if(it != graph.results.end())
        it->second = id;
}

void removeResult(ScriptGraph& graph, std::string tag)
{
    graph.results.erase(
            std::remove_if(graph.results.begin(), graph.results.end(), [&tag](auto x) { return x.first == tag; }),
            graph.results.end()
    );
}