TEST_GEN_PROGS := copy_unaligned copy_first_unaligned paste_unaligned \
	paste_last_unaligned alignment_handler

top_srcdir = ../../../../..
include ../../lib.mk

$(TEST_GEN_PROGS): ../harness.c ../utils.c copy_paste_unaligned_common.c
