# 目标
TARGET :=out
#build 目录
BUILD_DIR=build

CC :=gcc

SZ :=size
# 头文件目录
INCDIRS:=\
midi_dec/inc \
audio_port/inc \
# 源文件目录
SRCDIRS:=\
user \
midi_dec/src \
audio_port/src

INCLUDE:= $(patsubst %, -I %, $(INCDIRS))

CFILES:=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
# 库
LIBS:=\
-lpthread

OBJECTS:=$(addprefix $(BUILD_DIR)/,$(notdir $(CFILES:.c=.o)))

.PHONLYS:clean

VPATH:=$(SRCDIRS)

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $<  $(INCLUDE) -o $@

$(TARGET):$(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

$(BUILD_DIR):
	mkdir $@

clean:
# echo CFILES=$(CFILES)
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)



