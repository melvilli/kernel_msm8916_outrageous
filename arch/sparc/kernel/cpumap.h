#ifndef _CPUMAP_H
#define _CPUMAP_H

#ifdef CONFIG_SMP
<<<<<<< HEAD
extern void cpu_map_rebuild(void);
extern int  map_to_cpu(unsigned int index);
=======
void cpu_map_rebuild(void);
int map_to_cpu(unsigned int index);
>>>>>>> v3.18
#define cpu_map_init() cpu_map_rebuild()
#else
#define cpu_map_init() do {} while (0)
static inline int map_to_cpu(unsigned int index)
{
	return raw_smp_processor_id();
}
#endif

#endif
