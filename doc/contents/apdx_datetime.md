# Datetime formats {#apdx_datetime}


OpenFLUID uses the ANSI strftime() standard formats for date time formatting to and from a format string.
As an example, this format string can be used in CSV observer in parameters to customize date formats.  
The format string consists of zero or more conversion specifications and ordinary characters.
A conversion specification consists of a \% character and a terminating conversion character that determines the conversion specification's behaviour.
All ordinary characters are copied unchanged into the array.
<br/>


For example, the nineteenth of April, two-thousand seven, at eleven hours, ten minutes and twenty-five seconds formatted using different format strings:

* `%%d/%%m/%%Y %%H:%%M:%%S` will give `19/04/2007 10:11:25`
* `%%Y-%%m-%%d %%H.%%M` will give `2007-04-19 10.11`
* `%%Y\t%%m\t%%d\t%%H\t%%M\t%%S` will give `2007 04  19  10  11  25`

List of available conversion specifications:

* \%a : locale's abbreviated weekday name. 
* \%A : locale's full weekday name. 
* \%b : locale's abbreviated month name. 
* \%B : locale's full month name. 
* \%c : locale's appropriate date and time representation. 
* \%C : century number (the year divided by 100 and truncated to an integer) as a decimal number [00-99]. 
* \%d : day of the month as a decimal number [01,31]. 
* \%D : same as \%m/\%d/\%y. 
* \%e : day of the month as a decimal number [1,31]; a single digit is preceded by a space. 
* \%h : same as \%b. 
* \%H : hour (24-hour clock) as a decimal number [00,23]. 
* \%I : hour (12-hour clock) as a decimal number [01,12]. 
* \%j : day of the year as a decimal number [001,366]. 
* \%m : month as a decimal number [01,12]. 
* \%M : minute as a decimal number [00,59]. 
* \%n : is replaced by a newline character. 
* \%p : locale's equivalent of either a.m. or p.m. 
* \%r : time in a.m. and p.m. notation; in the POSIX locale this is equivalent to \%I:\%M:\%S \%p. 
* \%R : time in 24 hour notation (\%H:\%M). 
* \%S : second as a decimal number [00,61]. 
* \%t : is replaced by a tab character. 
* \%T : time (\%H:\%M:\%S). 
* \%u : weekday as a decimal number [1,7], with 1 representing Monday. 
* \%U : week number of the year (Sunday as the first day of the week) as a decimal number [00,53]. 
* \%V : week number of the year (Monday as the first day of the week) as a decimal number [01,53]. If the week containing 1 January has four or more days in the new year, then it is considered week 1. Otherwise, it is the last week of the previous year, and the next week is week 1. 
* \%w : weekday as a decimal number [0,6], with 0 representing Sunday. 
* \%W : week number of the year (Monday as the first day of the week) as a decimal number [00,53]. All days in a new year preceding the first Monday are considered to be in week 0. 
* \%x : locale's appropriate date representation. 
* \%X : locale's appropriate time representation. 
* \%y : year without century as a decimal number [00,99]. 
* \%Y : year with century as a decimal number. 
* \%Z : timezone name or abbreviation, or by no bytes if no timezone information exists. 
* \%\% : character \%. 

