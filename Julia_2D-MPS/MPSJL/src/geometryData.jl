module GeometryData

using ..ParticleData: Particle, ParticlePool
using ..BasicTypes

# Box domain datatype
struct BoxGeometry
    xMin_::Float64
    yMin_::Float64
    zMin_::Float64

    xMax_::Float64
    yMax_::Float64
    zMax_::Float64

    filledHeight_::Float64

    numWallLayer_::Int64
    numDummyLayer_::Int64

    function BoxGeometry(
        xMin::Float64,
        yMin::Float64,
        zMin::Float64,
        xMax::Float64,
        yMax::Float64,
        zMax::Float64,
        filledHeight::Float64,
    )
        new(xMin, yMin, zMin, xMax, yMax, zMax, filledHeight, 1, 3)
    end
end

function generateParticle(geom::BoxGeometry, pool::ParticlePool, particleRadius::Float64)

    particleDiameter = 2 * particleRadius

    distX = geom.xMax_ - geom.xMin_
    distY = geom.yMax_ - geom.yMin_
    distZ = geom.zMax_ - geom.zMin_

    numParticleInX = Int64(round(distX / particleDiameter))
    numParticleInX = numParticleInX == 0 ? 1 : numParticleInX
    numParticleInX = numParticleInX == 1 ? 1 :
        numParticleInX + 2 * (geom.numWallLayer_ + geom.numDummyLayer_)

    numParticleInY = Int64(round(distY / particleDiameter))
    numParticleInY = numParticleInY == 0 ? 1 : numParticleInY
    numParticleInY = numParticleInY == 1 ? 1 :
        numParticleInY + 2 * (geom.numWallLayer_ + geom.numDummyLayer_)

    numParticleInZ = Int64(round(distZ / particleDiameter))
    numParticleInZ = numParticleInZ == 0 ? 1 : numParticleInZ
    numParticleInZ = numParticleInZ == 1 ? 1 :
        numParticleInZ + 2 * (geom.numWallLayer_ + geom.numDummyLayer_)

    maxNumParticles = numParticleInX * numParticleInY * numParticleInZ

    sizehint!(pool, maxNumParticles)

    generateFluidParticle(geom, pool, particleRadius, particleDiameter)
    numFluidParticles = length(pool)
    println("generate fluid particles: ", numFluidParticles)

    generateWallParticle(geom, pool, particleRadius, particleDiameter)
    numWallParticles = length(pool) - numFluidParticles
    println("generate wall particles: ", numWallParticles)

    generateDummyParticle(geom, pool, particleRadius, particleDiameter)
    numDummyParticles = length(pool) - numFluidParticles - numWallParticles
    println("generate dummy particles: ", numDummyParticles)




end

function generateFluidParticle(
    geom::BoxGeometry,
    pool::ParticlePool,
    particleRadius::Float64,
    particleDiameter::Float64,
)

    distX = geom.xMax_ - geom.xMin_
    distY = geom.filledHeight_ - geom.yMin_
    distZ = geom.zMax_ - geom.zMin_

    numParticleInX = Int64(round(distX / particleDiameter))
    numParticleInX = numParticleInX == 0 ? 1 : numParticleInX

    numParticleInY = Int64(round(distY / particleDiameter))
    numParticleInY = numParticleInY == 0 ? 1 : numParticleInY

    numParticleInZ = Int64(round(distZ / particleDiameter))
    numParticleInZ = numParticleInZ == 0 ? 1 : numParticleInZ

    numParticles = numParticleInX * numParticleInY * numParticleInZ

    @inbounds for gk = 1:numParticleInZ, gj = 1:numParticleInY, gi = 1:numParticleInX
        posX = 0.0
        posY = 0.0
        posZ = 0.0

        if numParticleInX == 1
            posX = geom.xMin_
        else
            posX = geom.xMin_ - particleRadius + gi * particleDiameter
        end

        if numParticleInY == 1
            posY = geom.yMin_
        else
            posY = geom.yMin_ - particleRadius + gj * particleDiameter
        end

        if numParticleInZ == 1
            posZ = geom.zMin_
        else
            posZ = geom.zMin_ - particleRadius + gk * particleDiameter
        end

        push!(pool, Particle(posX, posY, posZ, fluid))

    end



end

