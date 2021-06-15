include("Primitives.jl")
using .Primitives

using LinearAlgebra, StaticArrays, Printf

mutable struct treeNode
    vertex_::Vertex
    leftChild_::Union{treeNode, Nothing}
    rightChild_::Union{treeNode, Nothing}

    function treeNode(vertex::Vertex)
        this = new()
        this.vertex_ = vertex
        this.leftChild_ = nothing
        this.rightChild_ = nothing
        return this
    end
end

mutable struct KDTree
    rootNode_::treeNode
    nodes_::Vector{treeNode}

    function KDTree(vertice::Vector{Vertex})
        this = new()

        this.nodes_= treeNode[]
        for i in eachindex(vertice)
            push!(this.nodes_, treeNode(vertice[i]))
        end

        return this
    end
end

function recurBuildTree(tree::KDTree, beginIndex::Int64, endIndex::Int64,
                        splitDim::Int64)::Union{treeNode, Nothing}

    if (endIndex <= beginIndex)
        return nothing
    end

    localMidIndex = (endIndex - beginIndex)/2;
    midIndex = beginIndex + localMidIndex;
    splitDim = mod1((splitDim + 1), 3)
    
    @views partialsort!(tree.nodes_[beginIndex:endIndex], localMidIndex,
                        lt=(x, y)->isless(x.vertex_[splitDim], y.vertex_[splitDim]))

    tree.nodes_[midIndex].leftChild_ = recurBuildTree(tree, beginIndex, midIndex, splitDim)
    tree.nodes_[midIndex].rightChild_ = recurBuildTree(tree, midIndex+1, endIndex, splitDim)

    return tree.nodes_[midIndex]
end

# node* make_tree(size_t begin, size_t end, size_t index) {
#     if (end <= begin)
#         return nullptr;
#     size_t n = begin + (end - begin)/2;
#     auto i = nodes_.begin();
#     std::nth_element(i + begin, i + n, i + end, node_cmp(index));
#     index = (index + 1) % dimensions;
#     nodes_[n].left_ = make_tree(begin, n, index);
#     nodes_[n].right_ = make_tree(n + 1, end, index);
#     return &nodes_[n];
# }


splitDim = 0

splitDim = mod1((splitDim + 1), 3)
println(splitDim)
splitDim = mod1((splitDim + 1), 3)
println(splitDim)
splitDim = mod1((splitDim + 1), 3)
println(splitDim)
splitDim = mod1((splitDim + 1), 3)
println(splitDim)
splitDim = mod1((splitDim + 1), 3)
println(splitDim)
splitDim = mod1((splitDim + 1), 3)
println(splitDim)

# @views partialsort!(a[2:end], 3)

# println(a)

nodes = [Vertex(0.0,0.0,0.0),Vertex(0.0,0.0,1.0)]
println(nodes)
model = KDTree(nodes)
