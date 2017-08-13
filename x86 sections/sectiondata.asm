section     .data

global msg
msg     db  'Hello, world!',0xa
global len
len     equ $ - msg

;nasm -f elf -o data.o sectiondata.asm
;nasm -f elf -o text.o sectiontext.asm
;ld -m elf_i386 -s -o program data.o text.o
;./program 
