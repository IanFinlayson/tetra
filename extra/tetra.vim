" Vim syntax file for tetra code
" place (or symlink) in .vim/syntax/
" also place 'au BufRead,BufNewFile *.ttr set filetype=tetra' in vimrc
" this is very basic for now

" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

" keywords
syn keyword tetraKeyword if elif else for in parallel while continue break def
syn keyword tetraKeyword or and not pass return background lock constant global
syn keyword tetraKeyword as open import lambda wait self class
syn keyword tetraKeyword init 
syn keyword tetraType int real string bool mutex task none

" comments and TODOs
syn keyword tetraTodo contained TODO FIXME
syn match tetraComment "#.*$" contains=tetraTodo
syn match tetraError '\t'

" values
syn keyword tetraValue true false
syn match tetraValue '\d\+'
syn match tetraValue '[-+]\d\+'
syn match tetraValue '\d\+\.\d*'
syn match tetraValue '[-+]\d\+\.\d*'
syn region tetraValue start='"' end='"' skip='\\"'

" hook it up
hi def link tetraKeyword Keyword
hi def link tetraType Type
hi def link tetraValue Constant
hi def link tetraComment Comment
hi def link tetraTodo Todo
hi def link tetraError Error

let b:current_syntax = "tetra"

