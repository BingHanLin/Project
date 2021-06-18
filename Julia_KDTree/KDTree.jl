using LinearAlgebra: Array
include("Primitives.jl")
using .Primitives

using LinearAlgebra, StaticArrays, Printf

mutable struct treeNode
    vertex_::Vertex
    leftChild_::Union{treeNode, Nothing}
    rightChild_::Union{treeNode, Nothing}
    splitDim_::Int64

    function treeNode(vertex::Vertex)
        this = new()
        this.vertex_ = vertex
        this.leftChild_ = nothing
        this.rightChild_ = nothing
        return this
    end
end

function distance(node::treeNode, vertex::Vertex)::Float64
    return norm(node.vertex_-vertex) 
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

        this.rootNode_ = recurBuildTree(this, 1, length(this.nodes_), 0)

        return this
    end
end

function recurBuildTree(tree::KDTree, beginIndex::Int64, endIndex::Int64,
                        splitDim::Int64)::Union{treeNode, Nothing}

    if (endIndex < beginIndex)
        return nothing
    end

    splitDim = mod1((splitDim + 1), 3)
    localMidIndex = cld((endIndex - beginIndex) , 2) + 1
    midIndex = beginIndex + localMidIndex - 1

    @views partialsort!(tree.nodes_[beginIndex:endIndex], localMidIndex,
                        lt=(x, y)->isless(x.vertex_[splitDim], y.vertex_[splitDim]))

    tree.nodes_[midIndex].leftChild_ = recurBuildTree(tree, beginIndex, midIndex-1, splitDim)
    tree.nodes_[midIndex].rightChild_ = recurBuildTree(tree, midIndex+1, endIndex, splitDim)
    tree.nodes_[midIndex].splitDim_ = splitDim

    return tree.nodes_[midIndex]
end

function traverseTree(node::Union{treeNode, Nothing})

    if (node === nothing)
        return
    end
    traverseTree(node.leftChild_)
    traverseTree(node.rightChild_)
end

function nearestDistance(vertex::Vertex, node::Union{treeNode, Nothing},
                         bestDis::Float64, bestNode::Union{treeNode, Nothing})::Tuple{Float64,Union{treeNode, Nothing}}

    if (node === nothing)
        return bestDis, bestNode
    end

    dis = distance(node, vertex)

    if (bestNode === nothing || dis < bestDis)
        bestDis = dis
        bestNode = node
    end

    if (bestDis == 0)
        return bestDis, bestNode
    end

    splitDim = node.splitDim_

    dimDelta = node.vertex_[splitDim] -vertex[splitDim]

    nextNode1 = dimDelta > 0 ? node.leftChild_ :  node.rightChild_
    bestDis, bestNode = nearestDistance(vertex, nextNode1, bestDis, bestNode)

    if (dimDelta >= bestDis)
        return bestDis, bestNode
    end

    nextNode2 = dimDelta > 0 ? node.rightChild_ :  node.leftChild_
    bestDis, bestNode = nearestDistance(vertex, nextNode2, bestDis, bestNode)

    return bestDis, bestNode
end

function nearestDistance(vertex::Vertex, node::Union{treeNode, Nothing})::Float64
    bestDis, _ = nearestDistance(vertex, node,  typemax(Float64), node)
    return bestDis
end

# void nearest(node* root, const point_type& point, size_t index) {
#     if (root == nullptr)
#         return;
#     ++visited_;
#     double d = root->distance(point);
#     if (best_ == nullptr || d < best_dist_) {
#         best_dist_ = d;
#         best_ = root;
#     }
#     if (best_dist_ == 0)
#         return;
#     double dx = root->get(index) - point.get(index);
#     index = (index + 1) % dimensions;
#     nearest(dx > 0 ? root->left_ : root->right_, point, index);
#     if (dx * dx >= best_dist_)
#         return;
#     nearest(dx > 0 ? root->right_ : root->left_, point, index);
# }

nodes = [Vertex(0.1,1.1,0.0),Vertex(0.2,0.7,1.0),Vertex(0.3,1.0,1.0),Vertex(1.0,0.6,1.1),Vertex(0.5,0.3,1.0)]

model = KDTree(nodes)

traverseTree(model.rootNode_)

bestDis = nearestDistance(Vertex(0.1,1.102,0.0), model.rootNode_)

println(bestDis)