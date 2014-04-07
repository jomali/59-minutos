! ---------------------------------------------------------------------------- !
!   StringMaker.h by Roger Firth (roger@firthworks.com)
!
!       1.0 Jul 2002
!           Original version.
!
! ---------------------------------------------------------------------------- !
!   Installation: add the line:
!
!       Include "StringMaker";
!
!   near the start of your game file.
!
! ---------------------------------------------------------------------------- !
!
!   The routine StringToChars(s,a) unpacks a string S into an array of
!   characters A, in which the first word (A-->0) holds the length of the
!   string, and the remaining bytes (A->2, A->3, ...) hold the characters.
!   You can also create such an array using output stream 3 (see section 42
!   of the DM4):
!
!       @output_stream 3 A;
!       print ... ;
!       @output_stream -3;
!
!   The routine CharsToString(a,s) reverses the process. Here A is an array
!   of characters in the format created by StringToChars(), and S is an
!   array of words into which the packed string is written. Note that this
!   array will be in the Z-machine's low memory (only the compiler can write
!   into the high memory region) and so therefore you cannot use
!   print (string) S to output the string that you have made; instead, use
!   print (address) S to print it.
!
!   For example:
!
!       Array AAA --> 100;  ! reserve 100 words for a character array
!       Array SSS --> 100;  ! and the same for the string.
!
!       StringToChars("This is^a short string", AAA);
!       AAA->13 = 'm';      ! overwrite "short" with "small"
!       AAA->14 = 'a';
!       AAA->15 = AAA->16 = 'l';
!       AAA->24 = '.';      ! add a full stop on the end
!       AAA-->0 = AAA-->0 + 1;
!       CharsToString(AAA, SSS);
!       print (address) SSS, "^";
!
!   produces:
!
!       This is
!       a small string.

System_file;

Array alphaTable ->
    'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 'u' 'v' 'w' 'x' 'y' 'z'
    'A' 'B' 'C' 'D' 'E' 'F' 'G' 'H' 'I' 'J' 'K' 'L' 'M' 'N' 'O' 'P' 'Q' 'R' 'S' 'T' 'U' 'V' 'W' 'X' 'Y' 'Z'
    ' ' '^' '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' '.' ',' '!' '?' '_' '#' 039 '"' '/' '\' '-' ':' '(' ')';

Global sCount = 0;                              ! current word in string
Global zCount = 0;                              ! current Z-char within word

[ StringToChars s a;        ! unpack a string into an array of ZSCII characters
    (s).print_to_array(a);
    return a-->0;
];

[ CharsToString a s         ! pack ZSCII characters into a Z-character string
    t i;
    t = $0034-->0; if (t == 0) t = alphaTable;  ! address of alphabet table
    sCount = zCount = 0;
    for (i=0 : i<(a-->0) : i++)
        ZSCIItoZchar(a->(i+2), s, t);           ! convert all ZSCII to Z-chars
    switch (zCount) {                           ! fix up last word of string
        1:  s-->sCount = s-->sCount | $80A5;    ! - two pad characters
        2:  s-->sCount = s-->sCount | $8005;    ! - one pad character
        0:  sCount--;
            s-->sCount = s-->sCount | $8000;    ! - no pad characters
        default: "StringMaker BUG1";
    }
    return sCount;
];

[ ZSCIItoZchar c s t        ! convert 10-bit ZSCII char to 5-bit Z-character(s)
    i;
    if (c == 32) {                              ! space
        Zchar(0, s); return;
    }
    if (c == 13) {                              ! newline
        Zchar(5, s); Zchar(7, s); return;
    }
    for (i=0 : i<78 : i++) if (c == t->i)
        switch (i/26) {                         ! found in alpha table
            0:  Zchar(6+i, s);                  ! - A0 (lower case)
                return;
            1:  Zchar(4, s); Zchar(6+i%26, s);  ! - A1 (upper case)
                return;
            2:  Zchar(5, s); Zchar(6+i%26, s);  ! - A2 (punctuation)
                return;
            default: "StringMaker BUG2";
        }
    Zchar(5, s); Zchar(6, s);                   ! not found in alpha table
    Zchar(c/$20, s); Zchar(c&$1F, s); 
];

[ Zchar c s;                ! pack a Z-character into the string being built
    switch (zCount++) {
        0:  s-->sCount = c * $400;
        1:  s-->sCount = s-->sCount + c * $20;
        2:  s-->sCount = s-->sCount + c; sCount++; zCount = 0;
        default: "StringMaker BUG3";
    }
];

! ---------------------------------------------------------------------------- !
