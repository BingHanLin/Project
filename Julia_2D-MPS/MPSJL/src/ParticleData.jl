module ParticleData

using ..BasicTypes


# Particle datatype
mutable struct Particle
    pos_::Vec3F
    pressure_::Float64
    numberDensity_::Float64
    vel_::Vec3F
    type_::particleType
end

Particle(type::particleType) =
    Particle(Vec3F(0.0, 0.0, 0.0), 0.0, 0.0, Vec3F(0.0, 0.0, 0.0), type)

Particle(x::Float64, y::Float64, z::Float64, type::particleType) =
    Particle(Vec3F(x, y, z), 0.0, 0.0, Vec3F(0.0, 0.0, 0.0), type)


const ParticlePool = Vector{Particle}


function sqrDis(p1::Particle, p2::Particle)::Float64
    return sum((p1.pos_ - p2.pos_) .^ 2)
end

if abspath(PROGRAM_FILE) == @__FILE__
    # pool = ParticlePool()
    # push!(pool, Particle(1.0, 2.0, 1.0))
    # push!(pool, Particle(1.0, 1.0, 1.0))
    # pool
    # println(pool)
end

end
