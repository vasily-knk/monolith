#include "middefs.h"
#include "brez.h"

void BrezInit (brez_t* p, int x1, int y1, int x2, int y2) {
	if (!p)
		return;
	int deltax = abs(x2 - x1);
	int deltay = abs(y2 - y1);
	if (deltax >= deltay) {
		/* x is independent variable */
		p->numpixels = deltax;
		p->d = (2 * deltay) - deltax;
		p->dinc1 = deltay << 1;
		p->dinc2 = (deltay - deltax) << 1;
		p->xinc1 = 1;
		p->xinc2 = 1;
		p->yinc1 = 0;
		p->yinc2 = 1;
	} else {
		/* y is independent variable */
		p->numpixels = deltay;
		p->d = (2 * deltax) - deltay;
		p->dinc1 = deltax << 1;
		p->dinc2 = (deltax - deltay) << 1;
		p->xinc1 = 0;
		p->xinc2 = 1;
		p->yinc1 = 1;
		p->yinc2 = 1;
	}
	/* Make sure x and y move in the right directions */
	if (x1 > x2) {
		p->xinc1 = - p->xinc1;
		p->xinc2 = - p->xinc2;
	}
	if (y1 > y2) {
		p->yinc1 = - p->yinc1;
		p->yinc2 = - p->yinc2;
	}
	/* Start drawing at  */
	p->x = x1;
	p->y = y1;
}
