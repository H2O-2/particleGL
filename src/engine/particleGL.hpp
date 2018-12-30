#pragma once

#include "camera/camera.hpp"
#include "emitter/emitter.hpp"
#include "particle/particle.hpp"

class ParticleGL {
public:
    ParticleGL();
    ~ParticleGL();

    std::vector<Emitter> emitters;
};
