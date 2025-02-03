# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nnourine <nnourine@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 16:45:01 by asohrabi          #+#    #+#              #
#    Updated: 2025/02/03 15:09:00 by nnourine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX = c++ -fsanitize=thread -g
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -Iincludes #changed from -std=c++11 to -std=c++17 for filesystem functions of parser

SRCS_DIR = srcs
OBJs_DIR = objs

SRCS = $(wildcard $(SRCS_DIR)/*.cpp)
OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJs_DIR)/%.o)

# Target executable
TARGET = webserv

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(OBJs_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJs_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJs_DIR):
	@mkdir -p $(OBJs_DIR)

clean:
	@rm -rf $(OBJs_DIR)

fclean: clean
	@rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
