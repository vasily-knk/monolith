#include "../common/middefs.h"
#include "parser.h"

bool CC_Digit (char c) {return (c>='0'&&c<='9');}
bool CC_DigitNeg (char c) {return (c>='0'&&c<='9') || (c=='-');}
bool CC_DigitFloat (char c) {return (c>='0'&&c<='9') || (c=='-') || (c=='.');}
bool CC_Letter (char c) {return (c>='a'&&c<'z') || (c>='A'&&c<='z');}
bool CC_ID (char c) {return CC_Digit(c)||CC_Letter(c)||(c=='_');}
bool CC_NotSpace (char c) {return (c>' ' && c<127);}

FILE* parse_file=0;
char* parse_dst=0;
int parse_max=0;
int parse_lastchar;

v_result ParseFile(FILE* fff) {
	parse_file=fff;
	if (!fff)
		return V_FAIL;
	return V_OK;
}
void ParseOutput (char* dst, int max) {
	parse_dst=dst;
	parse_max=max;
}

v_result ParseSkipWhile (char c) {
	if (!parse_file) return V_FAIL;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (parse_lastchar==c) {
		if (parse_lastchar==EOF) return V_FAIL;
		parse_lastchar=getc(parse_file);
	}
	if (parse_lastchar!=EOF)
		fseek (parse_file, -1, SEEK_CUR);
	return V_OK;
}

v_result ParseSkipWhile (bool (*func)(char)) {
	if (parse_file==0 || func==0) return V_FAIL;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (func(parse_lastchar)) {
		if (parse_lastchar==EOF) return V_FAIL;
		parse_lastchar=getc(parse_file);
	}
	fseek (parse_file, -1, SEEK_CUR);
	return V_OK;

}
v_result ParseSkipUntil (char c) {
	if (!parse_file) return V_FAIL;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (parse_lastchar!=c) {
		if (parse_lastchar==EOF) return V_FAIL;
		parse_lastchar=getc(parse_file);
	}
	if (parse_lastchar!=EOF)
		fseek (parse_file, -1, SEEK_CUR);
	return V_OK;
}

v_result ParseSkipUntil (bool (*func)(char)) {
	if (parse_file==0 || func==0) return V_FAIL;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (!func(parse_lastchar)) {
		if (parse_lastchar==EOF) return V_FAIL;
		parse_lastchar=getc(parse_file);
	}
	if (parse_lastchar!=EOF)
		fseek (parse_file, -1, SEEK_CUR);
	return V_OK;
}

v_result ParseReadWhile (char c) {
	if (parse_file==0 || parse_dst==0) return V_FAIL;
	int pos=0;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (parse_lastchar==c) {
		if (parse_lastchar==EOF||pos>=parse_max-1) {
			parse_dst[pos]=0;
			return V_FAIL;
		}
		parse_dst[pos]=parse_lastchar;
		parse_lastchar=getc(parse_file);
		pos++;
	}
	parse_dst[pos]=0;
	if (parse_lastchar!=EOF)
		fseek (parse_file, -1, SEEK_CUR);
	return V_OK;
}

v_result ParseReadWhile (bool (*func)(char)) {
	if (parse_file==0 || parse_dst==0) return V_FAIL;
	int pos=0;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (func(parse_lastchar)) {
		if (parse_lastchar==EOF||pos>=parse_max-1) {
			parse_dst[pos]=0;
			return V_FAIL;
		}
		parse_dst[pos]=parse_lastchar;
		parse_lastchar=getc(parse_file);
		pos++;
	}
	parse_dst[pos]=0;
	if (parse_lastchar!=EOF)
		fseek (parse_file, -1, SEEK_CUR);
	return V_OK;
}

v_result ParseReadUntil (char c) {
	if (parse_file==0 || parse_dst==0) return V_FAIL;
	int pos=0;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (parse_lastchar!=c) {
		if (parse_lastchar==EOF||pos>=parse_max-1) {
			parse_dst[pos]=0;
			return V_FAIL;
		}
		parse_dst[pos]=parse_lastchar;
		parse_lastchar=getc(parse_file);
		pos++;
	}
	parse_dst[pos]=0;
	if (parse_lastchar!=EOF)
		fseek (parse_file, -1, SEEK_CUR);
	return V_OK;
}

v_result ParseReadUntil (bool (*func)(char)) {
	if (parse_file==0 || parse_dst==0) return V_FAIL;
	int pos=0;
	parse_lastchar=getc(parse_file);
	if (parse_lastchar==EOF)
		return V_FAIL;
	while (!func(parse_lastchar)) {
		if (parse_lastchar==EOF||pos>=parse_max-1) {
			parse_dst[pos]=0;
			return V_FAIL;
		}
		parse_dst[pos]=parse_lastchar;
		parse_lastchar=getc(parse_file);
		pos++;
	}
	parse_dst[pos]=0;
	if (parse_lastchar!=EOF)
		fseek (parse_file, -1, SEEK_CUR);
	return V_OK;
}

v_result ParseSkipChar () {
	if (parse_file==0) return V_FAIL;
	if (getc(parse_file)==EOF) return V_FAIL;
	return V_OK;
}

v_result ParseReadChar () {
	if (parse_file==0 || parse_dst==0 || parse_max<=2) return V_FAIL;
	int c;
	if ((c=getc(parse_file))==EOF) return V_FAIL;
	parse_dst[0]=c;
	parse_dst[1]=0;
	return V_OK;
}

char ParseLastChar () {
	return parse_lastchar;
}