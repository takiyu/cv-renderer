TARGET   = renderer.out
INCLUDES = 
NOMAKEDIR= .git%
OBJDIR = objs
OPTION := -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_imgproc -lopencv_ml -lopencv_video -lGL -lglut


GCC = g++ -fopenmp
CFLAGS = -O3
D_OPTION = -MD

CPPS = $(shell find * | grep '.cpp$$')
SRCS = $(filter-out $(IGNORE), $(filter-out $(NOMAKEDIR), $(CPPS)))
DIRS = $(dir $(SRCS))
BINDIRS = $(addprefix $(OBJDIR)/, $(DIRS))

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)
TILS = $(SRCS:.cpp=.cpp~)

ifeq "$(strip $(OBJDIR))" ""
  OBJDIR = .
endif

ifeq "$(strip $(DIRS))" ""
  OBJDIR = .
endif

default:
	@[ -d  $(OBJDIR)   ] || mkdir -p $(OBJDIR)
	@[ -d "$(BINDIRS)" ] || mkdir -p $(BINDIRS)
	@make all --no-print-directory
	./$(TARGET)

all : $(OBJS) $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(GCC) -o $@ $^ $(OPTION)

$(OBJDIR)/%.o: %.cpp
	$(GCC) $(CFLAGS) $(INCLUDES) -o $@ -c $< $(D_OPTION)

clean:
	@rm -rf $(TARGET) $(TILS) $(OBJDIR)

-include $(DEPS)
