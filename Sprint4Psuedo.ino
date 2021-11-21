


We plan to rework a lot of our code from Sprint 3 to fit Sprint 4 by changing the direction of movement and which direction we check for the IR beacons.
For the maze, and the wall section it will be as easy as changing a function call. The hard parts of the course such as the Salamander rescue will be harder 
to apply to Sprint 4 as they require logic to navigate around the pen.




 Mirrored Maze/Salamander Psuedocode
{
  drive across bridge
  using ping sensor, drive until within 20cm of maze wall
  Move right until sense 20 distance measurements over 40cm
  go forward until within 20cm of next wall
  go left or right until hit the next opening
  go right until sense the IR beacon for left to right positioning
  if pen is turned go around the box until get to opening by grabbing ping sensor data around the edges
  push all salamanders to left half of box
  move forwards to grab all of them
  back out of "danger zone"
}


Mirrored Ramp/Salinity Psuedocode
{
  Drive along wall until reach opening
  Move Left until IR beacon is read
  Turn to see where start point is
  Move forward onto ramp
  Take inclination
  Move forward to top of hill
  Turn towards water
  drive to edge (very carefully)
  drop salinity into water and take reading
}
