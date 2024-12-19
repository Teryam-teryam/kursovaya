#include "communicator.h"
#include "logger.h"
#include "error.h"
#include "calc.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <memory>
#include <fstream>
#include <limits>

#define BUFFER_SIZE 1024

std::string Communicator::generateSalt() {
    boost::mt19937 generator(static_cast<unsigned int>(time(0)));
    boost::random::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

    std::stringstream saltStream;
    saltStream << std::hex << distribution(generator);
    std::string saltResult(saltStream.str());

    // Дополняем соль до 16 символов
    while (saltResult.length() < 16) {
        saltResult = '0' + saltResult;
    }
    return saltResult;
}

std::string Communicator::MD5(const std::string& inputString) {
    using namespace CryptoPP;
    Weak::MD5 hash;
    std::string newHash;
    StringSource(inputString, true, new HashFilter(hash, new HexEncoder(new StringSink(newHash))));
    return newHash;
}

int Communicator::connection(int port, const std::map<std::string, std::string>& database, Logger* logger) {
    try {
        const int queueLength = 100;
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_aton("127.0.0.1", &address.sin_addr);

        int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socketDescriptor <= 0) {
            throw CritError("Socket creation error: CRITICAL ERROR");
        }

        if (bind(socketDescriptor, (const sockaddr*)&address, sizeof(sockaddr_in)) == -1) {
            throw CritError("Socket binding error: CRITICAL ERROR");
        }

        if (listen(socketDescriptor, queueLength) == -1) {
            throw CritError("Socket listening error: CRITICAL ERROR");
        }

        std::cout << "Server started on: " << inet_ntoa(address.sin_addr) << ":" << port << std::endl;

        while (true) {
            sockaddr_in clientAddress{};
            socklen_t length = sizeof(sockaddr_in);
            int workingSocket = accept(socketDescriptor, (sockaddr*)(&clientAddress), &length);

            if (workingSocket <= 0) {
                logger->writeLog("Socket didn't accept: NO CRITICAL ERROR");
                continue; // Продолжаем, если не удалось принять соединение
            }

            // Создаем новый поток для обработки клиента
            std::thread clientThread([this, workingSocket, &database, logger]() {
                std::thread::id threadId = std::this_thread::get_id();
                std::ostringstream oss;
                oss << threadId;

                std::cout << "Thread " << oss.str() << " started work" << std::endl;

                std::string currentStage;

                try {
                    std::unique_ptr<char[]> buffer(new char[BUFFER_SIZE]);

                    // Этап 1: Получение идентификатора клиента
                    currentStage = "ID retrieval";
                    int receivedCount = recv(workingSocket, buffer.get(), BUFFER_SIZE, 0);
                    if (receivedCount <= 0) {
                        close(workingSocket);
                        throw NoCritError("ID retrieval error in thread: " + oss.str());
                    }
                    buffer[receivedCount] = 0;
                    std::string clientId(buffer.get(), receivedCount);

                    // Этап 2: Проверка существования клиента в базе данных
                    currentStage = "Client ID check";
                    if (database.find(clientId) == database.end()) {
                        close(workingSocket);
                        send(workingSocket, "ERR", 3, 0);
                        throw NoCritError("Unknown ID in thread: " + oss.str());
                    }

                    // Этап 3: Генерация соли и отправка её клиенту
                    currentStage = "Salt generation and sending";
                    std::string salt = generateSalt();
                    receivedCount = send(workingSocket, salt.c_str(), salt.size(), 0);
                    if (receivedCount <= 0) {
                        close(workingSocket);
                        throw NoCritError("Salt sending error in thread: " + oss.str());
                    }

                    // Этап 4: Получение хеша пароля от клиента
                    currentStage = "Hash retrieval";
                    receivedCount = recv(workingSocket, buffer.get(), 56, 0);
                    if (receivedCount <= 0) {
                        close(workingSocket);
                        throw NoCritError("Hash retrieval error in thread: " + oss.str());
                    }
                    buffer[receivedCount] = 0;
                    std::string clientHash(buffer.get(), receivedCount);

                    // Этап 5: Проверка хеша на соответствие
                    currentStage = "Hash verification";
                    if (MD5(salt + database.at(clientId)) != clientHash) {
                        close(workingSocket);
                        send(workingSocket, "ERR", 3, 0);
                        throw NoCritError("Authorization error in thread: " + oss.str());
                    }

                    // Этап 6: Отправка подтверждения клиенту
                    currentStage = "Sending confirmation";
                    receivedCount = send(workingSocket, "OK", 2, 0);
                    if (receivedCount <= 0) {
                        close(workingSocket);
                        throw NoCritError("OK sending error in thread: " + oss.str());
                    }

                    // Этап 7: Получение количества векторов от клиента
                    currentStage = "Vector count retrieval";
                    int vectorCount;
                    receivedCount = recv(workingSocket, &vectorCount, sizeof(vectorCount), 0);
                    if (receivedCount <= 0) {
                        close(workingSocket);
                        throw NoCritError("Vector count not received in thread: " + oss.str());
                    }

                    // Этап 8: Обработка каждого вектора
                    for (int i = 0; i < vectorCount; i++) {
                        unsigned int vectorLength;
                        currentStage = "Vector length retrieval";
                        receivedCount = recv(workingSocket, &vectorLength, sizeof(vectorLength), 0);
                        if (receivedCount <= 0) {
                            close(workingSocket);
                            throw NoCritError("Vector length not received in thread: " + oss.str());
                        }

                        std::unique_ptr<double[]> vectorData(new double[vectorLength]);
                        currentStage = "Vector data retrieval";
                        receivedCount = recv(workingSocket, vectorData.get(), vectorLength * sizeof(double), 0);
                        if (receivedCount <= 0) {
                            close(workingSocket);
                            throw NoCritError("Vector not received in thread: " + oss.str());
                        }
                        if (receivedCount / sizeof(double) != vectorLength) {
                            close(workingSocket);
                            throw NoCritError("Received data has incorrect size in thread: " + oss.str());
                        }

                        // Этап 9: Выполнение вычислений
                        currentStage = "Calculating result";
                        std::vector<double> vector(vectorData.get(), vectorData.get() + vectorLength);
                        Calc calculator(vector);
                        auto result = calculator.getResult();

                        // Этап 10: Отправка результата обратно клиенту
                        currentStage = "Sending result";
                        receivedCount = send(workingSocket, &result, sizeof(result), 0);
                        if (receivedCount <= 0) {
                            close(workingSocket);
                            throw NoCritError("Result not sent in thread: " + oss.str());
                        }
                    }

                    close(workingSocket);
                    std::cout << "Thread " << oss.str() << " finished work" << std::endl;
                }
                catch (const NoCritError& e) {
                    std::cerr << "Error in thread ID: " << oss.str() << " at stage: " << currentStage << std::endl;
                    logger->writeLog(e.what() + std::string(" at stage: ") + currentStage);
                }
                catch (...) {
                    std::cerr << "Error in thread ID: " << oss.str() << " at stage: " << currentStage << std::endl;
                    logger->writeLog("An unknown error occurred in the client thread at stage: " + currentStage);
                }
            });

            clientThread.detach(); // Отсоединяем поток, чтобы он мог работать независимо
        }
    }
    catch (const CritError& e) {
        logger->writeLog(e.what());
        return 1;
    }
    catch (...) {
        logger->writeLog("An unknown error occurred during server setup.");
        return 1;
    }
}
