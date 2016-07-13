#include <stdlib.h>
#include <list.h>

/*typedef struct {
	DWORD	Capacity;
	DWORD	Count;
	DWORD	Step;
	PVOID	*Pointers;
} LIST, *PLIST;*/

static BOOL ListSetCapacity(
				PLIST List,
				DWORD NewCapacity)
{
	if (NewCapacity % List->Step)
		NewCapacity += List->Step - (NewCapacity % List->Step);

	if (List->Capacity != NewCapacity)
	{
		if (List->Pointers != NULL)
			List->Pointers = realloc(List->Pointers, NewCapacity * sizeof(PVOID));
		else
			List->Pointers = calloc(NewCapacity, sizeof(PVOID));

		if (!List->Pointers)
			return FALSE;

		List->Capacity = NewCapacity;
	}

	return TRUE;
}

BOOL ListCreate(
		PLIST List,
		DWORD Step,
		DWORD PreAlloc)
{
	List->Capacity = 0;
	List->Count = 0;
	List->Step = Step;
	List->Pointers = NULL;

	if (PreAlloc)
	{
		if (!ListSetCapacity(List, PreAlloc))
			return FALSE;
	}

	return TRUE;
}

BOOL ListAdd(
				PLIST List,
				PVOID Data)
{
	if (List->Count >= List->Capacity)
	{
		if (!ListSetCapacity(List, List->Count + 1))
			return FALSE;
	}

	List->Pointers[List->Count] = Data;
	List->Count++;

	return TRUE;
}

BOOL ListSort(
				PLIST List,
				INT32 (*Compare)(CONST PVOID, CONST PVOID))
{
	qsort(List->Pointers, List->Count, sizeof(PVOID), Compare);

	return TRUE;
}

BOOL ListDestroy(
		PLIST List,
		BOOL Cleanup)
{
	if ( (Cleanup) && (List->Pointers) )
	{
		DWORD	i;

		for(i = 0;i < List->Count;i++)
			free(List->Pointers[i]);
	}

	if (List->Pointers)
		free(List->Pointers);

	List->Capacity = 0;
	List->Count = 0;
	List->Pointers = NULL;

	return TRUE;
}


