#include <stdio.h>
#include "my_mem.h"

#ifdef _DEBUG

mem_record_t* mem_record_ptr=0;
FILE* mem_file=0;

v_result _MemInit (const char* filename) {
	if ((mem_record_ptr=new mem_record_t)==0)
		return V_FAIL;
	mem_record_ptr->next=0;
	mem_record_ptr->ptr=0;
	mem_record_ptr->size=0;
	if ((mem_file=fopen (filename, "wt"))==0)
		return V_FAIL;
	fprintf (mem_file, "Memory management started.\n");
	fflush (mem_file);
	return V_OK;
}

void _MemClose () {
	if (mem_record_ptr->next!=0) {
		fprintf (mem_file, "There were memory leaks:\n");
		for (mem_record_t* p=mem_record_ptr->next; p!=0; p=p->next) {
			fprintf (mem_file, "   %d bytes at %p (Allocated: %s line %d)\n", p->size, p->ptr, p->filename, p->line);
		}
	}
	fprintf (mem_file, "Memory management finished.\n");
	fclose (mem_file);
}

void _MemFree (void* p, const char* file, int line) {
	for (mem_record_t* p1=mem_record_ptr; p1->next!=0; p1=p1->next) {
		if (p1->next->ptr==p) {
			mem_record_t* temp=p1->next;
			p1->next=temp->next;
			delete[] temp->ptr;
			delete temp;
			return;
		}
	}
	fprintf (mem_file, "MemFree without MemAlloc at (%s, %d). Address: %p\n", file, line, p);
	fflush (mem_file);
}

#endif //_DEBUG