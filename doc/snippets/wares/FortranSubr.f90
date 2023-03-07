subroutine displayvector(Fsize,vect)

implicit none

integer Fsize,i
real*8 vect(Fsize)

write(*,*) 'size',Fsize

do i=1,Fsize
  write(*,*) vect(i)
end do

return

end