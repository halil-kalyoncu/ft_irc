
NAME		=	ircserv

CPPC		=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98
DEPFLAGS	=	-MT $@ -MMD -MP -MF $(DEPDIR)$*.d
RM			=	rm -f

SRCDIR		=	src/
SRCFILES	=	main.cpp \
				ACommand.cpp \
				Channel.cpp \
				Client.cpp \
				CommandInvite.cpp \
				CommandJoin.cpp \
				CommandKick.cpp \
				CommandMode.cpp \
				CommandNick.cpp \
				CommandPart.cpp \
				CommandPass.cpp \
				CommandPrivmsg.cpp \
				CommandQuit.cpp \
				CommandTopic.cpp \
				CommandUser.cpp \
				Reply.cpp \
				Server.cpp

OBJDIR		=	build/
OBJS		=	$(SRCFILES:%.cpp=$(OBJDIR)%.o)

DEPDIR		=	$(OBJDIR).deps/
DEPS		=	$(SRCFILES:%.cpp=$(DEPDIR)%.d)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPPC) $(CPPFLAGS) -o $@ $(OBJS)
	@echo "$(NAME) compiled"

$(OBJDIR)%.o : $(SRCDIR)%.cpp $(DEPDIR)%.d
	@mkdir -p $(dir $(OBJDIR))
	@mkdir -p $(dir $(DEPDIR))
	@$(CPPC) -c $(CPPFLAGS) $(DEPFLAGS) -o $@ $<

clean:
	@$(RM) $(OBJS) $(DEPS)
	@echo "object files of $(NAME) removed"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME) removed"

re: fclean all

.PHONY: all clean fclean re

$(DEPS):
include $(wildcard $(DEPS))