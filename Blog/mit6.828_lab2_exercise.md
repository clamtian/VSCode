# Exercise 1
> In the file *kern/pmap.c*, you must implement code for the following functions (probably in the order given).
>
> `boot_alloc()`
> 
> `mem_init()` (only up to the call to `check_page_free_list(1)`)
> 
> `page_init()`
> 
> `page_alloc()`
> 
> `page_free()`
>
> `check_page_free_list()` and `check_page_alloc()` test your physical page allocator. You should boot JOS and see whether `check_page_alloc()` reports success. Fix your code so that it passes. You may find it helpful to add your own assert()s to verify that your assumptions are correct.


## `boot_alloc()`

我们首先来实现`boot_alloc()`，这个函数的作用是申请n个字节的连续内存，并且返回所申请空间的开始地址：

```Javascript
static void *
boot_alloc(uint32_t n)
{
	static char *nextfree;
	char *result;

	if (!nextfree) {
		extern char end[];
		nextfree = ROUNDUP((char *) end, PGSIZE);
	}

	if(n == 0) return nextfree;
	result = nextfree;
	nextfree = ROUNDUP(nextfree + n, PGSIZE);
	if(nextfree <= (char *)0xf0400000) return result;

	nextfree = result;
	panic("boot_alloc : out of memory, nothing changed, return NULL...\n");

	return NULL;
}
```
`nextfree`指针首先被初始化为紧挨着 Kernel Code 之后的一个页帧。`ROUNDUP`的作用是使`nextfree`指针式中对准4KB(一个页帧)的整数倍。由于我们目前只映射了虚拟地址 0xf0000000:0xf0400000，所以一旦超出 0xf0400000 我们需要提醒错误。

## `mem_init()`

接下来我们需要在`mem_init()`函数中添加代码，完成的功能是分配一块内存，用来存放一个`struct PageInfo`的数组，数组中的每一个`PageInfo`代表内存当中的一页。操作系统内核就是通过这个数组来追踪所有内存页的使用情况的。

```JavaScript
  	pages = (struct PageInfo *)boot_alloc(npages * sizeof(struct PageInfo));
	memset(pages, 0, npages * sizeof(struct PageInfo));
```

这部分已经在解析篇中详细介绍过了，这里不再赘述。

## `page_init()`

接下来是`page_init()`函数，这个子函数的功能包括初始化`pages`数组和`pages_free_list`链表，这个链表中存放着所有空闲页的信息。我们可以到这个函数的定义处具体查看，整个函数是由一个循环构成，它会遍历所有内存页所对应的在`npages`数组中的`PageInfo`结构体，并且根据这个页当前的状态来修改这个结构体的状态，如果页已被占用，那么要把`PageInfo`结构体中的`pp_ref`属性置一；如果是空闲页，则要把这个页加入`pages_free_list`链表中。

```JavaScript
void
page_init(void)
{
	size_t i;
	page_free_list = NULL;
	pages[0].pp_ref = 1;
	pages[0].pp_link = NULL;
	size_t index_IOhole_begin = npages_basemem;
	size_t index_alloc_end = PADDR(boot_alloc(0)) / PGSIZE;

	for (i = 1; i < npages; i++) {
		if(i >= index_IOhole_begin && i < index_alloc_end){
			pages[i].pp_ref = 1;
			pages[i].pp_link = NULL;
		}else{
			pages[i].pp_ref = 0;
			pages[i].pp_link = page_free_list;
			page_free_list = &pages[i];
		}
	}
}
```
其中`index_IOhole_begin`和`index_alloc_end`分别是IOhole的起始地址以及`nextfree`指针所在的地址，在解析篇中我们说过，目前为止内存只有开头的一个页帧以及IOhole和内核段被使用，所以这些区域不应该被插入到`page_free_list`链表中。

## `page_alloc()`

`page_alloc()`函数功能是分配一个物理页，然后返回这个物理页所对应的`PageInfo`结构体。

```JavaScript
void
struct PageInfo *
page_alloc(int alloc_flags)
{
	struct PageInfo *res = page_free_list;
	if(!res) return NULL;
	page_free_list = page_free_list->pp_link;
	res->pp_link = NULL;
	if(alloc_flags & ALLOC_ZERO){
		memset(page2kva(res), '\0', PGSIZE);
	}
	return res;
}
```

我们首先从`free_page_list`中取出一个空闲页的`PageInfo`结构体，然后修改链表表头，接着需要修改取出的空闲页的`PageInfo`结构体信息，初始化该页的内存。

## `page_free()`

`page_free()`函数的功能就是把一个页的`PageInfo`结构体插入回`page_free_list`空闲页链表，代表回收了这个页。

```JavaScript
void
page_free(struct PageInfo *pp)
{
	if(!pp || pp->pp_ref != 0 || pp->pp_link){
		panic("page_free : Invalid address, nothing changed...\n");
		return;
	}
	pp->pp_link = page_free_list;
	page_free_list = pp;
}
```

我们首先需要修改被回收的页的`PageInfo`结构体的相应信息，然后把该结构体插入回`page_free_list`空闲页链表。


# Exercise 2 and Exercise 3

> 一些理论和操作性的东西，理论篇已经分析过了。


# Exercise 4
> In the file *kern/pmap.c*, you must implement code for the following functions.
>
> `pgdir_walk()`
> 
> `boot_map_region()`
>        
> `page_lookup()`
>        
> `page_remove()`
>        
> `page_insert()`
>	
> `check_page()`, called from `mem_init()`, tests your page table management routines. You should make sure it reports success before proceeding.