function generateWallParticle(
    geom::BoxGeometry,
    pool::ParticlePool,
    particleRadius::Float64,
    particleDiameter::Float64,
)

    distX = geom.xMax_ - geom.xMin_
    distY = geom.yMax_ - geom.yMin_
    distZ = geom.zMax_ - geom.zMin_

    numParticleInX = Int64(round(distX / particleDiameter))
    numParticleInX = numParticleInX == 0 ? 1 : numParticleInX
    numParticleInX = numParticleInX == 1 ? 1 : numParticleInX + 2 * (geom.numWallLayer_)

    numParticleInY = Int64(round(distY / particleDiameter))
    numParticleInY = numParticleInY == 0 ? 1 : numParticleInY
    numParticleInY = numParticleInY == 1 ? 1 : numParticleInY + 2 * (geom.numWallLayer_)

    numParticleInZ = Int64(round(distZ / particleDiameter))
    numParticleInZ = numParticleInZ == 0 ? 1 : numParticleInZ
    numParticleInZ = numParticleInZ == 1 ? 1 : numParticleInZ + 2 * (geom.numWallLayer_)

    numParticles = numParticleInX * numParticleInY * numParticleInZ


    @inbounds for gk = 1:numParticleInZ, gj = 1:numParticleInY, gi = 1:numParticleInX
        posX = 0.0
        posY = 0.0
        posZ = 0.0

        if numParticleInX == 1
            posX = geom.xMin_
        else
            coeff = (gi - geom.numWallLayer_)
            posX = geom.xMin_ - particleRadius + coeff * particleDiameter
        end

        if numParticleInY == 1
            posY = geom.yMin_
        else
            coeff = (gj - geom.numWallLayer_)
            posY = geom.yMin_ - particleRadius + coeff * particleDiameter
        end

        if numParticleInZ == 1
            posZ = geom.zMin_
        else
            coeff = (gk - geom.numWallLayer_)
            posZ = geom.zMin_ - particleRadius + coeff * particleDiameter
        end

        if (
            geom.xMin_ <= posX <= geom.xMax_ &&
            geom.yMin_ <= posY <= geom.yMax_ &&
            geom.zMin_ <= posZ <= geom.zMax_
        )
            continue
        end

        push!(pool, Particle(posX, posY, posZ, wall))

    end

end

function generateDummyParticle(
    geom::BoxGeometry,
    pool::ParticlePool,
    particleRadius::Float64,
    particleDiameter::Float64,
)

    distX = geom.xMax_ - geom.xMin_
    distY = geom.yMax_ - geom.yMin_
    distZ = geom.zMax_ - geom.zMin_

    numParticleInX = Int64(round(distX / particleDiameter))
    numParticleInX = numParticleInX == 0 ? 1 : numParticleInX
    numParticleInX = numParticleInX == 1 ? 1 :
        numParticleInX + 2 * (geom.numWallLayer_ + geom.numDummyLayer_)

    numParticleInY = Int64(round(distY / particleDiameter))
    numParticleInY = numParticleInY == 0 ? 1 : numParticleInY
    numParticleInY = numParticleInY == 1 ? 1 :
        numParticleInY + 2 * (geom.numWallLayer_ + geom.numDummyLayer_)

    numParticleInZ = Int64(round(distZ / particleDiameter))
    numParticleInZ = numParticleInZ == 0 ? 1 : numParticleInZ
    numParticleInZ = numParticleInZ == 1 ? 1 :
        numParticleInZ + 2 * (geom.numWallLayer_ + geom.numDummyLayer_)

    numParticles = numParticleInX * numParticleInY * numParticleInZ


    @inbounds for gk = 1:numParticleInZ, gj = 1:numParticleInY, gi = 1:numParticleInX
        posX = 0.0
        posY = 0.0
        posZ = 0.0

        if numParticleInX == 1
            posX = geom.xMin_
        else
            coeff = (gi - geom.numWallLayer_ - geom.numDummyLayer_)
            posX = geom.xMin_ - particleRadius + coeff * particleDiameter
        end

        if numParticleInY == 1
            posY = geom.yMin_
        else
            coeff = (gj - geom.numWallLayer_ - geom.numDummyLayer_)
            posY = geom.yMin_ - particleRadius + coeff * particleDiameter
        end

        if numParticleInZ == 1
            posZ = geom.zMin_
        else
            coeff = (gk - geom.numWallLayer_ - geom.numDummyLayer_)
            posZ = geom.zMin_ - particleRadius + coeff * particleDiameter
        end

        if (
            geom.xMin_ - geom.numWallLayer_ * particleDiameter <=
            posX <=
            geom.xMax_ + geom.numWallLayer_ * particleDiameter &&
            geom.yMin_ - geom.numWallLayer_ * particleDiameter <=
            posY <=
            geom.yMax_ + geom.numWallLayer_ * particleDiameter &&
            geom.zMin_ - geom.numWallLayer_ * particleDiameter <=
            posZ <=
            geom.zMax_ + geom.numWallLayer_ * particleDiameter
        )
            continue
        end

        push!(pool, Particle(posX, posY, posZ, dummy))

    end
end



if abspath(PROGRAM_FILE) == @__FILE__
    println(methods(BoxDomain))
end

end
