cmd_/shared_folder/shared_folder/hw4_kernel/test_mod00.mod := printf '%s\n'   test_mod00.o | awk '!x[$$0]++ { print("/shared_folder/shared_folder/hw4_kernel/"$$0) }' > /shared_folder/shared_folder/hw4_kernel/test_mod00.mod