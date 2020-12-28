# Create a board outline STL file
 
The release.sh release creates a SVG (and BMP) file which
has the outline of the board.

This can converted to STL files for helping to create
a box for it.

The sequence is as follow:

- Install Inkscape 0.92 (don't use 1.01) as the extension does not work
  with later versions.

- Install in .config/inkscape/extensions the
  https://github.com/l0b0/paths2openscad.git extension
  (the .py and .inx file).

- In Inkscape import the SVG file (created from the
  release.sh - the h-<release>.svg file. (See Wash_Hand_dim.svg)

- Select All

- Extensions|Generate from Path|Paths to OpenSCAD
- Output file Wash_Hand_dim.scad, keep the defaults, let it rip

- In OpenSCAD, load it, edit it (change height, etc) and render it and export as STL.
  (Wash_Hand_dim.stl)

- Then you have a 3D-ish presentation of what outline of the board
  and can use it to create boxes for the board.