## `pgdir_walk()`

这个函数的原型是 `pgdir_walk(pde_t *pgdir, const void *va, int create)`，该函数的功能:给定一个页目录表指针 `pgdir` ，该函数应该返回线性地址`va`所对应的页表项指针。这个`pgdir`其实就是part1中我们初始化的`kern_pgdir`指针，它指向JOS中惟一的页目录表。所以在这里我们应该完成以下几个步骤：
* 通过页目录表求得这个虚拟地址页目录项地址 `pg_dir_entry`；
* 判断这个页目录项对应的页表是否已经在内存中；
* 如果在，计算这个页表的基地址`page_table`，然后返回`va`所对应页表项的地址 `&page_table[PTX(va)]``;
* 如果不在则分配新的页，并且把这个页的信息添加到页目录项`pg_dir_entry`中;
* 如果create为false，则返回NULL。

```JavaScript
pte_t *
pgdir_walk(pde_t *pgdir, const void *va, int create)
{
	pde_t pgdir_index = PDX(va);
	pde_t pgtab_index = PTX(va);
	pde_t pg_offset = PGOFF(va);
	pde_t *pgdir_add = &pgdir[pgdir_index];
	
	if(!(*pgdir_add & PTE_P)){
		if(!create) return NULL;
		struct PageInfo* new_page = page_alloc(ALLOC_ZERO);
		if(!new_page) return NULL;
		new_page->pp_ref++;
		*pgdir_add = (page2pa(new_page) | PTE_P | PTE_W | PTE_U);
	}
	pde_t *pgtab = KADDR(PTE_ADDR(*pgdir_add));
	return &pgtab[pgtab_index];
}
```

## `boot_map_region()`

函数原型 `static void boot_map_region(pde_t *pgdir, uintptr_t va, size_t size, physaddr_t pa, int perm)`，这个函数的功能是把虚拟地址空间范围`[va, va+size)`映射到物理空间`[pa, pa+size)`的映射关系加入到页表`pgdir`中。这个函数主要的目的是为了设置虚拟地址UTOP之上的地址范围，这一部分的地址映射是静态的，在操作系统的运行过程中不会改变，所以这些页的`PageInfo`结构体中的`pp_ref`域的值不会发生改变。

```JavaScript
static void
boot_map_region(pde_t *pgdir, uintptr_t va, size_t size, physaddr_t pa, int perm)
{
	uint32_t len = size / PGSIZE;
	for(uint32_t i = 0; i < len; ++i){
		pte_t *pg_table_entry = pgdir_walk(pgdir, (void *)va, 1);
		if(!pg_table_entry){
			panic("boot_map_region : something error happened...\n");
			return;
		}
		*pg_table_entry = pa | perm | PTE_P;
		pa += PGSIZE;
		va += PGSIZE;
	}
}
```

## `page_lookup()`
接下来继续完成`page_lookup()`函数，函数原型`struct PageInfo * page_lookup(pde_t *pgdir, void *va, pte_t **pte_store)`， 函数的功能为返回虚拟地址`va`所映射的物理页的`PageInfo`结构体的指针，如果`pte_store`参数不为0，则把这个物理页的页表项地址存放在`pte_stor`e中。

```JavaScript
struct PageInfo *
page_lookup(pde_t *pgdir, void *va, pte_t **pte_store)
{
	pte_t *pg_table_entry = pgdir_walk(pgdir, (void *)va, 0);
	if(!pg_table_entry || !(*pg_table_entry & PTE_P)) return NULL;

	struct PageInfo *page = pa2page(PTE_ADDR(*pg_table_entry));
	if(pte_store) *pte_store = pg_table_entry ;

	return page;
}
```

## `page_remove()`

`page_remove`函数，它的原型是`void page_remove(pde_t *pgdir, void *va)`，功能就是把虚拟地址va和物理页的映射关系删除。

```JavaScript
void
page_remove(pde_t *pgdir, void *va)
{
	pte_t *entry = NULL;
	struct PageInfo *page = page_lookup(pgdir, va, &entry);
	tlb_invalidate(pgdir, va);
	page_decref(page);
	*entry = 0;
}
```

## `page_insert()`

接下来继续查看`page_insert()`，函数原型 `page_insert(pde_t *pgdir, struct PageInfo *pp, void *va, int perm)`，把一个物理内存中页`pp`与虚拟地址`va`建立映射关系。
* 首先通过`pgdir_walk`函数求出虚拟地址`va`所对应的页表项；
* 修改`pp_ref`的值；
* 查看这个页表项，确定`va`是否已经被映射，如果被映射，则删除这个映射；
* 把`va`和`pp`之间的映射关系加入到页表项中。

```JavaScript
int
page_insert(pde_t *pgdir, struct PageInfo *pp, void *va, int perm)
{
	pte_t *pte = pgdir_walk(pgdir, va, 1);
	if (pte == NULL) {
	    return -E_NO_MEM;
	}
	if (*pte & PTE_P) {
	    if (PTE_ADDR(*pte) != page2pa(pp)) {
            page_remove(pgdir, va);
        }
	}
	if (PTE_ADDR(*pte) != page2pa(pp)) {
	    ++pp->pp_ref;
	}
	*pte = page2pa(pp) | perm | PTE_P;
	return 0;
}
```



