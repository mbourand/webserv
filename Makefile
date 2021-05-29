NAME	= webserv
NAME_TESTER	= webserv-tester
CC		= clang++
CFLAGS	= -Wall -Werror -Wextra -std=c++98
SRC_PATH= srcs/
OBJ_PATH= objs/
INC_PATH= incs/

MODULES=	Headers Methods Socket Utils Config
SRC_DIR=	$(addprefix $(SRC_PATH),$(MODULES))
INC_DIR=	$(addprefix $(INC_PATH),$(MODULES))
OBJ_DIRS=	$(addprefix $(OBJ_PATH),$(MODULES))

SRC=	$(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
SRC+= $(SRC_PATH)tcp-server.cpp $(SRC_PATH)Request.cpp  $(SRC_PATH)Response.cpp $(SRC_PATH)VirtualHost.cpp

INC= -I incs/ $(foreach sdir,$(INC_DIR),-I $(wildcard $(sdir))/)
OBJ= $(patsubst $(SRC_PATH)%.cpp,$(OBJ_PATH)%.o,$(SRC))
DEF= \033[0m
RED= \033[31;1m
GRN= \033[32;1m
YEL= \033[33;1m

ifeq ($(DB),1)
	CFLAGS	+= -ggdb -D DEBUG=1
else ifeq ($(DB),2)
	CFLAGS	+= -g3 -fsanitize=address -D DEBUG=1
else ifeq ($(DB),3)
	CFLAGS	= -g3 -std=c++98 -Wall -Wextra -Werror
else ifeq ($(DB),4)
	CFLAGS	+= -g3 -fsanitize=address
endif

.PHONY: all re clean fclean multi re_multi tester

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(RED)[$(NAME)] : $(DEF)Compiling..."
	@$(CC) $(CFLAGS) -lpthread -lz $(INC) -o $(NAME) $(OBJ)
	@echo "$(RED)[$(NAME)] : $(DEF)Compilation $(GRN)[OK]$(DEF)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(OBJ_DIRS) 2> /dev/null || true
	@echo "$(RED)[$(NAME)] :$(DEF) Compiling $@"
	@$(CC) $(CFLAGS) $(INC) -o $@ -c $<

tester:
	@make -C tester
	@mv tester/$(NAME_TESTER) $(NAME_TESTER)

clean:
	@rm -rf $(OBJ_PATH)
	@echo "$(RED)[$(NAME)] : $(DEF)Cleaning $(GRN)[OK]$(DEF)"
	@make clean -C tester

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)[$(NAME)] : $(DEF)Full Cleaning $(GRN)[OK]$(DEF)"
	@rm -rf $(NAME_TESTER)

re: fclean all

multi:
	make all -j

re_multi:
	make fclean -j
	make all -j
