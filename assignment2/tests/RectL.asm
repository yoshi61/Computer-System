// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/06/rect/RectL.asm

// Symbol-less version of the Rect.asm program.

       @0
       M=>D
       @23
       D;JLE
       @16
       D=>M
       @16384
       A=>D
       @17
       D=>M
       @17
       M=>A
       -1=>M
       @17
       M=>D
       @32
       D+A=>D
       @17
       D=>M
       @16
       M-1=>MD
       @10
       D;JGT
       @23
       0;JMP
