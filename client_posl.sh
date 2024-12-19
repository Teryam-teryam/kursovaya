#!/bin/bash

# Запускаем 100 клиентов последовательно
for i in $(seq 1 100); do
    ./client_double -H MD5 -S s
done
