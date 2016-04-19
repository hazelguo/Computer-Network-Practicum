CC = g++
#CPPFLAGS = -g -Wall -Werror -pthread
CPPFLAGS = -O2 -pthread
OBJS = StudentInfo.o BPNeuralNet.o training.o 
OBJS2 = StudentInfo.o BPNeuralNet.o k-nearest.o 
OBJS3 = StudentInfo.o BPNeuralNet.o generate_similarity_matrix.o 
OTHFILES = Makefile
TARGET = training
TARGET2 = k-nearest
TARGET3 = generate_similarity_matrix

all: training k-nearest generate_similarity_matrix

StudentInfo.o: StudentInfo.cc StudentInfo.h
	$(CC) $(CPPFLAGS) -c StudentInfo.cc

BPNeuralNet.o: BPNeuralNet.cc BPNeuralNet.h
	$(CC) $(CPPFLAGS) -c BPNeuralNet.cc	

training.o: training.cc
	$(CC) $(CPPFLAGS) -c training.cc

k-nearest.o: k-nearest.cc
	$(CC) $(CPPFLAGS) -c k-nearest.cc

generate_similarity_matrix.o: generate_similarity_matrix.cc
	$(CC) $(CPPFLAGS) -c generate_similarity_matrix.cc

training: $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(TARGET)

k-nearest: $(OBJS2)
	$(CC) $(CPPFLAGS) $(OBJS2) -o $(TARGET2)

generate_similarity_matrix: $(OBJS3)
	$(CC) $(CPPFLAGS) $(OBJS3) -o $(TARGET3)

prof:
	$(CC) $(CPPFLAGS) -c -pg StudentInfo.cc
	$(CC) $(CPPFLAGS) -c -pg BPNeuralNet.cc
	$(CC) $(CPPFLAGS) -c -pg training.cc
	$(CC) $(CPPFLAGS) -c -pg k-nearest.cc
	$(CC) $(CPPFLAGS) -c -pg generate_similarity_matrix.cc
	$(CC) $(CPPFLAGS) $(OBJS) -pg -o $(TARGET)
	$(CC) $(CPPFLAGS) $(OBJS2) -pg -o $(TARGET2)
	$(CC) $(CPPFLAGS) $(OBJS3) -pg -o $(TARGET3)

clean:
	rm -f *~ $(OBJS) $(TARGET) $(OBJS2) $(TARGET2) $(OBJS3) $(TARGET3)

