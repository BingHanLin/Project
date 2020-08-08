module ParticleData

using ..BasicTypes


# Particle datatype
mutable struct Particle
    pos_::Vec3F
    pressure_::Float64
    vel_::Vec3F
    type_::particleType
end

Particle(type::particleType) = Particle(
    Vec3F(0.0, 0.0, 0.0), 0.0, Vec3F(0.0, 0.0, 0.0), type)

Particle(x::Float64,y::Float64,z::Float64,type::particleType) = Particle(
    Vec3F(x, y, z), 0.0, Vec3F(0.0, 0.0, 0.0), type)


const ParticlePool = Vector{Particle}


function sqrDis(p1::Particle, p2::Particle)::Float64
    return sum((p1.pos_ - p2.pos_).^2)
end

# function findNeighbors(targetP::Particle, pool::ParticlePool, rangeR::Float32)::ParticlePool

#     neighborsPool = ParticlePool()
#     sqrRangeR = rangeR^2

#     for p in pool
#         sqrR = (targetP.x_ - p.x_)^2 + (targetP.y_ - p.y_)^2 + (targetP.z_ - p.z_)^2

#         if sqrR < sqrRangeR
#             push!(neighborsPool, p)
#         end
#     end

#     return neighborsPool
# end

if abspath(PROGRAM_FILE) == @__FILE__
    # pool = ParticlePool()
    # push!(pool, Particle(1.0, 2.0, 1.0))
    # push!(pool, Particle(1.0, 1.0, 1.0))
    # pool
    # println(pool)
end

end
