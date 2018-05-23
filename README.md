# Compiler GR8
A programming langauge based on Donald Trump crazy words, to Compile you have to install libcdk13 and rts4, use the following links:
https://www.l2f.inesc-id.pt/~david/wiki/pt/images/b/b8/Libcdk13-201802181144.tar.bz2
https://www.l2f.inesc-id.pt/~david/wiki/pt/images/c/cc/Librts4-201602111900.tar.bz2

to install run make in the command line.
to compile the program run ./gr8 *.gr8
to run the program run in the command line:
yasm -felf32 *.asm
ld * {*.o} ${HOME}\usr\librts -lrts
