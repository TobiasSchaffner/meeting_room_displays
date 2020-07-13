#ifndef POWER_H
#define POWER_H

#if defined(CONFIG_DISPLAY)
int power_init(void);
void power_suspend(int seconds);

void on_power_suspend(void);
void on_power_resume(void);
#else
int power_init(void) { return 0; }
void power_suspend(int seconds) { }
#endif // 

#endif