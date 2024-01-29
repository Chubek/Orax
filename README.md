# Orax(Aurocks) : The Optimizing C Compiler


Orax is an optimizing compiler. It has its own lexer and parser. It relies on S-Expressions for its various passes. Besides S-Expressions for the IR, it relines on ASTs, DAGs, the SSA form, Maximal Munch, Control Flow analysis, and so on.

I have made a good amount of progress in the implementation, but the code is full of syntactic errors. Thankfully, I can safely say that it has little semantic errors, just a few sprinkled around.

The S-Expression parser works fine, and tested (`orax-sexp.c`). It has syntax objects too, although it will probably go unused? Not sure, I shall probably tend to the whole concept of IR S-Expressions soon.

I am hoping to get a paper out of this. Also, I am hoping to make it a real 'compiler compiler' (not the fake Yacc kind, jk!). The decision to integrate the lexer and parser was a big one.

Don't think because I am implementing a C compiler, I am a strickler for the rules --- as I am implementing a Non-Compliant [OS](https://github.com/Chubek/Oryx) too!
