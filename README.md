# ParticleGL
![](https://github.com/H2O-2/particleGL/blob/master/particles.jpg)
An OpenGL particle generation tool, inspired mainly by the plugin Trapcode Particular for Adobe After Effects.

## Demo
Simple Snow

![](https://github.com/H2O-2/particleGL/blob/master/snow.gif)

Simple Fire

![](https://github.com/H2O-2/particleGL/blob/master/fire.gif)

Sprite

![](https://github.com/H2O-2/particleGL/blob/master/sprite.gif)


## Usage
Most of the functionalities of ParticleGL reproduces those in the plugin Particular, so definitely check the [documentation for Trapcode Particular](https://www.redgiant.com/user-guide/trapcode-particular/). There are, however, some extra/modified behaviour in ParticularGL which will be stated below.

### Emitter
1. `Velocity Distribution` behaves differently compared to Particular, which is exactly the portion of the randomly generated initial velocity that is higher than the given initial velocity. I.e. consider a random offset for the particle's initial velocity, a value of `0.2` will give a 20% chance to add the offset to the specified initial velocity and 80% chance to subtract the offsest from the specified initial velocity. This parameter does nothing if `Velocity Random` is set to `0`.

### Particle
1. An extra particle type `Triangle` is added. It is a equilateral triangle and the shape is (for now) unmodifiable.
2. `Sprite` particle type can be colorized and blended.
3. `Feather` is implemented differently compared to Particular. Particular does a per-particle feathering while ParticleGL simply applies gaussian blur on top of the particles (think of it as an adjustment layer). The visual effect of feathering also varies across platform where on Windows the blurry feel is more apparent compared to UNIX based systems.
4. Since the underlying implementation of `Feather` is different, values and visual effect differs as well where `5` in ParticleGL approximates `50` in Particular. However, as particle size changes blurness of the two becomes different.
5. An extra control `Color Blend` is added for sprite particles. This value controls the ratio of the color from original texture against user defined color, i.e. `1.0` indicates 100% of texture color is used.

You can get the binary [here](https://github.com/H2O-2/particleGL/releases/tag/0.1.0)

