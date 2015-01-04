#ifndef INC_117_LIST_H
#define INC_117_LIST_H

#ifdef DEF_117_LIST_DEBUG
#include <stdio.h>
#endif

template <class T> class C117List;

template <class T>
class C117ListItem  {
public:
	C117ListItem () {owner=0; next=prev=0;}
	C117ListItem (C117List<T>* new_owner, C117ListItem<T>* new_prev, C117ListItem<T>* new_next) {
		owner=new_owner; next=new_next; prev=new_prev;
	}
public:
	C117List<T>* GetOwner () {return owner;}
	C117ListItem<T>* GetNext () {return next;}
	C117ListItem<T>* GetPrev () {return prev;}
	bool IsHead () {return (prev==0);}
	bool IsTail () {return (next==0);}
	bool IsOnly () {return (next==0 && prev==0);}
public:
	T& GetVal ()						{return val;						}
	T& SetVal (const T& new_val)	{val=new_val;	}
public:
	C117List<T>* owner;
	C117ListItem *next, *prev;
	T val;
};

template <class T>
class C117List {
public:
	typedef C117ListItem<T>* C117ListIT;
public:
	C117List	() {head=tail=0;};
	~C117List	() {Clear();}
public:
	bool IsEmpty () {return (head==0);}
	C117ListIT GetHead () {return head;}
	C117ListIT GetTail () {return tail;}
	bool CheckItem (C117ListIT p) {
		if (!p) return true;
		return p->owner==this;
	}
public:
	C117ListIT AddFirst	(const T&);
	C117ListIT AddLast	(const T&);
	C117ListIT AddAfter	(C117ListIT, const T&);
	C117ListIT AddBefore	(C117ListIT, const T&);
	bool RemoveFirst ();
	bool RemoveLast ();
	bool Remove (C117ListIT);
	void Clear();
public:
	C117ListIT Find (const T&);
public:
	C117ListIT head, tail;
};

template <class T> 
C117ListItem<T>* C117List<T>::AddFirst (const T& val) {
	C117ListIT p = new C117ListItem<T> (this, 0, head);
	if (!p) return 0;
	p->val=val;
	if (head) head->prev=p;
	head=p;
	if (!tail) tail=p;
#ifdef DEF_117_LIST_DEBUG
	printf ("Adding FIRST element: %d\n", val);
#endif
	return p;
}

template <class T> 
C117ListItem<T>* C117List<T>::AddLast (const T& val) {
	C117ListIT p = new C117ListItem<T> (this, tail, 0);
	if (!p) return 0;
	p->val=val;
	if (tail) tail->next=p;
	tail=p;
	if (!head) head=p;
#ifdef DEF_117_LIST_DEBUG
	printf ("Adding LAST element: %d\n", val);
#endif
	return p;
}

template <class T> 
C117ListItem<T>* C117List<T>::AddAfter	(C117ListIT p0, const T& val) {
	if (!CheckItem(p0)) return 0;
	if (p0==tail || p0==0)
		return AddLast (val);
	C117ListIT p = new C117ListItem<T> (this, p0, p0->next);
	if (!p) return 0;
	p->val=val;
	p0->next->prev=p;
	p0->next=p;
#ifdef DEF_117_LIST_DEBUG
	printf ("Adding %d after %d\n", val, p0->val);
#endif
	return p;
}

template <class T> 
C117ListItem<T>* C117List<T>::AddBefore	(C117ListIT p0, const T& val) {
	if (!CheckItem(p0)) return 0;
	if (p0==head || p0==0)
		return AddFirst (val);
	C117ListIT p = new C117ListItem<T> (this, p0->prev, p0);
	if (!p) return 0;
	p->val=val;
	p0->prev->next=p;
	p0->prev=p;
#ifdef DEF_117_LIST_DEBUG
	printf ("Adding %d before %d\n", val, p0->val);
#endif
	return p;
}

template <class T> 
bool C117List<T>::RemoveFirst () {
	if (!head) return false;
	C117ListIT p=head->next;
	if (p) p->prev=0;
#ifdef DEF_117_LIST_DEBUG
	printf ("Removing FIRST element: %d\n", head->val);
#endif
	delete (head);
	head=p;
	if (!head) tail=0;
	return true;
}

template <class T> 
bool C117List<T>::RemoveLast () {
	if (!tail) return false;
	C117ListIT p=tail->prev;
	if (p) p->next=0;
#ifdef DEF_117_LIST_DEBUG
	printf ("Removing LAST element: %d\n", tail->val);
#endif
	delete (tail);
	tail=p;
	if (!tail) head=0;
	return true;
}

template <class T> 
bool C117List<T>::Remove (C117ListIT p0) {
	if (!CheckItem(p0)) return false;
	if (p0==head) return RemoveFirst();
	if (p0==tail) return RemoveLast();
	p0->prev->next=p0->next;
	p0->next->prev=p0->prev;
#ifdef DEF_117_LIST_DEBUG
	printf ("Removing element: %d\n", p0->val);
#endif
	delete (p0);
	return true;
}

template <class T> 
void C117List<T>::Clear() {
	while (RemoveLast ()) {};
}

template <class T> 
C117ListItem<T>* C117List<T>::Find (const T& val) {
	C117ListIT ptr=head;
	while (ptr) {
		if (ptr->val == val)
			return ptr;
		ptr=ptr->next;
	}
	return 0;
}


#endif