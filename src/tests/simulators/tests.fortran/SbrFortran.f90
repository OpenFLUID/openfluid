!*
!
!  This file is part of OpenFLUID software
!  Copyright(c) 2007, INRA - Montpellier SupAgro
!
!
! == GNU General Public License Usage ==
!
!  OpenFLUID is free software: you can redistribute it and/or modify
!  it under the terms of the GNU General Public License as published by
!  the Free Software Foundation, either version 3 of the License, or
!  (at your option) any later version.
!
!  OpenFLUID is distributed in the hope that it will be useful,
!  but WITHOUT ANY WARRANTY; without even the implied warranty of
!  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!  GNU General Public License for more details.
!
!  You should have received a copy of the GNU General Public License
!  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
!
!
! == Other Usage ==
!
!  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
!  license, and requires a written agreement between You and INRA.
!  Licensees for Other Usage of OpenFLUID may use this file in accordance
!  with the terms contained in the written agreement between You and INRA.
!  
!*



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


! ==============================================================================


subroutine multrealmatrix(matrix,dim1,dim2,mult,result)

implicit none
integer dim1
integer dim2
real*8 matrix(dim1,dim2)
real*8 mult
real*8 result(dim1,dim2)
integer i, j, k

do j=1,dim2
  do i=1,dim1
    result(i,j) = matrix(i,j) * mult
  end do
  print*,result(:,j)
end do


return
end
