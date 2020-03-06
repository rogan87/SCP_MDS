#
# TimeManagement test suite - runs all tests
#

# SUITE_NAME should be the same as the file name
SUITE_NAME := TimeManagement_alltests

# list the files in the suite
$(SUITE_NAME): TEST_SUITE_SRC  := $(SRC_DIR)/TP_SRD_TIM_0001.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0051.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0053.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0061.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0081.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0082.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0101.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0102.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0111.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0121.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0131.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0132.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0133.c
$(SUITE_NAME): TEST_SUITE_SRC  += $(SRC_DIR)/TP_SRD_TIM_0135.c

########################################
# Do not modify anything below this line
########################################
$(SUITE_NAME): TEST_SUITE_TMP  := $(subst $(SRC_DIR),$(OBJ_DIR),$(TEST_SUITE_SRC))
$(SUITE_NAME): TEST_SUITE_OBJS := $(TEST_SUITE_TMP:%.c=%.o)
$(SUITE_NAME): TEST_SUITE_ID   := $(OBJ_DIR)/$(SUITE_NAME)_ID.o
$(SUITE_NAME): GEN_SUITE_RULES := $(eval $(call TEST_SUITE_template,$(SUITE_NAME),$(TEST_SUITE_OBJS),$(TEST_SUITE_ID)))
