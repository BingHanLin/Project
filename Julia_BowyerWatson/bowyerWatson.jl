
using LinearAlgebra, StaticArrays, Printf

include("Primitives.jl")
include("BasicTypes.jl")
using .Primitives
using .BasicTypes

mutable struct DelaunayTriangulation
    triangles_::Array{Triangle,1}

    function DelaunayTriangulation()

        v1 = Vertex(0.0, 0.0, 0.0)
        v2 = Vertex(99.0, 0.0, 0.0)
        v3 = Vertex(99.0, 99.0, 0.0)
        v4 = Vertex(0.0, 99.0, 0.0)

        tri1 = Triangle(v1, v4, v2)
        tri2 = Triangle(v3, v2, v4)
        tri1.neighbors_[1] = tri2
        tri2.neighbors_[1] = tri1

        this = new()
        this.triangles_ = Triangle[]
        push!(this.triangles_, tri1)
        push!(this.triangles_, tri2)
        return this
    end
end

function addVertex(d::DelaunayTriangulation, v::Vertex)

    badTris = Triangle[]

    for (index, tri) in enumerate(d.triangles_)
        if isInInCircumcircle(v, tri)
            push!(badTris, tri)
        end
    end

    boundaries = calculateBoundary(d, badTris)

    for oneBounary in boundaries
        println(oneBounary)
    end
end

function calculateBoundary(d::DelaunayTriangulation, badTris::Array{Triangle,1})

    edgeIndex::Int64 = 1
    oneTri = badTris[1]
    boundary = Tuple{Vertex,Vertex,Triangle}[]

    while true

        if length(boundary) > 1
            if boundary[1] == boundary[end]
                break
            end
        end
        println("=====================")
        println(edgeIndex)
        println(oneTri)
        println(isassigned(oneTri.neighbors_, edgeIndex))
        println("=====================")

        if isassigned(oneTri.neighbors_, edgeIndex) && oneNeighborTri ∈ badTris
            println("oneTri changed")
            oneNeighborTri = oneTri.neighbors_[edgeIndex]
            sameEdgeIndex = findfirst(isequal(oneTri), oneNeighborTri.neighbors_)
            edgeIndex = mod1((sameEdgeIndex + 1), 3)
            oneTri = oneNeighborTri
        else
            if isassigned(oneTri.neighbors_, edgeIndex)
                oneNeighborTri = oneTri.neighbors_[edgeIndex]
            else
                oneNeighborTri = oneTri
            end
            edgetV1 = vertexByIndex(oneTri, mod1((edgeIndex), 3))
            edgetV2 = vertexByIndex(oneTri, mod1((edgeIndex + 1), 3))
            push!(boundary, (edgetV1, edgetV2, oneNeighborTri))
            edgeIndex = mod1((edgeIndex + 1), 3)
        end
    end

    return boundary
end


# d = DelaunayTriangulation()

# addVertex(d, Vertex(4.0, 5.0, 0.0))


v1 = Vertex(0.0, 0.0, 0.0)
v2 = Vertex(99.0, 0.0, 0.0)
v3 = Vertex(99.0, 99.0, 0.0)
v4 = Vertex(0.0, 99.0, 0.0)
tri = Triangle(v1, v4, v2)
z = Array{Triangle,1}(undef, 3)

println(z[1])
