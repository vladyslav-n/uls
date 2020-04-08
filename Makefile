NAME = uls

INC = inc/uls.h

SRCR = acc_stat_cmp.c \
	birth_size_cmp.c \
	color.c \
	display1.c \
	fts.c \
	fts_children.c \
	main.c \
	mx_bBcClqtTuUvwx1.c \
	mx_display.c \
	mx_getopt.c \
	mx_parse_flags.c \
	mx_printcol.c \
	mx_printlong.c \
	mx_printsize.c \
	mx_printtime.c \
	mx_printtype.c \
	mx_sort_fts_chlist.c \
	mx_traverse.c \
	name_mod_str_cmp.c \
	nonprintables.c \
	print.c \
	util.c \
	util1.c \
	util2.c \
	util3.c \
	util4.c \
	util5.c \
	util6.c \
	util7.c \
	util8.c \
	
SRC = $(addprefix src/, $(SRCR))

OBJR = $(SRCR:.c=.o)

OBJ = $(addprefix obj/, $(OBJR))

FLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

LIB = libmx/libmx.a

all: install

install: clean
	@make -C libmx
	@mkdir obj
	@clang $(FLAGS) -c $(SRC)
	@mv $(OBJR) obj
	@clang $(FLAGS) $(OBJ) $(LIB) -o $(NAME)
	@make clean

uninstall: clean
	@rm -rf $(NAME)
	@make uninstall -C libmx/

clean:
	@rm -rf uls.dSYM
	@rm -rf $(OBJR)
	@rm -rf obj/
	@make clean -C libmx/

reinstall: uninstall install

debug: uninstall
	@make -C libmx
	@cp $(LIB) .
	@clang $(FLAGS) $(LIB) $(SRC) -o $(NAME) --debug
