SetFactory("Built-in");

// Half-section of the equilateral triangular-core waveguide used for Figure 7.
// The scale parameter t is normalized to 1 in this geometry.
t = 1.0;
X = 3.0*t;
Y = 5.0*t;
Hc = Sqrt(3.0)*t;
lc = 0.75*t;

Point(1) = {0, 0, 0, lc/2};
Point(2) = {1*t, 0, 0, lc/2};
Point(3) = {0, Hc, 0, lc/2};
Point(4) = {X, 0, 0, lc};
Point(5) = {X, Hc, 0, lc};
Point(6) = {X, Y, 0, lc};
Point(7) = {0, Y, 0, lc};

Line(1) = {1, 2}; // lower core edge
Line(2) = {2, 3}; // slanted core edge
Line(3) = {3, 1}; // symmetry core edge
Line(4) = {2, 4}; // bottom cladding
Line(5) = {4, 5}; // right cladding below core top
Line(6) = {5, 3}; // interface continuation at core top
Line(7) = {5, 6}; // right cladding above core top
Line(8) = {6, 7}; // top boundary
Line(9) = {7, 3}; // symmetry upper boundary

Curve Loop(301) = {1, 2, 3};
Plane Surface(301) = {301}; // triangular core

Curve Loop(302) = {4, 5, 6, -2};
Plane Surface(302) = {302}; // lower/right cladding

Curve Loop(303) = {-6, 7, 8, 9};
Plane Surface(303) = {303}; // upper cladding

Physical Surface("core", 301) = {301};
Physical Surface("cladding", 302) = {302, 303};

Physical Curve("symmetry_x0", 31) = {3, 9};
Physical Curve("outer_bottom", 32) = {1, 4};
Physical Curve("outer_right", 33) = {5, 7};
Physical Curve("outer_top", 34) = {8};
Physical Curve("core_interface", 35) = {2, 6};

Mesh.MshFileVersion = 4.1;
Mesh.Binary = 0;
