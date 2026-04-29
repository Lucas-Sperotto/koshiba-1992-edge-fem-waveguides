SetFactory("Built-in");

// Half-section of the shielded microstrip used for Figure 3.
// Coordinates are in SI units.
W = 1.27e-3;
h = 1.27e-3;
X = 12.7e-3;
Y = 12.7e-3;
lc = 2.2e-3;

Point(1) = {0, 0, 0, lc};
Point(2) = {X/2, 0, 0, lc};
Point(3) = {X/2, h, 0, lc};
Point(4) = {W/2, h, 0, lc/2};
Point(5) = {0, h, 0, lc/2};
Point(6) = {X/2, Y, 0, lc};
Point(7) = {0, Y, 0, lc};

Line(1) = {1, 2}; // ground
Line(2) = {2, 3}; // right wall below substrate top
Line(3) = {3, 4}; // dielectric interface outside the strip
Line(4) = {4, 5}; // conducting strip
Line(5) = {5, 1}; // symmetry plane below strip
Line(6) = {3, 6}; // right wall above substrate
Line(7) = {6, 7}; // top wall
Line(8) = {7, 5}; // symmetry plane above strip

Curve Loop(101) = {1, 2, 3, 4, 5};
Plane Surface(102) = {101}; // substrate

Curve Loop(103) = {-4, -3, 6, 7, 8};
Plane Surface(101) = {103}; // air

Physical Surface("air", 101) = {101};
Physical Surface("substrate", 102) = {102};

Physical Curve("pec_ground", 11) = {1};
Physical Curve("symmetry_x0", 12) = {5, 8};
Physical Curve("outer_right", 13) = {2, 6};
Physical Curve("outer_top", 14) = {7};
Physical Curve("pec_strip", 15) = {4};
Physical Curve("dielectric_interface", 16) = {3};

Mesh.MshFileVersion = 4.1;
Mesh.Binary = 0;
