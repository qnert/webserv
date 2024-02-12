C++ = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g
NAME = webserv

GREEN=\033[32m
RESET=\033[0m

SOURCES = ./src/main.cpp ./src/server/Server.cpp  ./src/server/ServerUtils.cpp \
					./src/server/RequestParser.cpp ./src/data_classes/MIME_type.cpp \
					./src/data_classes/Statuscodes.cpp ./src/server/RequestUtils.cpp ./src/cfg/Config.cpp \
					./src/server/ServerResponse.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@$(C++) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJECTS)
	@echo "$(GREEN)Linking...$(RESET)"
	@$(C++) $(OBJECTS) -o $(NAME) -fsanitize=address -static-libsan

clean:
	@rm -f $(OBJECTS)

fclean: clean
	@rm -f $(NAME)

re:	fclean all
