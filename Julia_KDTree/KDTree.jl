using LinearAlgebra: Array
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

        this.rootNode_ = recurBuildTree(this, 1, length(this.nodes_), 0);

        return this
    end
end

function recurBuildTree(tree::KDTree, beginIndex::Int64, endIndex::Int64,
                        splitDim::Int64)::Union{treeNode, Nothing}

    if (endIndex < beginIndex)
        return nothing
    end

    splitDim = mod1((splitDim + 1), 3)
    localMidIndex = cld((endIndex - beginIndex) , 2) + 1;
    midIndex = beginIndex + localMidIndex - 1;

    @views partialsort!(tree.nodes_[beginIndex:endIndex], localMidIndex,
                        lt=(x, y)->isless(x.vertex_[splitDim], y.vertex_[splitDim]))

    tree.nodes_[midIndex].leftChild_ = recurBuildTree(tree, beginIndex, midIndex-1, splitDim)
    tree.nodes_[midIndex].rightChild_ = recurBuildTree(tree, midIndex+1, endIndex, splitDim)

    return tree.nodes_[midIndex]
end

function traverseTree(node::Union{treeNode, Nothing})

    if (node === nothing)
        return
    end
    traverseTree(node.leftChild_);
    traverseTree(node.rightChild_);
end


nodes = [Vertex(0.1,1.1,0.0),Vertex(0.2,0.7,1.0),Vertex(0.3,1.0,1.0),Vertex(1.0,0.6,1.1),Vertex(0.5,0.3,1.0)]

model = KDTree(nodes)

traverseTree(model.rootNode_)