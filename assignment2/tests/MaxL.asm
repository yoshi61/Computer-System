// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/06/max/MaxL.asm

// Symbol-less version of the Max.asm program.

        @0
        M=>D
        @1
        D-M=>D
        @10
        D;JGT
        @1
        M=>D
        @12
        0;JMP
        @0
        M=>D
        @2
        D=>M
        @14
        0;JMP
