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

#define BUFFER_SIZE 1024

std::string Communicator::generateSalt()
{
    boost::mt19937 generator(static_cast<unsigned int>(time(0)));
    boost::random::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());
    
    std::stringstream saltStream;
    saltStream << std::hex << distribution(generator);
    std::string saltResult(saltStream.str());
    
    while (saltResult.length() < 16)
    {
        saltResult = '0' + saltResult;
    }
    return saltResult;
}

std::string Communicator::MD5(const std::string& inputString)
{
    using namespace CryptoPP;
    Weak::MD5 hash;
    std::string newHash;
    StringSource(inputString, true, new HashFilter(hash, new HexEncoder(new StringSink(newHash))));
    return newHash;
}

int Communicator::connection(int port, const std::map<std::string, std::string>& database, Logger* logger)
{
    try
    {
        int count = 0;
        const int queueLength = 100;
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_aton("127.0.0.1", &address.sin_addr);

        int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socketDescriptor <= 0)
        {
            throw CritError("Socket creation error: CRITICAL ERROR");
        }
        std::cout << "Socket has been created" << std::endl;
        
        if (bind(socketDescriptor, (const sockaddr*)&address, sizeof(sockaddr_in)) == -1)
        {
            throw CritError("Socket binding error: CRITICAL ERROR");
        }
        std::cout << "Socket has been binded" << std::endl;
        
        if (listen(socketDescriptor, queueLength) == -1)
        {
            throw CritError("Socket listening error: CRITICAL ERROR");
        }
        std::cout << "Socket is listening" << std::endl;
        std::cout << "Server started: " << inet_ntoa(address.sin_addr) << ":" << port << std::endl;

        while (true)
        {
            sockaddr_in clientAddress{};
            socklen_t length = sizeof(sockaddr_in);
            int workingSocket = accept(socketDescriptor, (sockaddr*)(&clientAddress), &length);
            
            if (workingSocket <= 0)
            {
                logger->writeLog("Socket didn't accept: NO CRITICAL ERROR");
                continue; // Продолжаем, если не удалось принять соединение
            }

            // Создаем новый поток для обработки клиента
            std::thread clientThread([this, workingSocket, &database, logger, &count]()
            {
                count++;
                std::cout << "Thread " << count << " started work" << std::endl;

                try
                {
                    std::unique_ptr<char[]> buffer(new char[BUFFER_SIZE]);

                    // 1. Получение идентификатора клиента
                    int receivedCount = recv(workingSocket, buffer.get(), BUFFER_SIZE, 0);
                    if (receivedCount <= 0)
                    {
                        throw NoCritError("NO CRITICAL ERROR ID retrieval error in thread count: " + std::to_string(count));
                    }
                    buffer[receivedCount] = 0;
                    std::string clientId(buffer.get(), receivedCount);

                    // 2. Проверка, существует ли клиент в базе данных
                    if (database.find(clientId) == database.end())
                    {
                    		send(workingSocket, "ERR", 3, 0);
                        throw NoCritError("NO CRITICAL ERROR Unknown ID in thread count: " + std::to_string(count));
                    }

                    // 3. Генерация соли и отправка её клиенту
                    std::string salt = generateSalt();
                    receivedCount = send(workingSocket, salt.c_str(), salt.size(), 0);
                    if (receivedCount <= 0)
                    {
                    
                        throw NoCritError("NO CRITICAL ERROR Salt sending error in thread count: " + std::to_string(count));
                    }

                    // 4. Получение хеша пароля от клиента
                    receivedCount = recv(workingSocket, buffer.get(), 56, 0);
                    if (receivedCount <= 0)
                    {
                        throw NoCritError("NO CRITICAL ERROR Hash retrieval error in thread count: " + std::to_string(count));
                    }
                    buffer[receivedCount] = 0;
                    std::string clientHash(buffer.get(), receivedCount);

                    // 5. Проверка хеша на соответствие
                    if (MD5(salt + database.at(clientId)) != clientHash)
                    {
                    		send(workingSocket, "ERR", 3, 0);
                        throw NoCritError("NO CRITICAL ERROR Authorization error in thread count: " + std::to_string(count));
                    }

                    // 6. Отправка подтверждения клиенту
                    receivedCount = send(workingSocket, "OK", 2, 0);
                    if (receivedCount <= 0)
                    {
                        throw NoCritError("NO CRITICAL ERROR OK sending error in thread count: " + std::to_string(count));
                    }

                    // 7. Получение количества векторов от клиента
                    int vectorCount;
                    receivedCount = recv(workingSocket, &vectorCount, sizeof(vectorCount), 0);
                    if (receivedCount <= 0)
                    {
                        throw NoCritError("NO CRITICAL ERROR Vector count not received in thread count: " + std::to_string(count));
                    }

                    // 8. Обработка каждого вектора
                    for (int i = 0; i < vectorCount; i++)
                    {
                        unsigned int vectorLength;
                        receivedCount = recv(workingSocket, &vectorLength, sizeof(vectorLength), 0);
                        if (receivedCount <= 0)
                        {
                            throw NoCritError("Vector length not received: NO CRITICAL ERROR in thread count: " + std::to_string(count));
                        }

                        std::unique_ptr<double[]> vectorData(new double[vectorLength]);
                        receivedCount = recv(workingSocket, vectorData.get(), vectorLength * sizeof(double), 0);
                        if (receivedCount <= 0)
                        {
                            throw NoCritError("Vector not received: NO CRITICAL ERROR in thread count: " + std::to_string(count));
                        }
                        if (receivedCount / sizeof(double) != vectorLength)
                        {
                            throw NoCritError("Received data has incorrect size: NO CRITICAL ERROR in thread count: " + std::to_string(count));
                        }

                        // 9. Выполнение вычислений
                        std::vector<double> vector(vectorData.get(), vectorData.get() + vectorLength);
                        Calc calculator(vector);
                        auto result = calculator.getResult();

                        // 10. Отправка результата обратно клиенту
                        receivedCount = send(workingSocket, &result, sizeof(result), 0);
                        if (receivedCount <= 0)
                        {
                            throw NoCritError("Result not sent: NO CRITICAL ERROR in thread count: " + std::to_string(count));
                        }
                    }

                    close(workingSocket);
                    std::cout << "Thread " << count << " finished work" << std::endl;
                }
                catch (const NoCritError& e)
                {
                    std::cerr << "Error in thread number: " << count << std::endl; // Выводим номер потока в консоль
                    logger->writeLog(e.what());
                }
                catch (...)
                {
                    std::cerr << "Error in thread number: " << count << std::endl; // Выводим номер потока в консоль
                    logger->writeLog("An unknown error occurred in the client thread.");
                }
                // Уменьшаем счетчик потоков после завершения работы потока
                count--;
            });

            clientThread.detach(); // Отсоединяем поток, чтобы он мог работать независимо
        }
    }
    catch (const CritError& e)
    {
        logger->writeLog(e.what());
        return 1;
    }
    catch (...)
    {
        logger->writeLog("An unknown error occurred during server setup.");
        return 1;
    }
}
