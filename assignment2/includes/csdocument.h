#ifndef LIB_CS_DOCUMENT_H
#define LIB_CS_DOCUMENT_H

#include <string>

// cs_node is a pointer to an XML style document tree
class cs_node_ ;
typedef cs_node_ *cs_node ; 

// external functions to create and walk over our XML style document tree
extern cs_node cs_new_node(std::string) ;                  // create a new element node with the given name, the name must not be "#text"
extern cs_node cs_new_text(std::string) ;                  // create a new text node containing the given text
extern cs_node cs_new_node_text(std::string,std::string) ; // create a new node with a child text node containing the given text
extern std::string cs_node_name(cs_node) ;                 // return the name of a node, "" if passed nullptr
extern std::string cs_node_value(cs_node) ;                // return concatenated text from all descendant text nodes, "" if passed nullptr
extern cs_node cs_parent_node(cs_node) ;                   // return parent of a node, nullptr if passed nullptr
extern void cs_append_child(cs_node,cs_node) ;             // make a node a child of another node if parent is not a text node and
                                                           // remove the new child from its previous parent if it had one
                                                           // does nothing if either parent or child is a nullptr
extern int cs_children(cs_node) ;                          // return the number of child nodes, 0 if passed a nullptr
extern cs_node cs_get_child(cs_node,int) ;                 // get a numbered child node, nullptr is passed a nullptr or index is invalid
extern void cs_reset_visit_markers() ;                     // generate a new mark value
extern bool cs_already_visited(cs_node) ;                  // record a new mark value and return the previous value, returns true if passed nullptr
extern cs_node cs_copy(cs_node) ;                          // copy a node but ignore any child nodes, return nullptr if passed nullptr
extern cs_node cs_deep_copy(cs_node) ;                     // recursively copy a node and all of its descendants, return nullptr if passed nullptr
extern void cs_print(cs_node) ;                            // recursively print a node, prints nothing if passed nullptr
extern void cs_pretty_print(cs_node) ;                     // recursively print a node with newlines and indentation, prints newline if passed nullptr
extern cs_node cs_parse_xml() ;                            // parse XML from cin and construct a document if XML was produced by cs_print()

/********   EXAMPLES OF HOW TO USE   ************/

/********  CREATING A DOCUMENT TREE  ************/

// The first node we create is the root of a document
// For workshop 8 this would be a class node and every VM
// command would be a child node of the class node
//
//     cs_node document = cs_new_element_node("class") ;

// Assume we have the following VM command from the workshop example:
//     push static 6

// If this VM command is our entire program it should produce the following - shown in XML
// <class>
//         <a-stack>
//                 <stack>push</stack>
//                 <segment>static</segment>
//                 <number>6</number>
//         </a-stack>
// </class>

// We can create the document tree with the following calls
//     cs_node document = cs_new_node("class") ;                    // the root "class" node
//
//     cs_node a_stack = cs_new_node("a-stack") ;                   // the "a-stack" node
//     cs_append_child(document,a_stack) ;                          // the "class" node is its parent
//
//     cs_node stack = cs_new_node("stack") ;                       // a "stack" node
//     cs_append_child(stack,cs_new_text("push")) ;                 // a text node containing "push" is added as a child of the "stack" node
//     cs_append_child(a_stack,stack) ;                             // the "stack" node is the first child of the "a-stack" node
//
//     cs_node segment = cs_new_node_text("segment","static") ;     // a new node and a child text node can be created in one call
//     cs_append_child(a_stack,segment) ;                           // the "segment" node is the next child after the "stack" node
//
//     cs_node number = cs_new_node_text("number",to_string(6)) ;   // to_string is an easy way to turn an int into a string
//                                                                  // use stoi to turn a string into an int
//     cs_append_child(a_stack,number) ;                            // the "number" node is the next child after the "segment" node
//

// Note:
// . each structural element was appended to its parent after it was created
// . the text "push", "static" and "6" are all wrapped in separate child text nodes
// . text nodes all have the tagname "#text"
// . this closely mirrors how you do this in Javascript in a web browser
// . nodes contain a parent pointer, do not append the same node to more than one parent
// . when a node is appended to a parent it is removed from its previous parent

/********  PRINTING / READING A DOCUMENT TREE  ************/

// To print a document tree formatted as XML we can simply call the printer
//    cs_print(document) ;

// If you would like a more human friendly view of the XML call the pretty printer instead but the
// added whitespace means that a document tree constructed from this output would have extra text nodes
//    cs_pretty_print(document) ;

// If you want to pass a document tree from one program, p1, to another program, p2.
// The first program, p1, finishes by calling cs_print(document) to write XML to standard output
// The second program, p2, starts by calling cs_parse_xml() to reconstruct the document tree by parsing standard input
// The following will run program p1 and sends its standard output (cout) to the standard input (cin) of program p2:
//    ./p1 | ./p2

// All *nix programs normally have 3 open files when they run, standard input (cin),
// standard output (cout) and standard error (cerr). Only bytes written to standard
// output (cout) are sent down a pipe so errors messages can be displayed separately
// by writing them to standard error (cerr).


/********  WALKING A DOCUMENT TREE  ************/

// If we have a document tree we need to be able to
// walk over its structure in order to extract information.

// Assume you want to write your own cs_print()
// You need to use the available functions to interrogate
// the nodes and find their children. The following code
// assumes you are passed an arbitrary node and attempts
// to print out its structure as XML

//    void not_pretty_printer(cs_node node)
//    {
//        if ( node == nullptr ) return ;            // do nothing if given a bad pointer
//        if ( cs_already_visited(node) ) return ;   // do nothing if the node has already been visited
//        string name = cs_node_name(node) ;         // what is the name?
//    
//        if ( name == "#text" )                     // if we have a text node
//        {
//            cout << cs_node_value(node) ;          // print the text as is - you must also translate < > & ' and " into XML entities
//        } else
//        {
//            // to get here we have a structural node that may contain children
//            // write out the start of an XML node with the name
//            cout << "<" << name << ">" ;
//    
//            // now print all this node's children, if there are any
//            int children = cs_children(node) ;
//            for ( int i = 0 ; i < children ; i++ )
//            {
//                not_pretty_printer(cs_get_child(node,i)) ;
//            }
//    
//            // write out the close of an XML node with the name
//            cout << "</" << name << ">" ;
//        }
//    }
//....
//    // this call will print the tree rooted at mynode
//    // but first we reset the visited markers so that the cs_visited() function will work correctly
//    cs_reset_visit_markers() ;
//    not_pretty_printer(mynode) ;

#endif //LIB_CS_DOCUMENT_H
