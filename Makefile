NAME	= webserv
CC		= clang++
CFLAGS	= -std=c++98
SRC_PATH= srcs/
OBJ_PATH= objs/
INC_PATH= incs/

MODULES=	Headers Methods Socket Utils
SRC_DIR=	$(addprefix $(SRC_PATH),$(MODULES))
INC_DIR=	$(addprefix $(INC_PATH),$(MODULES))
OBJ_DIRS=	$(addprefix $(OBJ_PATH),$(MODULES))

SRC=	$(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
SRC+= $(SRC_PATH)tcp-server.cpp $(SRC_PATH)Request.cpp

INC= -I incs/ $(foreach sdir,$(INC_DIR),-I $(wildcard $(sdir))/)
OBJ= $(patsubst $(SRC_PATH)%.cpp,$(OBJ_PATH)%.o,$(SRC))
DEF= \033[0m
RED= \033[31;1m
GRN= \033[32;1m
YEL= \033[33;1m

ifeq ($(DB),1)
	CFLAGS	+= -g3
endif

.PHONY: all re clean fclean

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(RED)[$(NAME)] : $(DEF)Compiling..."
	@$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ)
	@echo "$(RED)[$(NAME)] : $(DEF)Compilation $(GRN)[OK]$(DEF)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(OBJ_DIRS) 2> /dev/null || true
	@echo "$(RED)[$(NAME)] :$(DEF) Compiling $@"
	@$(CC) $(CFLAGS) $(INC) -o $@ -c $<

client: $(NAME)
		@$(CC) $(CFLAGS) $(INC) -o $(OBJ_PATH)tcp-client.o -c $(SRC_PATH)tcp-client.cpp
		@$(CC) $(CFLAGS) $(INC) -o client $(OBJ_PATH)tcp-client.o $(OBJ_PATH)/Socket/Socket.o $(OBJ_PATH)/Socket/ClientSocket.o

clean:
	@rm -rf $(OBJ_PATH)
	@echo "$(RED)[$(NAME)] : $(DEF)Cleaning $(GRN)[OK]$(DEF)"

fclean: clean
	@rm -rf $(NAME) client
	@echo "$(RED)[$(NAME)] : $(DEF)Full Cleaning $(GRN)[OK]$(DEF)"

re: fclean all