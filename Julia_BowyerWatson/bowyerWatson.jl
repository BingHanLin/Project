include("BasicTypes.jl")
using .BasicTypes

mutable struct Point

    position_::Vec3F

    Point(position::Vec3F) = new(position)

end


mutable struct DelaunayTriangulation

    bounding_::Array{Vec3F,1}

    function DelaunayTriangulation()
        this = new()
        this.bounding_ = [
            Vec3F(0.0, 0.0, 0.0),
            Vec3F(100.0, 0.0, 0.0),
            Vec3F(0.0, 100.0, 0.0),
            Vec3F(100.0, 100.0, 0.0),
        ]

        return this
    end

end

tri = Triangle(Vec3F(0.0, 0.0, 0.0), Vec3F(4.0, 0.0, 0.0), Vec3F(4.0, 3.0, 0.0))
pt = Vec3F(4.0, 5.0, 0.0)

result = isInCircumcirCle(tri, pt)

println(result)