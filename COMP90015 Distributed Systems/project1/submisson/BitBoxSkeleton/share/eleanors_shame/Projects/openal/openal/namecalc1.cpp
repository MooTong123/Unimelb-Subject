#include <stdio.h>
int namecalc(char* string){
	int namecount, aijqy, bkr, cgls, dmt, ehnx, uvw, oz, fp, i;
			i = 0;
		
		namecount = 0;
		
		aijqy = 1;
		bkr = 2;
		cgls = 3;
		dmt = 4;
		ehnx = 5;
		uvw = 6;
		oz = 7;
		fp = 8;

		while (string[i] !='\0') {
			char ch = string[i];
			
			switch(ch) {
			case'a':
			case'i':
			case'j':
			case'q':
			case'y':
			case'A':
			case'I':
			case'J':
			case'Q':
			case'Y':
				namecount += aijqy;
					break;
			case'b':
			case'k':
			case'r':
			case'B':
			case'K':
			case'R':
				namecount += bkr;
				break;
			case'c':
			case'g':
			case'l':
			case's':
			case'C':
			case'G':
			case'L':
			case'S':
				namecount += cgls;
				break;
			case'd':
			case'm':
			case't':
			case'D':
			case'M':
			case'T':
				namecount += dmt;
				break;
			case'e':
			case'h':
			case'n':
			case'x':
			case'E':
			case'H':
			case'N':
			case'X':
				namecount += ehnx;
				break;
			case'u':
			case'v':
			case'w':
			case'U':
			case'V':
			case'W':
				namecount += uvw;
				break;
			case'o':
			case'z':
			case'O':
			case'Z':
				namecount +=oz;
				break;
			case'f':
			case'p':
			case'F':
			case'P':
				namecount +=fp;
				break;
			case' ':
				namecount;
				break;
			default: 
				printf ("error invalid character[%c]", ch);
				scanf("%s", string );
				return (123);
		}
		i++;
	}
	return (namecount);
}

