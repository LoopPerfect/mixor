# mixor
functional template library 

The goal of this minimalistic library is to provide tools to compose and transform data in a declarative way.

It relies on functional concepts and data-driven design.

```c++
#include "mixin.hpp"
#include "compose.hpp"
#include <string>

struct Position {
  float x=0.0f;
  float y=0.0f;
};

struct Speed {
  float vx=0.0f;
  float vy=0.0f;
};

struct Size {
 float sizeX= 0.0f;
 float sizeY= 0.0f;
}

struct Image {
  std::string ImagePath="";
}

using namespace mixor;
auto bird = mix( Pos{10,10} , Image{"bird.png"} , Size{10,10} );
auto movingBird = mix( bird, Speed{0,10} );
float dt = 1;

auto move = [dt](auto const& object) {
  // lets define default values in case object has no Position or Speed
  auto defaults = mix(Position{0,0}, Speed{0,0});
  auto o = mix(defaults, object);  // or mix( Position{}, Speed{}, object )
  return mix(object, Position{ 
    o.x + o.vx*dt , o.y + o.vy *dt } 
  });
};

auto applyGravity = [dt](auto const& object) { 
  return mix(object, Speed{ 
    object.vx, object.vy - 2*dt } 
  });  
}

auto clipToGround = [](auto const& o) {
  return mix(o, Position{o.x, (o.y>0) * o.y }); // make sure object is above ground
}

auto simulationStep = compose(move, applyGravity, clipToGround );
using Drawable = decltype( mix( Position{}, Image{} ) );

auto draw( Drawable const& obj) { std::cout << obj.y << std::endl; };

while(bird.y!=0) {
  draw(obj);
  bird = simulationStep(bird);
} // prints 10 , 20 , 10 , 0

```

In case you have many iterable objects:
```c++

using Object = decltype(mix(Position{}, Speed{}, Image{}));

std::vector<Object> objects;

auto simulationStepforAll = compose( 
  map(move), 
  map(applyGravity), 
  map(clipToGround) 
); //or map(compose(move, applyGravity, clipToGround));
  
objects = simulationStepforAll(objects);

```

You can see it in action:
- https://github.com/LoopPerfect/boids
- https://www.youtube.com/watch?v=viVroGmeQDw
