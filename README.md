#Reflector Workbench (r-bench)

R-Bench models how well various shapes of reflectors concentrate light from different angles.

In particular this software is designed to analyze the performance of different solar trough designs for use in renewable energy production.

##Intended scope:

- Single reflector
- Single light source (directional or point-source)
- One optional 'receiver' or 'light sink' (the pipe or panel in a solar application- r-bench will provide performance characteristics of the receiver for a given angle or averaged over a range of angles)

##Current Features:

- handles angles between 5 and 175 degrees
- supports catenary, parabola and semicircle reflector shapes
- adjustable reflector attributes (function min/max, alpha or 'steepness')
- recursively bounces light rays until they no longer intersect reflector
- both circle and line shaped 'light sinks'

Current version is programmed using Qt4.
