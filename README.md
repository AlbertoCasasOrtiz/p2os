ROS+Gazebo Exercises
-----------

Set of exercises in ROS controlling a pioneer3at from [p2os](https://github.com/allenh1/p2os)

Exercise 1
----------

Create a world with bars in slalom and operate a pioneer3at with a camera. First from world view and then from first person view.

Execute ros+gazebo:

<code>roslaunch p2os_urdf pioneer3at.gazebo.launch</code>

Execute first person view with rqt:

<code>rosrun rqt_gui rqt_gui</code>

Operate robot with keyboard:

<code>rosrun teleop_twist_keyboard teleop_twist_keyboard.py</code>

Exercise 2
----------

Make the robot follow objects of a determined color.

Execute ros+gazebo:

<code>roslaunch p2os_urdf pioneer3at.gazebo.launch</code>

Make robot follow objects:

<code>rosrun follow_color follow_color</code>

Exercise 3
----------

Make the robot follow a goal (Red ball) and avoid obstacles (Yellow and green balls) using potential fields method.

Execute ros+gazebo:

<code>roslaunch p2os_urdf pioneer3at.gazebo.launch</code>

Make robot follow objects:

<code>rosrun potential_fields potential_fields</code>

Exercise 4
----------

Make the robot go accross a full room to create a map with odometry. Later, use that map to find shotest paths between two points, generate a set of commands and execute them.

Execute ros+gazebo:

<code>roslaunch p2os_urdf pioneer3at.gazebo.launch</code>

Make the robot follow the full map:

<code>rosrun odometry_map odometry_map</code>

Once has ended, the map has been created. Establish two points, origin and destiny with initial heading, and execute path finder and command genedator:

<code>rosrun planning_trayectory planning_trayectory</code>

It will generate a commands.txt with the commands that the robot has to follow. Put the robot in the starting position and execute the pilot to take the robot to its goal:

<code> rosrun command_pilot pilot</code>

Exercise 5
----------

Make the robot use a map to find the shortest path between two points using A* algorithm and a map represented as an adjacency graph. The map was a screenshot from a building created in Gazebo, and later processed and converted into a gridmap using [Quadtree+GridTree](https://github.com/AlbertoCasasOrtiz/QuadTree-RegularGrid). Once the algorithm has the shortest path it creates a set of commands. It can be executed with the pilot of the previous exercise.

*Requisite have the files generated by [Quadtree+GridTree](https://github.com/AlbertoCasasOrtiz/QuadTree-RegularGrid) in the ~/catkin_ws directory.

Execute ros+gazebo:

<code>roslaunch p2os_urdf pioneer3at.gazebo.launch</code>

Calculate path and commands with the algorithm A*:

<code>rosrun a_star a_star</code>

It will generate a command.txt with the commands that the robot has to follow. Put the robot in the starting position and execute the pilot to take the robot to its goal:

<code>rosrun command_pilot pilot </code>
