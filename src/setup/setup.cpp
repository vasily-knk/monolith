#include "../common/middefs.h"

int width, height, bpp, fullscreen;

void main () {
	char str[MAX_LOADSTRING];
	printf ("X-Res: ");
	gets (str);
	width=atoi(str);
	printf ("Y-Res: ");
	gets (str);
	height=atoi(str);
	printf ("BPP: ");
	gets (str);
	bpp=atoi(str);
	printf ("Fullscreen? ");
	int c=getch();
	while (c!='y'&&c!='n')
		c=getch();
	if (c=='y')
		fullscreen=true;
	else
		fullscreen=false;
	printf ("%c\n", c);
	printf ("Dll name: ");
	gets(str);
	
	
	FILE* fff=fopen ("vid", "wb");
	fwrite (&width, sizeof(int), 1, fff);
	fwrite (&height, sizeof(int), 1, fff);
	fwrite (&bpp, sizeof(int), 1, fff);
	fwrite (&fullscreen, sizeof(int), 1, fff);
	for (int i=0; str[i]!=0; i++) {
		fputc (str[i],fff);
	}
	fclose (fff);
}