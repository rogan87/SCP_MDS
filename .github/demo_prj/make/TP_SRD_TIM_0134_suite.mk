#
# TimeManagement test suite - runs tests
#

# SUITE_NAME should be the same as the file name
SUITE_NAME := TP_SRD_TIM_0134_suite

# list the files in the suite
$(SUITE_NAME): TEST_SUITE_SRC  := $(SRC_DIR)/TP_SRD_TIM_0134.c

########################################
# Do not modify anything below this line
########################################
$(SUITE_NAME): TEST_SUITE_TMP  := $(subst $(SRC_DIR),$(OBJ_DIR),$(TEST_SUITE_SRC))
$(SUITE_NAME): TEST_SUITE_OBJS := $(TEST_SUITE_TMP:%.c=%.o)
$(SUITE_NAME): TEST_SUITE_ID   := $(OBJ_DIR)/$(SUITE_NAME)_ID.o
$(SUITE_NAME): GEN_SUITE_RULES := $(eval $(call TEST_SUITE_template,$(SUITE_NAME),$(TEST_SUITE_OBJS),$(TEST_SUITE_ID)))
