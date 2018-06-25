#!/bin/sh

check_output(){

if [ "${1/with no line terminators/}" = "$1" ] &&
   [ "${1/with very long lines/}" = "$1" ] && 
   [ "${1/Non-ISO extended-ASCII text/}" = "$1" ] ;then
   return 0
fi
return 1
}

if [ ! -x /usr/bin/strings ] ; then
	echo "need strings binary to operate"
	exit 1
fi

if [ "x$1" = "x" ] || [ "x$2" = "x" ] ;then
	echo "need in- and outputfile as parameter"
	exit 1
fi
if [ ! -e "$1" ] ;then
	echo "cannot open inputfile"
	exit 1
fi
STRINGS="/usr/bin/strings -n 4"
tmpfile=${2}.tmp

# try strings with different encodings

# 7 bit encoding
$STRINGS -es "$1" > "$tmpfile" 
out=$(file "$tmpfile")
check_output "$out"  
if [ 0 -eq $? ]  ;then
	cat "$tmpfile" >> "$2"
fi

# 8 bit encoding
$STRINGS -eS "$1" > "$tmpfile" 
out=$(file "$tmpfile")
check_output "$out" 
if [ 0 -eq $? ]  ;then
	cat "$tmpfile" >> "$2"
fi
# 16 bit LE
$STRINGS -el "$1" > "$tmpfile" 
out=$(file "$tmpfile")
check_output "$out" 
if [ 0 -eq $? ]  ;then
	cat "$tmpfile" >> "$2"
fi
# 32 bit LE
$STRINGS -eL "$1" > "$tmpfile" 
out=$(file "$tmpfile")
check_output "$out" 
if [ 0 -eq $? ]  ;then
	cat "$tmpfile" >> "$2"
	rm -f "$tmpfile"
	exit 0
fi

rm -f "$tmpfile"
exit 1
