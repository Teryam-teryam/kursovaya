.PHONY: all clean format server test

CXX = g++
CXXFLAGS = -O2 -Wall -DNDEBUG -std=c++17
LDFLAGS = -lcryptopp -lboost_program_options -lUnitTest++
# Проекты
PROJECT_SERVER = serv
PROJECT_TEST = utest
# Исходные файлы
SERVER_SOURCES := main.cpp $(filter-out test_main.cpp, $(wildcard *.cpp))
TEST_SOURCES := test_main.cpp $(filter-out main.cpp, $(wildcard *.cpp))
# Заголовочные файлы
HEADERS := $(wildcard *.h)
# Объекты
SERVER_OBJECTS := $(SERVER_SOURCES:%.cpp=%.o)
TEST_OBJECTS := $(TEST_SOURCES:%.cpp=%.o)
# Основная цель
all: server test
# Сборка сервера
server: $(SERVER_OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $(PROJECT_SERVER)
# Сборка тестов
test: $(TEST_OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $(PROJECT_TEST)
# Правило для компиляции объектов
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
# Обновление объектов при изменении заголовков
$(SERVER_OBJECTS): $(HEADERS)
$(TEST_OBJECTS): $(HEADERS)
# Форматирование кода
format:
	astyle *.cpp *.h
# Очистка
clean:
	rm -f $(PROJECT_SERVER) $(PROJECT_TEST) *.o *.orig
