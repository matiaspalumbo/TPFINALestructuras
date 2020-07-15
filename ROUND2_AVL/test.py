
conjuntos = ["A = {1,2,3,4,5,6,7,8,9,10}", "B = {1,2,3,4,5}", "C = {8,9,10,11,12,25,26,30,40,45,46,47,120,121,127}", "D = {7,10,11,14,16}", "E = {x : 5 <= x <= 20}", "F = {y : 100 <= y <= 150}"]



operaciones = ["TEST = A | A","TEST = A & A","TEST = ~A","TEST = A | B","TEST = A & B","TEST = A & C","TEST = A | D","TEST = A & D","TEST = A | A"]


A = {1,2,3,4,5,6,7,8,9}
B = {9,10,11,12,13,14,15}
C = {x : 6 <= x <= 3}
VACIO = C
VACIO2 = C & A
A2 = C | A
AMENOSB = A - B
BMENOSA = B - A