#ifndef INC_117_PARSER_H
#define INC_117_PARSER_H

extern bool CC_Digit (char c);
extern bool CC_DigitNeg (char c);
extern bool CC_DigitFloat (char c);
extern bool CC_Letter (char c);
extern bool CC_ID (char c);
extern bool CC_NotSpace (char c);

v_result ParseFile (FILE*);
void ParseOutput (char*, int m=MAX_LOADSTRING);
v_result ParseSkipWhile (char);
v_result ParseSkipWhile (bool (__cdecl*)(char));
v_result ParseSkipUntil (char);
v_result ParseSkipUntil (bool (__cdecl*)(char));
v_result ParseReadWhile (char);
v_result ParseReadWhile (bool (__cdecl*)(char));
v_result ParseReadUntil (char);
v_result ParseReadUntil (bool (__cdecl*)(char));
v_result ParseSkipChar ();
v_result ParseReadChar ();
char ParseLastChar ();

#endif