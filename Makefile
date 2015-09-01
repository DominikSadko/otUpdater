STANDARD_TYPE      = gnu++11
PLATFORM_TYPE      = #-m32

IGNORE_FLAGS       = -fno-strict-aliasing
CXXFLAGS           = -O1 -ftime-report -fmessage-length=0 -static-libgcc -static-libstdc++
DEFINITIONS        = 

LIBS               = -mwindows -lwininet -lComctl32

###########################################################################################################
RELEASE            = updater.exe
RELEASE_DIRECTORY  = release
OBJECTS_DIRECTORY  = objects
SOURCE_DIRECTORY   = source

ifeq ($(BUILD_TYPE),debug)
	RELEASE             = $(RELEASE_DIRECTORY)\debug.exe
	OBJECTS_DIRECTORY   = objects/bebug
	CXXFLAGS		   +=  -g
else ifeq ($(BUILD_TYPE),release)
	RELEASE             = $(RELEASE_DIRECTORY)\updater.exe
	OBJECTS_DIRECTORY   = objects/release
	CXXFLAGS		   +=  
endif

CXXFLAGS      += $(IGNORE_FLAGS) -std=$(STANDARD_TYPE) $(PLATFORM_TYPE)

SOURCE         = $(wildcard $(SOURCE_DIRECTORY)/*.cpp)
SOURCE         += $(wildcard $(SOURCE_DIRECTORY)/unzip/*.cpp)
OBJECTS        = $(patsubst %.cpp, %.o, $(patsubst source%,$(OBJECTS_DIRECTORY)%,$(SOURCE)))

release: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEFINITIONS) -o $(RELEASE) $(OBJECTS) $(LIBS) $(FLAGS) $(DEFINITIONS)

debug: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEFINITIONS) -o $(RELEASE) $(OBJECTS) $(LIBS) $(FLAGS) $(DEFINITIONS)

$(OBJECTS): $(OBJECTS_DIRECTORY)/%.o : $(SOURCE_DIRECTORY)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINITIONS) -c $< $(LIBS) -o $(patsubst %.cpp, %.o, $(patsubst source%,$(OBJECTS_DIRECTORY)%,$<))

clean:
	rmdir     $(subst /,\,$(OBJECTS_DIRECTORY)) /s /q
	md        $(subst /,\,$(OBJECTS_DIRECTORY))
	md        $(subst /,\,$(OBJECTS_DIRECTORY))\unzip
	del       $(subst /,\,$(RELEASE))
