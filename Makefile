C++ = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g # -fsanitize=address
NAME = webserv

GREEN=\033[32m
RESET=\033[0m

SOURCES = ./src/main.cpp ./src/server/Server.cpp  ./src/utils/ServerUtils.cpp \
					./src/request/RequestParser.cpp ./src/data_classes/MIME_type.cpp \
					./src/data_classes/Statuscodes.cpp ./src/request/RequestUtils.cpp ./src/cfg/Config.cpp \
					./src/response/ErrorResponses.cpp ./src/server/Clients.cpp ./src/server/CGI.cpp \
					./src/server/ServerManager.cpp ./src/utils/ManagerUtils.cpp ./src/response/ResponseMethods.cpp \
					./src/response/ProcessResponse.cpp ./src/cfg/Locations.cpp ./src/utils/Utils.cpp ./src/utils/CGI_Utils.cpp

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
