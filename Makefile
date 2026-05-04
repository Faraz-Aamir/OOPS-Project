CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET = online_judge

SRCS = MyString.cpp Permission.cpp ExtendedAttribute.cpp User.cpp VersionRecord.cpp \
       FileSystemEntity.cpp RegularFile.cpp SymbolicLink.cpp Directory.cpp \
       MountPoint.cpp Partition.cpp StorageMedium.cpp Volume.cpp \
       TestCase.cpp Problem.cpp Submission.cpp \
       Contest.cpp BeginnerContest.cpp IntermediateContest.cpp AdvancedContest.cpp \
       ProblemBank.cpp EvaluationEngine.cpp ScoringEngine.cpp \
       EmailService.cpp SessionManager.cpp UserManager.cpp ContestManager.cpp \
       HttpServer.cpp OnlineJudge.cpp main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
