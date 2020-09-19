#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xfece093d, "module_layout" },
	{ 0x51c503e0, "i2c_del_driver" },
	{ 0x25c0e901, "i2c_register_driver" },
	{ 0x985798bb, "__devm_iio_device_register" },
	{ 0xb1b939e, "kmemdup" },
	{ 0x2d6fcc06, "__kmalloc" },
	{ 0x24d273d1, "add_timer" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xdf9208c0, "alloc_workqueue" },
	{ 0x201a4b32, "__mutex_init" },
	{ 0x85e0d78a, "of_find_property" },
	{ 0x495c0622, "of_property_read_variable_u32_array" },
	{ 0x7618537d, "of_match_node" },
	{ 0xdd0aa5d5, "of_get_next_child" },
	{ 0x958326fc, "_dev_info" },
	{ 0x5f754e5a, "memset" },
	{ 0x4a175f8, "devm_iio_device_alloc" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x12a38747, "usleep_range" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x42160169, "flush_workqueue" },
	{ 0x4205ad24, "cancel_work_sync" },
	{ 0x7e3191f6, "try_to_del_timer_sync" },
	{ 0x37a0cba, "kfree" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x5fc262cb, "mutex_unlock" },
	{ 0x195a71c2, "mutex_lock" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x9d669763, "memcpy" },
	{ 0x349cba85, "strchr" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xb65a643b, "i2c_transfer" },
	{ 0xda02d67, "jiffies" },
	{ 0xb2d48a2e, "queue_work_on" },
	{ 0x43cb86a2, "_dev_err" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "industrialio");

MODULE_ALIAS("of:N*T*Cmicrochip,pac1934");
MODULE_ALIAS("of:N*T*Cmicrochip,pac1934C*");
MODULE_ALIAS("of:N*T*Cmicrochip,pac1932");
MODULE_ALIAS("of:N*T*Cmicrochip,pac1932C*");
MODULE_ALIAS("of:N*T*Cmicrochip,pac1931");
MODULE_ALIAS("of:N*T*Cmicrochip,pac1931C*");
MODULE_ALIAS("i2c:pac1934");
MODULE_ALIAS("i2c:pac1932");
MODULE_ALIAS("i2c:pac1931");

MODULE_INFO(srcversion, "78A9D8599AD13FAF5EC5600");
