NAME	=	ircserv

SRCS_DIR	=	src
INCS_DIR	=	includes
OBJS_DIR	=	.objs
DEPS_DIR	=	.deps

SRCS	=	$(wildcard $(SRCS_DIR)/Command/*.cpp) \
			$(wildcard $(SRCS_DIR)/Network/*.cpp) \
			$(SRCS_DIR)/main.cpp
OBJS	=	$(addprefix $(OBJS_DIR)/, $(subst $(SRCS_DIR)/,,$(SRCS:.cpp=.o)))
DEPS	=	$(addprefix $(DEPS_DIR)/, $(subst $(SRCS_DIR)/,,$(SRCS:.cpp=.d)))

CC	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 $(addprefix -I, $(INCS_DIR))
CPPFLAGS	+=	-g -MMD -MP -fsanitize=address

RM	=	rm -rf
MKDIR	=	mkdir -p

# Regla principal
all: directories $(NAME)

# Crear el ejecutable
$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

# Compilar archivos .cpp en .o, y sus dependencias en .d
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(MKDIR) $(dir $@)
	$(MKDIR) $(DEPS_DIR)/$(subst $(SRCS_DIR)/,,$(dir $<))
	$(CC) $(CPPFLAGS) -c $< -o $@ -MF $(DEPS_DIR)/$(subst $(SRCS_DIR)/,,$(<:.cpp=.d))

# Crear los directorios necesarios
directories:
	$(MKDIR) $(OBJS_DIR)
	$(MKDIR) $(DEPS_DIR)

# Limpiar archivos generados
clean:
	$(RM) $(OBJS) $(DEPS)
	$(RM) $(OBJS_DIR) $(DEPS_DIR)

# Limpiar todo
fclean: clean
	$(RM) $(NAME)

# Regenerar todo
re: fclean all

# Incluir archivos de dependencias
-include $(DEPS)
