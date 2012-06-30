#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <asm/page.h>
#include <asm/pgtable.h>

MODULE_LICENSE("GPL");

static unsigned long transform1(unsigned vaddr)
{

unsigned long real_addr;

struct task_struct *cur_task = get_current();//��ȡ��ǰ���̿��ƿ�

struct mm_struct *mm = cur_task ->  mm;//��������ռ�

pgd_t *pgd;//����ҳȫ��Ŀ¼��

pmd_t *pmd;//����ҳ�м���

pte_t *pte;//ҳ����



pgd = pgd_offset(mm, addr);//�ҳ�����Ŀ¼

if (pgd_none(*pgd)){

        goto out;

}

pmd = pmd_offset(pgd, addr);//�ҳ������м���



if (pmd_none(*pmd)){

    goto out;

}

pte = pte_offset(pmd, addr);//�ҳ�����ҳ��





if (pte_none(*pte)) {

    goto out;

}



//����ÿҳ4KB

real_addr = vaddr & 0x00003fff; //ȡ��ҳ��ƫ����

real_addr += pte;//�ں˿ռ���ʵĵ�ַ

real_addr -= PAGE_OFFSET;//���������ַ()

return real_addr;



out:

printk("û���ڴ�ӳ��",real_addr);

}

unsigned long base;
__asm
{
    MOV   EAX,   CR3
    MOV   base,   EAX
}

static unsigned long transform2(unsigned long base,unsigned long vaddr)
{
unsigned long VAddress = (unsigned long)vaddr;

unsigned long PDE,PTE,PAddress;

PDE = Base[VAddress>>22];

if((PDE&1)==0)

return 0;

unsigned long PageFlage = PDE&0x00000080;

if (PageFlage != 0)

{

//PSλ��Ϊ�㣬����4MB��ҳ��ʽ

PAddress = (PDE&0xffc00000) + (VAddress&0x003fffff);

}

else

{

//PSλΪ�㣬����4kb��ҳ��ʽ

PDE = (unsigned long)MapViewOfFile(hmp,4,0,PDE&0xfffff000,0x1000);//��һ���ļ�ӳ�����ӳ�䵽��ǰӦ�ó���ĵ�ַ�ռ�

PTE = ((unsigned long)PDE)[(VAddress&0x003ff000)>>12];

if((PTE&1) == 0)

return 0;

PAddress = (PTE&0xfffff000)+(VAddress&0x00000fff);

UnmapViewOfFile((void)PDE);//�ڵ�ǰӦ�ó�����ڴ��ַ�ռ�����һ���ļ�ӳ������ӳ��

}

return (unsigned long)PAddress;
}

static int __init trans_init(void)
{
	//printk("KERN_ALERT Hello World!\n");
	return 0;
}

static void __exit trans_exit(void)
{
	//printk(KERN_ALERT "Goodbye World!\n");
}

module_init(trans_init);
module_exit(trans_exit);
