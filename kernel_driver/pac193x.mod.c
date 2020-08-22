#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
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
__used
__attribute__((section("__versions"))) = {
	{ 0xfeab0981, "module_layout" },
	{ 0x8ed06951, "i2c_del_driver" },
	{ 0x1898ad4a, "i2c_register_driver" },
	{ 0x526448a6, "__devm_iio_device_register" },
	{ 0xe2fae716, "kmemdup" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x24d273d1, "add_timer" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x43a53735, "__alloc_workqueue_key" },
	{ 0x201a4b32, "__mutex_init" },
	{ 0xa45cb8e7, "of_find_property" },
	{ 0xf4b27db6, "of_property_read_variable_u32_array" },
	{ 0x1dadacee, "of_match_node" },
	{ 0xac101bf0, "of_get_next_child" },
	{ 0x8ec826d9, "_dev_info" },
	{ 0x5f754e5a, "memset" },
	{ 0xe8ba4ab, "devm_iio_device_alloc" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x12a38747, "usleep_range" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x42160169, "flush_workqueue" },
	{ 0x4205ad24, "cancel_work_sync" },
	{ 0x7e3191f6, "try_to_del_timer_sync" },
	{ 0x37a0cba, "kfree" },
	{ 0x91715312, "sprintf" },
	{ 0x5fc262cb, "mutex_unlock" },
	{ 0x195a71c2, "mutex_lock" },
	{ 0x373db350, "kstrtoint" },
	{ 0x9d669763, "memcpy" },
	{ 0x349cba85, "strchr" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0xeace2518, "i2c_transfer" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xda02d67, "jiffies" },
	{ 0xa8ebe000, "_dev_err" },
	{ 0xb2d48a2e, "queue_work_on" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=industrialio";

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
