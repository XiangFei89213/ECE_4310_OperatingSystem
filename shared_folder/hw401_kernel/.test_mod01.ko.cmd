cmd_/shared_folder/shared_folder/hw401_kernel/test_mod01.ko := ld -r -m elf_x86_64 -z noexecstack --build-id=sha1  -T scripts/module.lds -o /shared_folder/shared_folder/hw401_kernel/test_mod01.ko /shared_folder/shared_folder/hw401_kernel/test_mod01.o /shared_folder/shared_folder/hw401_kernel/test_mod01.mod.o;  true