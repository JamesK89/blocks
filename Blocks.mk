##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Blocks
ConfigurationName      :=Debug
WorkspacePath          :=/home/james/Source/Portfolio/Blocks
ProjectPath            :=/home/james/Source/Portfolio/Blocks
IntermediateDirectory  :=./out
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=James John Kelly Jr.
Date                   :=08/02/19
CodeLitePath           :=/home/james/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Blocks.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./inc $(IncludeSwitch)/usr/include $(IncludeSwitch)/usr/include/SDL2 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)SDL2 $(LibrarySwitch)SDL2main 
ArLibs                 :=  "SDL2" "SDL2main" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_states_gs_game_play.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_states_gs_main_menu.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_vector3.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shapes.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_blocks.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./out || $(MakeDirCommand) ./out


$(IntermediateDirectory)/.d:
	@test -d ./out || $(MakeDirCommand) ./out

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_states_gs_game_play.cpp$(ObjectSuffix): src/states/gs_game_play.cpp $(IntermediateDirectory)/src_states_gs_game_play.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/james/Source/Portfolio/Blocks/src/states/gs_game_play.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_states_gs_game_play.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_states_gs_game_play.cpp$(DependSuffix): src/states/gs_game_play.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_states_gs_game_play.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_states_gs_game_play.cpp$(DependSuffix) -MM src/states/gs_game_play.cpp

$(IntermediateDirectory)/src_states_gs_game_play.cpp$(PreprocessSuffix): src/states/gs_game_play.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_states_gs_game_play.cpp$(PreprocessSuffix) src/states/gs_game_play.cpp

$(IntermediateDirectory)/src_states_gs_main_menu.cpp$(ObjectSuffix): src/states/gs_main_menu.cpp $(IntermediateDirectory)/src_states_gs_main_menu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/james/Source/Portfolio/Blocks/src/states/gs_main_menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_states_gs_main_menu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_states_gs_main_menu.cpp$(DependSuffix): src/states/gs_main_menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_states_gs_main_menu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_states_gs_main_menu.cpp$(DependSuffix) -MM src/states/gs_main_menu.cpp

$(IntermediateDirectory)/src_states_gs_main_menu.cpp$(PreprocessSuffix): src/states/gs_main_menu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_states_gs_main_menu.cpp$(PreprocessSuffix) src/states/gs_main_menu.cpp

$(IntermediateDirectory)/src_vector3.cpp$(ObjectSuffix): src/vector3.cpp $(IntermediateDirectory)/src_vector3.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/james/Source/Portfolio/Blocks/src/vector3.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_vector3.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_vector3.cpp$(DependSuffix): src/vector3.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_vector3.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_vector3.cpp$(DependSuffix) -MM src/vector3.cpp

$(IntermediateDirectory)/src_vector3.cpp$(PreprocessSuffix): src/vector3.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_vector3.cpp$(PreprocessSuffix) src/vector3.cpp

$(IntermediateDirectory)/src_shapes.cpp$(ObjectSuffix): src/shapes.cpp $(IntermediateDirectory)/src_shapes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/james/Source/Portfolio/Blocks/src/shapes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shapes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shapes.cpp$(DependSuffix): src/shapes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shapes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shapes.cpp$(DependSuffix) -MM src/shapes.cpp

$(IntermediateDirectory)/src_shapes.cpp$(PreprocessSuffix): src/shapes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shapes.cpp$(PreprocessSuffix) src/shapes.cpp

$(IntermediateDirectory)/src_blocks.cpp$(ObjectSuffix): src/blocks.cpp $(IntermediateDirectory)/src_blocks.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/james/Source/Portfolio/Blocks/src/blocks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_blocks.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_blocks.cpp$(DependSuffix): src/blocks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_blocks.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_blocks.cpp$(DependSuffix) -MM src/blocks.cpp

$(IntermediateDirectory)/src_blocks.cpp$(PreprocessSuffix): src/blocks.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_blocks.cpp$(PreprocessSuffix) src/blocks.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./out/


