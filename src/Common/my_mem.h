#ifndef INC_MY_MEM_H
#define INC_MY_MEM_H

#include "lowdefs.h"

#ifdef _DEBUG

#include <stdio.h>
#include <string.h>

struct mem_record_t {
	void* ptr;
	int size;
	mem_record_t* next;
	char filename[100];
	int line;
};

extern mem_record_t* mem_record_ptr;
extern FILE* mem_file;

template <class T>
T* _MemAlloc (int n, const char* filename, int line) {
	T* p=new T[n];
	if (p!=0) {
		mem_record_t* p_rec=new mem_record_t;
		if (p_rec==0) {
			delete[] p;
			return 0;
		}
		p_rec->ptr=p;
		p_rec->size=sizeof(T)*n;
		p_rec->next=mem_record_ptr->next;
		strcpy (p_rec->filename, filename);
		p_rec->line=line;
		mem_record_ptr->next=p_rec;
	}
	return p;
}

void _MemFree (void* p, const char*, int);
v_result _MemInit (const char*);
void _MemClose ();

#define MemFree(p) _MemFree((p),__FILE__,__LINE__);
#define MemAlloc(type,n) _MemAlloc<type>(n,__FILE__,__LINE__)
#define MemInit() _MemInit("mem.txt")
#define MemClose _MemClose

#else	//_DEBUG

template <class T>
T* _MemAlloc (int n) {
	return new T[n];
}
inline void _MemFree (void* p) {
	delete[] p;
}
inline void _MemInit (const char*) {}
inline void MemInit () {}
inline void MemClose () {}

#define MemFree(p) _MemFree(p);
#define MemAlloc(type,n) _MemAlloc<type>(n)


#endif	//_DEBUG

#endif