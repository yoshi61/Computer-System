#ifndef JACK_IOBUFFER_H
#define JACK_IOBUFFER_H

// the iobuffer buffers data to be written to cout until explicitly discarded or flushed
// this allows all output to be discarded if an error occurs

extern void write_to_buffer(std::string) ;               // adds the string parameter to the end of the current buffer
extern void erase_buffer() ;                             // erase the output buffer contents, nothing is output
extern void output_buffer() ;                            // uses cout to output the buffer contents, then it calls erase_buffer

// ***************   EXAMPLES OF HOW TO USE THE SYMBOL TABLES   *********************

// Example of how to write a line containing "hello" and 45 to the output buffer
//
//     write_to_buffer("hello ") ;
//     write_to_buffer(to_string(45)) ;
//     write_to_buffer("\n") ;
//
// To throw away everything written to the buffer so far
//     erase_buffer() ;
//
// To write out the current contents of the buffer - and erase the contents
//     output_buffer() ;
//
// Notes:
//  .  if you have written too much data to the buffer, an error message will
//     be printed and your program will exit. This is intended to catch programs
//     stuck in infinite loops, a correct program should never be affected.

#endif //JACK_IOBUFFER_H
