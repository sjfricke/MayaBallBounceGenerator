# Maya Ball Bounce Generator Plugin

This was made for easily creating a collections of falling marbles, bouncing balls, rocks, etc.

## Current Version - 0.1
Features
  - Pick quantity
  - Select ball radius

## Future Plans

 - Allow for own shading to be put on the balls 
 - Have balls move forward or in any direction
 - Collision detection

## HOW TO USE

Install the plugin to Maya and load it up

in your mel box on the bottom left of screen
```sh
BallDrop -q <Quantity> -r <Radius>
```

Example:
```sh
BallDrop -q 500 -r 2.5
```
This will create 500 balls with a radius of 2.5 units each

### -q <Quantity> 

This is an **optional** argument used to pick the number of balls generated, default is 1

### -r <Radius> 

This is an **optional** argument used to pick the radius of the ball, default is 4