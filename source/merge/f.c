//some attempt to see if this can be dynamically loaded.  right now i'm just satisfying the linker to verify I can make a MIPS only version


void ppc32_init(void){}
void ppc32_exec_run_cpu(void){}
void ppc32_jit_init(void){}
void ppc32_jit_run_cpu(void){}
void ppc32_delete(void){}

void ppc32_set_pvr(void){}
void ppc32_load_elf_image(void){}
void ppc32_reset(void){}
void ppc32_load_bat_array(void){}

void ppc32_trigger_timer_irq(void){}
void ppc32_set_sdr1(void){}
void ppc32_init_page_table(void){}
void ppc32_map_zone(void){}

void ppc32_jit_create_ilt(void){}
void ppc32_exec_create_ilt(void){}

void ppc32_dump_stats(void){}