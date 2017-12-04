// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/06/rect/Rect.asm

// Draws a rectangle at the top-left corner of the screen.
// The rectangle is 16 pixels wide and R0 pixels high.

   @0
   M=>D
   @INFINITE_LOOP
   D;JLE 
   @counter
   D=>M
   @SCREEN
   A=>D
   @address
   D=>M
(LOOP)
   @address
   M=>A
   -1=>M
   @address
   M=>D
   @32
   D+A=>D
   @address
   D=>M
   @counter
   M-1=>MD
   @LOOP
   D;JGT
(INFINITE_LOOP)
   @INFINITE_LOOP
   0;JMP
