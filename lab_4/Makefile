SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

MFLAGS = -Wall -Wextra -I$(INC_DIR)
SHARED_FLAGS = -shared -fPIC

LIB1_SRC = $(SRC_DIR)/lib_1.c
LIB2_SRC = $(SRC_DIR)/lib_2.c
STATIC_SRC = $(SRC_DIR)/main_static.c
DINAMIC_SRC = $(SRC_DIR)/main_dinamic.c

LIBS = liba1.so liba2.so
PROG = static dinamic

all: $(LIBS) $(PROG)

liba1.so: $(LIB1_SRC)
	gcc $(SHARED_FLAGS) $(MFLAGS) -o $@ $<

liba2.so: $(LIB2_SRC)
	gcc $(SHARED_FLAGS) $(MFLAGS) -o $@ $<

static: $(STATIC_SRC) liba1.so
	gcc $(MFLAGS) -o $@ $(STATIC_SRC) -L. -la1 -Wl,-rpath,'$$ORIGIN'

dinamic: $(DINAMIC_SRC) liba2.so
	gcc $(MFLAGS) -o $@ $(DINAMIC_SRC) $(DFLAGS)

clean:
	rm -f $(LIBS) $(PROG)