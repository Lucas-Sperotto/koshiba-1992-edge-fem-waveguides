SetFactory("Built-in");

// Quarter-section of the rectangular dielectric waveguide used for Figure 5.
// The scale parameter t is normalized to 1 in this geometry.
t = 1.0;
X = 5.0*t;
Y = 2.5*t;
lc = 0.65*t;

Point(1) = {0, 0, 0, lc};
Point(2) = {1*t, 0, 0, lc/2};
Point(3) = {X, 0, 0, lc};
Point(4) = {0, 1*t, 0, lc/2};
Point(5) = {1*t, 1*t, 0, lc/2};
Point(6) = {X, 1*t, 0, lc};
Point(7) = {0, Y, 0, lc};
Point(8) = {1*t, Y, 0, lc};
Point(9) = {X, Y, 0, lc};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {1, 4};
Line(4) = {4, 7};
Line(5) = {2, 5};
Line(6) = {5, 8};
Line(7) = {3, 6};
Line(8) = {6, 9};
Line(9) = {4, 5};
Line(10) = {5, 6};
Line(11) = {7, 8};
Line(12) = {8, 9};

Curve Loop(201) = {1, 5, -9, -3};
Plane Surface(201) = {201}; // core

Curve Loop(202) = {2, 7, -10, -5};
Plane Surface(202) = {202}; // lower right cladding

Curve Loop(203) = {9, 6, -11, -4};
Plane Surface(203) = {203}; // upper left cladding

Curve Loop(204) = {10, 8, -12, -6};
Plane Surface(204) = {204}; // upper right cladding

Physical Surface("core", 201) = {201};
Physical Surface("cladding", 202) = {202, 203, 204};

Physical Curve("symmetry_y0", 21) = {1, 2};
Physical Curve("symmetry_x0", 22) = {3, 4};
Physical Curve("outer_right", 23) = {7, 8};
Physical Curve("outer_top", 24) = {11, 12};
Physical Curve("core_interface", 25) = {5, 9};

Mesh.MshFileVersion = 4.1;
Mesh.Binary = 0;
