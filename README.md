traffic-inspector-board
=======================

Inspector machine that inspects the working status of traffic lights

#Bug Fix: Sudden Reboot Due To Incorrect Use Of Kernel Timer
Dair 4/20/2016 

**问题描述**

TCA300的3G模块有一个pin脚，SOC可以向其发送脉冲方波以此来power on/off 3G模块。
Export出来了一个接口用来开关3G。脉冲波的时延在接口中用timer做实现，以此来延时0.6秒。
不幸的是，timer中的错误参数导致了这个时延变成了600秒。而在这600秒时间内，stress test会不停的开关3G，造成这个timer变量不断的重复着init, add操作。当第一个timer到时的时候，系统reboot。
下面的模块是模拟了这个问题发生的场景。

**代码如下**

*Makefile:*

`obj-m += test.o`

*test.c:*

    #include <linux/module.h>
    #include <linux/delay.h>
    
    struct timer_list dairui_timer;
    
    void timer_recur_function(unsigned long arg)
    {
    	printk("timer_recur_function called\n");
    }
    
    static int __init timer_recur_init(void)
    {
    	int i;
    
    	printk("%lu: %s() called\n",jiffies,__func__);
    	init_timer(&dairui_timer);
    	dairui_timer.expires = jiffies + 30*HZ;
    	dairui_timer.data = 0;
    	dairui_timer.function = timer_recur_function;
    	add_timer(&dairui_timer);

    	for (i = 0; i < 10; i++)
    	{
    		printk("delay 1s, then re-add the timer\n");
    		mdelay(500);
    		init_timer(&dairui_timer);
    		dairui_timer.expires = jiffies + 30*HZ;
    		dairui_timer.data = 0;
    		dairui_timer.function = timer_recur_function;
    		add_timer(&dairui_timer);
    	}
    	return 0;
    }
    module_init(timer_recur_init);
    
    static void __exit timer_recur_exit(void)
    {
    	printk("%lu: %s() called\n",jiffies,__func__);
    }
    module_exit(timer_recur_exit);
    
    MODULE_AUTHOR("Dai Rui <rui.dai@technicolor.com>");
    MODULE_DESCRIPTION("Dair's runtime debug kernel module");
    MODULE_LICENSE("GPL");


**编译kernel module**

    make -C kernel_imx ARCH=arm CROSS_COMPILE="/home/dair/tba300/prebuilts/misc/linux-x86/ccache/ccache /home/dair/tba300/prebuilts/gcc/linux-x86/arm/arm-eabi-4.7/bin/arm-eabi-"  M="drivers/dairui" modules
