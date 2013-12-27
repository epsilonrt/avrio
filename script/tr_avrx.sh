#!/bin/bash

#  define DATASECTION .data
#  define CODESECTION .text
#  define ENDFUNC   .endfunc
#  define ENDMOD
#  define END       .end
#  define DB        .asciz
#  define MACRO     .macro
#  define ENDM      .endm
#  define ALIGN     .balign 2

#  define FUNCTION(A) 
#  define EXTERN(A) .extern A
#  define PUBLIC(A) .global A
#  define MODULE(A) .section .text.A

#  define GLOBAL(Label, Size) .comm Label, Size
#  define STATIC(Label, Size) .lcomm Label, Size
#  define EQU(A,B)  .equ A, B
#  define AVRX_MODULE(A) .section .text.avrx_A,"ax",@progbits
 
for fn in $(find -name "*.S" -o -name "*.inc" -maxdepth 1)
do
  echo "Process $fn ..."
  sed -i '.bak' -e 's/^[[:space:]]*AVRX_MODULE[[:space:]]*(\([[:alnum:]_]*\))/\ \ \ \ \.section .text.avrx_\1,"ax",@progbits/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*EXTERN[[:space:]]*(\([[:alnum:]_]*\))/\ \ \ \ \.extern\ \1/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*PUBLIC[[:space:]]*(\([[:alnum:]_]*\))/\ \ \ \ \.global\ \1/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*FUNCTION[[:space:]]*(\([[:alnum:]_]*\))/\ \ \ \ \.global\ \1\n\ \ \ \ .func\ \1/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*DATASECTION/\ \ \ \ \.data/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*CODESECTION/\ \ \ \ \.text/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*ENDFUNC/\ \ \ \ \.endfunc/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*DB/\ \ \ \ \.asciz/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*MACRO/\ \ \ \ \.macro/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*DATASECTION/\ \ \ \ \.endm/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*ENDM/\ \ \ \ \.endm/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*ALIGN/\ \ \ \ \.balign\ 2/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*END/\ \ \ \ \.end/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*GLOBAL[[:space:]]*(\([[:alnum:]_]*\)[[:space:]]*,[[:space:]]*\([[:alnum:]_]*\))/\ \ \ \ \.comm\ \1,\ \2/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*STATIC[[:space:]]*(\([[:alnum:]_]*\)[[:space:]]*,[[:space:]]*\([[:alnum:]_]*\))/\ \ \ \ \.lcomm\ \1,\ \2/g' $fn
  sed -i '.bak' -e 's/^[[:space:]]*EQU[[:space:]]*(\([[:alnum:]_]*\)[[:space:]]*,[[:space:]]*\([[:alnum:]_]*\))/\ \ \ \ \.set\ \1,\ \2/g' $fn
done

