# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Jokarrii <jonathan.bayet@gmail.com>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/22 17:05:49 by jbayet            #+#    #+#              #
#    Updated: 2026/08/24 19:46:20 by Jokarrii         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.cpp Wav.cpp printData.cpp

OBJS = $(SRCS:.cpp=.o)
NAME = wav_viewer
CXXFLAGS = -Wall -Wextra -Werror
CXX = c++
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
