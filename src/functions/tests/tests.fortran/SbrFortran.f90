subroutine multrealvalue(value,mult,result)

implicit none
real*8 value
real*8 mult
real*8 result

result=value*mult


return
end


! ==============================================================================


subroutine multintvalue(value,mult,result)

implicit none
integer value
integer mult
integer result

result=value*mult

return
end


! ==============================================================================


subroutine catstrings(str1,str2,resultstr)

implicit none
character*(*) str1
character*(*) str2
character*(*) resultstr

resultstr=str1//' '//str2

return
end
