#ifndef INC_117_BREZ_H
#define INC_117_BREZ_H

typedef struct {
	int numpixels,
		d, dinc1, dinc2,
		x, xinc1, xinc2,
		y, yinc1, yinc2;
} brez_t;

void BrezInit (brez_t*, int, int, int, int);

#define BREZ_INCREASE(brez) if((brez).d<0){(brez).d+=(brez).dinc1;(brez).x+=(brez).xinc1;(brez).y+=(brez).yinc1;}else{(brez).d+=(brez).dinc2;(brez).x+=(brez).xinc2;(brez).y+=(brez).yinc2;};(brez).numpixels--;
#define BREZ_FINISHED(brez) ((brez).numpixels<0)


#endif